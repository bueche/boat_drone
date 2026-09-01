
# Basic Servo setup
The servo motor will be used to control the rudder. In this section we will get the basic servo motor working with the ESP32. 

## Hardware
For this exercise you need the following parts:
1. ESP32 and usb cable to the pc
2. Servo motor (e.g., the MG90S 9g Metal Gear Micro Servo)
3. three wires (preferably red, black, and yellow)

Now for this exercise we are assuming you are using the Lonely Binary 3-Pack ESP32 Gold WiFi BT 3 Expansion Bases for Arduino IDE (see amazon: https://www.amazon.com/dp/B0FR3GVRD5). The type of wires will depend on the base extension board used or whether you are using a plain breadboard. 

## Step 1: Wire up the servo

The picture below shows an example setup using a bread board.
<p align="center">
  <img src="./images/esp32-to-servo.jpg" alt="servo connection via breadboard" width="400">
</p>

The servo has a three wire female plug with a brown, red, and orange wire.
1. connect your red wire to the ESP32 5V pin and the red servo wire
2. connect your black wire to the ESP32 grnd pin and the brown servo wire
3. connect your yellow wire to the ESP32 pin 18 and the yellow servo wire

## Step 2: attach any of the arms to the servo gear
There are three choices that come with the servo.

## Step 3: compile the basic_servo sketch to the ESP32

There is a program in the firmware directory called `basic_servo/basic_servo.ino`. This will step through various servo changes.

the first step is to compile:

```
cd ~/boat_drone/firmware
arduino-cli compile --fqbn esp32:esp32:esp32 ./basic_servo/basic_servo.ino 
```

Example output:

```
$ cd ~/boat_drone/firmware
$ arduino-cli compile --fqbn esp32:esp32:esp32 ./basic_servo/basic_servo.ino 
Sketch uses 284559 bytes (21%) of program storage space. Maximum is 1310720 bytes.
Global variables use 22488 bytes (6%) of dynamic memory, leaving 305192 bytes for local variables. Maximum is 327680 bytes.
```


The second step is to upload. Once this is successful the servo arm will start moving through various positions.

```
cd ~/boat_drone/firmware
arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 ./basic_servo
```

Example output.

```
$ arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 ./basic_servo
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

Writing '/home/bueche/.cache/arduino/sketches/58AE8CACAD0A7C0D053756AB92F9970B/basic_servo.ino.bootloader.bin' at 0x00001000...
Flash will be erased from 0x00001000 to 0x00007fff...
Wrote 24992 bytes (16001 compressed) at 0x00001000 in 0.4 seconds (530.1 kbit/s).
Hash of data verified.

Writing '/home/bueche/.cache/arduino/sketches/58AE8CACAD0A7C0D053756AB92F9970B/basic_servo.ino.partitions.bin' at 0x00008000...
Flash will be erased from 0x00008000 to 0x00008fff...
Wrote 3072 bytes (146 compressed) at 0x00008000 in 0.0 seconds (1029.9 kbit/s).
Hash of data verified.

Writing '/home/bueche/.arduino15/packages/esp32/hardware/esp32/3.3.11/tools/partitions/boot_app0.bin' at 0x0000e000...
Flash will be erased from 0x0000e000 to 0x0000ffff...
Wrote 8192 bytes (47 compressed) at 0x0000e000 in 0.0 seconds (1331.2 kbit/s).
Hash of data verified.

Writing '/home/bueche/.cache/arduino/sketches/58AE8CACAD0A7C0D053756AB92F9970B/basic_servo.ino.bin' at 0x00010000...
Flash will be erased from 0x00010000 to 0x00055fff...
Wrote 284704 bytes (165569 compressed) at 0x00010000 in 2.2 seconds (1012.9 kbit/s).
Hash of data verified.

Hard resetting via RTS pin...
New upload port: /dev/ttyUSB0 (serial)
```

## Step 4: monitor the movement.

```
arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200
```

Example output.

```
$ arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200
Monitor port settings:
  baudrate=115200
  bits=8
  dtr=on
  parity=none
  rts=on
  stop_bits=1

Connecting to /dev/ttyUSB0. Press CTRL-C to exit.
�����������������Sweep Angle: 180
Sweep Angle: 178
Sweep Angle: 176
Sweep Angle: 174
Sweep Angle: 172
Sweep Angle: 170
Sweep Angle: 168
Sweep Angle: 166
Sweep Angle: 164
Sweep Angle: 162
Sweep Angle: 160
Sweep Angle: 158
Sweep Angle: 156
Sweep Angle: 154
Sweep Angle: 152
Sweep Angle: 150
Sweep Angle: 148
Sweep Angle: 146
Sweep Angle: 144
Sweep Angle: 142
Sweep Angle: 140
Sweep Angle: 138
Sweep Angle: 136
Sweep Angle: 134
Sweep Angle: 132
Sweep Angle: 130
Sweep Angle: 128
Sweep Angle: 126
Sweep Angle: 124
Sweep Angle: 122
Sweep Angle: 120
Sweep Angle: 118
Sweep Angle: 116
Sweep Angle: 114
Sweep Angle: 112
Sweep Angle: 110
Sweep Angle: 108
Sweep Angle: 106
Sweep Angle: 104
Sweep Angle: 102
Sweep Angle: 100
Sweep Angle: 98
Sweep Angle: 96
Sweep Angle: 94
Sweep Angle: 92
Sweep Angle: 90
Sweep Angle: 88
Sweep Angle: 86
Sweep Angle: 84
Sweep Angle: 82
Sweep Angle: 80
Sweep Angle: 78
Sweep Angle: 76
Sweep Angle: 74
Sweep Angle: 72
Sweep Angle: 70
Sweep Angle: 68
Sweep Angle: 66
Sweep Angle: 64
Sweep Angle: 62
Sweep Angle: 60
Sweep Angle: 58
Sweep Angle: 56
Sweep Angle: 54
Sweep Angle: 52
Sweep Angle: 50
Sweep Angle: 48
Sweep Angle: 46
Sweep Angle: 44
Sweep Angle: 42
Sweep Angle: 40
Sweep Angle: 38
Sweep Angle: 36
Sweep Angle: 34
Sweep Angle: 32
Sweep Angle: 30
Sweep Angle: 28
Sweep Angle: 26
Sweep Angle: 24
Sweep Angle: 22
Sweep Angle: 20
Sweep Angle: 18
Sweep Angle: 16
Sweep Angle: 14
Sweep Angle: 12
Sweep Angle: 10
Sweep Angle: 8
Sweep Angle: 6
Sweep Angle: 4
Sweep Angle: 2
Sweep Angle: 0

[MODE 3] Wiggle Time!
 -> Wiggle Right!
 -> Wiggle Left!
 -> Wiggle Right!
 -> Wiggle Left!
 -> Wiggle Right!
 -> Wiggle Left!
 -> Wiggle Right!
 -> Wiggle Left!
 -> Wiggle Right!
 -> Wiggle Left!

Resetting to center position for 3 seconds...

```

