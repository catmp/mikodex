/* 2026-04-03 
 * catmp | menu.c
 *
 * @brief draw menu elements
 *
 */

#include "menu.h"
#include "state.h"
#include "hal.h"
#include <stdio.h>

void draw_frame(void){

    // draw bg
    hal_fill_screen(RGB565_BLACK);

    // title box
    hal_draw_rect(TITLE_X, TITLE_Y, TITLE_W, TITLE_H, RGB565_WHITE);
    hal_draw_txt(TITLE_X + 10, TITLE_Y + 10, "MikoDex v0.2", RGB565_WHITE, 2, 1);

    // gif display box
    hal_draw_rect(GIF_BOX_X, GIF_BOX_Y, GIF_BOX_W, GIF_BOX_H, RGB565_WHITE);

    // menu display box
    hal_draw_rect(MENU_BOX_X, MENU_BOX_Y, MENU_BOX_W, MENU_BOX_H, RGB565_WHITE);

}

void draw_tabs(const app_state_t *state){
    //menu tabs
    const char* tab_labels[] = {"Stats", "Moves", "Evos", "Locations"};

    hal_fill_rect(MENU_BOX_X+5, MENU_BOX_Y+5, MENU_BOX_W-10, MENU_BOX_H-10, RGB565_BLACK); 
    for (int i = 0; i < POKE_TABS; i++) {
        int tab_x = MENU_BOX_X + (i * TAB_W); 
        hal_draw_txt(tab_x + 10, TITLE_Y + 10, tab_labels[i], RGB565_WHITE, 2, true);
        if (i == state->selected_tab) {
            hal_draw_rect(tab_x, 10, 126, 35, RGB565_WHITE);
        } else {
            hal_draw_rect(tab_x, 10, 126, 35, RGB565_BLACK);
        }
    }

    // navigation
    switch (state->selected_tab) {
        case 0: draw_stats(); break;
        case 1: //draw_menu(state);
        // ...
    }

}

void draw_stats(){

  const char* statsArr[] = {"HP","Attack","Defense","Sp.Atk","Sp.Def","Speed"};
  uint32_t seed = 0;
  char buffer[8];

  for(int i = 0; i < (sizeof(statsArr)/sizeof(statsArr[0])); i++){
    seed = hal_get_rng();
    uint8_t statVal = 255*((double)seed/UINT32_MAX);
    int stat = snprintf(buffer, 4, "%u\n", statVal);
    int16_t statBarW = STAT_BAR_MAX_W*(statVal/255.0);
    int16_t statBarY = MENU_BOX_Y+10+(i*STAT_H);
    int16_t statY = MENU_BOX_Y+20+(i*STAT_H);

    hal_draw_txt(MENU_BOX_X+10, statY, statsArr[i], RGB565_WHITE, 2, false);
    hal_draw_rect(MENU_BOX_X+100, statBarY, statBarW, 31, RGB565_WHITE);
    hal_draw_txt(STAT_VAL_X, statY, buffer, RGB565_WHITE, 2, false);
  }

}
