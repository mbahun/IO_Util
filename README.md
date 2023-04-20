# I/O Utility
An “old fashioned” Windows Desktop native (toy) application written in C++ using MFC that communicates with I/O hardware using communication over serial port.

## The problem
There are plenty IO devices available (e.g., https://www.hw-group.com/device/io-controller-2) to buy. Each device uses different protocol and sometimes it is useful to simulate test scenarios, record predefined output actions, monitor inputs, etc. 

## The solution
I/O Util Utility enables support for multiple devices and protocols as well as creation of “playable programs”. Program is a set of up to 8 signals (outputs) and signal monitors (inputs) where each output is defined by state (on / off), start time and duration. “Playing” the program sends device specific commands to serial port thus controlling hardware outputs. At the same time, it listens for input changes. 

https://user-images.githubusercontent.com/127845072/233173590-090bc94b-a21f-4e2f-9968-cd54c7ac564a.mp4

Example: If LEDs are connected to device outputs, “on” signals will cause LED to emit light, while “off” signal will turn the LED off. If an input device (e.g., physical button) is connected to one of device’s inputs, pressing it can pause the current program, start another, etc. 
