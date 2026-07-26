# SPDX-FileCopyrightText: 2026 ESP-Vision Project
# SPDX-License-Identifier: Apache-2.0

freeze("$(PORT_DIR)/modules")
freeze("$(ESP_VISION_ROOT)/modules", "py_inisetup.py")
freeze("$(ESP_VISION_ROOT)/boards/MY_ESP32S3_VISION", "board_inisetup.py")
include("$(MPY_DIR)/extmod/asyncio")
require("webrepl")