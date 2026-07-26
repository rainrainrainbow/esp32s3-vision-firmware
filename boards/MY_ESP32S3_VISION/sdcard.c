/*
 * SPDX-FileCopyrightText: 2026 ESP-Vision Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "boardconfig.h"
#include "esp_log.h"

static const char *TAG = "sdcard";

int sdcard_init(void)
{
    ESP_LOGW(TAG, "SD card not supported on this board");
    return -1;
}

int sdcard_deinit(void)
{
    return 0;
}