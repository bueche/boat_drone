# Controlling the servo from the ESP32 wifi

This next task will involve controlling the servo using your mobile phone. We will activate the wifi on the ESP32 and start a web server. Then we will connect to that webserver from our cell phone and move the rudder using an HTTP interface.

## Hardware
No need to make any changes from the previous task. see [here](./basic_servo.md#hardware) for more details on the setup.

## Examine the http code

Take a quick look at the code we will be compiling and uploading on the server.

```
cd ~/boat_drone/firmware/
cat -n ./servo_http/servo_http.ino
```
Note -n option to cat will generate line numbers on the output.

Example output
```
$ cat -n ./servo_http/servo_http.ino 
     1	#include <WiFi.h>
     2	#include <WebServer.h>
     3	#include <ESP32Servo.h>
     4	#include "soc/soc.h"
     5	#include "soc/rtc_cntl_reg.h"
     6	
     7	Servo rudderServo;
     8	Servo motorESC;
     9	
    10	const int RUDDER_PIN = 18;
    11	const int ESC_PIN    = 19;
    12	
    13	WebServer server(80);
    14	
    15	const char* ssid = "ESP32-Boat-Control";
    16	const char* password = "password123";
    17	
    18	int currentSpeed = 90;
    19	
    20	// --- FAILSAFE VARIABLES ---
    21	unsigned long lastHeartbeat = 0;
    22	const unsigned long FAILSAFE_TIMEOUT = 2000; // 2.0 seconds timeout limit
    23	bool isFailsafeActive = false;
    24	
    25	const char HTML_PAGE[] = R"rawliteral(
    26	<!DOCTYPE html>
    27	<html>
    28	<head>
    29	  <meta name="viewport" content="width=device-width, initial-scale=1">
    30	  <title>ESP32 Boat Controller</title>
    31	  <style>
    32	    body { font-family: Arial, sans-serif; text-align: center; margin-top: 15px; background-color: #f4f4f9; }
    33	    h1 { color: #333; margin-bottom: 5px; }
    34	    #status-box {
    35	      font-weight: bold; font-size: 16px; color: #2c3e50;
    36	      background: #e2e8f0; padding: 10px; border-radius: 8px;
    37	      margin: 10px auto; max-width: 320px;
    38	    }
    39	    .btn {
    40	      display: inline-block; width: 80%; max-width: 300px; margin: 6px auto; padding: 14px;
    41	      font-size: 18px; font-weight: bold; color: white; border: none;
    42	      border-radius: 8px; cursor: pointer; touch-action: manipulation;
    43	    }
    44	    .btn-green  { background-color: #2ecc71; }
    45	    .btn-red    { background-color: #e74c3c; }
    46	    .btn-blue   { background-color: #3498db; }
    47	    .btn-dark   { background-color: #34495e; }
    48	    .btn:active { transform: scale(0.96); opacity: 0.8; }
    49	  </style>
    50	  <script>
    51	    function sendCmd(url, statusText) {
    52	      document.getElementById('status-box').innerText = "Sending: " + statusText + "...";
    53	      document.getElementById('status-box').style.background = "#f1c40f";
    54	
    55	      fetch(url)
    56	        .then(response => response.text())
    57	        .then(data => {
    58	          document.getElementById('status-box').innerText = "Active: " + statusText;
    59	          document.getElementById('status-box').style.background = "#2ecc71";
    60	        })
    61	        .catch(error => {
    62	          document.getElementById('status-box').innerText = "DISCONNECTED!";
    63	          document.getElementById('status-box').style.background = "#e74c3c";
    64	        });
    65	    }
    66	
    67	    // Ping the ESP32 every 1000ms to keep connection alive
    68	    setInterval(() => {
    69	      fetch('/ping').catch(e => {
    70	        document.getElementById('status-box').innerText = "LOST CONNECTION!";
    71	        document.getElementById('status-box').style.background = "#e74c3c";
    72	      });
    73	    }, 1000);
    74	  </script>
    75	</head>
    76	<body>
    77	  <h1> ESP32 Airboat</h1>
    78	  <div id="status-box">Status: Connected</div>
    79	  
    80	  <h3>Rudder Steering</h3>
    81	  <button class="btn btn-blue" onclick="sendCmd('/steer?angle=45',  'Steer Left')"> LEFT</button><br>
    82	  <button class="btn btn-blue" onclick="sendCmd('/steer?angle=90',  'Steer Center')"> CENTER</button><br>
    83	  <button class="btn btn-blue" onclick="sendCmd('/steer?angle=135', 'Steer Right')"> RIGHT</button>
    84	</body>
    85	</html>
    86	)rawliteral";
    87	
    88	void setMotorSpeed(int targetSpeed) {
    89	  targetSpeed = constrain(targetSpeed, 50, 130);
    90	  
    91	  if (targetSpeed > currentSpeed) {
    92	    for (int s = currentSpeed; s <= targetSpeed; s++) {
    93	      motorESC.write(s);
    94	      delay(15);
    95	    }
    96	  } else {
    97	    for (int s = currentSpeed; s >= targetSpeed; s--) {
    98	      motorESC.write(s);
    99	      delay(15);
   100	    }
   101	  }
   102	  currentSpeed = targetSpeed;
   103	}
   104	
   105	void handleRoot() {
   106	  lastHeartbeat = millis();
   107	  server.send(200, "text/html", HTML_PAGE);
   108	}
   109	
   110	void handlePing() {
   111	  lastHeartbeat = millis(); // Refresh connection timestamp
   112	  isFailsafeActive = false;
   113	  server.send(200, "text/plain", "PONG");
   114	}
   115	
   116	void handleThrottle() {
   117	  lastHeartbeat = millis();
   118	  isFailsafeActive = false;
   119	  if (server.hasArg("speed")) {
   120	    int speed = server.arg("speed").toInt();
   121	    setMotorSpeed(speed);
   122	  }
   123	  server.send(200, "text/plain", "OK");
   124	}
   125	
   126	void handleSteer() {
   127	  lastHeartbeat = millis();
   128	  isFailsafeActive = false;
   129	  if (server.hasArg("angle")) {
   130	    int angle = server.arg("angle").toInt();
   131	    angle = constrain(angle, 0, 180);
   132	    rudderServo.write(angle);
   133	  }
   134	  server.send(200, "text/plain", "OK");
   135	}
   136	
   137	void setup() {
   138	  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
   139	  delay(2000);
   140	  Serial.begin(115200);
   141	  delay(15);
   142	  Serial.println("starting to initilialize");
   143	
   144	
   145	  ESP32PWM::allocateTimer(0);
   146	  ESP32PWM::allocateTimer(1);
   147	  Serial.println("timers allocated");
   148	
   149	  rudderServo.setPeriodHertz(50);
   150	  rudderServo.attach(RUDDER_PIN, 500, 2400);
   151	  rudderServo.write(90);
   152	
   153	  motorESC.setPeriodHertz(50);
   154	  motorESC.attach(ESC_PIN, 1000, 2000);
   155	  motorESC.write(90);
   156	  delay(1500); 
   157	
   158	  WiFi.softAP(ssid, password);
   159	
   160	  server.on("/", handleRoot);
   161	  server.on("/ping", handlePing);
   162	  server.on("/throttle", handleThrottle);
   163	  server.on("/steer", handleSteer);
   164	  server.begin();
   165	  Serial.println("webserver booted");
   166	
   167	  lastHeartbeat = millis();
   168	}
   169	
   170	void loop() {
   171	  server.handleClient();
   172	
   173	  // --- FAILSAFE MONITORING ---
   174	  // If more than 2 seconds pass without a packet from the phone, kill the motor!
   175	  if (millis() - lastHeartbeat > FAILSAFE_TIMEOUT) {
   176	    if (!isFailsafeActive && currentSpeed != 90) {
   177	      setMotorSpeed(90); // Hard stop
   178	      isFailsafeActive = true;
   179	      Serial.println("⚠️ FAILSAFE TRIGGERED: Connection lost! Stopping motor.");
   180	    }
   181	  }
   182	}

```

There is a alot going on but I want to highlight some of the important pieces.
- TBD 



## Compile and upload
```
cd ~/boat_drone/firmware
arduino-cli compile --fqbn esp32:esp32:esp32 ./servo_http/servo_http.ino 
```

Example output:
```
$ arduino-cli compile --fqbn esp32:esp32:esp32 ./servo_http/servo_http.ino 
Sketch uses 936063 bytes (71%) of program storage space. Maximum is 1310720 bytes.
Global variables use 47404 bytes (14%) of dynamic memory, leaving 280276 bytes for local variables. Maximum is 327680 bytes.

```

Then upload

```
arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 ./servo_http
```

example output
```
$ arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 ./servo_http
esptool v5.3.1
Connected to ESP32 on /dev/ttyUSB0:
Chip type:          ESP32-D0WD-V3 (revision v3.1)
Features:           Wi-Fi, BT, Dual Core + LP Core, 240MHz, Vref calibration in eFuse, Coding Scheme None
Crystal frequency:  40MHz
MAC:                20:e7:c8:ab:2f:50

Stub flasher running.
Changing baud rate to 921600...
Changed.

Configuring flash size...

Writing '/home/bueche/.cache/arduino/sketches/5F2166292ECED92EDE58256991F4D0E6/servo_http.ino.bootloader.bin' at 0x00001000...
'/home/bueche/.cache/arduino/sketches/5F2166292ECED92EDE58256991F4D0E6/servo_http.ino.bootloader.bin' at 0x00001000 verified.

Writing '/home/bueche/.cache/arduino/sketches/5F2166292ECED92EDE58256991F4D0E6/servo_http.ino.partitions.bin' at 0x00008000...
'/home/bueche/.cache/arduino/sketches/5F2166292ECED92EDE58256991F4D0E6/servo_http.ino.partitions.bin' at 0x00008000 verified.

Writing '/home/bueche/.arduino15/packages/esp32/hardware/esp32/3.3.11/tools/partitions/boot_app0.bin' at 0x0000e000...
Flash will be erased from 0x0000e000 to 0x0000ffff...
Wrote 8192 bytes (47 compressed) at 0x0000e000 in 0.1 seconds (1285.5 kbit/s).
Hash of data verified.

Writing '/home/bueche/.cache/arduino/sketches/5F2166292ECED92EDE58256991F4D0E6/servo_http.ino.bin' at 0x00010000...
Changed data sectors found, fast reflashing...
Reflashing 229 changed sectors at 0x00010000...
Flash will be erased from 0x00010000 to 0x000f4fff...
Wrote 937984 bytes (612434 compressed) at 0x00010000 in 7.9 seconds (948.2 kbit/s).
Hash of data verified.

Hard resetting via RTS pin...
New upload port: /dev/ttyUSB0 (serial)

```

## connect your phone to the ESP32 wifi 

<p align="center">
  <img src="./images/esp32-ui-1.jpg" alt="ui" width="400">
</p>

