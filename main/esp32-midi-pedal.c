#include "oled.h"
#include "led.h"
#include "midi.h"
#include "btn.h"
#include "esp_log.h"

void app_main()
{
   oledInit();
   ledInit();
   midiInit();
   btnInit();
   oledDrawLogo();
   ledSet(BTN1_LED, F_BLUE);
   ledSet(BTN2_LED, F_BLUE);
   btnEvent_t ev;
   ledStates_t ledStateBtn1 = BLUE, ledStateBtn2 = BLUE;
   for(;;){
      ev = btnWaitForEvent();
      switch(ev.evType){
         case BTN1_CLICK:
            midiSendNoteOn(11, 60);
            if(ledStateBtn1 == BLUE){
               ledStateBtn1 = RED;
               ledSet(BTN1_LED, RED);
            }else if(ledStateBtn1 == RED){
               ledStateBtn1 = PURPLE;
               ledSet(BTN1_LED, PURPLE);
            }else{
               ledStateBtn1 = BLUE;
               ledSet(BTN1_LED, BLUE);
            }
            break;
         case BTN2_CLICK:
            midiSendNoteOn(11, 64);
            if(ledStateBtn2 == BLUE){
               ledStateBtn2 = RED;
               ledSet(BTN2_LED, RED);
            }else{
               ledStateBtn2 = BLUE;
               ledSet(BTN2_LED, BLUE);
            }
            break;
         default:  
            break;
      }
   }  
}
