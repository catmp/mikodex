/* 2026-04-03 
 * catmp | hal.cpp
 *
 * @brief hardware abstraction layer to allow C/C++ code interactions
 *
 */
#include <Arduino_GFX_Library.h>
#include "hal.h"

extern "C" {

static Arduino_XCA9554SWSPI *expander = new Arduino_XCA9554SWSPI (
  PCA_TFT_RESET, PCA_TFT_CS, PCA_TFT_SCK, PCA_TFT_MOSI,
  &Wire, 0x3F);

static Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
  TFT_DE, TFT_VSYNC, TFT_HSYNC, TFT_PCLK,
  TFT_R1, TFT_R2, TFT_R3, TFT_R4, TFT_R5,
  TFT_G0, TFT_G1,TFT_G2, TFT_G3, TFT_G4, TFT_G5,
  TFT_B1, TFT_B2, TFT_B3, TFT_B4, TFT_B5,
  1, 150, 3, 251,
  1, 100, 6, 90, 1, 16000000);

static Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
  320, 820, rgbpanel, 0, false,
  expander, GFX_NOT_DEFINED,
  tl032fwv01_init_operations, sizeof(tl032fwv01_init_operations));

void hal_init(void){
    // set I2C clock
    Wire.setClock(1000000);

    // check if gfx object was successfully created
    if(!gfx->begin()) { 
        Serial.println("ERROR: failed to initialize display"); 
        while(1) vTaskDelay(pdMS_TO_TICKS(100)); }
    Serial.println("Display Initialized");
    gfx->setRotation(1);

    // turn on backlight (must use expander IO pins)
    expander->pinMode(PCA_TFT_BACKLIGHT, OUTPUT);
    expander->digitalWrite(PCA_TFT_BACKLIGHT, HIGH);

    // initialize pins
    expander->pinMode(PCA_BUTTON_UP, INPUT_PULLUP);
    expander->pinMode(PCA_BUTTON_DOWN, INPUT_PULLUP);

    // flash screen white
    gfx->fillScreen(HAL_WHITE);
    vTaskDelay(pdMS_TO_TICKS(500));

    // render
    gfx->flush(); 
}
}