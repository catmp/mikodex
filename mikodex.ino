/* 2026-04-03 
 * catmp | mikodex.ino
 *
 * @brief main entry point for esp32 application
 *
 */
#include "state.h"
#include "hal.h"
#include "menu.h"
#include "button.h"

static app_state_t state;

void setup() {
  hal_init();
  state.selected_tab = 0;
  state.needs_redraw = true;
}


void loop() {
  buttons_update();

  if (button_pressed(&up_btn)) {
    hal_serial_out("Up Pressed\n");
    state.selected_tab = (state.selected_tab + 1) % POKE_TABS;
    state.needs_redraw = true;
  }
  
  if (button_pressed(&dn_btn)) {
    hal_serial_out("Down Pressed\n");
    state.selected_tab = (state.selected_tab - 1 + POKE_TABS) % POKE_TABS;
    state.needs_redraw = true;
  }

  if (state.needs_redraw) {
    draw_menu(&state);
    hal_flush();
    state.needs_redraw = false;
  }

}
