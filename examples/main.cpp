#include <Arduino.h>
#include <SPI.h>
#include "S6D0154X.h"

S6D0154X tft(TFT_CS, TFT_RST);

unsigned long testFillScreen();
unsigned long testFastLines();
unsigned long testLines();
unsigned long testRects();
unsigned long testText();
unsigned long testFilledRects();
unsigned long testPixels();
unsigned long testFilledCircles(uint8_t radius, uint16_t color);
unsigned long testCircles(uint8_t radius, uint16_t color);
unsigned long testTriangles();
unsigned long testFilledTriangles();
unsigned long testRoundRects();
unsigned long testFilledRoundRects();

void setup() 
{
    Serial.begin(9600);
    tft.begin();
    tft.setRotation(3);


    delay(10);

    testFillScreen();
    delay(500);
    testFastLines();
    delay(500);
    testLines();
    delay(500);
    testRects();
    delay(500);
    testFilledRects();
    delay(500);

    testFilledCircles(10, MAGENTA);
    delay(500);
    testCircles(10,WHITE);
    delay(500);
    testTriangles();
    delay(500);
    testFilledTriangles();
    delay(500);
    testRoundRects();
    delay(500);
    testFilledRoundRects();
    delay(500);
    testPixels();
    delay(500);
    testText();
    delay(500);
}

void loop() 
{
    for(uint8_t rotation=0; rotation<4; rotation++) 
    {
        tft.setRotation(rotation);
        testText();
        delay(1000);
    }
}

unsigned long testFillScreen() 
{
    unsigned long start = micros();
    tft.fillScreen(BLACK);
    delay(200);
    tft.fillScreen(RED);
    delay(200);
    tft.fillScreen(GREEN);
    delay(200);
    tft.fillScreen(BLUE);
    delay(200);
    tft.fillScreen(BLACK);
    return micros() - start;
}

unsigned long testFastLines() 
{
    unsigned long start;
    int           x, y, w = tft.width(), h = tft.height();

    tft.fillScreen(BLACK);
    start = micros();
    for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, random(0xFFFF));
    for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, random(0xFFFF));

    return micros() - start;
}

unsigned long testLines() 
{
    unsigned long start, t;
    int           x1, y1, x2, y2,
                    w = tft.width(),
                    h = tft.height();

    tft.fillScreen(BLACK);

    x1 = y1 = 0;
    y2    = h - 1;
    start = micros();
    for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));
    x2    = w - 1;
    for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));
    t     = micros() - start; // fillScreen doesn't count against timing

    tft.fillScreen(BLACK);

    x1    = w - 1;
    y1    = 0;
    y2    = h - 1;
    start = micros();
    for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));
    x2    = 0;
    for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));
    t    += micros() - start;

    tft.fillScreen(BLACK);

    x1    = 0;
    y1    = h - 1;
    y2    = 0;
    start = micros();
    for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));
    x2    = w - 1;
    for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));
    t    += micros() - start;

    tft.fillScreen(BLACK);

    x1    = w - 1;
    y1    = h - 1;
    y2    = 0;
    start = micros();
    for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));
    x2    = 0;
    for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));

    return micros() - start;
}

unsigned long testFilledRects() 
{
    unsigned long start;

    tft.fillScreen(BLACK);

    start = micros();

    for (int i = 0; i<20; i++)
    {
        int x = random(tft.width() - 50);
        int y = random(tft.height() - 50);
        tft.fillRect(x, y, 50, 50, random(0xFFFF));
    }

    return micros() - start;
}

unsigned long testRects() 
{
    unsigned long start;

    tft.fillScreen(BLACK);

    start = micros();

    for (int i = 0; i<20; i++)
    {
        int x = random(tft.width() - 50);
        int y = random(tft.height() - 50);
        tft.drawRect(x, y, 50, 50, random(0xFFFF));
    }

    return micros() - start;
}

unsigned long testPixels()
{
    unsigned long start;

    tft.fillScreen(BLACK);

    start = micros();

    tft.startWrite();
    for (int i = 50; i < 150; i++)
    {
        for (int j = 50; j < 150; j++)
        {
            tft.drawPixel(i, j, random(0xFFFF));
        }
    }
    tft.endWrite();
    return micros() - start;
}

unsigned long testText() 
{
    tft.fillScreen(BLACK);
    unsigned long start = micros();
    tft.setCursor(0, 0);
    
    tft.setTextColor(WHITE, BLACK);  
    tft.setTextSize(1);
    tft.println("Hello World!");
    
    tft.setTextColor(YELLOW); 
    tft.setTextSize(2);
    tft.println(1234.56);
    
    tft.setTextColor(RED);    
    tft.setTextSize(3);
    tft.println(0xDEADBEEF, HEX);
    tft.println();

    tft.setTextColor(GREEN);
    tft.setTextSize(5);
    tft.println("Groop");

    tft.setTextSize(2);
    tft.println("I implore thee,");

    tft.setTextSize(1);
    tft.println("my foonting turlingdromes.");
    tft.println("And hooptiously drangle me");
    tft.println("with crinkly bindlewurdles,");
    tft.println("Or I will rend thee");
    tft.println("in the gobberwarts");
    tft.println("with my blurglecruncheon,");
    tft.println("see if I don't!");
    return micros() - start;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) 
{
    unsigned long start;
    int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

    tft.fillScreen(BLACK);
    start = micros();
    for(x=radius; x<w; x+=r2) {
        for(y=radius; y<h; y+=r2) {
        tft.fillCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) 
{
    unsigned long start;
    int           x, y, r2 = radius * 2,
                    w = tft.width()  + radius,
                    h = tft.height() + radius;

    // Screen is not cleared for this one -- this is
    // intentional and does not affect the reported time.
    start = micros();
    for(x=0; x<w; x+=r2) 
    {
        for(y=0; y<h; y+=r2) 
        {
            tft.drawCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

unsigned long testTriangles() 
{
    unsigned long start;
    int           n, i, cx = tft.width()  / 2 - 1,
                        cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    n     = min(cx, cy);
    start = micros();
    for(i=0; i<n; i+=5) 
    {
        tft.drawTriangle(
            cx    , cy - i, // peak
            cx - i, cy + i, // bottom left
            cx + i, cy + i, // bottom right
            tft.color565(i, i, i));
    }

    return micros() - start;
}

unsigned long testFilledTriangles() 
{
    unsigned long start, t = 0;
    int           i, cx = tft.width()  / 2 - 1,
                    cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    start = micros();
    for(i=min(cx,cy); i>10; i-=5) 
    {
        start = micros();
        tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
        tft.color565(0, i*10, i*10));
        t += micros() - start;
        tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
        tft.color565(i*10, i*10, 0));
        yield();
    }

    return t;
}

unsigned long testRoundRects() 
{
    unsigned long start;
    int           w, i, i2,
                    cx = tft.width()  / 2 - 1,
                    cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    w     = min(tft.width(), tft.height());
    start = micros();
    for(i=0; i<w; i+=6) 
    {
        i2 = i / 2;
        tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
    }

    return micros() - start;
}

unsigned long testFilledRoundRects() 
{
    unsigned long start;
    int           i, i2,
                    cx = tft.width()  / 2 - 1,
                    cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    start = micros();
    for(i=min(tft.width(), tft.height()); i>20; i-=6) 
    {
        i2 = i / 2;
        tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
        yield();
    }

    return micros() - start;
}
