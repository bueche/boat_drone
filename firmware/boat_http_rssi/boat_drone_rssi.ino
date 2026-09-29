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
const unsigned long FAILSAFE_TIMEOUT = 1500; // 1.5 seconds connection timeout
bool isFailsafeActive = false;

const char HTML_PAGE[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Boat Controller</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 10px; background-color: #f4f4f9; }
    h1 { color: #333; margin-bottom: 5px; font-size: 24px; }
    
    /* STATUS & RSSI CONTAINER */
    #status-card {
      background: #ffffff; padding: 12px; border-radius: 12px;
      margin: 10px auto; max-width: 320px; box-shadow: 0 4px 6px rgba(0,0,0,0.1);
    }
    #status-box {
      font-weight: bold; font-size: 16px; color: #2c3e50;
      background: #2ecc71; color: white; padding: 10px; border-radius: 8px;
      margin-bottom: 8px; transition: background 0.3s;
    }
    #rssi-container {
      display: flex; align-items: center; justify-content: space-between;
      font-size: 14px; font-weight: bold; color: #555; margin-top: 6px;
    }
    #rssi-bar-outer {
      width: 60%; height: 12px; background: #e0e0e0; border-radius: 6px; overflow: hidden;
    }
    #rssi-bar-inner {
      width: 100%; height: 100%; background: #2ecc71; transition: width 0.4s, background 0.4s;
    }

    /* CONTROLS */
    .btn {
      display: inline-block; width: 85%; max-width: 300px; margin: 5px auto; padding: 14px;
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
      fetch(url)
        .then(response => response.text())
        .then(data => {
          // Handled smoothly via telemetry loop
        })
        .catch(error => {
          triggerDisconnectUI("COMMAND FAILED");
        });
    }

    function triggerDisconnectUI(msg) {
      const statusBox = document.getElementById('status-box');
      const rssiBar = document.getElementById('rssi-bar-inner');
      const rssiText = document.getElementById('rssi-val');

      statusBox.innerText = msg;
      statusBox.style.background = "#e74c3c";
      rssiBar.style.width = "0%";
      rssiText.innerText = "OFFLINE";
      
      // Haptic feedback on phone
      if (navigator.vibrate) navigator.vibrate([200, 100, 200]);
    }

    // Ping the ESP32 every 800ms to fetch live RSSI telemetry
    setInterval(() => {
      fetch('/ping')
        .then(response => response.json())
        .then(data => {
          const rssi = data.rssi;
          const statusBox = document.getElementById('status-box');
          const rssiBar = document.getElementById('rssi-bar-inner');
          const rssiText = document.getElementById('rssi-val');

          rssiText.innerText = rssi + " dBm";

          // Calculate Signal Percentage (-30 dBm = 100%, -90 dBm = 0%)
          let pct = Math.min(100, Math.max(0, (rssi + 90) * 1.66));
          rssiBar.style.width = pct + "%";

          // Evaluate Thresholds & Set UI Colors
          if (rssi >= -65) {
            // OPTIMAL ZONE (Green)
            statusBox.innerText = "Status: Optimal Signal";
            statusBox.style.background = "#2ecc71";
            rssiBar.style.background = "#2ecc71";
          } else if (rssi >= -78) {
            // CAUTION ZONE (Yellow)
            statusBox.innerText = "CAUTION: Signal Weakening";
            statusBox.style.background = "#f1c40f";
            rssiBar.style.background = "#f1c40f";
          } else if (rssi >= -84) {
            // WARNING ZONE (Orange)
            statusBox.innerText = "WARNING: TURN BOAT BACK!";
            statusBox.style.background = "#e67e22";
            rssiBar.style.background = "#e67e22";
            if (navigator.vibrate) navigator.vibrate(100); // Short pulse
          } else {
            // DROPOUT / FAILSAFE (Red)
            statusBox.innerText = "CRITICAL: SIGNAL LOSS!";
            statusBox.style.background = "#e74c3c";
            rssiBar.style.background = "#e74c3c";
            if (navigator.vibrate) navigator.vibrate([150, 50, 150]);
          }
        })
        .catch(e => {
          triggerDisconnectUI("DISCONNECTED!");
        });
    }, 800);
  </script>
</head>
<body>
  <h1>⛵ ESP32 Airboat</h1>
  
  <div id="status-card">
    <div id="status-box">Connecting...</div>
    <div id="rssi-container">
      <span>Signal: <span id="rssi-val">-- dBm</span></span>
      <div id="rssi-bar-outer">
        <div id="rssi-bar-inner"></div>
      </div>
    </div>
  </div>
  
  <h3>Propeller Throttle</h3>
  <button class="btn btn-green" onclick="sendCmd('/throttle?speed=105', 'Forward (Slow)')">FORWARD (Slow)</button><br>
  <button class="btn btn-green" onclick="sendCmd('/throttle?speed=125', 'Forward (Fast)')">FORWARD (Fast)</button><br>
  <button class="btn btn-red"   onclick="sendCmd('/throttle?speed=90',  'MOTOR STOP')">⛔ STOP MOTOR</button><br>
  <button class="btn btn-dark"  onclick="sendCmd('/throttle?speed=75',  'Reverse')">REVERSE</button>

  <h3>Rudder Steering</h3>
  <button class="btn btn-blue" onclick="sendCmd('/steer?angle=45',  'Steer Left')">⬅️ LEFT</button><br>
  <button class="btn btn-blue" onclick="sendCmd('/steer?angle=90',  'Steer Center')">⏹️ CENTER</button><br>
  <button class="btn btn-blue" onclick="sendCmd('/steer?angle=135', 'Steer Right')">➡️ RIGHT</button>
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

  // Read current RSSI of the connected client
  int rssi = WiFi.RSSI();

  // Return formatted JSON string
  String jsonResponse = "{\"rssi\":" + String(rssi) + "}";
  server.send(200, "application/json", jsonResponse);
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
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout detector if needed
  delay(2000);
  Serial.begin(115200);
  delay(15);
  Serial.println("Starting Boat System Initialization...");

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  Serial.println("Timers allocated.");

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
  Serial.println("Web server booted successfully!");

  lastHeartbeat = millis();
}

void loop() {
  server.handleClient();

  // --- HARDWARE FAILSAFE MONITORING ---
  // If more than 1.5 seconds pass without a heartbeat, cut motor power!
  if (millis() - lastHeartbeat > FAILSAFE_TIMEOUT) {
    if (!isFailsafeActive && currentSpeed != 90) {
      setMotorSpeed(90); // Hard stop (neutral throttle)
      isFailsafeActive = true;
      Serial.println("FAILSAFE TRIGGERED: Connection lost! Motor stopped.");
    }
  }
}