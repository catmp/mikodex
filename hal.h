/* 2026-04-03 
 * catmp | hal.h
 *
 * @brief hal header
 *
 */
#ifndef HAL_H
#define HAL_H


// https://github.com/moononournation/Arduino_GFX/blob/master/src/Arduino_GFX.h#L32
#define RGB565(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))

#define RGB565_BLACK RGB565(0, 0, 0)
#define RGB565_RED RGB565(248, 0, 0)
#define RGB565_GREEN RGB565(0, 128, 0)
#define RGB565_BLUE RGB565(0, 0, 248)
#define RGB565_YELLOW RGB565(248, 252, 0)
#define RGB565_BROWN RGB565(168, 44, 40)
#define RGB565_GRAY RGB565(128, 128, 128)
#define RGB565_ORANGE RGB565(248, 164, 0)
#define RGB565_PINK RGB565(248, 192, 200)
#define RGB565_PURPLE RGB565(128, 0, 128)
#define RGB565_WHITE RGB565(255, 255, 255)


#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// lifecycle
void hal_init(void);
void hal_flush(void);
uint32_t hal_get_rng(void);
unsigned long hal_millis(void);

// input
bool hal_button_up(void);
bool hal_button_dn(void);

// drawing
void hal_serial_out(char* text);
void hal_fill_screen(uint16_t color);
void hal_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void hal_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void hal_draw_txt(int16_t x, int16_t y, const char* text, uint16_t color, uint8_t size, bool bold);
void hal_draw_bitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t* pixels);
int16_t hal_height(void);
int16_t hal_width(void);

#ifdef __cplusplus
}
#endif

#endif

