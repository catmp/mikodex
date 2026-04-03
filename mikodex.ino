/* 2026-04-03 
 * catmp | mikodex.ino
 *
 * @brief main entry point for esp32 application
 *
 */
#include "state.h"
#include "hal.h"

static app_state_t state;

void setup() {

  hal_init();
  hal_draw_txt(20, 130, "MikoDex v0.1", RGB565_RED, 8, 1);
  state.selected_tab = 0;
  state.needs_redraw = true;

}

void loop() {

  if (state.needs_redraw) {
    hal_flush();
    state.needs_redraw = false;
  }

}
