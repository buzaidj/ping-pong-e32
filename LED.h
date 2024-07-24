#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED {
public:
    LED(int pin);
    void turnOn();
    void turnOff();
    void turnOnFor(unsigned long duration);
    void blink();
    void update();

private:
    int pin;
    bool isOn;
    unsigned long onDuration;
    unsigned long lastChangeTime;
};

#endif // LED_H
