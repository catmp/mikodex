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

void draw_menu(const app_state_t *state){
    // draw bg
    hal_fill_screen(RGB565_BLACK);

    // title box
    hal_draw_rect(10, 10, 162, 35, RGB565_WHITE);
    hal_draw_txt(20, 20, "MikoDex v0.2", RGB565_WHITE, 2, 1);

    // gif display box
    hal_draw_rect(10, 55, 255, 255, RGB565_WHITE);

    // menu display box
    hal_draw_rect(275, 55, 535, 255, RGB565_WHITE);

    //menu tabs
    const char* tab_labels[] = {"Stats", "Moves", "Evos", "Locations"};
    for (int i = 0; i < POKE_TABS; i++) {
        int tab_x = 275 + (i * 136);
        if (i == state->selected_tab) {
            hal_fill_rect(tab_x, 10, 126, 35, RGB565_WHITE);
            hal_draw_txt(tab_x + 10, 20, tab_labels[i], RGB565_BLACK, 2, true);
        } else {
            hal_draw_rect(tab_x, 10, 126, 35, RGB565_WHITE);
            hal_draw_txt(tab_x + 10, 20, tab_labels[i], RGB565_WHITE, 2, true);
        }
    }
    
    // navigation
    switch (state->selected_tab) {
        case 0: draw_stats();
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
    int16_t statBarW = 367*(statVal/255.0);
    int16_t statBarY = 65+(i*41);
    int16_t statX = 75+(i*41);
    
    hal_draw_txt(285, statX, statsArr[i], RGB565_WHITE, 2, false);
    hal_draw_rect(375, statBarY, statBarW, 31, RGB565_WHITE);
    hal_draw_txt(752, statX, buffer, RGB565_WHITE, 2, false);
  }

}