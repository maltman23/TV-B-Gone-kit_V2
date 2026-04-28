#include <stdbool.h>
#include <stdint.h>

#include "button_gpio.h"
#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "iot_button.h"
#include "sdkconfig.h"
#include "tvbgone_core.h"

#define VISLED CONFIG_TVBGONE_VISLED_GPIO
#define IRLED CONFIG_TVBGONE_IRLED_GPIO
#define BUTTON_NA CONFIG_TVBGONE_BUTTON_NA_GPIO
#define BUTTON_EU CONFIG_TVBGONE_BUTTON_EU_GPIO

#define VIS_LED_BLIP_TIME_MS 25
#define BLIP_TIME_MS 125
#define SEND_START_DELAY_MS 500
#define SEND_END_DELAY_MS 500

static const char *TAG = "TV-B-Gone";

static TaskHandle_t s_send_task;
static volatile bool s_restart_requested;
static volatile tvbgone_core_region_t s_requested_region;
static portMUX_TYPE s_send_lock = portMUX_INITIALIZER_UNLOCKED;
static button_handle_t s_na_button;
static button_handle_t s_eu_button;

static void send_task(void *arg);
static void status_led_task(void *arg);
static void handle_single_click(void *button_handle, void *user_data);
static void handle_double_click(void *button_handle, void *user_data);

static void set_visible_led(bool on)
{
    gpio_set_level((gpio_num_t)VISLED, on ? 0 : 1);
}

static void blink_visible_led_n_times(int num_blinks)
{
    for (int i = 0; i < num_blinks; i++) {
        set_visible_led(true);
        vTaskDelay(pdMS_TO_TICKS(VIS_LED_BLIP_TIME_MS));
        set_visible_led(false);
        vTaskDelay(pdMS_TO_TICKS(BLIP_TIME_MS));
    }
}

static int get_region_blink_count(tvbgone_core_region_t region)
{
    return (region == TVBGONE_CORE_REGION_NA) ? 3 : 6;
}

static const char *get_region_name(tvbgone_core_region_t region)
{
    return (region == TVBGONE_CORE_REGION_NA) ? "NA" : "EU";
}

static void request_region_send(tvbgone_core_region_t region)
{
    bool start_task = false;

    taskENTER_CRITICAL(&s_send_lock);
    if (s_send_task == NULL) {
        s_requested_region = region;
        s_restart_requested = false;
        start_task = true;
    } else {
        s_requested_region = region;
        s_restart_requested = true;
    }
    taskEXIT_CRITICAL(&s_send_lock);

    if (start_task) {
        BaseType_t send_task_created = xTaskCreate(
            send_task,
            "tvbg_send",
            TVBGONE_CORE_DEFAULT_TASK_STACK_SIZE,
            (void *)(intptr_t)region,
            TVBGONE_CORE_DEFAULT_TASK_PRIORITY,
            &s_send_task
        );
        ESP_RETURN_VOID_ON_FALSE(send_task_created == pdPASS, TAG,
                                 "failed to create send task");
        return;
    }

    ESP_ERROR_CHECK(tvbgone_core_stop());
}

static void request_stop_send(void)
{
    taskENTER_CRITICAL(&s_send_lock);
    s_restart_requested = false;
    taskEXIT_CRITICAL(&s_send_lock);

    if (s_send_task != NULL) {
        ESP_ERROR_CHECK(tvbgone_core_stop());
    }
}

static void send_task(void *arg)
{
    tvbgone_core_region_t region = (tvbgone_core_region_t)(intptr_t)arg;

    while (true) {
        int blink_count = get_region_blink_count(region);
        esp_err_t err;

        blink_visible_led_n_times(blink_count);
        ESP_LOGI(TAG, "%s button pressed", get_region_name(region));
        vTaskDelay(pdMS_TO_TICKS(SEND_START_DELAY_MS));

        err = tvbgone_core_send(region, TVBGONE_CORE_SEND_MODE_SINGLE);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "Finished transmitting entire database for %s.", get_region_name(region));
            blink_visible_led_n_times(blink_count);
            vTaskDelay(pdMS_TO_TICKS(SEND_END_DELAY_MS));
        } else if (err == ESP_ERR_INVALID_STATE) {
            ESP_LOGI(TAG, "Transmission interrupted");
        } else {
            ESP_LOGE(TAG, "Transmission failed: %s", esp_err_to_name(err));
        }

        set_visible_led(false);

        taskENTER_CRITICAL(&s_send_lock);
        if (s_restart_requested) {
            region = s_requested_region;
            s_restart_requested = false;
            taskEXIT_CRITICAL(&s_send_lock);
            continue;
        }

        s_send_task = NULL;
        taskEXIT_CRITICAL(&s_send_lock);
        break;
    }

    vTaskDelete(NULL);
}

static void status_led_task(void *arg)
{
    (void)arg;

    uint16_t last_code_number = 0;

    while (true) {
        tvbgone_core_status_t status;
        esp_err_t err = tvbgone_core_get_status(&status);

        if (err != ESP_OK) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        if (status.run_state == TVBGONE_CORE_RUN_STATE_RUNNING) {
            if ((status.current_code_number != 0) &&
                (status.current_code_number != last_code_number)) {
                set_visible_led(true);
                vTaskDelay(pdMS_TO_TICKS(VIS_LED_BLIP_TIME_MS));
                set_visible_led(false);
                last_code_number = status.current_code_number;
            } else {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        } else {
            last_code_number = 0;
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}

static void handle_single_click(void *button_handle, void *user_data)
{
    (void)button_handle;

    request_region_send((tvbgone_core_region_t)(intptr_t)user_data);
}

static void handle_double_click(void *button_handle, void *user_data)
{
    (void)button_handle;
    (void)user_data;

    request_stop_send();
}

static button_handle_t configure_button(gpio_num_t gpio_num,
                                        tvbgone_core_region_t region)
{
    const button_config_t button_config = {
        .long_press_time = 0,
        .short_press_time = 0,
    };
    const button_gpio_config_t gpio_config = {
        .gpio_num = gpio_num,
        .active_level = 0,
        .enable_power_save = false,
        .disable_pull = false,
    };
    button_handle_t button = NULL;
    esp_err_t err = iot_button_new_gpio_device(&button_config, &gpio_config, &button);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "failed to create button on GPIO %d: %s",
                 gpio_num, esp_err_to_name(err));
        return NULL;
    }
    ESP_ERROR_CHECK(iot_button_register_cb(button, BUTTON_SINGLE_CLICK,
                                           NULL, handle_single_click,
                                           (void *)(intptr_t)region));
    ESP_ERROR_CHECK(iot_button_register_cb(button, BUTTON_DOUBLE_CLICK,
                                           NULL, handle_double_click, NULL));
    return button;
}

void app_main(void)
{
    tvbgone_core_config_t config;
    gpio_config_t output_gpio_cfg = {
        .pin_bit_mask = (1ULL << VISLED),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&output_gpio_cfg));
    set_visible_led(false);

    tvbgone_core_get_default_config(&config);
    config.ir_led_gpio = (gpio_num_t)IRLED;
    config.rmt_channel_mode = TVBGONE_CORE_RMT_CHANNEL_MODE_INTERNAL;
    ESP_ERROR_CHECK(tvbgone_core_init(&config));
    BaseType_t status_led_task_created = xTaskCreate(
        status_led_task,
        "tvbg_led",
        2048,
        NULL,
        TVBGONE_CORE_DEFAULT_TASK_PRIORITY,
        NULL
    );
    ESP_RETURN_VOID_ON_FALSE(status_led_task_created == pdPASS, TAG,
                             "failed to create LED status task");
    s_na_button = configure_button((gpio_num_t)BUTTON_NA, TVBGONE_CORE_REGION_NA);
    s_eu_button = configure_button((gpio_num_t)BUTTON_EU, TVBGONE_CORE_REGION_EU);
}
