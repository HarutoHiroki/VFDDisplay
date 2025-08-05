#ifndef VFDDisplay_h
#define VFDDisplay_h

#include "Arduino.h"

class VFDDisplay {
  public:
    VFDDisplay(int cs, int clk, int din, int numDigits = 16);
    void init();
    void writeChar(unsigned char position, unsigned char character);
    void writeString(unsigned char position, const char* str);
    void setBrightness(unsigned char brightness);
    void clear();
    void show();
    
  private:
    int _cs_pin;
    int _clk_pin;
    int _din_pin;
    int _num_digits;
    void spiWrite(unsigned char data);
    void sendCommand(unsigned char command);
    unsigned char digitsToByte(int digits);
};

#endif
