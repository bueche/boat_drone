# boat_drone
This repository supports a learning activity to build a small boat drone. It is organized into a set of learning tasks and can be useful basis for lab or class on the topic. 

## Repository goal in more detail
There is an experimental small electronic motor boat outlined on one of the 3D printing model websites ([here](https://cults3d.com/en/3d-model/game/motor-boat-rc-small-experimental). Although that website provided the STL files, very little information was provided on the electronics. In addition, some of the STL files had issues as well. In this repository we provide a series of step-by-step activities to take the basic model and enhance it so that it can be controlled by a standard iOS or Android mobile phone. 

## BOM

## Tasks
1. [Software Environment setup and getting ESP32 to blink](./doc/setup.md): this is an initial task to get the PC environment setup to compile and load some simple code to the ESP32.
2. [Controlling the Servo (Rudder) using the EPS32](./doc/basic_servo.md). In this task we learn how to setup and control the servo from the ESP32. This servo will eventually power the rudder of the boat drone.
3. [Controlling the Servo (Rudder) using a mobile phone and ESP32 Ethernet and Webserver](./doc/esp32_wifi_and_servo_control.md): We fire up the ESP32's wifi and run a web server to allow us to control the servo from a mobile phone.
4. [Wiring DC motor and battery](./doc/dc_motor_and_battery.md): In this task we hookup the DC motor and the battery to the boat electronics.
5. [Propelling_the_boat](./doc/propelling_the_boat.md): An interesting challenge from the original boat...how should we propell it?
6. [Powering the rudder and propellor](./doc/rudder_and_propellor.md): This task is about getting a working rudder and propellor.
7. [Does the boat float?](./doc/does_the_boat_float.md): With all of the electronics and the other pieces we need to test bouyancy. 
8. [Wifi range testing](./doc/wifi_range_testing.md): how far can the boat be away from the cell phone? what happens when we go out of that range? can we improve that behavior?
9. [Water-proofing the boat](./doc/waterproofing_the_boat.md): water and electricity don't mix well. 
10. [Testing on a simulated lake](./doc/simulated_lake.md): we are now ready to test on a simulated lake.
11. [Testing on a real lake](./doc/real_lake.md): We are ready to try the boat out on a real lake!

