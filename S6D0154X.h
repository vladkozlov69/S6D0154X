#ifndef S6D0154X_H_
#define S6D0154X_H_

#include <Arduino.h>
#include <Print.h>
#include <SPI.h>

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

#define swap(a, b) { int16_t t = a; a = b; b = t; }

class S6D0154X: public Print
{
private:
    SPISettings * _spiSettings;
    uint8_t _cs_pin;
    uint8_t _reset_pin;
    uint8_t _rotation;
    int16_t _width;
    int16_t _height;
    int16_t _cursor_x, _cursor_y;
    uint16_t _textcolor, _textbgcolor;
    uint8_t _textsize;
    boolean _wrap;
    void writeRegister16(uint16_t command, uint16_t data);
public:
    S6D0154X(uint8_t cs_pin, uint8_t reset_pin);
    void init(void);
    void setRotation(uint8_t x);
    void setAddrWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawFastHLine(int16_t x0, int16_t y0, int16_t w, uint16_t color);
    void drawFastVLine(int16_t x0, int16_t y0, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c);
    void fillScreen(uint16_t color);

    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void setCursor(int16_t x, int16_t y);
    void setTextColor(uint16_t c);
    void setTextColor(uint16_t c, uint16_t bg);
    void setTextSize(uint8_t s);
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
    virtual size_t write(uint8_t);
    void flood(uint16_t color, uint32_t len);
    void SPI_WriteComm(uint16_t CMD);
    void SPI_WriteData(uint16_t tem_data);
    int16_t width() { return _width; };
    int16_t height() { return _height; };
    SPISettings * spiSettings() { return _spiSettings; };
};

#endif