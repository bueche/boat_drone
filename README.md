# Air Boat Drone (DRAFT)
This repository supports a learning activity to build a small boat drone. It is organized into a set of learning tasks and can be useful basis for lab or class on the topic. 

<p align="center">
  <img src="./doc/images/boat-drone-development.jpg" alt="boat drone in development" width="300">
</p>

## Repository goal in more detail

Want to build a motor boat drone? In this repository we walk through how to do this...

There is an experimental small electronic motor boat outlined on one of the 3D printing model websites ([here](https://cults3d.com/en/3d-model/game/)motor-boat-rc-small-experimental). 

Although that website provided the STL files so you can 3D print the boat, very little information was provided on the electronics. In addition, some of the STL files had issues as well. In this repository we provide a series of step-by-step activities to take the basic model and enhance it so that it can be controlled by a standard iOS or Android mobile phone. 


## Bill of Materials
TBD

## Tasks and Notes

1. [Software Environment setup and getting ESP32 to blink &#9989;](./doc/setup.md): this is an initial task to get the PC environment setup to compile and load some simple code to the ESP32.
2. [Controlling the Servo (Rudder) using the EPS32 -- &#9989;](./doc/basic_servo.md). In this task we learn how to setup and control the servo from the ESP32. This servo will eventually power the rudder of the boat drone.
3. [Controlling the Servo (Rudder) using a mobile phone and ESP32 Ethernet and Webserver -- &#9989; ](./doc/esp32_wifi_and_servo_control.md): We fire up the ESP32's wifi and run a web server to allow us to control the servo from a mobile phone.
4. [Wiring DC motor and battery -- &#9989;](./doc/dc_motor_and_battery.md): In this task we hookup the DC motor and the battery to the boat electronics.
5. [Everything working off of the battery -- &#9989;](./doc/battery_powered.md): In the previous task only the DC motor was working off of the battery. Now lets get everything working all off of the battery.
6. [Propelling_the_boat -- &#9989;](./doc/propelling_the_boat.md): An interesting challenge from the original boat...how should we propell it?
7. [Does the boat float? -- &#128679; TBD](./doc/does_the_boat_float.md): With all of the electronics and the other pieces we need to test bouyancy. How can we do this without a water-proofed boat?
8. [Wifi range testing -- &#128679; TBD](./doc/wifi_range_testing.md): how far can the boat be away from the cell phone? what happens when we go out of that range? can we improve that behavior?
9. [A working rudder and propellor -- &#128679; TBD](./doc/rudder_and_propellor.md): This task is about getting a working rudder and propellor. All of the electrical work has been done earlier. Now we need to make sure they manipulate the boat as desired.
10. [Water-proofing the boat -- &#128679; TBD](./doc/waterproofing_the_boat.md): water and electricity don't mix well. Worse yet, water is very corrosive.
11. [Testing on a simulated lake -- &#128679; TBD](./doc/simulated_lake.md): we are now ready to test on a simulated lake.
12. [Testing on a real lake -- &#128679; TBD](./doc/real_lake.md): We are ready to try the boat out on a real lake!

&#9989; = initial documentation drafted

&#128679; = initial documentation to-be-written

If you have any questions, run into any issues, or come up with a better way to approach a task, please enter an **issue** for thie github repository to track the item.
