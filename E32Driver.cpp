#include "E32Driver.h"

E32Driver::E32Driver(HardwareSerial &serialPort, int aux, int m0, int m1)
    : serial(serialPort), auxPin(aux), m0Pin(m0), m1Pin(m1) {
    pinMode(m0, OUTPUT);
    pinMode(m1, OUTPUT);
}

void E32Driver::putToSleep() {
    digitalWrite(m0Pin, HIGH);
    digitalWrite(m1Pin, HIGH);
    delay(50);
}

void E32Driver::wakeUp() {
    digitalWrite(m0Pin, LOW);
    digitalWrite(m1Pin, LOW);
    delay(50);
}

bool E32Driver::setParams() {
    // The command outlined in section 7.5 of the E32-900T20D manual
    // https://www.ebyte.com/en/pdf-down.aspx%3Fid%3D2520&ved=2ahUKEwid5s_43cqHAxVqFTQIHUgzBe0QFnoECBAQAQ&usg=AOvVaw1q1a_G1t4Mmccc89gPNgeX
    uint8_t HEAD = 0xC0;
    uint8_t ADDH = 0xFF;
    uint8_t ADDL = 0xFF;
    uint8_t SPED = 0b00 << 6;
    SPED |= 0b011 << 3;
    SPED |= 0b000;
    uint8_t CHAN = 0b000 << 5;
    // On the E32-900T20D channels 0x00-0x45 correspond to 862 to 931MHz in 1 MhZ increments.
    // Set to at least 0x28 to legally use this module in North America (40).
    CHAN |= 0x3A;
    uint8_t OPTION = 0 << 7;
    OPTION |= 1 << 6;
    OPTION |= 0b000 << 3;
    OPTION |= 0b1 << 2;
    OPTION |= 0b00;

    std::array<uint8_t, 6> params = {HEAD, ADDH, ADDL, SPED, CHAN, OPTION};

    serial.write(params.data(), params.size());

    delay(50);

    bool configSuccess = readConfig(params);
    return configSuccess;
}

bool E32Driver::sendMessage(uint8_t* msg, uint16_t len) {
    // TODO: Fix isReady check. I don't think the AUX pin is working correctly on the rPI.
    // if (!isReady()) return false;
    serial.write(msg, len);
    return true;
}

int E32Driver::available() {
    return serial.available();
}

void E32Driver::readMessage(std::vector<uint8_t>& msg) {
    // FIXME: before adding this delay, serial.available() was reporting 1
    // and each character was being read faster than the E32900T20D module could
    // produce them. Adding this delay helped, but it's not ideal.
    delay(10);
    while (serial.available()) {
        uint8_t byte_read = serial.read();
        msg.push_back(byte_read);
        Serial.println(byte_read);
    }
}

bool E32Driver::readConfig(std::array<uint8_t, 6>& expectedResult) {
    uint8_t commands[] = {0xC1, 0xC1, 0xC1};
    serial.write(commands, sizeof(commands));

    delay(50);

    // We look for expectedResult as a sub-buffer of the current
    // serial buffer, until we run out of serial. This discards
    // messages that are received in the initializion steps and
    // may appear in this buffer.
    std::array<uint8_t, 6> lastSix;
    size_t index = 0;
    size_t bytesRead = 0;

    while (serial.available() > 0) {
        int curr_byte = serial.read();

        if (curr_byte == -1) {
            Serial.println("bad byte");
            return false;
        }

        lastSix[index] = static_cast<uint8_t>(curr_byte);
        index = (index + 1) % 6;
        bytesRead++;

        if (bytesRead >= 6) {
            bool match = true;
            for (size_t i = 0; i < 6; ++i) {
                Serial.println(lastSix[index + i] % 6);
                Serial.println(expectedResult[index + i] % 6);
                if (lastSix[(index + i) % 6] != expectedResult[i]) {
                    match = false;
                    break;
                }
            }

            if (match) {
                return true;
            }
        }
    }

    return false;
}


bool E32Driver::isReady() {
    return digitalRead(auxPin) == HIGH;
}
