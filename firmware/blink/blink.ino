#include <Arduino.h>

#define LED_PIN     2    // Controlled via GPIO2 for this kit
#define BRIGHTNESS  50   // Set safe brightness limit (0-255)


void setup() {
  Serial.begin(115200);
  // Initialize the WS2812 LED configuration
 
}

void loop() {
  // Blink Red: neopixelWrite(pin, Red, Green, Blue)
  neopixelWrite(LED_PIN, BRIGHTNESS, 0, 0);
  Serial.println("red on");
  delay(1000);

  // Turn LED Off (All values zero)
  neopixelWrite(LED_PIN, 0, 0, 0);
  Serial.println("red off");
  delay(1000);
}
