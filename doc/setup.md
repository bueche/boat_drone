# Task 1 — Arduino IDE + ESP32 setup

This document describes how to set up the Arduino command line for an ESP32 development board and the corresponding CH340/CH341 USB-to-Serial adapter software (Vendor ID: `0x1a86`, Product ID: `0x7522`) on macOS, Ubuntu 24.04, and ChromeOS (Linux/Crostini). It also walks through installing the ESP32 boards package and uploading the first Blink sketch.

The ESP32 development board is a special controller that has an Arduino-compatible development interface. Normally, one would use the Ardunio IDE, but for this tutorial I'm going to use the command line interface so we don't have to worry about different IDE versions.

These instructions focus primarily on common Linux installation (which should also work for ChromeOS).

## Background

Before jumping into this section, I'll define a few concepts that will be helpful to understand what this task is all about.

### Program / Software 

### Functions and (software) Libraries

### Development Computer

### ESP32 controller board

### USB port

### Command line Interface 

### Arduino Development Environment


### Arduino code structure

### IDE (Integrated Development Environment)

### Compile

### Upload


## Hardware
For this exercise you need the following parts:
1. ESP32 and usb cable to the pc

## Installation steps (overview) 
- Install Arduino command line software (official download or package manager)... or the IDE, but again, we will only be showing the command line.
- Add the ESP32 Boards Manager URL to Arduino Preferences and install `esp32 by Espressif Systems` from command line (or using the IDE the Boards Manager).
- Install any vendor USB drivers if your OS needs them (see OS-specific section).
- Select board `ESP32 Dev Module`, select the serial port, compile and upload the Blink sketch below.


## Common command lin e approach
### 1. create a terminal or command line window
We will be using a command line interface rather than a windowed IDE. All of the operating systems support this: macOS, Ubuntu, Chrome OS, and even Windows. Starting such a window varies per OS. 

### 2. Download the command line software
Run the following on you linux command line.
```
mkdir -p ~/.local/bin
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=~/.local/bin sh
```
Note:
 - ~/.local/bin is a path to a hidden directory in linux (files that start with "." are hidden).  the mkdir command will create it 
 - This command should install the appropriate code on your system, but more code will need to be downloaded later.
 - the dollar sign "$" is the symbol for the prompt. when you copy these commands don't add that when you copy the above commands.

 Example output:

```
 $ curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=~/.local/bin sh
Installing in /home/bueche/Downloads/bin
ARCH=ARM64
OS=Linux
Using curl as download tool
Downloading https://downloads.arduino.cc/arduino-cli/arduino-cli_1.5.1_Linux_ARM64.tar.gz
install.sh: arduino-cli not found. You might want to add "/home/bueche/Downloads/bin" to your $PATH
arduino-cli  Version: 1.5.1 Commit: 01f3d4f2b Date: 2026-06-05T10:22:11Z installed successfully in /home/bueche/.local/bin
$ 
```
You should also make sure that ~/.local/bin is part of your path.


### 3. Basic test of the installation
```
arduino-cli version
```
An Example output:
```
$ arduino-cli version
arduino-cli  Version: 1.5.1 Commit: 01f3d4f2b Date: 2026-06-05T10:22:11Z

```

### 4. Add the ESP32 board index and install the code
```
arduino-cli config init
arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
arduino-cli core update-index
arduino-cli core install esp32:esp32

```

And example of these commands being run and their output.
```
$ arduino-cli config init
arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
arduino-cli core update-index
arduino-cli core install esp32:esp32

Config file written to: /home/bueche/.arduino15/arduino-cli.yaml
Downloading index: library_index.tar.bz2 downloaded                                                            
Downloading index: package_index.tar.bz2 downloaded                                                            
Downloading missing tool builtin:ctags@5.8-arduino11...
builtin:ctags@5.8-arduino11 downloaded                                                                         
Installing builtin:ctags@5.8-arduino11...
Skipping tool configuration....
builtin:ctags@5.8-arduino11 installed
Downloading missing tool builtin:dfu-discovery@0.1.2...
builtin:dfu-discovery@0.1.2 downloaded                                                                         
Installing builtin:dfu-discovery@0.1.2...
Skipping tool configuration....
builtin:dfu-discovery@0.1.2 installed
Downloading missing tool builtin:mdns-discovery@1.1.0...
builtin:mdns-discovery@1.1.0 downloaded                                                                        
Installing builtin:mdns-discovery@1.1.0...
Skipping tool configuration....
builtin:mdns-discovery@1.1.0 installed
Downloading missing tool builtin:serial-discovery@1.5.2...
builtin:serial-discovery@1.5.2 downloaded                                                                      
Installing builtin:serial-discovery@1.5.2...
Skipping tool configuration....
builtin:serial-discovery@1.5.2 installed
Downloading missing tool builtin:serial-monitor@0.15.0...
builtin:serial-monitor@0.15.0 downloaded                                                                       
Installing builtin:serial-monitor@0.15.0...
Skipping tool configuration....
builtin:serial-monitor@0.15.0 installed
Downloading index: package_index.tar.bz2 downloaded 
:
:
esp32:esp32c6-libs@3.3.11 installed
Installing esp32:esp32h2-libs@3.3.11...
esp32:esp32h2-libs@3.3.11 installed
Installing esp32:esp32p4-libs@3.3.11...
esp32:esp32p4-libs@3.3.11 installed
Installing esp32:esp32p4_es-libs@3.3.11...
esp32:esp32p4_es-libs@3.3.11 installed
Installing esp32:esp32s2-libs@3.3.11...
esp32:esp32s2-libs@3.3.11 installed
Installing esp32:esp32s3-libs@3.3.11...
esp32:esp32s3-libs@3.3.11 installed
Installing platform esp32:esp32@3.3.11...
Platform esp32:esp32@3.3.11 installed
$ 
```

### 5. Set some permissions for the user to access the USB device

```
sudo usermod -a -G dialout $USER
```
it is often required to log out and back in for these to take effect.
### 6. plug in board to USB that connects to PC and check device

Initially, there will be two lights on the board that blink or shine. 

```
arduino-cli board list
```

Sample output:
```
$ arduino-cli board list
Port         Protocol Type              Board Name FQBN Core
/dev/ttyAMA0 serial   Serial Port       Unknown
/dev/ttyUSB0 serial   Serial Port (USB) Unknown

```
Notice that there is a `/dev/ttyUSB0`. This is our "port" for talking to the device.

if you get an error later that says permission denied to access `/dev/ttyUSB0` then you may need to logout and bacak in, or run the following command.

```
chmod ugo+rwx /dev/ttyUSB0
```
### 7. pull the code from github
The next two commands will do the following: (1) change the current working directory to the home directory and (2) download the git project directory within a directory called: `boat_drone`.
```
cd 
git clone https://github.com/bueche/boat_drone.git
ls -l boat_drone
```
Example output:
```
$ git clone https://github.com/bueche/boat_drone.git
Cloning into 'boat_drone'...
remote: Enumerating objects: 40, done.
remote: Counting objects: 100% (40/40), done.
remote: Compressing objects: 100% (25/25), done.
remote: Total 40 (delta 4), reused 32 (delta 1), pack-reused 0 (from 0)
Receiving objects: 100% (40/40), 3.64 MiB | 2.76 MiB/s, done.
Resolving deltas: 100% (4/4), done.
$ ls -l boat_drone
otal 24
-rw-r--r-- 1 ubuntu ubuntu 11357 Aug 27 19:04 LICENSE
-rw-r--r-- 1 ubuntu ubuntu  2409 Aug 27 19:04 README.md
drwxr-xr-x 3 ubuntu ubuntu  4096 Aug 27 19:04 doc
drwxr-xr-x 6 ubuntu ubuntu  4096 Aug 27 19:04 firmware

```
You have cloned the source repository and documentation to your local drive. 

### 8. Compile the basic blink sketch
Now you will change the current working directory to the `boat_drone/firmware` directory. Next you will  display the source and then compile the basic sketch (or source code) called blink.ino.

```
cd ~/boat_drone/firmware
pwd
cat ./blink/blink.ino
arduino-cli compile --fqbn esp32:esp32:esp32 ./blink/blink.ino
```

Example output:
```
$ pwd
/home/..../boat_drone/firmware
$ cat blink/blink.ino
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

$ arduino-cli compile --fqbn esp32:esp32:esp32 ./blink/blink.ino 
Sketch uses 285345 bytes (21%) of program storage space. Maximum is 1310720 bytes.
Global variables use 22804 bytes (6%) of dynamic memory, leaving 304876 bytes for local variables. Maximum is 327680 bytes.
$
```

### 9. upload the basic blink sketch
```
cd ~/boat_drone/firmware
arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 ./blink
```

Example output
```
$ arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 ./blink
esptool v5.3.1
Connected to ESP32 on /dev/ttyUSB0:
Chip type:          ESP32-D0WD-V3 (revision v3.1)
Features:           Wi-Fi, BT, Dual Core + LP Core, 240MHz, Vref calibration in eFuse, Coding Scheme None
Crystal frequency:  40MHz
MAC:                20:e7:c8:aa:d1:28

Stub flasher running.
Changing baud rate to 921600...
Changed.

Configuring flash size...

Writing '/home/bueche/.cache/arduino/sketches/EEF363903EBDB96E96CF1E3B49B8861F/blink.ino.bootloader.bin' at 0x00001000...
Flash will be erased from 0x00001000 to 0x00007fff...
Wrote 24992 bytes (16001 compressed) at 0x00001000 in 0.4 seconds (527.4 kbit/s).
Hash of data verified.

Writing '/home/bueche/.cache/arduino/sketches/EEF363903EBDB96E96CF1E3B49B8861F/blink.ino.partitions.bin' at 0x00008000...
Flash will be erased from 0x00008000 to 0x00008fff...
Wrote 3072 bytes (146 compressed) at 0x00008000 in 0.0 seconds (988.7 kbit/s).
Hash of data verified.

Writing '/home/bueche/.arduino15/packages/esp32/hardware/esp32/3.3.11/tools/partitions/boot_app0.bin' at 0x0000e000...
Flash will be erased from 0x0000e000 to 0x0000ffff...
Wrote 8192 bytes (47 compressed) at 0x0000e000 in 0.0 seconds (1328.4 kbit/s).
Hash of data verified.

Writing '/home/bueche/.cache/arduino/sketches/EEF363903EBDB96E96CF1E3B49B8861F/blink.ino.bin' at 0x00010000...
Flash will be erased from 0x00010000 to 0x00055fff...
Wrote 285488 bytes (165548 compressed) at 0x00010000 in 2.3 seconds (1013.5 kbit/s).
Hash of data verified.

Hard resetting via RTS pin...
New upload port: /dev/ttyUSB0 (serial)

```

### 10 exmamin the serial port output

```
arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200
```
This will allow you to see the messages being sent via the device back to the PC via the USB interface. Just type in CTL-C to exit this monitor.

Example output:
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
����������������������red on
red off
red on
red off
red on
red off
red on
red off
red on

```

## Looking at the code

Now lets dive a little deeper into the code we uploaded to the device.

```
cd ~/boat_drone/firmware
cat ./blink/blink.ino
cat -n ./blink/blink.ino
```
These three commands to the following.
1. `cd ~/boat_drone/firmware` will change the current working directory to be where the code was stored to.
2. `cat ./blink/blink.ino` will display the contents of the `blink.ino` program file. This is just a text file that has the program high-level language instruction. There are many different programming languages and this one is a variant of C/C++ (not important for this tutorial). `cat` is a program that outputs the contents of a file to the computer screen.
3. `cat -n ./blink/blink.ino` does the same thing as the previous command except that it also adds in the line numbers for each line in the file. this is useful for this tutorial as it allows us to identify and refer to a specific file.

Example output:
```
$ cd ~/boat_drone/firmware
$ cat ./blink/blink.ino
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

$ cat -n ./blink/blink.ino
     1	#include <Arduino.h>
     2	
     3	#define LED_PIN     2    // Controlled via GPIO2 for this kit
     4	#define BRIGHTNESS  50   // Set safe brightness limit (0-255)
     5	
     6	
     7	void setup() {
     8	  Serial.begin(115200);
     9	  // Initialize the WS2812 LED configuration
    10	 
    11	}
    12	
    13	void loop() {
    14	  // Blink Red: neopixelWrite(pin, Red, Green, Blue)
    15	  neopixelWrite(LED_PIN, BRIGHTNESS, 0, 0);
    16	  Serial.println("red on");
    17	  delay(1000);
    18	
    19	  // Turn LED Off (All values zero)
    20	  neopixelWrite(LED_PIN, 0, 0, 0);
    21	  Serial.println("red off");
    22	  delay(1000);
    23	}

```

Ok, lets briefly walk through this program.

### #include and other definitions
The first line of the program is `#include Arduino.h`. This lets our program access some pre-built libraries. For example `neopixelWrite()`  that is used later in the program. Some libraries are "built in", and so we don't need to include them separately. In this case, the ones that provide the functions `delay()` and `Serial.println()` are provided by built-in libraries.

AFter this (lines 3 & 4) two symbols are defined: `LED_PIN` and `BRIGHTNESS`. These are just labels assigned to the numbers (2 and 50) respectivey that we use to make the program more readable. The numbers are used in the functions later.

### program structure
Notice as described earlier in the background, the Ardunio program structure has two high-level functions: setup() and loop(). When the board is booted, the setup() function is called once, and the loop() one is called repeatedly until the board is stopped or rebooted.

### setup()
The setup function (lines 7-11) initializes the baud rate (or transmission speed) of the Serial line .. this is pysically the USB cable between the ESP32 and the host development PC. Both sides have to agree on how fast the transmission speed will be.

### loop()
The loop function (lines 13-23) will repeatedly turn on the pin (show it shines red), then wait for a second and then turn its brightness to zero (causing it to turn off). 

The function `delay(1000)` causes the program to delay 1000 msec (i.e., 1 second) before executing the next instruction.

## Bonus: Enhancing the code
Now that we have executed the code and understood it a little, it might be good to change it a bit to get more familiarity. The following are some suggested changes to make. After making each of the changes you should compile and upload the program to see if it worked.

### Change 1: Speed up the blinking

To speed up the blinking we can lower the delay from 1000 ms to 250 ms. So the light should blink 4 fimes faster. So `delay(1000);` becomes `delay(250);`

### Change 2: change the output

Insert some other text or add new `Serial.println()` statements

## FAQ and Trouble shooting
### Permission error 
If you run an arduino command and get a permission error. For example like this:
```
$ arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 ./blink
esptool v5.3.1
Serial port /dev/ttyUSB0:

A fatal error occurred: Could not open /dev/ttyUSB0, the port is busy or doesn't exist.
([Errno 2] could not open port /dev/ttyUSB0: [Errno 2] No such file or directory: '/dev/ttyUSB0')

Hint: Check if the port is correct and ESP connected

```
Its likely either that you have the wrong name for the port (e.g., perhaps its `/dev/ttyUSB1`) or inadequate permissions. Steps to resolve: (1) Check name and change the name on your command if its wrong, and (2) change the permission

Example of wrong name
```
$ ls -lt /dev/ttyUSB*
crw-rw-rw- 1 root dialout 188, 0 Aug 28 21:59 /dev/ttyUSB1
```
So use tne name /dev/ttyUSB1 in your arduino command instead.

Example of inadequate permission and how to to fix it
```
$ ls -lt /dev/ttyUSB*
crw-rw---- 1 root dialout 188, 0 Aug 28 21:59 /dev/ttyUSB0
$ chmod ugo+rw /dev/ttyUSB0
$ ls -lt /dev/ttyUSB*
crw-rw-rw- 1 root dialout 188, 0 Aug 28 21:59 /dev/ttyUSB0
```
### How do I get a command window on ChromeOS?
ChromeOS (Linux / Crostini)

#### Enable Linux (Beta / Crostini)

- In ChromeOS Settings, enable "Developers" -> "Linux development environment (Beta)" if not already enabled.
- Follow the prompts to create the container. This installs the Linux VM and adds a `Terminal` app to your Launcher.

- To open the Linux Terminal after enabling Linux:

	- Open the Launcher (bottom-left), search for "Terminal" or look under "Linux apps" and click `Terminal`.
	- Alternatively press the Search key (or Launcher key) and type `Terminal`.

- If you see only the ChromeOS shell (crosh) after pressing Ctrl+Alt+T, you can start the Linux Terminal from crosh by typing:

	```text
	vmc start termina
	```

	Then open the Terminal app from the Launcher. (Most users do not need to use crosh; the Launcher Terminal is the Linux container shell.)

#### Share the USB device with the Linux container

- Plug in the ESP32 board.
- Open system tray (clock) -> Manage USB devices (or Settings -> Linux -> Manage USB devices) and enable sharing the ESP32/CH340 device with the Linux container. If you do not see the device, unplug and replug and check again.



-----
