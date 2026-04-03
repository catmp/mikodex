/* 2026-04-03 
 * catmp | mikodex.ino
 *
 * @brief main entry point for esp32 application
 *
 */
#include "state.h"
#include "hal.h"
#include "menu.h"

static app_state_t state;

void setup() {

  hal_init();
  state.selected_tab = 0;
  state.needs_redraw = true;

}

void loop() {

  if (state.needs_redraw) {
    draw_menu(&state);
    hal_flush();
    state.needs_redraw = false;
  }

}
