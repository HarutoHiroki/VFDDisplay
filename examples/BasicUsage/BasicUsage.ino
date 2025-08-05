#include <VFDDisplay.h>

// Define pins
#define VFD_CS  9
#define VFD_CLK 11
#define VFD_DIN 13

// Create VFD display object with 16 digits
VFDDisplay vfd(VFD_CS, VFD_CLK, VFD_DIN, 16);

// For 8 digits, you would use:
// VFDDisplay vfd(VFD_CS, VFD_CLK, VFD_DIN, 8);

void setup() {
  // Initialize the display
  vfd.init();
  
  // Clear the display
  vfd.clear();
  
  // Display a welcome message
  vfd.writeString(0, "VFD Library Test");
}

void loop() {
  // Example usage
  vfd.writeString(0, "Hello World!    ");
  delay(2000);
  
  vfd.writeString(0, "1234567890ABCDEF");
  delay(2000);
  
  // Test brightness control
  for (int brightness = 255; brightness >= 50; brightness -= 50) {
    vfd.setBrightness(brightness);
    vfd.writeString(0, "Bright Test     ");
    delay(1000);
  }
  
  // Reset to full brightness
  vfd.setBrightness(255);
}
