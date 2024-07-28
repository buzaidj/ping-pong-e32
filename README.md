# ping-pong

A simple example of how to interface with the the EByte E32-900T20D. The E32-900T20D is LoRA module that can be had for about $7.00 in AliExpress and supports sending small byte streams to other modules. I've gotten a range of about a half mile with these attached to 3dBi gain 915 MhZ antennas.

`E32Driver.h` is a small library for the module. It includes methods to initialize the module on a certain channel, read messages, and send messages. It's somewhat extensible, but relies on delays and has a few TODO's. 

`ping-pong.ino` is program that either (1) listens for pings, turns on an LED when it receives one, and sends pongs, or (2) sends pings and turns on a LED when it receives a response from pong.