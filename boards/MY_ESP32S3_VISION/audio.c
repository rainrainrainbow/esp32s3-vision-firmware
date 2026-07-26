/*
 * SPDX-FileCopyrightText: 2026 ESP-Vision Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "boardconfig.h"
#include "esp_log.h"
#include "driver/i2s.h"

static const char *TAG = "audio";

int audio_init(void)
{
    ESP_LOGI(TAG, "Initializing ES8311 audio codec");

    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX,
        .sample_rate = ESP_VISION_AUDIO_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true,
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = ESP_VISION_AUDIO_I2S_BCLK_PIN,
        .ws_io_num = ESP_VISION_AUDIO_I2S_WS_PIN,
        .data_out_num = ESP_VISION_AUDIO_I2S_DOUT_PIN,
        .data_in_num = ESP_VISION_AUDIO_I2S_DIN_PIN,
    };

    esp_err_t err = i2s_driver_install(ESP_VISION_AUDIO_I2S_PORT, &i2s_config, 0, NULL);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2S driver install failed: %d", err);
        return -1;
    }

    err = i2s_set_pin(ESP_VISION_AUDIO_I2S_PORT, &pin_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2S set pin failed: %d", err);
        return -1;
    }

    ESP_LOGI(TAG, "Audio codec initialized: %d Hz", ESP_VISION_AUDIO_SAMPLE_RATE);
    return 0;
}

int audio_play(const uint8_t *data, size_t len)
{
    size_t bytes_written;
    return i2s_write(ESP_VISION_AUDIO_I2S_PORT, data, len, &bytes_written, portMAX_DELAY);
}

int audio_record(uint8_t *data, size_t len)
{
    size_t bytes_read;
    return i2s_read(ESP_VISION_AUDIO_I2S_PORT, data, len, &bytes_read, portMAX_DELAY);
}