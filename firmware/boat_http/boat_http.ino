#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

Servo rudderServo;
Servo motorESC;

const int RUDDER_PIN = 18;
const int ESC_PIN    = 19;

WebServer server(80);

const char* ssid = "ESP32-Boat-Control";
const char* password = "password123";

int currentSpeed = 90;

// --- FAILSAFE VARIABLES ---
unsigned long lastHeartbeat = 0;
const unsigned long FAILSAFE_TIMEOUT = 2000; // 2.0 seconds timeout limit
bool isFailsafeActive = false;

const char HTML_PAGE[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Boat Controller</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 15px; background-color: #f4f4f9; }
    h1 { color: #333; margin-bottom: 5px; }
    #status-box {
      font-weight: bold; font-size: 16px; color: #2c3e50;
      background: #e2e8f0; padding: 10px; border-radius: 8px;
      margin: 10px auto; max-width: 320px;
    }
    .btn {
      display: inline-block; width: 80%; max-width: 300px; margin: 6px auto; padding: 14px;
      font-size: 18px; font-weight: bold; color: white; border: none;
      border-radius: 8px; cursor: pointer; touch-action: manipulation;
    }
    .btn-green  { background-color: #2ecc71; }
    .btn-red    { background-color: #e74c3c; }
    .btn-blue   { background-color: #3498db; }
    .btn-dark   { background-color: #34495e; }
    .btn:active { transform: scale(0.96); opacity: 0.8; }
  </style>
  <script>
    function sendCmd(url, statusText) {
      document.getElementById('status-box').innerText = "Sending: " + statusText + "...";
      document.getElementById('status-box').style.background = "#f1c40f";

      fetch(url)
        .then(response => response.text())
        .then(data => {
          document.getElementById('status-box').innerText = "Active: " + statusText;
          document.getElementById('status-box').style.background = "#2ecc71";
        })
        .catch(error => {
          document.getElementById('status-box').innerText = "DISCONNECTED!";
          document.getElementById('status-box').style.background = "#e74c3c";
        });
    }

    // Ping the ESP32 every 1000ms to keep connection alive
    setInterval(() => {
      fetch('/ping').catch(e => {
        document.getElementById('status-box').innerText = "LOST CONNECTION!";
        document.getElementById('status-box').style.background = "#e74c3c";
      });
    }, 1000);
  </script>
</head>
<body>
  <h1> ESP32 Airboat</h1>
  <div id="status-box">Status: Connected</div>
  
  <h3>Propeller Throttle</h3>
  <button class="btn btn-green" onclick="sendCmd('/throttle?speed=105', 'Forward (Slow)')">FORWARD (Slow)</button><br>
  <button class="btn btn-green" onclick="sendCmd('/throttle?speed=125', 'Forward (Fast)')">FORWARD (Fast)</button><br>
  <button class="btn btn-red"   onclick="sendCmd('/throttle?speed=90',  'MOTOR STOP')"> STOP MOTOR</button><br>
  <button class="btn btn-dark"  onclick="sendCmd('/throttle?speed=75',  'Reverse')">REVERSE</button>

  <h3>Rudder Steering</h3>
  <button class="btn btn-blue" onclick="sendCmd('/steer?angle=45',  'Steer Left')"> LEFT</button><br>
  <button class="btn btn-blue" onclick="sendCmd('/steer?angle=90',  'Steer Center')"> CENTER</button><br>
  <button class="btn btn-blue" onclick="sendCmd('/steer?angle=135', 'Steer Right')"> RIGHT</button>
</body>
</html>
)rawliteral";

void setMotorSpeed(int targetSpeed) {
  targetSpeed = constrain(targetSpeed, 50, 130);
  
  if (targetSpeed > currentSpeed) {
    for (int s = currentSpeed; s <= targetSpeed; s++) {
      motorESC.write(s);
      delay(15);
    }
  } else {
    for (int s = currentSpeed; s >= targetSpeed; s--) {
      motorESC.write(s);
      delay(15);
    }
  }
  currentSpeed = targetSpeed;
}

void handleRoot() {
  lastHeartbeat = millis();
  server.send(200, "text/html", HTML_PAGE);
}

void handlePing() {
  lastHeartbeat = millis(); // Refresh connection timestamp
  isFailsafeActive = false;
  server.send(200, "text/plain", "PONG");
}

void handleThrottle() {
  lastHeartbeat = millis();
  isFailsafeActive = false;
  if (server.hasArg("speed")) {
    int speed = server.arg("speed").toInt();
    setMotorSpeed(speed);
  }
  server.send(200, "text/plain", "OK");
}

void handleSteer() {
  lastHeartbeat = millis();
  isFailsafeActive = false;
  if (server.hasArg("angle")) {
    int angle = server.arg("angle").toInt();
    angle = constrain(angle, 0, 180);
    rudderServo.write(angle);
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  delay(2000);
  Serial.begin(115200);
  delay(15);
  Serial.println("starting to initilialize");


  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  Serial.println("timers allocated");

  rudderServo.setPeriodHertz(50);
  rudderServo.attach(RUDDER_PIN, 500, 2400);
  rudderServo.write(90);

  motorESC.setPeriodHertz(50);
  motorESC.attach(ESC_PIN, 1000, 2000);
  motorESC.write(90);
  delay(1500); 

  WiFi.softAP(ssid, password);

  server.on("/", handleRoot);
  server.on("/ping", handlePing);
  server.on("/throttle", handleThrottle);
  server.on("/steer", handleSteer);
  server.begin();
  Serial.println("webserver booted");

  lastHeartbeat = millis();
}

void loop() {
  server.handleClient();

  // --- FAILSAFE MONITORING ---
  // If more than 2 seconds pass without a packet from the phone, kill the motor!
  if (millis() - lastHeartbeat > FAILSAFE_TIMEOUT) {
    if (!isFailsafeActive && currentSpeed != 90) {
      setMotorSpeed(90); // Hard stop
      isFailsafeActive = true;
      Serial.println("FAILSAFE TRIGGERED: Connection lost! Stopping motor.");
    }
  }
}
