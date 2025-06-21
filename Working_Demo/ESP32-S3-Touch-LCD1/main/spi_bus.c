
#include <string.h>
#include "driver/gpio.h"
#include "i2c_bus.h"
#include "tca9535.h"
#include "esp_log.h"

#include "spi_bus.h"

#define TAG "SPI BUS"

#define SPI_BUS_CS      TCA9535_GPIO_NUM_3
#define SPI_BUS_SCK     TCA9535_GPIO_NUM_2
#define SPI_BUS_MOSI    TCA9535_GPIO_NUM_1

esp_err_t spi_bus_init()
{
    tca9535_set_output_state(SPI_BUS_CS, TCA9535_IO_HIGH);
    return ESP_OK;
}

uint8_t Write_SPI_COM_Byte(uint8_t Byte)
{
    tca9535_set_output_state(SPI_BUS_CS, TCA9535_IO_LOW);
    esp_rom_delay_us(20);
    tca9535_set_output_state(SPI_BUS_MOSI, TCA9535_IO_HIGH);
    esp_rom_delay_us(100);
    tca9535_set_output_state(SPI_BUS_SCK, TCA9535_IO_LOW);
    esp_rom_delay_us(100);
    tca9535_set_output_state(SPI_BUS_SCK, TCA9535_IO_HIGH);

	for(uint8_t i = 0; i < 8; i++) {
        esp_rom_delay_us(100);
        tca9535_set_output_state(SPI_BUS_SCK, TCA9535_IO_LOW);
		
		if(Byte & 0x80) {
            tca9535_set_output_state(SPI_BUS_MOSI, TCA9535_IO_HIGH);
		} else {
            tca9535_set_output_state(SPI_BUS_MOSI, TCA9535_IO_LOW);
		}
		Byte <<= 1;
        esp_rom_delay_us(100);
        tca9535_set_output_state(SPI_BUS_SCK, TCA9535_IO_HIGH);
	}
    esp_rom_delay_us(50);
    tca9535_set_output_state(SPI_BUS_SCK, TCA9535_IO_LOW);
    esp_rom_delay_us(50);
    tca9535_set_output_state(SPI_BUS_CS, TCA9535_IO_HIGH);
	
	return Byte;
}

uint8_t Write_SPI_DATA_Byte(uint8_t Byte)
{
    tca9535_set_output_state(SPI_BUS_CS, TCA9535_IO_LOW);
    esp_rom_delay_us(20);
    tca9535_set_output_state(SPI_BUS_MOSI, TCA9535_IO_LOW);
    esp_rom_delay_us(100);
    tca9535_set_output_state(SPI_BUS_SCK, TCA9535_IO_LOW);
    esp_rom_delay_us(100);
    tca9535_set_output_state(SPI_BUS_SCK, TCA9535_IO_HIGH);

	for(uint8_t i = 0; i < 8; i++) {
        esp_rom_delay_us(100);
        tca9535_set_output_state(SPI_BUS_SCK, TCA9535_IO_LOW);
		
		if(Byte & 0x80) {
            tca9535_set_output_state(SPI_BUS_MOSI, TCA9535_IO_HIGH);
		} else {
            tca9535_set_output_state(SPI_BUS_MOSI, TCA9535_IO_LOW);
		}
		Byte <<= 1;
        esp_rom_delay_us(100);
        tca9535_set_output_state(SPI_BUS_SCK, TCA9535_IO_HIGH);
	}
    esp_rom_delay_us(50);
    tca9535_set_output_state(SPI_BUS_SCK, TCA9535_IO_LOW);
    esp_rom_delay_us(50);
    tca9535_set_output_state(SPI_BUS_CS, TCA9535_IO_HIGH);
	
	return Byte;
}

uint8_t Write_SPI_Byte(uint8_t Byte)
{
    // tca9535_set_output_state(SPI_BUS_CS, TCA9535_IO_LOW);
	for(uint8_t i = 0; i < 8; i++) {
        tca9535_set_output_state(SPI_BUS_SCK, TCA9535_IO_LOW);
        esp_rom_delay_us(50);
		
		if(Byte & 0x80) {
            tca9535_set_output_state(SPI_BUS_MOSI, TCA9535_IO_HIGH);
		} else {
            tca9535_set_output_state(SPI_BUS_MOSI, TCA9535_IO_LOW);
		}
		Byte <<= 1;
        tca9535_set_output_state(SPI_BUS_SCK, TCA9535_IO_HIGH);
        esp_rom_delay_us(50);
	}
    tca9535_set_output_state(SPI_BUS_SCK, TCA9535_IO_HIGH);
    esp_rom_delay_us(20);
    // tca9535_set_output_state(SPI_BUS_CS, TCA9535_IO_HIGH);
    // esp_rom_delay_us(50);
	
	return Byte;
}

esp_err_t spi_bus_write_data(uint8_t addr, uint8_t *data, int datalen)
{
    if (data == NULL) {
        ESP_LOGE(TAG, "Not initialized input data pointer");
        return ESP_FAIL;
    }
    printf("write %02X", addr);
    Write_SPI_Byte(addr);
    for (int i = 0; i < datalen; i++) {
        Write_SPI_Byte(data[i]);
        printf(" %02X", data[i]);
    }
    printf("\n");

    return ESP_OK;
}

