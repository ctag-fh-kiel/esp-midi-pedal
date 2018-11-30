#include "midi.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"

#define PIN_MIDI_OUT 25

void midiInit(){
    ESP_LOGI("MIDI", "Initializing Midi");
        uart_config_t uart_config = {
        .baud_rate = 31250,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, PIN_MIDI_OUT, -1, -1, -1);
    uart_driver_install(UART_NUM_1, 256, 256, 0, 0, 0);
}

void midiSendNoteOn(const uint8_t channel, const uint8_t note){
    uint8_t data[3];
    data[0] = 0x90 | (channel & 0x0F);
    data[1] = note;
    data[2] = 0x7F;
    ESP_LOGI("MIDI", "Sending %02x %02x %02x", data[0], data[1], data[2]);
    uart_write_bytes(UART_NUM_1, (const char *) &data, 3);
}

void midiSendNoteOff(const uint8_t channel, const uint8_t note){
    uint8_t data[3];
    data[0] = 0x80 | (channel & 0x0F);
    data[1] = note;
    data[2] = 0x7F;
    uart_write_bytes(UART_NUM_1, (const char *) &data, 3);
}