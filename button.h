/* 2026-04-03 
 * catmp | button.h
 *
 * @brief: 
 *
 */
#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include <stdbool.h>
#include "hal.h"
#include "state.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEBOUNCE_MS 50

struct button {
    bool state;
    int lastStableState;
    int lastUnstableState;
    int64_t start;
};

extern struct button up_btn;
extern struct button dn_btn;

void buttons_update(void);
bool button_pressed(struct button *button);


#ifdef __cplusplus
}
#endif

#endif

