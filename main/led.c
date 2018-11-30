#include "led.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"

#define BT1_LED_BLUE    13
#define BT1_LED_RED     12
#define BT2_LED_BLUE    15
#define BT2_LED_RED     16

ledc_channel_config_t ledc_channel[4];

void ledInit(){
    ESP_LOGI("LED", "Initializing led");
    int ch;

    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
        .timer_num = LEDC_TIMER_0            // timer index
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel[0] = (ledc_channel_config_t ){
            .channel    = LEDC_CHANNEL_0,
            .duty       = 0,
            .gpio_num   = BT1_LED_RED,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_sel  = LEDC_TIMER_0
    };

    ledc_channel[1] = (ledc_channel_config_t ){
            .channel    = LEDC_CHANNEL_1,
            .duty       = 0,
            .gpio_num   = BT1_LED_BLUE,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_sel  = LEDC_TIMER_0
    };

    ledc_channel[2] = (ledc_channel_config_t ){
            .channel    = LEDC_CHANNEL_2,
            .duty       = 0,
            .gpio_num   = BT2_LED_RED,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_sel  = LEDC_TIMER_0
    };

    ledc_channel[3] = (ledc_channel_config_t ){
            .channel    = LEDC_CHANNEL_3,
            .duty       = 0,
            .gpio_num   = BT2_LED_BLUE,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_sel  = LEDC_TIMER_0
    };

    // Set LED Controller with previously prepared configuration
    for (ch = 0; ch < 4; ch++) {
        ledc_channel_config(&ledc_channel[ch]);
    }

    // Initialize fade service.
    ledc_fade_func_install(0);

    /*
    while (1) {
        ledSet(0, RED);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ledSet(0, BLUE);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ledSet(0, PURPLE);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ledSet(0, BLACK);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ledSet(0, FF_RED);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        ledSet(0, F_PURPLE);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        ledSet(0, FF_BLUE);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        ledSet(0, F_BLACK);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
    }
    */
}

static void ledSetChannel(const int c, const int duty){
    ledc_set_duty(ledc_channel[c].speed_mode, ledc_channel[c].channel, duty);
    ledc_update_duty(ledc_channel[c].speed_mode, ledc_channel[c].channel);
}

static void ledFadeChannel(const int c, const int duty, const int fadeTimeMs){
    ledc_set_fade_with_time(ledc_channel[c].speed_mode, ledc_channel[c].channel, duty, fadeTimeMs);
    ledc_fade_start(ledc_channel[c].speed_mode, ledc_channel[c].channel, LEDC_FADE_NO_WAIT);
}

void ledSet(ledBtn_t id, ledStates_t state){
    switch(state){
        case BLUE:
            ledSetChannel((int)id, 8192);
            ledSetChannel(1 + (int)id, 0);
            break;
        case RED:
            ledSetChannel((int)id, 0);
            ledSetChannel(1 + (int)id, 8192); 
            break;

        case BLACK:
            ledSetChannel((int)id, 8192);
            ledSetChannel(1 + (int)id, 8192);
            break;

        case PURPLE:
            ledSetChannel((int)id, 0);
            ledSetChannel(1 + (int)id, 0);
            break;

        case FF_BLUE:
            ledFadeChannel((int)id, 8192, 2000);
            ledFadeChannel(1 + (int)id, 0, 2000);
            break;

        case FF_RED:
            ledFadeChannel((int)id, 0, 2000);
            ledFadeChannel(1 + (int)id, 8192, 2000);
            break;

        case FF_BLACK:
            ledFadeChannel((int)id, 8192, 2000);
            ledFadeChannel(1 + (int)id, 8192, 2000);
            break;

        case FF_PURPLE:
            ledFadeChannel((int)id, 0, 2000);
            ledFadeChannel(1 + (int)id, 0, 2000);
            break;

        case F_BLUE:
            ledFadeChannel((int)id, 8192, 4000);
            ledFadeChannel(1 + (int)id, 0, 4000);
            break;

        case F_RED:
            ledFadeChannel((int)id, 0, 4000);
            ledFadeChannel(1 + (int)id, 8192, 4000);
            break;

        case F_BLACK:
            ledFadeChannel((int)id, 8192, 4000);
            ledFadeChannel(1 + (int)id, 8192, 4000);
            break;

        case F_PURPLE:
            ledFadeChannel((int)id, 0, 4000);
            ledFadeChannel(1 + (int)id, 0, 4000);
            break;

        default:
            break;

    }
}
