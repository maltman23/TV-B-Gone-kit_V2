#!/bin/zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/firmware/component/tvbgone_core/examples/tvbgone_esp32c3_supermini/build"
OUTPUT_BIN="${ROOT_DIR}/webflasher/tvbgone_esp32c3.bin"

if [[ ! -f "${BUILD_DIR}/bootloader/bootloader.bin" ]] || \
   [[ ! -f "${BUILD_DIR}/partition_table/partition-table.bin" ]] || \
   [[ ! -f "${BUILD_DIR}/tvbgone_esp32c3_supermini.bin" ]]; then
  echo "Missing build outputs in ${BUILD_DIR}"
  echo "Build the example first, for example:"
  echo "  cd firmware/component/tvbgone_core/examples/tvbgone_esp32c3_supermini"
  echo "  idf.py set-target esp32c3 build"
  exit 1
fi

mkdir -p "${ROOT_DIR}/webflasher"

esptool --chip esp32c3 merge-bin \
  --flash-mode dio \
  --flash-freq 80m \
  --flash-size 2MB \
  -o "${OUTPUT_BIN}" \
  0x0 "${BUILD_DIR}/bootloader/bootloader.bin" \
  0x8000 "${BUILD_DIR}/partition_table/partition-table.bin" \
  0x10000 "${BUILD_DIR}/tvbgone_esp32c3_supermini.bin"

echo "Prepared local webflasher binary:"
echo "  ${OUTPUT_BIN}"
