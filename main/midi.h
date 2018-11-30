#pragma once
#include "stdint.h"

void midiInit();
void midiSendNoteOn(const uint8_t channel, const uint8_t note);
void midiSendNoteOff(const uint8_t channel, const uint8_t note);
