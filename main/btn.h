#pragma once
#include <stdint.h>

#define BTN1_CLICK 1
#define BTN2_CLICK 2

typedef struct{
    uint32_t timeStamp;
    uint32_t evType;
} btnEvent_t;

void btnInit();
btnEvent_t btnWaitForEvent();