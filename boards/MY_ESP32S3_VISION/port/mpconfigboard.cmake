set(IDF_TARGET esp32s3)

set(SDKCONFIG_DEFAULTS
    boards/sdkconfig.base
    boards/sdkconfig.usb
    boards/sdkconfig.240mhz
    boards/sdkconfig.spiram_sx
    boards/MY_ESP32S3_VISION/sdkconfig.board
)

set(MICROPY_PY_BTREE OFF)
