/* 2026-04-03 
 * catmp | hal.h
 *
 * @brief hal header
 *
 */
#ifndef HAL_H
#define HAL_H

#define HAL_BLACK   0x0000
#define HAL_WHITE   0xFFFF
#define HAL_RED     0xF800
#define HAL_GREEN   0x07E0
#define HAL_BLUE    0x001F

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// lifecycle
void hal_init(void);
void hal_flush(void);
void hal_set_backlight(bool mode);

// input
bool hal_button_up(void);
bool hal_button_dn(void);

// drawing
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

