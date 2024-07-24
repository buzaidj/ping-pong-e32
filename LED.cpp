#include "LED.h"

LED::LED(int pin) : pin(pin), isOn(false), onDuration(0), lastChangeTime(0) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void LED::turnOn() {
    digitalWrite(pin, HIGH);
    isOn = true;
    onDuration = 0;
}

void LED::turnOff() {
    digitalWrite(pin, LOW);
    isOn = false;
}

void LED::turnOnFor(unsigned long duration) {
    onDuration = duration;
    lastChangeTime = millis();
    digitalWrite(pin, HIGH);
    isOn = true;
}

void LED::blink() {
    turnOnFor(200);
}

void LED::update() {
    if (isOn && onDuration > 0 && (millis() - lastChangeTime >= onDuration)) {
        turnOff();
    }
}
