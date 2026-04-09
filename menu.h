/* 2026-04-03 
 * catmp | menu.h
 *
 * @brief menu header
 *
 */
#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <stdbool.h>
#include "hal.h"
#include "state.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAIN_TABS 2
#define POKE_TABS 4

void draw_main_menu(void);
void draw_menu(const app_state_t *state);
void draw_pokedex(const app_state_t *state);
void draw_party(void);
void draw_stats(void);
void draw_moves(void);

#ifdef __cplusplus
}
#endif

#endif
