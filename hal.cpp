/* 2026-04-03 
 * catmp | hal.cpp
 *
 * @brief hardware abstraction layer to allow C/C++ code interactions
 *
 */
#include <Arduino_GFX_Library.h>
#include "hal.h"

// initialize I2C expander object
static Arduino_XCA9554SWSPI *expander = new Arduino_XCA9554SWSPI (
  PCA_TFT_RESET, PCA_TFT_CS, PCA_TFT_SCK, PCA_TFT_MOSI,
  &Wire, 0x3F);

// initialize display pins
static Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
  TFT_DE, TFT_VSYNC, TFT_HSYNC, TFT_PCLK,
  TFT_R1, TFT_R2, TFT_R3, TFT_R4, TFT_R5,
  TFT_G0, TFT_G1,TFT_G2, TFT_G3, TFT_G4, TFT_G5,
  TFT_B1, TFT_B2, TFT_B3, TFT_B4, TFT_B5,
  1, 150, 3, 251,
  1, 100, 6, 90, 1, 16000000);

// initialize tl032fwv01 panel
static Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
  320, 820, rgbpanel, 0, false,
  expander, GFX_NOT_DEFINED,
  tl032fwv01_init_operations, sizeof(tl032fwv01_init_operations));

extern "C" {

  // initialization sequence
  void hal_init(void){
      // set I2C clock
      Wire.setClock(1000000);

      // check if gfx object was successfully created
      if(!gfx->begin()) { 
          Serial.println("ERROR: failed to initialize display"); 
          while(1) vTaskDelay(pdMS_TO_TICKS(100)); }
      gfx->setRotation(1);
      Serial.println("Display Initialized");
      Serial.printf("Height: %" PRIu16 ", Width: %" PRIu16 "\n", hal_height(), hal_width());
      

      // turn on backlight (must use expander IO pins)
      expander->pinMode(PCA_TFT_BACKLIGHT, OUTPUT);
      expander->digitalWrite(PCA_TFT_BACKLIGHT, HIGH);

      // initialize pins
      expander->pinMode(PCA_BUTTON_UP, INPUT_PULLUP);
      expander->pinMode(PCA_BUTTON_DOWN, INPUT_PULLUP);

      // flash screen white
      gfx->fillScreen(RGB565_WHITE);
      vTaskDelay(pdMS_TO_TICKS(500));
  }

  // display buffer flush
  void hal_flush(void) {
      gfx->flush();
  }

  // random number generator
  uint32_t hal_get_rng(void){
    return esp_random();
  }

  // button states
  bool hal_button_up(void){
    return expander->digitalRead(PCA_BUTTON_UP) == LOW;
  }
  bool hal_button_dn(void){
    return expander->digitalRead(PCA_BUTTON_DOWN) == LOW;
  }

  // drawing functions
  void hal_serial_out(char* text){
    Serial.println(text);
  }

  void hal_fill_screen(uint16_t color){
    gfx->fillScreen(color);
  }
  void hal_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
    gfx->fillRect(x, y, w, h, color);
  }
  void hal_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
    gfx->drawRect(x, y, w, h, color);
  }
  void hal_draw_txt(int16_t x, int16_t y, const char* text, uint16_t color, uint8_t size, bool bold){
    gfx->setCursor(x, y);
    gfx->setTextColor(color);
    gfx->setTextSize(size);
    gfx->println(text);

    if(bold){
      gfx->setCursor(x+1, y);
      gfx->println(text);
    }
    if(color == RGB565_BLACK){
      gfx->setCursor(x+2, y);
      gfx->println(text);
      gfx->setCursor(x, y+1);
      gfx->println(text);
    }
  }
  void hal_draw_bitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t* pixels){
    gfx->draw16bitRGBBitmap(x, y, (uint16_t*)pixels, w, h);
  }
  int16_t hal_height(void){
    return gfx->height();
  }
  int16_t hal_width(void){
    return gfx->width();
  }
}