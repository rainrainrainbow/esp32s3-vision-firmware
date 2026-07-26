# ESP-Vision MY_ESP32S3_VISION Board Support

Custom board support package for ESP32-S3 dual-core development board with camera, audio, touch, and display.

## Hardware Specifications

### Main Controller
- **Chip**: ESP32-S3 (Dual-core Xtensa LX7 @ 320MHz)
- **Flash**: 16MB (DIO, 80MHz)
- **PSRAM**: 8MB OPI PSRAM (Octal SPI, 80MHz)

### Camera - GalaxyCore GC2145
- **Interface**: DVP parallel (8-bit)
- **XCLK**: GPIO15 (8MHz via LEDC)
- **PCLK**: GPIO13
- **VSYNC**: GPIO6, **HSYNC**: GPIO7
- **Data Pins**: D0-D7 on GPIO11/9/8/10/12/18/17/16
- **I2C**: Shared bus (SDA=GPIO38, SCL=GPIO48)

### Audio - ES8311 Codec
- **I2C**: Shared with camera (SDA=GPIO38, SCL=GPIO48, addr=0x18)
- **I2S**: MCLK=GPIO45, WS=GPIO41, BCLK=GPIO39, DIN=GPIO40, DOUT=GPIO42
- **PA Enable**: GPIO46
- **Sample Rate**: 16kHz input/output

### Display - 320x240 SPI LCD
- **Interface**: SPI Mode 0
- **DC**: GPIO1, **CS**: GPIO2, **CLK**: GPIO21, **MOSI**: GPIO47
- **Backlight**: GPIO14 (PWM controlled)
- **Orientation**: Mirror X, Swap XY

### Touch - FT5x06
- **I2C**: Shared bus (SDA=GPIO38, SCL=GPIO48, addr=0x38)

### Buttons
- **Left**: GPIO0 (BOOT)
- **Right**: GPIO43

### UART0 (Voice Chip)
- **RXD**: GPIO44, **TXD**: Not used

## Features

### 1. OTA Firmware Update + WiFi File Management
- **WiFi Mode**: Station + SoftAP coexistence
- **SoftAP SSID**: `ESP32S3_Vision_XXXX` (XXXX = MAC suffix)
- **SoftAP Password**: `vision123`
- **Web UI**: http://192.168.4.1
  - `/` - Desktop launcher
  - `/ota` - Firmware upload page
  - `/files` - File browser
  - `/wifi` - WiFi configuration
  - `/api/status` - System status JSON

### 2. MicroPython Desktop System
Android-like application architecture:
- **Launcher**: Desktop UI with app grid
- **App Manager**: Install/uninstall/launch `.app` packages
- **System Services**: WiFi, OTA, file management, audio, camera, touch

### 3. Hardware Drivers
- GC2145 camera (via esp32-camera)
- ES8311 audio codec (via esp_codec_dev)
- FT5x06 touch controller (via esp_lcd_touch_ft5x06)
- SPI LCD display with LVGL integration

## Project Structure

```
boards/MY_ESP32S3_VISION/
├── boardconfig.h          # Pin definitions & board constants
├── imlib_config.h         # OpenMV imlib algorithm switches
├── board.cmake            # Board CMake configuration
├── manifest.py            # Frozen Python modules manifest
├── camera.c               # GC2145 camera backend
├── display.c              # SPI LCD + backlight control
├── touch.c                # FT5x06 touch backend
├── audio.c                # ES8311 audio codec backend
├── sdcard.c               # Stub (no SD card on this board)
├── port/
│   ├── mpconfigboard.cmake  # IDF target & SDK config chain
│   ├── mpconfigboard.h      # MicroPython feature switches
│   ├── sdkconfig.board      # Board-specific Kconfig overrides
│   ├── sdkconfig.defaults   # PSRAM/Flash/WiFi/OTA defaults
│   └── partitions.csv       # OTA dual-slot partition table
├── system/
│   ├── launcher.py          # Desktop launcher entry point
│   ├── app_manager.py       # Application lifecycle manager
│   ├── wifi_manager.py      # WiFi STA+AP management
│   ├── ota_service.py       # OTA firmware upgrade service
│   ├── file_manager.py      # File system management
│   ├── camera_service.py    # Camera Python API
│   ├── audio_service.py     # Audio Python API
│   ├── touch_service.py     # Touch Python API
│   ├── display_service.py   # Display Python API
│   ├── http_server.py       # Web server with UI
│   └── system_info.py       # System information
└── apps/
    ├── settings/            # System settings app
    ├── camera_demo/         # Camera preview app
    └── face_detect/         # Face detection demo
```

## Partition Table

```
Name       Type  SubType   Offset    Size      Flags
nvs        data  nvs       0x9000    0x6000
otadata    data  ota       0xf000    0x2000
phy_init   data  phy       0x11000   0x1000
ota_0      app   ota_0     0x20000   0x300000  (3MB)
ota_1      app   ota_1     0x320000  0x300000  (3MB)
storage    data  littlefs  0x620000  0x9E0000  (~9.8MB)
```

## Building

### Prerequisites
- ESP-IDF v5.5
- MicroPython v1.28.0 (managed by ESP-Vision)
- Git with submodule support

### Local Build
```bash
# Clone with submodules
git clone --recursive https://github.com/your-repo/esp-vision.git
cd esp-vision

# Configure
idf.py --board MY_ESP32S3_VISION reconfigure

# Build
idf.py --board MY_ESP32S3_VISION build

# Merge firmware
cd build/MY_ESP32S3_VISION
esptool.py --chip esp32s3 merge_bin \
  --flash_mode dio --flash_size 16MB --flash_freq 80m \
  -o esp32s3_vision_firmware.bin \
  0x0 bootloader/bootloader.bin \
  0x8000 partition_table/partition-table.bin \
  0x20000 micropython.bin
```

### GitHub Actions
Push to `main` branch or manually trigger the workflow:
- Workflow: `.github/workflows/build-firmware.yml`
- Output: `esp32s3_vision_firmware.bin` (artifact)

## Flashing

```bash
# Flash merged firmware
esptool.py --chip esp32s3 -p /dev/ttyUSB0 write_flash 0x0 esp32s3_vision_firmware.bin

# Or use WebSerial (built into ESP-Vision)
# Visit http://192.168.4.1 after first boot
```

## Verification Checklist

After flashing, verify via serial monitor:
- [ ] Boot log shows "ESP-VISION ready"
- [ ] MicroPython REPL accessible
- [ ] WiFi SoftAP active (SSID: ESP32S3_Vision_XXXX)
- [ ] Web UI accessible at http://192.168.4.1
- [ ] OTA page accepts .bin uploads
- [ ] Camera captures valid images
- [ ] LCD displays correctly
- [ ] Touch responds to input
- [ ] Audio playback/recording works
- [ ] App manager loads .app packages

## Important Notes

1. **I2C Bus Sharing**: Camera (SCCB), ES8311, and FT5x06 share GPIO38/48 I2C bus. Bus is initialized once.
2. **No SD Card**: All storage uses LittleFS on internal Flash.
3. **OPI PSRAM**: Must configure Octal SPI mode in sdkconfig.
4. **GC2145**: Uses esp32-camera v2.x with native GC2145 support.
5. **Memory**: WiFi + Camera + PSRAM requires `CONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP=y`.

## License

Based on ESP-Vision (Espressif Systems) and MicroPython (MIT License).