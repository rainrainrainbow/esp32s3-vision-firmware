/*
 * SPDX-FileCopyrightText: 2026 ESP-Vision Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "boardconfig.h"
#include "esp_log.h"

static const char *TAG = "touch";

int touch_init(void)
{
    ESP_LOGI(TAG, "Touch controller not yet implemented for this board");
    return 0;
}

int touch_read(int *x, int *y, int *pressed)
{
    *x = 0;
    *y = 0;
    *pressed = 0;
    return 0;
}