#include "oled.h"
#include "esp_err.h"
#include "esp_log.h"
#include "u8g2_esp32_hal.h"

u8g2_t u8g2;

// SDA - GPIO21
#define PIN_SDA 5

// SCL - GPIO22
#define PIN_SCL 4

void oledInit(){
    ESP_LOGI("OLED", "Initializing oled");
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.sda   = PIN_SDA;
	u8g2_esp32_hal.scl  = PIN_SCL;
	u8g2_esp32_hal_init(u8g2_esp32_hal);

	u8g2_Setup_ssd1306_i2c_128x64_noname_f(
	//u8g2_Setup_ssd1306_i2c_128x32_univision_f(
		&u8g2,
		U8G2_R1,
		u8g2_esp32_i2c_byte_cb,
		u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure
	u8x8_SetI2CAddress(&u8g2.u8x8,0x78);

    ESP_LOGI("OLED", "u8g2_InitDisplay");
	u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,

	ESP_LOGI("OLED", "u8g2_SetPowerSave");
	u8g2_SetPowerSave(&u8g2, 0); // wake up display
}

void oledSet(oledStates_t state){
    switch(state){
        case STOP: 
            u8g2_ClearBuffer(&u8g2);
            u8g2_SetFont(&u8g2, u8g2_font_fub30_tr);
            u8g2_DrawStr(&u8g2, 0, 32, "SP");
            u8g2_SendBuffer(&u8g2);
            break;
        case START: 
            u8g2_ClearBuffer(&u8g2);
            u8g2_SetFont(&u8g2, u8g2_font_fub30_tr);
            u8g2_DrawStr(&u8g2, 0, 32, "ST");
            u8g2_SendBuffer(&u8g2);
            break;
        case DUB: 
            u8g2_ClearBuffer(&u8g2);
            u8g2_SetFont(&u8g2, u8g2_font_fub30_tr);
            u8g2_DrawStr(&u8g2, 0, 32, "D");
            u8g2_SendBuffer(&u8g2);
            break;
        case REPLACE: 
            u8g2_ClearBuffer(&u8g2);
            u8g2_SetFont(&u8g2, u8g2_font_fub30_tr);
            u8g2_DrawStr(&u8g2, 0, 32, "R");
            u8g2_SendBuffer(&u8g2);
            break;
        default:
            break;
    }
}