/*
 * SPDX-FileCopyrightText: 2026 ESP-Vision Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "boardconfig.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "driver/spi_master.h"
#include "esp_log.h"

static const char *TAG = "display";

static esp_lcd_panel_handle_t panel_handle = NULL;

int display_init(void)
{
    ESP_LOGI(TAG, "Initializing SPI LCD display");

    spi_bus_config_t buscfg = {
        .mosi_io_num = ESP_VISION_LCD_PIN_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = ESP_VISION_LCD_PIN_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = ESP_VISION_LCD_WIDTH * ESP_VISION_LCD_HEIGHT * 2,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(ESP_VISION_LCD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));

    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = ESP_VISION_LCD_PIN_DC,
        .cs_gpio_num = ESP_VISION_LCD_PIN_CS,
        .pclk_hz = ESP_VISION_LCD_PIXEL_CLOCK_HZ,
        .trans_queue_depth = ESP_VISION_LCD_TRANS_QUEUE_DEPTH,
        .on_color_trans_done = NULL,
        .lcd_cmd_bits = ESP_VISION_LCD_CMD_BITS,
        .lcd_param_bits = ESP_VISION_LCD_PARAM_BITS,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)ESP_VISION_LCD_SPI_HOST, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = ESP_VISION_LCD_PIN_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = ESP_VISION_LCD_BPP,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, false));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    ESP_LOGI(TAG, "Display initialized: %dx%d", ESP_VISION_LCD_WIDTH, ESP_VISION_LCD_HEIGHT);
    return 0;
}

esp_lcd_panel_handle_t display_get_panel(void)
{
    return panel_handle;
}