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

btnEvent_t btnWaitForEvent(){
    btnEvent_t ev;
    static uint32_t lastTBtn1 = 0, lastTBtn2 = 0;
    
    do{
        xQueueReceive(btn_ev_queue, &ev, portMAX_DELAY);
        
        if(ev.evType == BTN1_CLICK){
            uint32_t deltaT = ev.timeStamp - lastTBtn1;
            lastTBtn1 = ev.timeStamp;
            if(deltaT > 5){ // de-bounce by software --> i.e. accept only events after a certain time delays
                return ev;
            } 
        }

        if(ev.evType == BTN2_CLICK){
            uint32_t deltaT = ev.timeStamp - lastTBtn2;
            lastTBtn2 = ev.timeStamp;
            if(deltaT > 5) { // de-bounce by software --> i.e. accept only events after a certain time delays
                return ev;
            } 
        }
    }while(1);
}



static void IRAM_ATTR gpio_isr_handler1(void* arg){
    btnEvent_t ev;
    ev.timeStamp = xTaskGetTickCountFromISR();
    ev.evType = BTN1_CLICK;
    xQueueSendFromISR(btn_ev_queue, &ev, NULL);
}

static void IRAM_ATTR gpio_isr_handler2(void* arg){
    btnEvent_t ev;
    ev.timeStamp = xTaskGetTickCountFromISR();
    ev.evType = BTN2_CLICK;
    xQueueSendFromISR(btn_ev_queue, &ev, NULL);
}


void btnInit(){
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 1;
    io_conf.pull_down_en = 0;
    gpio_config(&io_conf);

    btn_ev_queue = xQueueCreate(10, sizeof(btnEvent_t));

    //install gpio isr service
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(BTN1_PIN, gpio_isr_handler1, (void*) BTN1_PIN);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(BTN2_PIN, gpio_isr_handler2, (void*) BTN2_PIN);
}