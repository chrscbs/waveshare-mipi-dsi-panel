/* Example code for JD9365 10.1" MIPI DSI LCD panel

    This example demonstrates how to use the JD9365 panel driver with a MIPI DSI interface.
    The example initializes the panel and fills the screen with a solid color.

LCD Touch Panel Used: Waveshare's 10.1-DSI-TOUCH-B
Microcontroller Used: ESP32-P4-NANO
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

#include "esp_ldo_regulator.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_lcd_jd9365_10_1.h"
#include <stdlib.h>
#include "esp_heap_caps.h"

/* Logging tag */
static const char *TAG = "JD9365";

/* ===== LCD configuration ===== */
#define EXAMPLE_MIPI_DPI_PX_FORMAT  LCD_COLOR_PIXEL_FORMAT_RGB565
/* Use 16 bpp (RGB565) to reduce memory usage */
#define EXAMPLE_LCD_BIT_PER_PIXEL  16
/* Panel resolution for JD9365 800x1280 */
#define EXAMPLE_LCD_H_RES 800
#define EXAMPLE_LCD_V_RES 1280

/* LCD reset pin
 * Set to -1 if not connected
 * Check Waveshare schematic if unsure
 */
#define EXAMPLE_LCD_IO_RST          -1


static void lcd_fill_color(esp_lcd_panel_handle_t panel, uint16_t color)
{
    const int width = EXAMPLE_LCD_H_RES;
    const int height = EXAMPLE_LCD_V_RES;

    size_t buf_size = width * height * sizeof(uint16_t);

    uint16_t *buf = heap_caps_malloc(buf_size, MALLOC_CAP_SPIRAM);
    assert(buf);

    for (int i = 0; i < width * height; i++) {
        buf[i] = color;
    }

    esp_lcd_panel_draw_bitmap(
        panel,
        0,
        0,
        width,
        height,
        buf
    );

    heap_caps_free(buf);
}


void app_main(void)
{
    ESP_LOGI(TAG, "MIPI DSI PHY Powered on");
    esp_ldo_channel_handle_t ldo_mipi_phy = NULL;
    esp_ldo_channel_config_t ldo_mipi_phy_config = {
        .chan_id = 3,
        .voltage_mv = 2500,
    };
    ESP_ERROR_CHECK(esp_ldo_acquire_channel(&ldo_mipi_phy_config, &ldo_mipi_phy));

    ESP_LOGI(TAG, "Initialize MIPI DSI bus");
    esp_lcd_dsi_bus_handle_t mipi_dsi_bus = NULL;
    esp_lcd_dsi_bus_config_t bus_config = JD9365_PANEL_BUS_DSI_2CH_CONFIG();
    ESP_ERROR_CHECK(esp_lcd_new_dsi_bus(&bus_config, &mipi_dsi_bus));

    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t mipi_dbi_io = NULL;
    esp_lcd_dbi_io_config_t dbi_config = JD9365_PANEL_IO_DBI_CONFIG();
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_dbi(mipi_dsi_bus, &dbi_config, &mipi_dbi_io));

    ESP_LOGI(TAG, "Install JD9365S panel driver");
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_dpi_panel_config_t dpi_config = JD9365_800_1280_PANEL_60HZ_DPI_CONFIG(EXAMPLE_MIPI_DPI_PX_FORMAT);
    jd9365_vendor_config_t vendor_config = {
        .flags = {
            .use_mipi_interface = 1,
        },
        .mipi_config = {
            .lane_num = 2,
            .dsi_bus = mipi_dsi_bus,
            .dpi_config = &dpi_config,
        },
    };
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_LCD_IO_RST,           // Set to -1 if not use
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,     // Implemented by LCD command `36h`
        .bits_per_pixel = EXAMPLE_LCD_BIT_PER_PIXEL,    // Implemented by LCD command `3Ah` (16/18/24)
        .vendor_config = &vendor_config,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_jd9365(mipi_dbi_io, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    // RGB565 colors
    #define LCD_COLOR_RED     0xF800
    #define LCD_COLOR_GREEN   0x07E0
    #define LCD_COLOR_BLUE    0x001F
    #define LCD_COLOR_WHITE   0xFFFF
    #define LCD_COLOR_BLACK   0x0000

    ESP_LOGI(TAG, "Fill screen with blue");
    lcd_fill_color(panel_handle, LCD_COLOR_BLACK);

}
