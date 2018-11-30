#include "btn.h"
#include "stdint.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include "esp_log.h"

#define BTN1_PIN 2
#define BTN2_PIN 26
#define GPIO_INPUT_PIN_SEL  ((1ULL<<BTN1_PIN) | (1ULL<<BTN2_PIN))

static xQueueHandle btn_ev_queue = NULL;

void btn_poll_task(void *arg){
    uint32_t preB1, preB2, lB1, lB2; 
    lB1 = gpio_get_level(BTN1_PIN);
    preB1 = lB1;
    lB2 = gpio_get_level(BTN2_PIN);
    preB2 = lB2;
    btnEvent_t ev;
    for(;;){
        lB1 = gpio_get_level(BTN1_PIN);
        lB2 = gpio_get_level(BTN2_PIN);
        if(lB1 != preB1){
            preB1 = lB1;
            ev = BTN1_CLICK;
            xQueueSend(btn_ev_queue, &ev, portMAX_DELAY);
        }
        if(lB2 != preB2){
            preB2 = lB2;
            ev = BTN2_CLICK;
            xQueueSend(btn_ev_queue, &ev, portMAX_DELAY);
        }
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

btnEvent_t btnWaitForEvent(){
    btnEvent_t ev;
    xQueueReceive(btn_ev_queue, &ev, portMAX_DELAY);
    return ev;
}

void btnInit(){
    gpio_config_t io_conf;
    //io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 1;
    io_conf.pull_down_en = 0;
    gpio_config(&io_conf);

    btn_ev_queue = xQueueCreate(10, sizeof(btnEvent_t));

    xTaskCreate(&btn_poll_task, "ptn_poll_task", 2048, 0, 20, 0);
}