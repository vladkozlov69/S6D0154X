#ifndef S6D0154X_H_
#define S6D0154X_H_

#include <Arduino.h>
#include <Print.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "glcdfont.c"

#define   BLACK                0x0000                // 黑色：    0,   0,   0 //
#define   BLUE                 0x001F                // 蓝色：    0,   0, 255 //
#define   GREEN                0x07E0                // 绿色：    0, 255,   0 //
#define   CYAN                 0x07FF                // 青色：    0, 255, 255 //
#define   RED                  0xF800                // 红色：  255,   0,   0 //
#define   MAGENTA              0xF81F                // 品红：  255,   0, 255 //
#define   YELLOW               0xFFE0                // 黄色：  255, 255, 0   //
#define   WHITE                0xFFFF                // 白色：  255, 255, 255 //
#define   NAVY                 0x000F                // 深蓝色：  0,   0, 128 //
#define   DGREEN               0x03E0                // 深绿色：  0, 128,   0 //
#define   DCYAN                0x03EF                // 深青色：  0, 128, 128 //
#define   MAROON               0x7800                // 深红色：128,   0,   0 //
#define   PURPLE               0x780F                // 紫色：  128,   0, 128 //
#define   OLIVE                0x7BE0                // 橄榄绿：128, 128,   0 //
#define   LGRAY                0xC618                // 灰白色：192, 192, 192 //
#define   DGRAY                0x7BEF                // 深灰色：128, 128, 128 //

#define TFTWIDTH   240
#define TFTHEIGHT  320

#if defined(ARDUINO_ARCH_ARC32) || defined(ARDUINO_MAXIM)
    #define SPI_DEFAULT_FREQ 16000000
    // Teensy 3.0, 3.1/3.2, 3.5, 3.6
#elif defined(__MK20DX128__) || defined(__MK20DX256__) ||                      \
        defined(__MK64FX512__) || defined(__MK66FX1M0__)
    #define SPI_DEFAULT_FREQ 24000000
#elif defined(__AVR__) || defined(TEENSYDUINO)
    #define SPI_DEFAULT_FREQ 8000000
#elif defined(ESP8266) || defined(ESP32)
    #define SPI_DEFAULT_FREQ 24000000
#elif defined(RASPI)
    #define SPI_DEFAULT_FREQ 24000000
#elif defined(ARDUINO_ARCH_STM32F1)
    #define SPI_DEFAULT_FREQ 24000000
#else
    #define SPI_DEFAULT_FREQ 24000000 ///< Default SPI data clock frequency
#endif


//#define swap(a, b) { int16_t t = a; a = b; b = t; }

class S6D0154X: public Adafruit_GFX
{
private:
    SPISettings * _spiSettings;
    uint8_t _cs_pin;
    uint8_t _reset_pin;
    uint8_t _rotation;
    bool _inTransaction;
    void writeRegister16(uint16_t command, uint16_t data);
    void drawPixelInternal(int16_t x, int16_t y, uint16_t color);
public:
    S6D0154X(uint8_t cs_pin, uint8_t reset_pin);
    void begin(uint32_t freq = SPI_DEFAULT_FREQ);
    void setRotation(uint8_t x);
    void setAddrWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawFastHLine(int16_t x0, int16_t y0, int16_t w, uint16_t color);
    void drawFastVLine(int16_t x0, int16_t y0, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c);
    void fillScreen(uint16_t color);
    void flood(uint16_t color, uint32_t len);
    void SPI_WriteComm(uint16_t CMD);
    void SPI_WriteData(uint16_t tem_data);
    SPISettings * spiSettings() { return _spiSettings; };
    virtual void startWrite(void);
    virtual void endWrite(void);
    uint16_t color565(uint8_t red, uint8_t green, uint8_t blue) 
    {
        return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
    }
};

#endif