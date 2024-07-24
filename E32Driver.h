#ifndef E32DRIVER_H
#define E32DRIVER_H

#include <Arduino.h>
#include <vector>
#include <array>

class E32Driver {
public:
    E32Driver(HardwareSerial &serialPort, int aux, int m0, int m1);

    void putToSleep();
    void wakeUp();
    bool setParams();
    bool sendMessage(uint8_t* msg, uint16_t len);
    int available();
    void readMessage(std::vector<uint8_t>& msg);

private:
    HardwareSerial &serial;
    int auxPin;
    int m0Pin;
    int m1Pin;

    bool readConfig(std::array<uint8_t, 6>& expectedResult);
    bool isReady();
};

#endif // E32DRIVER_H
