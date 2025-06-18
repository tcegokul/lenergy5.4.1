#pragma once

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/ledc.h"

#include "TCA9554PWR.h"
#include "LVGL_Driver.h"

#define SPI_METHOD 1
#define IOEXPANDER_METHOD 0


/********************* LCD *********************/

#define LCD_MOSI 5
#define LCD_SCLK 4
#define LCD_CS  18      // Using EXIO
// The pixel number in horizontal and vertical
#define EXAMPLE_LCD_H_RES              480
#define EXAMPLE_LCD_V_RES              480

#define EXAMPLE_LCD_PIXEL_CLOCK_HZ     (18 * 1000 * 1000)
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL  1
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL
#define EXAMPLE_PIN_NUM_BK_LIGHT       -1
#define EXAMPLE_PIN_NUM_HSYNC          46
#define EXAMPLE_PIN_NUM_VSYNC          3 
#define EXAMPLE_PIN_NUM_DE             7 
#define EXAMPLE_PIN_NUM_PCLK           9 
#define EXAMPLE_PIN_NUM_DATA0          10 // B0
#define EXAMPLE_PIN_NUM_DATA1          11 // B1
#define EXAMPLE_PIN_NUM_DATA2          12 // B2
#define EXAMPLE_PIN_NUM_DATA3          13 // B3
#define EXAMPLE_PIN_NUM_DATA4          14 // B4
#define EXAMPLE_PIN_NUM_DATA5          21 // G0
#define EXAMPLE_PIN_NUM_DATA6          8  // G1
#define EXAMPLE_PIN_NUM_DATA7          16 // G2
#define EXAMPLE_PIN_NUM_DATA8          45 // G3
#define EXAMPLE_PIN_NUM_DATA9          38 // G4
#define EXAMPLE_PIN_NUM_DATA10         39 // G5
#define EXAMPLE_PIN_NUM_DATA11         40 // R0
#define EXAMPLE_PIN_NUM_DATA12         41 // R1
#define EXAMPLE_PIN_NUM_DATA13         42  // R2
#define EXAMPLE_PIN_NUM_DATA14         2 // R3
#define EXAMPLE_PIN_NUM_DATA15         1 // R4
#define EXAMPLE_PIN_NUM_DISP_EN        -1

#if CONFIG_EXAMPLE_DOUBLE_FB
#define EXAMPLE_LCD_NUM_FB             2
#else
#define EXAMPLE_LCD_NUM_FB             1
#endif 


#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_LS_MODE           LEDC_LOW_SPEED_MODE
#define LEDC_HS_CH0_GPIO       EXAMPLE_PIN_NUM_BK_LIGHT
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_TEST_DUTY         (4000)
#define LEDC_ResolutionRatio   LEDC_TIMER_13_BIT
#define LEDC_MAX_Duty          ((1 << LEDC_ResolutionRatio) - 1)
#define Backlight_MAX   100      



extern esp_lcd_panel_handle_t panel_handle;
extern uint8_t LCD_Backlight;


typedef struct{
    char method_select;
    //SPI config_t
    spi_device_handle_t spi_device;
    spi_bus_config_t spi_io_config_t;
    spi_device_interface_config_t st7701s_protocol_config_t;
}ST7701S;

typedef ST7701S * ST7701S_handle;

ST7701S_handle ST7701S_newObject(int SDA, int SCL, int CS, char channel_select, char method_select);//Create new object
void ST7701S_screen_init(ST7701S_handle St7701S_handle, unsigned char type);//Screen initialization
void ST7701S_delObject(ST7701S_handle St7701S_handle);//Delete object
void ST7701S_WriteCommand(ST7701S_handle St7701S_handle, uint8_t cmd);//SPI write instruction
void ST7701S_WriteData(ST7701S_handle St7701S_handle, uint8_t data);//SPI write data
esp_err_t ST7701S_CS_EN(void);//Enables SPI CS
esp_err_t ST7701S_CS_Dis(void);//Disable SPI CS
esp_err_t ST7701S_reset(void);// LCD Reset


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LCD_Init(void);

/********************* BackLight *********************/
void Backlight_Init(void);
void Set_Backlight(uint8_t Light);