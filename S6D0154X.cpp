#include "S6D0154X.h"


S6D0154X::S6D0154X(uint8_t cs_pin, uint8_t reset_pin) : Adafruit_GFX(TFTWIDTH, TFTHEIGHT)
{
    this->_cs_pin = cs_pin;
	this->_reset_pin = reset_pin;
	_rotation  = 0;
  	// _cursor_y  = _cursor_x    = 0;
  	// _textsize  = 1;
  	// _textcolor = _textbgcolor = 0xFFFF;
  	// _wrap      = true;
	_inTransaction = false;
}

void S6D0154X::begin(uint32_t freq)
{
	_spiSettings = new SPISettings(freq, MSBFIRST, SPI_MODE0);
	SPI.begin();

    pinMode(_cs_pin, OUTPUT);
    digitalWrite(_cs_pin, HIGH);
	pinMode(_reset_pin, OUTPUT);
    digitalWrite(_reset_pin, HIGH);
	delay(5);
	digitalWrite(_reset_pin, LOW);
    delay(5);
    digitalWrite(_reset_pin, HIGH);

	startWrite();
	writeRegister16(0x11, 0x001A);
	writeRegister16(0x12, 0x3121);
	writeRegister16(0x13, 0x006C);
	writeRegister16(0x14, 0x4249);

	writeRegister16(0x10, 0x0800);
	delay(10);
	writeRegister16(0x11, 0x011A);
	delay(10);
	writeRegister16(0x11, 0x031A);
	delay(10);
	writeRegister16(0x11, 0x071A);
	delay(10);
	writeRegister16(0x11, 0x0F1A);
	delay(20);
	writeRegister16(0x11, 0x0F3A);
	delay(30);

	writeRegister16(0x01, 0x0128);
	writeRegister16(0x02, 0x0100);
	writeRegister16(0x03, 0x1030);
	writeRegister16(0x07, 0x1012);
	writeRegister16(0x08, 0x0303);
	writeRegister16(0x0B, 0x1100);
	writeRegister16(0x0C, 0x0000);
	writeRegister16(0x0F, 0x1801);
	writeRegister16(0x15, 0x0020);

  	writeRegister16(0x0050, 0x0101); 
	writeRegister16(0x0051, 0x0903); 
	writeRegister16(0x0052, 0x0e0e); 
	writeRegister16(0x0053, 0x0001); 
	writeRegister16(0x0054, 0x0200); 
	writeRegister16(0x0055, 0x0809); 
	writeRegister16(0x0056, 0x0e0e); 
	writeRegister16(0x0057, 0x0100); 
	writeRegister16(0x0058, 0x0606); 
	writeRegister16(0x0059, 0x0100);
	writeRegister16(0x000F, 0x1e01);
	
	writeRegister16(0x07,0x0012);
	delay(40);

	writeRegister16(0x07,0x0013);/*  GRAM Address Set */
	writeRegister16(0x07,0x0017);/*  Display Control  DISPLAY ON */
   	endWrite();

   	setRotation(0);
}

void S6D0154X::setRotation(uint8_t x)
{
	_rotation = x;
  	digitalWrite(_cs_pin, LOW);
 
    uint16_t t;
    switch(_rotation) 
	{
		case 1 : 
			t = 0x1028; 
			_width = TFTHEIGHT;
			_height = TFTWIDTH;
			break;
		case 2 : 
			t = 0x1000; 
			_width = TFTWIDTH;
			_height = TFTHEIGHT;
			break;
		case 3 : 
			t = 0x1018; 
			_width = TFTHEIGHT;
			_height = TFTWIDTH;
			break;
		default: 
			t = 0x1030; 
			break;
    }
	
	startWrite();
	writeRegister16(0x0003, t); // MADCTL
	endWrite();
	setAddrWindow(0, 0, _width - 1, _height - 1);
}

void S6D0154X::setAddrWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  	digitalWrite(_cs_pin, LOW);
    // Values passed are in current (possibly rotated) coordinate
    // system.  932X requires hardware-native coords regardless of
    // MADCTL, so rotate inputs as needed.  The address counter is
    // set to the top-left corner -- although fill operations can be
    // done in any direction, the current screen rotation is applied
    // because some users find it disconcerting when a fill does not
    // occur top-to-bottom.
    int x, y, t;
    switch(_rotation) {
     default:
      x  = x1;
      y  = y1;
      break;
     case 1:
      t  = y1;
      y1 = x1;
      x1 = TFTWIDTH  - 1 - y2;
      y2 = x2;
      x2 = TFTWIDTH  - 1 - t;
      x  = x2;
      y  = y1;
      break;
     case 2:
      t  = x1;
      x1 = TFTWIDTH  - 1 - x2;
      x2 = TFTWIDTH  - 1 - t;
      t  = y1;
      y1 = TFTHEIGHT - 1 - y2;
      y2 = TFTHEIGHT - 1 - t;
      x  = x2;
      y  = y2;
      break;
     case 3:
      t  = x1;
      x1 = y1;
      y1 = TFTHEIGHT - 1 - x2;
      x2 = y2;
      y2 = TFTHEIGHT - 1 - t;
      x  = x1;
      y  = y2;
      break;
    }
	
	startWrite();
	writeRegister16(0x37, x1); //HorizontalStartAddress
	writeRegister16(0x36, x2); //HorizontalEndAddress
	writeRegister16(0x39, y1); //VerticalStartAddress
	writeRegister16(0x38, y2); //VertocalEndAddress
	writeRegister16(0x20, x); //GRAM Address Set
	writeRegister16(0x21, y);
	writeRegister16(0x22, 0);
	endWrite();
  
	digitalWrite(_cs_pin, HIGH);
}

void S6D0154X::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	drawPixelInternal(x, y, color);
}

void S6D0154X::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  int16_t x2;

  // Initial off-screen clipping
  if((w <= 0     ) ||
     (y      <  0     ) || ( y                  >= _height) ||
     (x      >= _width) || ((x2 = (x+w-1)) <  0      )) return;

  if(x < 0) {        // Clip left
    w += x;
    x       = 0;
  }
  if(x2 >= _width) { // Clip right
    x2      = _width - 1;
    w  = x2 - x + 1;
  }

  setAddrWindow(x, y, x2, y);
  flood(color, w);
}

void S6D0154X::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	int16_t y2;

  // Initial off-screen clipping
  if((h <= 0      ) ||
     (x      <  0      ) || ( x                  >= _width) ||
     (y      >= _height) || ((y2 = (y+h-1)) <  0     )) return;
  if(y < 0) {         // Clip top
    h += y;
    y       = 0;
  }
  if(y2 >= _height) { // Clip bottom
    y2      = _height - 1;
    h  = y2 - y + 1;
  }

  setAddrWindow(x, y, x, y2);
  flood(color, h);
}

void S6D0154X::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c)
{
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	flood(c, (long)w * (long)h);
}

void S6D0154X::fillScreen(uint16_t color)
{
	setAddrWindow(0, 0, _width - 1, _height - 1);
	flood(color, (long)_width * (long)_height);
}

void S6D0154X::flood(uint16_t color, uint32_t len)
{
	uint16_t internalColor = ~color;
	startWrite();
	SPI_WriteComm(0x0022);
    digitalWrite(_cs_pin, LOW);
    SPI.transfer(0x72);
    for (uint32_t i = 0; i < len; i++)
    {
        SPI.transfer16(internalColor);
    }
    digitalWrite(_cs_pin, HIGH);
	endWrite();
}


void S6D0154X::writeRegister16(uint16_t command,uint16_t data)
{
	SPI_WriteComm(command);
	SPI_WriteData(data);
}

void S6D0154X::SPI_WriteComm(uint16_t command)
{			
	digitalWrite(_cs_pin, LOW);
	SPI.transfer(0x70);
	SPI.transfer16(command);
	digitalWrite(_cs_pin, HIGH);
}

void S6D0154X::SPI_WriteData(uint16_t data)
{			
	digitalWrite(_cs_pin, LOW);
	SPI.transfer(0x72);
	SPI.transfer16(data);
	digitalWrite(_cs_pin, HIGH);
}

void S6D0154X::startWrite() 
{
	if (!_inTransaction)
	{
		SPI.beginTransaction(*_spiSettings);
		_inTransaction = true;
	}
}

void S6D0154X::endWrite() 
{
	if (_inTransaction)
	{
		SPI.endTransaction();
		_inTransaction = false;
	}
}

void S6D0154X::drawPixelInternal(int16_t x, int16_t y, uint16_t color)
{
	// Clip
  	if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;

    int16_t t;
	switch(_rotation) 
	{
		case 1:
			t = x;
			x = TFTWIDTH  - 1 - y;
			y = t;
			break;
		case 2:
			x = TFTWIDTH  - 1 - x;
			y = TFTHEIGHT - 1 - y;
			break;
		case 3:
			t = x;
			x = y;
			y = TFTHEIGHT - 1 - t;
		break;
	}
	
    writeRegister16(0x0020, x);
    writeRegister16(0x0021, y);
    writeRegister16(0x0022, ~color);
}

