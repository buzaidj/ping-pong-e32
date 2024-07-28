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
    // Must be asleep when calling set params. Returns true if successful.
    bool setParams();
    // Returns false is the module is not available for sending a message.
    bool sendMessage(uint8_t* msg, uint16_t len);
    int available();
    // TODO: change this to support reading one character a time. The caller can
    // decide how they'd like to join sequences of messages.
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
