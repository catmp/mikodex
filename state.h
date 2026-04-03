/* 2026-04-03 
 * catmp | state.h
 *
 * @brief: app state main source of truth
 *
 */

#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int selected_tab;
    bool needs_redraw;
} app_state_t;

#ifdef __cplusplus
}
#endif

#endif
