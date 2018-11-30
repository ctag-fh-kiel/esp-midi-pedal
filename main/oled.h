#pragma once

typedef enum{
    STOP, START, DUB, REPLACE
} oledStates_t;

void oledInit();
void oledSet(oledStates_t state);
