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

#define TITLE_X 10
#define TITLE_Y 10
#define TITLE_W 162
#define TITLE_H 35

#define GIF_BOX_X 10
#define GIF_BOX_Y 55
#define GIF_BOX_H 255
#define GIF_BOX_W 255

#define MENU_BOX_X 275
#define MENU_BOX_Y 55
#define MENU_BOX_H 255
#define MENU_BOX_W 535

#define TAB_W 136

#define STAT_BAR_MAX_W 367
#define STAT_H 41
#define STAT_VAL_X 752

void draw_frame(void);
void draw_tabs(const app_state_t *state);
void draw_pokedex(const app_state_t *state);
void draw_party(void);
void draw_stats(void);
void draw_moves(void);

#ifdef __cplusplus
}
#endif

#endif
