#include <Arduino.h>
#include <SPI.h>
#include "S6D0154X.h"

#define RST_PIN A2
#define CS_PIN A1

S6D0154X tft(CS_PIN, RST_PIN);

unsigned long testFillScreen();
unsigned long testFastLines();
unsigned long testLines();
unsigned long testRects();
unsigned long testText();
unsigned long testFilledRects();
unsigned long testPixels();

void setup() 
{
    tft.init();
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
    testPixels();
    delay(500);
    testText();
    delay(500);
}

void loop() 
{

}

unsigned long testFillScreen() 
{
    unsigned long start = micros();
    tft.fillScreen(~BLACK);
    tft.fillScreen(~RED);
    tft.fillScreen(~GREEN);
    tft.fillScreen(~BLUE);
    tft.fillScreen(~BLACK);
    return micros() - start;
}

unsigned long testFastLines() 
{
    unsigned long start;
    int           x, y, w = tft.width(), h = tft.height();

    tft.fillScreen(~BLACK);
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

  tft.fillScreen(~BLACK);

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen(~BLACK);

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));
  t    += micros() - start;

  tft.fillScreen(~BLACK);

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, random(0xFFFF));
  t    += micros() - start;

  tft.fillScreen(~BLACK);

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

    tft.fillScreen(~BLACK);

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

    tft.fillScreen(~BLACK);

    start = micros();

    for (int i = 0; i<20; i++)
    {
        int x = random(tft.width() - 50);
        int y = random(tft.height() - 50);
        tft.drawRect(x, y, x+50, y+50, random(0xFFFF));
    }

    return micros() - start;
}

unsigned long testPixels()
{
    unsigned long start;

    tft.fillScreen(~BLACK);

    start = micros();

    SPI.beginTransaction(*(tft.spiSettings()));
    for (int i = 50; i < 150; i++)
    {
        for (int j = 50; j < 150; j++)
        {
            tft.drawPixel(i, j, random(0xFFFF));
        }
    }
    SPI.endTransaction();
    return micros() - start;
}

unsigned long testText() 
{
    tft.fillScreen(~ BLACK);
    unsigned long start = micros();
    tft.setCursor(0, 0);
    
    tft.setTextColor(~ WHITE, ~ BLACK);  
    tft.setTextSize(1);
    tft.println("Hello World!");
    
    tft.setTextColor(~ YELLOW); 
    tft.setTextSize(2);
    tft.println(1234.56);
    
    tft.setTextColor(~ RED);    
    tft.setTextSize(3);
    tft.println(0xDEADBEEF, HEX);
    tft.println();

    tft.setTextColor(~ GREEN);
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
