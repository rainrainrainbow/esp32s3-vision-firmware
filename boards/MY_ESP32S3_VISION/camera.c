/*
 * SPDX-FileCopyrightText: 2026 ESP-Vision Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "boardconfig.h"
#include "esp_camera.h"
#include "esp_log.h"

static const char *TAG = "camera";

int camera_init(camera_config_t *config)
{
    config->pin_pwdn = ESP_VISION_CAMERA_SENSOR_PWDN_PIN;
    config->pin_reset = ESP_VISION_CAMERA_SENSOR_RESET_PIN;
    config->pin_xclk = ESP_VISION_CAMERA_XCLK_PIN;
    config->pin_sccb_sda = ESP_VISION_CAMERA_SCCB_I2C_SDA_PIN;
    config->pin_sccb_scl = ESP_VISION_CAMERA_SCCB_I2C_SCL_PIN;
    config->pin_d7 = ESP_VISION_CAMERA_DVP_D7_PIN;
    config->pin_d6 = ESP_VISION_CAMERA_DVP_D6_PIN;
    config->pin_d5 = ESP_VISION_CAMERA_DVP_D5_PIN;
    config->pin_d4 = ESP_VISION_CAMERA_DVP_D4_PIN;
    config->pin_d3 = ESP_VISION_CAMERA_DVP_D3_PIN;
    config->pin_d2 = ESP_VISION_CAMERA_DVP_D2_PIN;
    config->pin_d1 = ESP_VISION_CAMERA_DVP_D1_PIN;
    config->pin_d0 = ESP_VISION_CAMERA_DVP_D0_PIN;
    config->pin_vsync = ESP_VISION_CAMERA_DVP_VSYNC_PIN;
    config->pin_href = ESP_VISION_CAMERA_DVP_HSYNC_PIN;
    config->pin_pclk = ESP_VISION_CAMERA_DVP_PCLK_PIN;

    config->xclk_freq_hz = ESP_VISION_CAMERA_XCLK_FREQ;
    config->ledc_timer = LEDC_TIMER_0;
    config->ledc_channel = LEDC_CHANNEL_0;
    config->pixel_format = PIXFORMAT_RGB565;
    config->frame_size = FRAMESIZE_QVGA;
    config->jpeg_quality = 12;
    config->fb_count = ESP_VISION_CAMERA_BUFFER_COUNT;
    config->grab_mode = CAMERA_GRAB_LATEST;

    esp_err_t err = esp_camera_init(config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed with error 0x%x", err);
        return -1;
    }

    ESP_LOGI(TAG, "Camera initialized successfully");
    return 0;
}