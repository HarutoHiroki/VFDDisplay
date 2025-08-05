#include "VFDDisplay.h"

VFDDisplay::VFDDisplay(int cs, int clk, int din, int numDigits) {
  _cs_pin = cs;
  _clk_pin = clk;
  _din_pin = din;
  _num_digits = numDigits;
}

void VFDDisplay::init() {
  pinMode(_cs_pin, OUTPUT);
  pinMode(_clk_pin, OUTPUT);
  pinMode(_din_pin, OUTPUT);

  // Set number of digits
  digitalWrite(_cs_pin, LOW);
  spiWrite(0xe0);
  delayMicroseconds(5);
  spiWrite(digitsToByte(_num_digits)); // Use converted byte value
  digitalWrite(_cs_pin, HIGH);
  delayMicroseconds(5);

  // Set brightness to max by default
  setBrightness(0xff);
}

void VFDDisplay::spiWrite(unsigned char data) {
  noInterrupts(); // Disable interrupts during critical SPI timing
  for (int i = 0; i < 8; i++) {
    digitalWrite(_clk_pin, LOW);
    delayMicroseconds(1); // Small delay for setup time
    if ((data & 0x01) == 0x01) {
      digitalWrite(_din_pin, HIGH);
    } else {
      digitalWrite(_din_pin, LOW);
    }
    data >>= 1;
    delayMicroseconds(1); // Small delay before clock high
    digitalWrite(_clk_pin, HIGH);
    delayMicroseconds(1); // Hold time
  }
  interrupts(); // Re-enable interrupts
}

void VFDDisplay::sendCommand(unsigned char command) {
  digitalWrite(_cs_pin, LOW);
  spiWrite(command);
  digitalWrite(_cs_pin, HIGH);
  delayMicroseconds(5);
}

void VFDDisplay::show() {
  digitalWrite(_cs_pin, LOW);
  spiWrite(0xe8); // Address register start position
  digitalWrite(_cs_pin, HIGH);
}

void VFDDisplay::writeChar(unsigned char position, unsigned char character) {
  digitalWrite(_cs_pin, LOW);
  spiWrite(0x20 + position);
  spiWrite(character + 0x30);
  digitalWrite(_cs_pin, HIGH);
  show();
}

void VFDDisplay::writeString(unsigned char position, const char* str) {
  digitalWrite(_cs_pin, LOW);
  spiWrite(0x20 + position);
  while (*str) {
    spiWrite(*str++);
  }
  digitalWrite(_cs_pin, HIGH);
  show();
}

void VFDDisplay::setBrightness(unsigned char brightness) {
  digitalWrite(_cs_pin, LOW);
  spiWrite(0xe4);
  delayMicroseconds(5);
  spiWrite(brightness);
  digitalWrite(_cs_pin, HIGH);
  delayMicroseconds(5);
}

void VFDDisplay::clear() {
  // Create a string with spaces equal to the number of digits
  String clearStr = "";
  for (int i = 0; i < _num_digits; i++) {
    clearStr += " ";
  }
  writeString(0, clearStr.c_str());
}

unsigned char VFDDisplay::digitsToByte(int digits) {
  // Convert number of digits to the appropriate byte value
  // Common VFD controllers use: digits - 1 as the byte value
  // For example: 16 digits = 0x0F, 8 digits = 0x07, etc.
  if (digits <= 0 || digits > 16) {
    return 0x0F; // Default to 16 digits if invalid
  }
  return (unsigned char)(digits - 1);
}
