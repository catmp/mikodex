# MikoDex — Portable Pokédex

## Project Summary

MikoDex is an ESP32-based handheld device that displays Pokémon data on a bar-style TFT display. The device connects to the [PokéAPI](https://pokeapi.co/) over Wi-Fi, retrieves species data (stats, moves, evolutions, locations), and renders it through a tabbed menu interface navigated with two physical buttons.

The firmware is written in C with a C++ hardware abstraction layer (HAL) to bridge Arduino/ESP32 library calls. All display rendering, input handling, and state management are implemented without RTOS task splitting — the main loop runs a polling-based update cycle with dirty-flag redraws.

**Current version:** v0.2
**Status:** UI framework and input handling complete. Wi-Fi and API integration not yet implemented — stat values are currently generated with `esp_random()` as placeholder data.

---

## Hardware Components

| Name | Role | Notes |
|------|------|-------|
| Adafruit Qualia ESP32-S3 (TTL RGB-666) | Microcontroller | ESP32-S3 with native RGB LCD peripheral. Includes onboard XCA9554 I2C GPIO expander (addr `0x3F`) for display control and button input. |
| TL032FWV01 TFT Display | 3.2" bar-style RGB-666 display | 320×820 native resolution. Connects to Qualia board via 40-pin ribbon cable. Driven as an RGB parallel panel, not SPI. |
| Two tactile buttons | Navigation input (Up / Down) | Directly connected to XCA9554 expander pins `PCA_BUTTON_UP` and `PCA_BUTTON_DOWN`. Active-low with internal pull-ups enabled. |
| USB-C cable | Power and serial programming | Currently the sole power source. LiPo battery support is planned. |
| Breadboard | Prototyping platform | 3D-printed enclosure planned for final build. |

---

## Software Dependencies

### Installed

| Name | Role | Notes |
|------|------|-------|
| Arduino_GFX_Library | Display driver | Provides `Arduino_ESP32RGBPanel`, `Arduino_RGB_Display`, and `Arduino_XCA9554SWSPI` classes. Handles RGB-666 panel timing, I2C expander SPI bridging, and framebuffer flushing. |
| Arduino ESP32 Board Package | Board support | Must include the Adafruit Qualia ESP32-S3 board definition. Supplies pin macros (`TFT_DE`, `TFT_VSYNC`, `PCA_TFT_RESET`, etc.) and `esp_random()`. |

### Planned (required for Wi-Fi / API features)

| Name | Role | Notes |
|------|------|-------|
| WiFi | Network connectivity | ESP32 Arduino core built-in. Required for connecting to a local network and reaching PokéAPI. |
| HTTPClient | HTTP GET requests | ESP32 Arduino core built-in. Used to issue REST calls to `https://pokeapi.co/api/v2/`. |
| ArduinoJson | JSON parsing | Third-party library. Required to deserialize PokéAPI response payloads into C-accessible structs. |

---

## Architecture

When the device boots, `setup()` calls `hal_init()`, which configures the I2C bus at 400 kHz, initializes the XCA9554 expander, brings up the RGB panel via the `Arduino_GFX_Library` initialization sequence defined in `tl032fwv01_init_operations`, sets display rotation to landscape (820×320 effective), enables the backlight through the expander, and configures the two button pins as active-low inputs with internal pull-ups. A white flash confirms the display is alive.

After initialization, the main loop in `mikodex.ino` runs a three-phase cycle: **poll → update state → conditional redraw**. On each iteration, `buttons_update()` reads the current physical state of both buttons through the HAL's `hal_button_up()` and `hal_button_dn()` functions, which read the expander's digital inputs. The `button_pressed()` function then applies a 50 ms debounce window: it tracks the last unstable reading and its timestamp, and only fires a press event when the reading has been stable for `DEBOUNCE_MS` and transitions from released to pressed. If a press is detected, the `app_state_t` struct's `selected_tab` index is incremented or decremented (wrapping with modular arithmetic across 4 tabs), and the `needs_redraw` flag is set.

When `needs_redraw` is true, `draw_tabs()` clears the menu region, renders the four tab labels (Stats, Moves, Evos, Locations), highlights the active tab with a white border, and dispatches to the active tab's draw function. Currently only `draw_stats()` is implemented — it generates six random stat values (HP, Attack, Defense, Sp.Atk, Sp.Def, Speed), scales each to a 0–255 range, and draws labeled horizontal bars with numeric readouts. After drawing completes, `hal_flush()` pushes the framebuffer to the panel and `needs_redraw` is cleared.

### Planned data flow (Wi-Fi integration)

Once networking is implemented, the data flow will work as follows. On startup (or on a search/navigation action), the firmware will connect to a configured Wi-Fi network using the ESP32's `WiFi` library. It will then issue an HTTP GET request via `HTTPClient` to a PokéAPI endpoint (e.g., `https://pokeapi.co/api/v2/pokemon/{id}`). The JSON response will be parsed with `ArduinoJson` into a local data struct holding the species name, base stats array, sprite URL, move list, evolution chain ID, and encounter locations. The stat bars, move list, evolution tree, and location list will then be populated from this struct rather than from random values. Sprite rendering will require a second HTTP request to fetch the image data from the sprite URL, followed by decoding and blitting the pixel data via `hal_draw_bitmap()`.

---

## Features

### Implemented

| Feature | Description |
|---------|-------------|
| Tab navigation | Four-tab menu (Stats, Moves, Evos, Locations) with wrapping Up/Down button navigation. |
| Stat bar rendering | Six Pokémon base stats rendered as labeled horizontal bars with numeric values. Currently uses random placeholder data. |
| Debounced button input | 50 ms debounce on both buttons via polling (no interrupts). Detects rising-edge press events only. |
| Dirty-flag redraw | Display only redraws when state changes, avoiding unnecessary framebuffer flushes. |
| Hardware abstraction layer | All hardware access (display, buttons, timing, RNG) isolated behind a C-linkage HAL, keeping application logic in pure C. |

### Planned

| Feature | Description |
|---------|-------------|
| Wi-Fi + PokéAPI integration | Fetch real Pokémon data (stats, moves, evolutions, locations) from PokéAPI over HTTPS. |
| Sprite rendering | Download and display Pokémon sprite images via `hal_draw_bitmap()`. |
| Pokémon search / browse | Navigate the National Pokédex by ID or search by name. |
| Party profiles | Save and display a team of up to 6 Pokémon with persistent EV tracking. |
| EV tracking | Track effort values across stats for each party member. |
| Moves tab | Display a Pokémon's learnable moves with level/method info. |
| Evolutions tab | Display the evolution chain for the current Pokémon. |
| Locations tab | Display wild encounter locations for the current Pokémon. |
| LiPo battery power | Battery operation with charging via USB-C. |
| 3D-printed enclosure | Custom case for handheld form factor. |
| ESP-IDF migration | Port from Arduino framework to ESP-IDF for finer hardware control. |

---

## Getting Started

### Prerequisites

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Add the ESP32 board package via Boards Manager. Install the **esp32 by Espressif Systems** package.
3. Select board: **Adafruit Qualia ESP32-S3 RGB666**.
4. Install the **Arduino_GFX_Library** via Library Manager.

### Build and Flash

1. Clone or download this repository.
2. Open `mikodex.ino` in the Arduino IDE.
3. Connect the Qualia board via USB-C.
4. Select the correct port under **Tools → Port**.
5. Click **Upload**. The board will reset automatically after flashing.
6. Open **Tools → Serial Monitor** at 115200 baud to see initialization messages and button press debug output.

### File Structure

```
mikodex/
├── mikodex.ino    # Entry point: setup(), loop(), state transitions
├── hal.h          # HAL function declarations and RGB565 color macros
├── hal.cpp        # HAL implementation (Arduino_GFX, expander, buttons)
├── button.h       # Button struct definition and debounce constants
├── button.c       # Button polling and debounce logic
├── menu.h         # Menu layout constants and draw function declarations
├── menu.c         # Frame, tab, and stat rendering
├── state.h        # app_state_t definition (selected_tab, needs_redraw)
```

---

## Wiring / Pin Map

The Qualia ESP32-S3 board routes most display and I/O pins internally. The TL032FWV01 connects directly via the board's 40-pin ribbon connector — no manual wiring is needed for the display.

### I2C Expander (XCA9554, addr `0x3F`)

| Expander Pin | Function | Direction | Notes |
|--------------|----------|-----------|-------|
| `PCA_TFT_RESET` | Display reset | Output | Directly into `Arduino_XCA9554SWSPI` constructor. |
| `PCA_TFT_CS` | Display chip select | Output | SPI-over-I2C bridge for display init commands. |
| `PCA_TFT_SCK` | Display SPI clock | Output | Software SPI via expander. |
| `PCA_TFT_MOSI` | Display SPI data | Output | Software SPI via expander. |
| `PCA_TFT_BACKLIGHT` | Backlight enable | Output | Set HIGH after init to turn on backlight. |
| `PCA_BUTTON_UP` | Up button | Input (pull-up) | Active-low. Returns `true` when pressed. |
| `PCA_BUTTON_DOWN` | Down button | Input (pull-up) | Active-low. Returns `true` when pressed. |

### RGB Panel Signals (directly on ESP32-S3 GPIO)

| Signal Group | Pins | Notes |
|--------------|------|-------|
| Sync / Clock | `TFT_DE`, `TFT_VSYNC`, `TFT_HSYNC`, `TFT_PCLK` | Active display enable, vertical sync, horizontal sync, pixel clock (16 MHz). |
| Red (5-bit) | `TFT_R1` – `TFT_R5` | RGB-666 format, but only 5 red bits wired. |
| Green (6-bit) | `TFT_G0` – `TFT_G5` | Full 6 green bits. |
| Blue (5-bit) | `TFT_B1` – `TFT_B5` | 5 blue bits. Matches RGB565 encoding used in firmware. |

All pin macros are defined by the Adafruit Qualia ESP32-S3 board variant file in the Arduino ESP32 board package. You do not need to define them manually.

### Display Timing Parameters (from `hal.cpp`)

```
Horizontal: hsync_front_porch=1, hsync_width=150, hsync_back_porch=3, active_width=251
Vertical:   vsync_front_porch=1, vsync_width=100, vsync_back_porch=6, active_height=90
Pixel clock: 16 MHz
Rotation: 1 (landscape, 820×320 effective)
```

---

## Known Limitations

| Limitation | Detail |
|------------|--------|
| No Wi-Fi or API connectivity | All Pokémon data is placeholder. Stats are generated with `esp_random()` on each redraw. |
| Only Stats tab renders content | Moves, Evos, and Locations tabs are declared but their draw functions are stubs or unimplemented. |
| No persistent storage | No NVS, SD card, or SPIFFS usage. Device state resets on every power cycle. |
| No power management | USB-only power. No deep sleep, light sleep, or battery monitoring. |
| Full-screen clear on tab switch | `draw_tabs()` clears the entire menu box on every redraw. This will cause visible flicker when framebuffer double-buffering is not in use. |
| Blocking I2C button reads | `buttons_update()` reads both buttons over I2C on every loop iteration. At 400 kHz I2C this is fast, but it blocks the main loop. |
| No error handling in HAL init | If `gfx->begin()` fails, the device enters an infinite loop with no recovery or user feedback beyond a serial message. |
| `draw_stats` buffer size | `snprintf(buffer, 4, ...)` can truncate 3-digit values that include the newline. Buffer should be at least 5 bytes. |
