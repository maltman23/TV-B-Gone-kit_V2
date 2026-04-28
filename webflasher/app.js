const BAUD_RATE = 115200;
const FIRMWARE_PATH = "./tvbgone_esp32c3.bin";
const FIRMWARE_ADDRESS = 0x0;
const ESPTOOL_JS_MODULE_URL = "./vendor/esptool-js.bundle.js";

const connectButton = document.getElementById("connect-button");
const flashButton = document.getElementById("flash-button");
const clearLogButton = document.getElementById("clear-log-button");
const connectionStatus = document.getElementById("connection-status");
const chipStatus = document.getElementById("chip-status");
const firmwareStatus = document.getElementById("firmware-status");
const progressLabel = document.getElementById("progress-label");
const progressPercent = document.getElementById("progress-percent");
const flashProgress = document.getElementById("flash-progress");
const resultBanner = document.getElementById("result-banner");
const logOutput = document.getElementById("log-output");

const state = {
  port: null,
  transport: null,
  esploader: null,
  chipName: null,
  busy: false,
  esptoolModule: null,
};

function formatError(error) {
  if (error instanceof Error && error.message) {
    return error.message;
  }

  return String(error);
}

function appendLog(message) {
  const line = `[${new Date().toLocaleTimeString()}] ${message}`;
  logOutput.textContent += `${line}\n`;
  logOutput.scrollTop = logOutput.scrollHeight;
}

function clearLog() {
  logOutput.textContent = "";
}

function setProgress(value, label) {
  const bounded = Math.max(0, Math.min(100, value));
  flashProgress.value = bounded;
  progressPercent.textContent = `${Math.round(bounded)}%`;
  progressLabel.textContent = label;
}

function setBanner(kind, message) {
  resultBanner.hidden = false;
  resultBanner.className = `result-banner ${kind}`;
  resultBanner.textContent = message;
}

function clearBanner() {
  resultBanner.hidden = true;
  resultBanner.className = "result-banner";
  resultBanner.textContent = "";
}

function setBusy(isBusy) {
  state.busy = isBusy;
  connectButton.disabled = isBusy;
  flashButton.disabled = isBusy || !state.esploader;
}

function getPortLabel(port) {
  const info = port.getInfo ? port.getInfo() : {};
  const vendor = info.usbVendorId ? `0x${info.usbVendorId.toString(16)}` : "unknown";
  const product = info.usbProductId ? `0x${info.usbProductId.toString(16)}` : "unknown";
  return `${vendor}:${product}`;
}

function uint8ArrayToBinaryString(bytes) {
  const chunkSize = 0x8000;
  let binary = "";

  for (let offset = 0; offset < bytes.length; offset += chunkSize) {
    const chunk = bytes.subarray(offset, offset + chunkSize);
    binary += String.fromCharCode(...chunk);
  }

  return binary;
}

function makeTerminal() {
  return {
    clean() {
      clearLog();
    },
    writeLine(data) {
      appendLog(data);
    },
    write(data) {
      if (data && data.trim()) {
        appendLog(data);
      }
    },
  };
}

async function loadEsptoolModule() {
  if (state.esptoolModule) {
    return state.esptoolModule;
  }

  appendLog(`Loading esptool-js from ${ESPTOOL_JS_MODULE_URL}`);
  setProgress(2, "Loading flasher library");

  try {
    state.esptoolModule = await import(ESPTOOL_JS_MODULE_URL);
    appendLog("esptool-js loaded");
    return state.esptoolModule;
  } catch (error) {
    const message =
      "Unable to load the local esptool-js bundle. Rebuild or re-copy the webflasher assets, then reload the page and inspect the browser console.";
    setBanner("error", message);
    appendLog(`esptool-js load error: ${formatError(error)}`);
    throw new Error(message);
  }
}

async function disconnectTransport() {
  if (!state.transport) {
    return;
  }

  try {
    await state.transport.disconnect();
  } catch (error) {
    appendLog(`Transport disconnect notice: ${formatError(error)}`);
  }
}

async function ensureFirmware() {
  const firmwareUrl = new URL(FIRMWARE_PATH, window.location.href);
  appendLog(`Downloading firmware from ${firmwareUrl.href}`);
  firmwareStatus.textContent = "Downloading merged image";
  setProgress(8, "Downloading firmware");

  const response = await fetch(firmwareUrl, { cache: "no-store" });
  if (!response.ok) {
    throw new Error(`Firmware download failed with HTTP ${response.status}`);
  }

  const total = Number(response.headers.get("content-length")) || 0;
  if (!response.body || !total) {
    const buffer = await response.arrayBuffer();
    const bytes = new Uint8Array(buffer);
    firmwareStatus.textContent = `Merged image ready (${buffer.byteLength} bytes)`;
    setProgress(22, "Firmware ready");
    appendLog(`Downloaded ${buffer.byteLength} bytes of firmware`);
    return uint8ArrayToBinaryString(bytes);
  }

  const reader = response.body.getReader();
  const chunks = [];
  let received = 0;

  while (true) {
    const { done, value } = await reader.read();
    if (done) {
      break;
    }

    chunks.push(value);
    received += value.length;
    const percent = (received / total) * 18;
    setProgress(4 + percent, "Downloading firmware");
  }

  const merged = new Uint8Array(received);
  let offset = 0;
  for (const chunk of chunks) {
    merged.set(chunk, offset);
    offset += chunk.length;
  }

  firmwareStatus.textContent = `Merged image ready (${received} bytes)`;
  setProgress(22, "Firmware ready");
  appendLog(`Downloaded ${received} bytes of firmware`);
  return uint8ArrayToBinaryString(merged);
}

async function connectDevice() {
  if (!("serial" in navigator)) {
    throw new Error("This browser does not support Web Serial.");
  }

  clearBanner();
  setBusy(true);
  setProgress(0, "Requesting serial port");
  connectionStatus.textContent = "Waiting for port selection";
  chipStatus.textContent = "Connecting";
  firmwareStatus.textContent = "Ready to download";

  try {
    const { ESPLoader, Transport } = await loadEsptoolModule();
    const port = await navigator.serial.requestPort();
    const transport = new Transport(port, true);
    const loader = new ESPLoader({
      transport,
      baudrate: BAUD_RATE,
      terminal: makeTerminal(),
      debugLogging: false,
    });

    appendLog(`Selected serial port ${getPortLabel(port)}`);
    setProgress(12, "Connecting to bootloader");
    const chipName = await loader.main();

    state.port = port;
    state.transport = transport;
    state.esploader = loader;
    state.chipName = chipName;

    connectionStatus.textContent = "Connected";
    chipStatus.textContent = chipName;
    firmwareStatus.textContent = "Ready to flash";
    setProgress(20, "Device ready");
    appendLog(`Connected to ${chipName}`);
  } catch (error) {
    await disconnectTransport();
    state.port = null;
    state.transport = null;
    state.esploader = null;
    state.chipName = null;
    connectionStatus.textContent = "Connection failed";
    chipStatus.textContent = "Retry required";
    setProgress(0, "Connection failed");
    setBanner("error", formatError(error));
    appendLog(`Connection error: ${formatError(error)}`);
  } finally {
    setBusy(false);
  }
}

async function flashFirmware() {
  if (!state.esploader) {
    return;
  }

  clearBanner();
  setBusy(true);

  try {
    const firmware = await ensureFirmware();
    firmwareStatus.textContent = "Flashing merged image";
    appendLog(`Starting flash at address 0x${FIRMWARE_ADDRESS.toString(16)}`);
    setProgress(24, "Starting flash");

    await state.esploader.writeFlash({
      fileArray: [{ data: firmware, address: FIRMWARE_ADDRESS }],
      flashMode: "dio",
      flashFreq: "80m",
      flashSize: "2MB",
      eraseAll: false,
      compress: true,
      reportProgress: (_fileIndex, written, total) => {
        const percent = total > 0 ? written / total : 0;
        setProgress(24 + percent * 70, "Writing and verifying flash");
      },
    });

    setProgress(96, "Resetting device");
    appendLog("Flash complete, issuing hard reset");
    await state.esploader.after("hard_reset");
    await disconnectTransport();

    connectionStatus.textContent = "Completed";
    firmwareStatus.textContent = "Firmware flashed successfully";
    setProgress(100, "Finished");
    setBanner(
      "success",
      "Flash complete. The board has been reset and should now run TV-B-Gone Kit V2.",
    );
    appendLog("Firmware flashed successfully");
  } catch (error) {
    firmwareStatus.textContent = "Flash failed";
    setProgress(flashProgress.value || 0, "Flash failed");
    setBanner("error", formatError(error));
    appendLog(`Flash error: ${formatError(error)}`);
  } finally {
    state.transport = null;
    state.esploader = null;
    state.port = null;
    state.chipName = null;
    connectionStatus.textContent =
      connectionStatus.textContent === "Completed" ? "Completed" : "Disconnected";
    chipStatus.textContent =
      connectionStatus.textContent === "Completed" ? "Reset after flash" : "Reconnect to retry";
    flashButton.disabled = true;
    connectButton.disabled = false;
    state.busy = false;
  }
}

function initializePage() {
  clearLog();

  if (!("serial" in navigator)) {
    connectButton.disabled = true;
    flashButton.disabled = true;
    connectionStatus.textContent = "Unsupported browser";
    chipStatus.textContent = "Use Chrome or Edge on desktop";
    firmwareStatus.textContent = "Web Serial unavailable";
    setBanner(
      "error",
      "This browser does not support Web Serial. Open this page in Chrome, Edge, or another Chromium browser.",
    );
    appendLog("Web Serial support not detected");
    return;
  }

  appendLog("Webflasher ready. Connect an ESP32-C3 Super Mini to begin.");
}

connectButton.addEventListener("click", () => {
  void connectDevice();
});

flashButton.addEventListener("click", () => {
  void flashFirmware();
});

clearLogButton.addEventListener("click", () => {
  clearLog();
  appendLog("Log cleared");
});

initializePage();
