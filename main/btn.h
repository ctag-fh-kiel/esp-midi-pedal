#pragma once

typedef enum{
    BTN1_CLICK, BTN2_CLICK
} btnEvent_t;

void btnInit();
btnEvent_t btnWaitForEvent();