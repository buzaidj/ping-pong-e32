# ping-pong

A simple example of how to interface with the the E32-900T20D. The E32-900T20D is LoRA module that supports sending small byte streams to other modules. I've gotten a range of about of a mile with these and some 915MhZ antennas attached.

`E32Driver.h` is a small library I built for the module. It includes methods to initialize the module on a certain channel, read messages, and send messages. It's pretty extensible, but has a few TODO's. 

`ping-pong.ino` is program that either (1) listens for pings, turns on an LED when it receives one, and sends pongs, or (2) sends pings and turns on a LED when it receives a response from pong.