# Task 1 — Arduino IDE + ESP32 setup

This document describes how to set up the Arduino command line for an ESP32 development board and the corresponding CH340/CH341 USB-to-Serial adapter software (Vendor ID: `0x1a86`, Product ID: `0x7522`) on macOS, Ubuntu 24.04, and ChromeOS (Linux/Crostini). It also walks through installing the ESP32 boards package and uploading the first Blink sketch.

The ESP32 development board is a special controller that has an Arduino-compatible development interface. Normally, one would use the Ardunio IDE, but for this tutorial I'm going to use the command line interface so we don't have to worry about different IDE versions.

Reference screenshots to add later for the key GUI steps:

- Placeholder: screenshot of the official Arduino IDE installer / start screen.
- Placeholder: screenshot of Arduino IDE -> Preferences -> Additional Boards Manager URLs.
- Placeholder: screenshot of Tools -> Board -> Boards Manager showing `esp32 by Espressif Systems`.
- Placeholder: screenshot of the macOS Privacy & Security dialog after installing the CH34x driver.
- Placeholder: screenshot of ChromeOS Settings -> Linux -> Manage USB devices.

**Notes:**
- EPS32 on-Board USB chip: WCH CH340 / CH341 (VID=1a86, PID=7522). Many OSes include drivers; sometimes manual install or permission changes are required.
- Typical ESP32 dev board built-in LED: pin `2` (verify for your specific board).

**Quick Links:**
- ESP32 Boards Manager URL: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`

**Common steps (overview):**
- Install Arduino command line software (official download or package manager)... or the IDE, but again, we will only be showing the command line.
- Add the ESP32 Boards Manager URL to Arduino Preferences and install `esp32 by Espressif Systems` from Boards Manager.
- Install any vendor USB drivers if your OS needs them (see OS-specific section).
- Select board `ESP32 Dev Module`, select the serial port, compile and upload the Blink sketch below.

**Contents**
- macOS (OSX)
- Ubuntu 24.04
- ChromeOS (Linux / Crostini)
- ESP32: Add boards + Blink sketch
- CH340/CH341 notes and udev rule (Linux)

** common approach **
1. **create a terminal or command line window**
We will be using a command line interface rather than a windowed IDE. All of the operating systems support this: macOS, Ubuntu, Chrome OS, and even Windows. Starting such a window varies per OS. 

2. **Download the command line software**
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

3. **Basic test of the installation***
```
arduino-cli version
```
An Example output:
```
$ arduino-cli version
arduino-cli  Version: 1.5.1 Commit: 01f3d4f2b Date: 2026-06-05T10:22:11Z

```

4. **Add the ESP32 board index and install the code**
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

4. **Set some permissions for the user to access the USB device**

```
sudo usermod -a -G dialout $USER
```
it is often required to log out and back in for these to take effect.

5. plug in board to USB that connects to PC and check device

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
Notice that there is a /dev/ttyUSB0. This is our "port" for talking to the device.

if you get an error later that says permission denied to access /dev/ttyUSB0 then you may need to logout and bacak in, or run the following command.

```
chmod ugo+rwx /dev/ttyUSB0
```
6. pull the code from github
```
cd 
git clone https://github.com/bueche/boat_drone.git
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
This will clone the source repository and documentation to your local drive. 

7. compile the basic blink sketch
```
cd ~/boat_drone
arduino-cli compile --fqbn esp32:esp32:esp32 ./blink/blink.ino
```

Example output:
```
$ arduino-cli compile --fqbn esp32:esp32:esp32 ./blink/blink.ino 
Sketch uses 285345 bytes (21%) of program storage space. Maximum is 1310720 bytes.
Global variables use 22804 bytes (6%) of dynamic memory, leaving 304876 bytes for local variables. Maximum is 327680 bytes.
$
```

8. upload the basic blink sketch
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


1. Download the app image

```
bueche@buecheutuntu2204:~$ cd Downloads
bueche@buecheutuntu2204:~/Downloads$ ls -lt
total 199632
-rw-rw-r-- 1 bueche bueche 201750502 Aug 24 20:49  arduino-ide_2.3.10_Linux_64bit.AppImage

```
2. make it executable and run it

```
bueche@buecheutuntu2204:~/Downloads$ chmod +x arduino-ide_*.AppImage
bueche@buecheutuntu2204:~/Downloads$ ls -lt arduino-ide_*.AppImage
-rwxrwxr-x 1 bueche bueche 201750502 Aug 24 20:49 arduino-ide_2.3.10_Linux_64bit.AppImage

```
**macOS (OSX)**

1. Install Arduino IDE

- Download the latest Arduino IDE (2.x) for macOS from the Arduino website and install the `.dmg`.

Placeholder: add a screenshot of the Arduino IDE installation or first-run window.

2. CH340/CH341 driver (if needed)

- Many modern macOS releases work without installing a driver. If your board does not create a serial device when plugged in, install the WCH CH34x macOS driver:

	- WCH driver page (example): https://www.wch.cn

- After installing the driver package, you may need to open `System Settings -> Privacy & Security` and "Allow" the kernel extension or driver for the installer developer, then reboot.

Placeholder: add a screenshot of the macOS Privacy & Security prompt that allows the CH34x driver.

3. Add ESP32 boards to Arduino IDE

- Open Arduino IDE -> Preferences -> Additional Boards Manager URLs and add:

	`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`

Placeholder: add a screenshot of Arduino IDE -> Preferences -> Additional Boards Manager URLs.

- Then open Tools -> Board -> Boards Manager, search `esp32`, and install `esp32 by Espressif Systems`.

Placeholder: add a screenshot of Boards Manager with `esp32 by Espressif Systems` visible and the Install button.

4. Select board and port

- Tools -> Board -> `ESP32 Dev Module` (or the specific ESP32 variant for your board).
- Tools -> Port -> macOS ports often appear as `/dev/cu.wchusbserial*` or `/dev/cu.usbserial-*`.

5. Upload Blink sketch (see ESP32 section below)


**Ubuntu 24.04**

1. Install Arduino IDE

- Option A — Snap (quick):

	```bash
	sudo snap install arduino --classic
	```

- Option B — Official Linux package: download the Arduino IDE 2.x Linux 64-bit tarball from arduino.cc, extract and run the `arduino-ide` executable.

2. CH340/CH341 driver and permissions

- Linux kernels include support for the CH341 chip via the `ch341` module. Load the module if needed:

	```bash
	sudo modprobe ch341
	dmesg | tail -n 20
	```

- Add your user to the `dialout` group so you can access serial devices:

	```bash
	sudo usermod -aG dialout $USER
	newgrp dialout    # or log out and back in
	```

- Create a udev rule so the device is accessible and consistently named. Create `/etc/udev/rules.d/99-ch340.rules` with the following contents:

	```text
	# WCH CH340 / CH341 USB-to-Serial
	ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7522", MODE="0666", GROUP="dialout", TAG+="uaccess"
	```

	Then reload udev rules and re-plug the board:

	```bash
	sudo udevadm control --reload-rules && sudo udevadm trigger
	```

- The ESP serial device will usually appear as `/dev/ttyUSB0` (or `/dev/ttyUSB1`, etc.). Check with `ls /dev/ttyUSB*` or `dmesg`.

3. Add ESP32 boards (same as macOS)

- In Arduino IDE, Preferences -> Additional Boards Manager URLs: add the ESP32 URL (above). Then Boards Manager -> install `esp32 by Espressif Systems`.

4. Select board and port, then upload Blink sketch (see ESP32 section below)


**ChromeOS (Linux / Crostini)**

1. Enable Linux (Beta / Crostini)

- In ChromeOS Settings, enable "Developers" -> "Linux development environment (Beta)" if not already enabled.

2. Share the USB device with the Linux container

- Plug in the ESP32 board.
- Open system tray (clock) -> Manage USB devices (or Settings -> Linux -> Manage USB devices) and enable sharing the ESP32/CH340 device with the Linux container. If you do not see the device, unplug and replug and check again.

Placeholder: add a screenshot of ChromeOS Settings -> Linux -> Manage USB devices with the board enabled for the container.

3. Install Arduino IDE inside the Linux container

- Inside the Terminal (Linux container), either install Arduino from the distro packages or download the official tarball. Example (deb-based container):

	```bash
	sudo apt update
	sudo apt install arduino -y
	```

	Note: the distro package may be older; prefer the official Arduino IDE 2.x tarball if you want the newest GUI.

4. Device access in the container

- Once shared with the container, the device will usually appear as `/dev/ttyUSB0` or `/dev/ttyACM0`. If permission errors occur, give the container user access:

	```bash
	sudo usermod -aG dialout $USER
	sudo chmod a+rw /dev/ttyUSB0
	```

5. Add ESP32 boards and upload (same as other OSes)

- In Arduino IDE inside the container, add the ESP32 Boards Manager URL and install `esp32 by Espressif Systems`.


How to open a command line on ChromeOS (step-by-step)

- If you haven't enabled Linux (Beta) yet:

	1. Open Settings -> Developers -> Linux development environment (Beta) -> Turn On.
	2. Follow the prompts to create the container. This installs the Linux VM and adds a `Terminal` app to your Launcher.

- To open the Linux Terminal after enabling Linux:

	- Open the Launcher (bottom-left), search for "Terminal" or look under "Linux apps" and click `Terminal`.
	- Alternatively press the Search key (or Launcher key) and type `Terminal`.

- If you see only the ChromeOS shell (crosh) after pressing Ctrl+Alt+T, you can start the Linux Terminal from crosh by typing:

	```text
	vmc start termina
	```

	Then open the Terminal app from the Launcher. (Most users do not need to use crosh; the Launcher Terminal is the Linux container shell.)

Tips:

- When a USB device is plugged in, open Settings -> Linux -> Manage USB devices and toggle the device to be available inside the container.
- The Linux Terminal behaves like a Debian/Ubuntu shell; use the commands in the Ubuntu section to install packages.


Exact download URLs and package names (attempts)

Note: direct package filenames change with versions; these links point to official pages and distribution names you can install.

- Arduino IDE (main download page): https://www.arduino.cc/en/software
- Arduino official downloads directory (tarballs/archives): https://downloads.arduino.cc/arduino-ide/
- Snap package (Ubuntu/Crostini quick install): `snap install arduino --classic` (Snap store page: https://snapcraft.io/arduino)
- ESP32 Boards Manager URL (for Arduino Preferences): `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
- CH340/CH341 vendor (WCH) website (drivers): https://www.wch.cn — search for "CH34x" or "CH341" and choose your OS-specific driver; Linux typically does not require a driver.
- Ubuntu package names (alternative): `arduino` (APT package, may be older), or use the Arduino tarball from the official downloads page.
- udev rule file (Linux): `/etc/udev/rules.d/99-ch340.rules` (create this file with the provided contents to set permissions for VID=1a86 PID=7522)

If you want, I can add direct example filenames for the current Arduino IDE release (I will check the downloads directory and insert the exact tarball and macOS installer names).


**ESP32: Add boards + Blink sketch**

1. Add Boards Manager URL (one time)

- Arduino IDE -> Preferences -> Additional Boards Manager URLs:

	`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`

2. Install the ESP32 core

- Tools -> Board -> Boards Manager -> search `esp32` -> install `esp32 by Espressif Systems`.

3. Select board and settings

- Tools -> Board -> `ESP32 Dev Module` (or match your board variant).
- Tools -> Flash Frequency, Partition Scheme, Upload Speed: defaults usually work; upload speed `115200` is fine.

4. Blink sketch (example)

Use the built-in LED (commonly pin 2). Copy/paste into Arduino IDE and click Upload.

No extra `#include` lines are needed for this simple sketch when using the ESP32 Arduino core. The functions `neopixelWrite()`, and `delay()` are already part of the standard Arduino API for ESP32 boards.

```cpp
/#include <Arduino.h>

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
```

Upload notes:
- Make sure the correct Port is selected in Tools -> Port.
- If upload fails, press and hold the board's BOOT button while clicking Upload (some older ESP32 dev boards need manual boot entry). Release when the IDE shows "Connecting".


**CH340 / CH341 driver notes (detailed)**

- Vendor ID: `0x1a86` (Nanjing Qinheng Microelectronics / WCH)
- Product ID: `0x7522` (CH340 / CH341)

- macOS: driver packages are available from WCH. Newer macOS versions may require allowing kernel extensions in System Settings and a reboot.
- Linux: kernel includes `ch341` module; on Ubuntu create a udev rule (see above) and add your user to `dialout` group.
- ChromeOS: share the USB device with the Linux (Crostini) container; device access behaves like a Linux guest.

Troubleshooting
- Check device presence:

	- macOS: `ls /dev/cu.*` and look for `wchusbserial` or `usbserial` entries.
	- Linux/Ubuntu/ChromeOS container: `dmesg | tail` and `ls /dev/ttyUSB*`.

- Serial monitor: set 115200 baud (common) in Arduino's Serial Monitor to view debug/boot logs from ESP32.

--

If you'd like, I can also:
- Add screenshots for each step.
- Package an example `README` or create a quickstart script for Ubuntu/ChromeOS to automate udev rule creation.

File updated: [doc/setup.md](doc/setup.md)
