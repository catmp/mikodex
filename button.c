/* 2026-04-03 
 * catmp | button.c
 *
 * @brief  
 *
 */

#include "button.h"
#include "hal.h"
#include <stdbool.h>

struct button up_btn = {false, false, false, 0};
struct button dn_btn = {false, false, false, 0};

void buttons_update(void){   
    up_btn.state = hal_button_up();
    dn_btn.state = hal_button_dn();

}

bool button_pressed(struct button *button){
    bool currentState = button->state;
    bool event_triggered = false;

    if(currentState != button->lastUnstableState){
        button->start = hal_millis();
        button->lastUnstableState = currentState;
    }
    if ((hal_millis() - button->start) >= DEBOUNCE_MS){
        if(currentState == true && button->lastStableState == false){
            event_triggered = true;
        }
        button->lastStableState = currentState;
    }
    return event_triggered;
}

