/*
 * MCP4921/MCP4922 12 bit DAC Library
 * By MichD
 *
 * GitHub: https://github.com/michd/Arduino-MCP492X
 *
 * The Microchip MCP4921 and MCP4922 are 12 bit digital to analog converters
 * with an SPI interface. The MCP4921 has one DAC output, the MCP4922 has two.
 *
 * This library is built around the Arduino default SPI library and uses the
 * designated SPI pins, in addition to a configurable chip select pin, passed
 * in the constructor.
 * Look up the designated pins for your arduino board, and wire up as follows:
 *
 * | Arduino | MCP492X   | MCP4921 pin | MCP4922 pin |
 * |---------|-----------|-------------|-------------|
 * | SCK     | SCK       | 3           | 4           |
 * | MOSI    | SDI       | 4           | 5           |
 * | your CS | CS        | 2           | 3           |
 * 
 * Where "your CS" is whichever pin you'd like to use as chip select on
 * your arduino board.
 * Modified by SL to use different SPI and added latch function
 */

#include <SPI.h>
#include "MCP492X.h"

MCP492X::MCP492X(uint8_t pinChipSelect) {
  _spi = &SPI;
  _pinChipSelect = pinChipSelect;
  _latchPin = 255;
  _spiSpeed = 4e6;
}

MCP492X::MCP492X(uint8_t pinChipSelect, uint8_t latchPin) { //added by SL
	_spi = &SPI;
	_pinChipSelect = pinChipSelect;
	_latchPin = latchPin;
	_spiSpeed = 4e6;
}


MCP492X::MCP492X(SPIClass* usedSPI, uint32_t spiSpeed, uint8_t pinChipSelect) { //added by SL
	_spi = usedSPI; 
	_pinChipSelect = pinChipSelect;
	_latchPin = 255;
	_spiSpeed = spiSpeed;
}

MCP492X::MCP492X(SPIClass* usedSPI, uint32_t spiSpeed, uint8_t pinChipSelect, uint8_t latchPin) { //added by SL
	_spi = usedSPI;
	_pinChipSelect = pinChipSelect;
	_latchPin = latchPin;
	_spiSpeed = spiSpeed;
}


void MCP492X::begin() {
  ::pinMode(_pinChipSelect, OUTPUT);
  ::digitalWrite(_pinChipSelect, 1);
  if (_latchPin != 255) {
	  pinMode(_latchPin, OUTPUT);
	  digitalWrite(_latchPin, HIGH);
  }
  _spiSettings = SPISettings(_spiSpeed, MSBFIRST, SPI_MODE0);
}

void MCP492X::analogWrite(unsigned int value) {
  analogWrite(0, value);
}

bool MCP492X::latch(void) { //added by SL
	if (_latchPin == 255) {
		return false; 
	}
	digitalWrite(_latchPin, LOW);
	delayMicroseconds(1); //min 100ns
	digitalWrite(_latchPin, HIGH);
	return true;
}

// Only applies to MCP4922
void MCP492X::analogWrite(bool odd, unsigned int value) {
  analogWrite(
    odd, // Pass channel
    0,   // Not buffered,
    1,   // Gain mode 1x (0 = 2x)
    1,   // Don't shut down output
    value);
}

// If you want full control, this method lets you set all config bits
// See MCP492X datasheet page 18 ("5.0 Serial interface") for details
void MCP492X::analogWrite(
  bool odd, bool buffered, bool gain, bool active, unsigned int value) {
  
  byte configBits = odd << 3 | buffered << 2 | gain << 1 | active;

  // Compose the first byte to send to the DAC:
  // the 4 control bits, and the 4 most significant bits of the value
  byte firstByte = configBits << 4 | (value & 0xF00) >> 8;
  // Second byte is the lower 8 bits of the value
  byte secondByte = value & 0xFF;

  _beginTransmission();
  _spi->transfer(firstByte);
  _spi->transfer(secondByte);
  _endTransmission();
}

void MCP492X::_beginTransmission() {
  ::digitalWrite(_pinChipSelect, 0);
  _spi->beginTransaction(_spiSettings);
}

void MCP492X::_endTransmission() {
  _spi->endTransaction();
  ::digitalWrite(_pinChipSelect, 1);
}