#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "esp_wifi.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// Hardware Objects
Servo rudderServo;
Servo motorESC;

const int RUDDER_PIN = 18;
const int ESC_PIN    = 19;

WebServer server(80);

const char* ssid = "ESP32-Boat-Control";
const char* password = "password123";

// System State Variables
int currentSpeed = 90;
int currentAngle = 90;
String currentStatusText = "MOTOR STOP / CENTER";

// Failsafe Mode Configuration: 0 = STOP MOTOR, 1 = CIRCLE IN PLACE
int failsafeMode = 0; 

unsigned long lastHeartbeat = 0;
const unsigned long FAILSAFE_TIMEOUT = 1500; // 1.5 seconds connection timeout
bool isFailsafeActive = false;

// Circle Failsafe Settings
const int FAILSAFE_THROTTLE = 100; // Gentle forward crawl
const int FAILSAFE_STEER    = 135; // Hard Right turn

const char HTML_PAGE[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Boat Controller</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 10px; background-color: #f4f4f9; }
    h1 { color: #333; margin-bottom: 5px; font-size: 22px; }
    
    /* STATUS & RSSI CONTAINER */
    #status-card {
      background: #ffffff; padding: 12px; border-radius: 12px;
      margin: 10px auto; max-width: 320px; box-shadow: 0 4px 6px rgba(0,0,0,0.1);
    }
    #status-box {
      font-weight: bold; font-size: 15px; color: white;
      background: #2ecc71; padding: 10px; border-radius: 8px;
      margin-bottom: 8px; transition: background 0.3s;
    }
    #active-cmd {
      font-size: 13px; font-weight: bold; color: #34495e; margin-bottom: 8px;
    }
    #rssi-container {
      display: flex; align-items: center; justify-content: space-between;
      font-size: 13px; font-weight: bold; color: #555; margin-top: 6px;
    }
    #rssi-bar-outer {
      width: 55%; height: 12px; background: #e0e0e0; border-radius: 6px; overflow: hidden;
    }
    #rssi-bar-inner {
      width: 100%; height: 100%; background: #2ecc71; transition: width 0.4s, background 0.4s;
    }

    /* FAILSAFE TOGGLE BAR */
    #failsafe-bar {
      margin: 10px auto; max-width: 320px;
    }
    .btn-toggle {
      width: 100%; padding: 10px; font-size: 14px; font-weight: bold;
      color: white; border: none; border-radius: 8px; cursor: pointer;
    }
    .mode-stop { background-color: #e67e22; }
    .mode-circle { background-color: #9b59b6; }

    /* CONTROLS */
    .btn {
      display: inline-block; width: 85%; max-width: 300px; margin: 4px auto; padding: 12px;
      font-size: 16px; font-weight: bold; color: white; border: none;
      border-radius: 8px; cursor: pointer; touch-action: manipulation;
    }
    .btn-green  { background-color: #2ecc71; }
    .btn-red    { background-color: #e74c3c; }
    .btn-blue   { background-color: #3498db; }
    .btn-dark   { background-color: #34495e; }
    .btn:active { transform: scale(0.96); opacity: 0.8; }
  </style>
  <script>
    let currentFailsafeMode = 0; // 0 = STOP, 1 = CIRCLE

    function sendCmd(url) {
      fetch(url)
        .then(response => response.text())
        .catch(error => {
          triggerDisconnectUI("COMMAND FAILED");
        });
    }

    function toggleFailsafeMode() {
      let newMode = currentFailsafeMode === 0 ? 1 : 0;
      fetch('/setFailsafeMode?mode=' + newMode)
        .then(response => response.text())
        .then(data => {
          currentFailsafeMode = newMode;
          updateFailsafeButtonUI();
        });
    }

    function updateFailsafeButtonUI() {
      const btn = document.getElementById('toggle-btn');
      if (currentFailsafeMode === 0) {
        btn.innerText = "Failsafe: STOP MOTOR (Bench Mode)";
        btn.className = "btn-toggle mode-stop";
      } else {
        btn.innerText = "Failsafe: CIRCLE IN PLACE (Water Mode)";
        btn.className = "btn-toggle mode-circle";
      }
    }

    function triggerDisconnectUI(msg) {
      const statusBox = document.getElementById('status-box');
      const activeCmd = document.getElementById('active-cmd');
      const rssiBar = document.getElementById('rssi-bar-inner');
      const rssiText = document.getElementById('rssi-val');

      statusBox.innerText = "DISCONNECTED!";
      statusBox.style.background = "#e74c3c";
      activeCmd.innerText = msg;
      rssiBar.style.width = "0%";
      rssiText.innerText = "OFFLINE";
      
      if (navigator.vibrate) navigator.vibrate([200, 100, 200]);
    }

    // Main telemetry loop: runs every 800ms
    setInterval(() => {
      fetch('/ping')
        .then(response => response.json())
        .then(data => {
          const rssi = data.rssi;
          const cmd = data.cmd;
          const statusBox = document.getElementById('status-box');
          const activeCmd = document.getElementById('active-cmd');
          const rssiBar = document.getElementById('rssi-bar-inner');
          const rssiText = document.getElementById('rssi-val');

          activeCmd.innerText = "Active: " + cmd;
          rssiText.innerText = rssi + " dBm";

          // Signal Percentage calculation (-30 dBm = 100%, -90 dBm = 0%)
          let pct = Math.min(100, Math.max(0, (rssi + 90) * 1.66));
          rssiBar.style.width = pct + "%";

          if (rssi >= -65) {
            statusBox.innerText = "Status: Optimal Signal";
            statusBox.style.background = "#2ecc71";
            rssiBar.style.background = "#2ecc71";
          } else if (rssi >= -78) {
            statusBox.innerText = "CAUTION: Signal Weakening";
            statusBox.style.background = "#f1c40f";
            rssiBar.style.background = "#f1c40f";
          } else if (rssi >= -84) {
            statusBox.innerText = "WARNING: TURN BOAT BACK!";
            statusBox.style.background = "#e67e22";
            rssiBar.style.background = "#e67e22";
            if (navigator.vibrate) navigator.vibrate(100);
          } else {
            statusBox.innerText = "FAILSAFE ACTIVE!";
            statusBox.style.background = "#e74c3c";
            rssiBar.style.background = "#e74c3c";
            if (navigator.vibrate) navigator.vibrate([150, 50, 150]);
          }
        })
        .catch(e => {
          triggerDisconnectUI("FAILSAFE ENGAGED (Signal Lost)");
        });
    }, 800);
  </script>
</head>
<body onload="updateFailsafeButtonUI()">
  <h1>ESP32 Airboat</h1>
  
  <div id="status-card">
    <div id="status-box">Connecting...</div>
    <div id="active-cmd">Active: Initializing...</div>
    <div id="rssi-container">
      <span>Signal: <span id="rssi-val">-- dBm</span></span>
      <div id="rssi-bar-outer">
        <div id="rssi-bar-inner"></div>
      </div>
    </div>
  </div>

  <div id="failsafe-bar">
    <button id="toggle-btn" class="btn-toggle mode-stop" onclick="toggleFailsafeMode()">⚙️ Loading Failsafe Mode...</button>
  </div>
  
  <h3>Propeller Throttle</h3>
  <button class="btn btn-green" onclick="sendCmd('/throttle?speed=105&label=Forward+(Slow)')">FORWARD (Slow)</button><br>
  <button class="btn btn-green" onclick="sendCmd('/throttle?speed=125&label=Forward+(Fast)')">FORWARD (Fast)</button><br>
  <button class="btn btn-red"   onclick="sendCmd('/throttle?speed=90&label=MOTOR+STOP')">STOP MOTOR</button><br>
  <button class="btn btn-dark"  onclick="sendCmd('/throttle?speed=75&label=Reverse')">REVERSE</button>

  <h3>Rudder Steering</h3>
  <button class="btn btn-blue" onclick="sendCmd('/steer?angle=45&label=Steer+Left')">LEFT</button><br>
  <button class="btn btn-blue" onclick="sendCmd('/steer?angle=90&label=Steer+Center')">CENTER</button><br>
  <button class="btn btn-blue" onclick="sendCmd('/steer?angle=135&label=Steer+Right')">RIGHT</button>
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


// Reads connected phone's RSSI in SoftAP mode (Compatible with Arduino Core v3.x / ESP-IDF v5)
int getSoftAPRSSI() {
  wifi_sta_list_t stationList;
  memset(&stationList, 0, sizeof(stationList));
  
  // Call low-level ESP-IDF Wi-Fi driver
  esp_err_t err = esp_wifi_ap_get_sta_list(&stationList);
  
  if (err == ESP_OK && stationList.num > 0) {
    // Returns signal strength of the connected mobile phone
    return stationList.sta[0].rssi; 
  }
  
  return -99; // Fallback if no phone is connected
}

void handleRoot() {
  lastHeartbeat = millis();
  server.send(200, "text/html", HTML_PAGE);
}

void handlePing() {
  lastHeartbeat = millis(); 
  isFailsafeActive = false;

  int rssi = getSoftAPRSSI();
  
  // Return both live RSSI and current active command string
  String jsonResponse = "{\"rssi\":" + String(rssi) + ",\"cmd\":\"" + currentStatusText + "\"}";
  server.send(200, "application/json", jsonResponse);
}

void handleSetFailsafeMode() {
  lastHeartbeat = millis();
  if (server.hasArg("mode")) {
    failsafeMode = server.arg("mode").toInt();
    Serial.print("Failsafe Mode updated to: ");
    Serial.println(failsafeMode == 0 ? "STOP MOTOR" : "CIRCLE IN PLACE");
  }
  server.send(200, "text/plain", "OK");
}

void handleThrottle() {
  lastHeartbeat = millis();
  isFailsafeActive = false;
  if (server.hasArg("speed")) {
    int speed = server.arg("speed").toInt();
    setMotorSpeed(speed);
  }
  if (server.hasArg("label")) {
    currentStatusText = server.arg("label");
  }
  server.send(200, "text/plain", "OK");
}

void handleSteer() {
  lastHeartbeat = millis();
  isFailsafeActive = false;
  if (server.hasArg("angle")) {
    int angle = server.arg("angle").toInt();
    angle = constrain(angle, 0, 180);
    currentAngle = angle;
    rudderServo.write(angle);
  }
  if (server.hasArg("label")) {
    currentStatusText = server.arg("label");
  }
  server.send(200, "text/plain", "OK");
}


void setup() {
  delay(2000);
  Serial.begin(115200);
  delay(15);
  Serial.println("Starting Boat System Initialization...");

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);

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
  server.on("/setFailsafeMode", handleSetFailsafeMode);
  server.on("/throttle", handleThrottle);
  server.on("/steer", handleSteer);
  server.begin();
  Serial.println("Web server booted successfully!");

  lastHeartbeat = millis();
}

void loop() {
  server.handleClient();

  // --- FAILSAFE MONITORING ---
  if (millis() - lastHeartbeat > FAILSAFE_TIMEOUT) {
    if (!isFailsafeActive) {
      if (failsafeMode == 0) {
        // MODE 0: STOP MOTOR (Bench/Testing)
        setMotorSpeed(90);
        currentStatusText = "FAILSAFE: MOTOR STOPPED";
        Serial.println("FAILSAFE TRIGGERED: Connection lost! Stopping motor.");
      } else {
        // MODE 1: CIRCLE IN PLACE (Open Water)
        setMotorSpeed(FAILSAFE_THROTTLE);
        rudderServo.write(FAILSAFE_STEER);
        currentStatusText = "FAILSAFE: CIRCLING IN PLACE";
        Serial.println("FAILSAFE TRIGGERED: Connection lost! Boat circling.");
      }
      isFailsafeActive = true;
    }
  }
}