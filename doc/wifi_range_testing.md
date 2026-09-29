# Wifi range testing

How far can the boat go away from us and still be controlled? What can we do about it if it goes out of range? The communication between the cell phone and the boat is through 2Gz wifi. Think of this as creating a communication circle around the boat and once the boat moves far enough away from the operator the connection will degrade and we can lose total connectivity. The concept is illustrated below.

<p align="center">
  <img src="./images/wifi.range.concept.jpg" alt="voltage booster" width="400">
</p>

In this section we cover these topics.

## Definitions

### wifi

**Wi-Fi** is a way for electronic devices to talk to each other without any physical wires. Instead of sending electrical signals through a copper cable, devices convert data into invisible **radio waves** that travel through the air. In your boat project, Wi-Fi allows your smartphone to send steering and throttle commands directly to the ESP32 chip inside the boat.

---

### Antenna

An **antenna** is the physical metal piece that translates electrical signals into radio waves (and vice versa). 
* When your phone wants to send a command, its antenna blasts the message out as a radio wave.
* The ESP32 on your boat has a tiny metal **PCB trace antenna** (a squiggly copper line printed directly on the board) that catches those radio waves out of the air and converts them back into electricity so the computer can understand them.

---

### Wifi end-point

A **Wi-Fi end-point** (or client device) is any device at the starting or ending point of a wireless connection. 
* In your boat setup, you have two key end-points:
  1. **The Smartphone:** The transmitting end-point where you control the boat.
  2. **The ESP32:** The receiving end-point inside the boat that listens for commands and translates them into motor and servo movements.

---

### 2 GZ wifi vs. 5 GZ wifi

Wi-Fi radio waves travel on different speed lanes called **frequencies**, measured in Gigahertz (GHz):

* **2.4 GHz Wi-Fi (Long Distance, Lower Speed):** 
  * Uses longer, wider radio waves. 
  * **Pros:** Travels much farther and passes through physical obstacles (like plastic boat hulls or dry bags) much better.
  * **Cons:** Slower data speeds and more prone to interference from other devices.
  * *Why your boat uses it:* Your ESP32 operates on 2.4 GHz because maximum distance across the water is much more important than streaming high-definition video.

* **5 GHz Wi-Fi (Short Distance, High Speed):** 
  * Uses shorter, faster radio waves.
  * **Pros:** Super fast data speeds with zero lag.
  * **Cons:** Terrible at traveling long distances and gets easily blocked by walls, water, or obstacles.

---

### wifi signal strength

**Wi-Fi signal strength** measures how loud and clear the radio wave is when it reaches the receiving antenna. It is measured in a special unit called **dBm (decibel-milliwatts)**, which is always represented as a negative number:

* **-30 dBm to -60 dBm (Strong Signal):** The boat is close to shore (0m – 30m). Control is instant and responsive.
* **-70 dBm to -80 dBm (Weak Signal):** The boat is drifting far out (30m – 70m). Signals start getting muffled by water reflections, causing slight control lag.
* **-85 dBm or lower (Dropout Zone):** The signal is too faint for the antenna to hear. The connection drops, triggering your ESP32's automatic motor safety cutoff!

## Tasks
1. Take your battery powered electronics outside. 
2. connect to the wifi of the boat and start the propellor (ensure it is in a stable position)
3. using a measuring tape. start walking back 10 meters, 20 meters, and 30 meters and confirm that the boat still can be controlled. does the signal degrade and do you lose connectivity earlier?
4. move beyond this to see when the signal gets spotty. 
5. how far are you when you drop the signal all together? what happens when you lose connectivity?

## How the code handles a disconnect event ... and what is the downside to this approach?
TBD

## Extra credit: can we change the logic to do something more constructive?
TBD
