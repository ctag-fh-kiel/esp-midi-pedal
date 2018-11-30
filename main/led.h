#pragma once

typedef enum{
    BLUE, RED, BLACK, PURPLE,
    FF_BLUE, FF_RED, FF_BLACK, FF_PURPLE,
    F_BLUE, F_RED, F_BLACK, F_PURPLE
} ledStates_t;

typedef enum{
    BTN1_LED = 0x0,
    BTN2_LED = 0x2
} ledBtn_t;

void ledInit();
void ledSet(ledBtn_t id, ledStates_t state);

