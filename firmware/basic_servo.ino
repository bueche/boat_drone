#include <ESP32Servo.h>

// Create servo object
Servo myServo;

// Pin and parameter settings
const int SERVO_PIN = 18;  // Connect orange signal wire to GPIO 18

void setup() {
  // Start the serial console at 115200 baud
  Serial.begin(115200);
  delay(1000); // Give serial monitor a second to connect
  
  Serial.println("\n--- ESP32 Servo Demo Ready! ---");

  // Allow allocation of all timers for ESP32 PWM channels
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // Standard 50Hz PWM frequency for micro servos
  myServo.setPeriodHertz(50);
  
  // Attach servo with min/max pulse widths in microseconds (standard MG90S defaults)
  myServo.attach(SERVO_PIN, 500, 2400);

  // Move to initial home position (center)
  Serial.println("[SETUP] Moving to home position (90 degrees)...");
  myServo.write(90);
  delay(1000);
}

void loop() {
  // --- MODE 1: Precision Positions ---
  Serial.println("\n[MODE 1] Jumping to exact angles...");
  
  Serial.println(" -> Moving to 0 degrees");
  myServo.write(0);
  delay(1200);

  Serial.println(" -> Moving to 90 degrees (Center)");
  myServo.write(90);
  delay(1200);

  Serial.println(" -> Moving to 180 degrees");
  myServo.write(180);
  delay(1200);

  // --- MODE 2: Smooth Scanning Sweep ---
  Serial.println("\n[MODE 2] Smoothly sweeping back and forth...");
  
  // Sweep from 0 to 180 degrees slowly
  for (int angle = 0; angle <= 180; angle += 2) {
    myServo.write(angle);
    Serial.print("Sweep Angle: ");
    Serial.println(angle);
    delay(20); // Small delay creates smooth motion
  }

  delay(300);

  // Sweep back from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle -= 2) {
    myServo.write(angle);
    Serial.print("Sweep Angle: ");
    Serial.println(angle);
    delay(20);
  }

  // --- MODE 3: Fast Wiggle Dance ---
  Serial.println("\n[MODE 3] Wiggle Time!");
  for (int i = 0; i < 5; i++) {
    Serial.println(" -> Wiggle Right!");
    myServo.write(70);
    delay(150);
    
    Serial.println(" -> Wiggle Left!");
    myServo.write(110);
    delay(150);
  }

  // Return to center before repeating the entire cycle
  Serial.println("\nResetting to center position for 3 seconds...");
  myServo.write(90);
  delay(3000);
}