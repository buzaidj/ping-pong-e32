#include <HardwareSerial.h>
#include <vector>
#include "E32Driver.h"
#include "LED.h"

#define IS_PING  1  // if 0, respond to ping with pong, otherwise send ping
#define BOARD    1  // if 1 my teensy board setup, if 0 my raspberry pi pico w board

#if IS_PING == 1 && BOARD == 1
    #define LoraSerial       Serial8
    #define RX_PIN           34
    #define TX_PIN           35
    #define M0_PIN           33
    #define M1_PIN           37
    #define AUX_PIN          36
    #define MSG_SEND_LED_PIN 6
    #define MSG_RCV_LED_PIN  LED_BUILTIN
    #define SEND_INTERVAL_MS 1000
#elif IS_PING == 0 && BOARD == 0
    #define LoraSerial       Serial1
    #define RX_PIN           17
    #define TX_PIN           16
    #define M0_PIN           18
    #define M1_PIN           19
    #define AUX_PIN          20
    #define AWAKE_LED_PIN    1
    #define MSG_RCV_LED_PIN  0
    #define AWAKE_BLINK_INTERVAL_MS 1000
#elif IS_PING == 1 && BOARD == 0
    #define LoraSerial       Serial1
    #define RX_PIN           17
    #define TX_PIN           16
    #define M0_PIN           18
    #define M1_PIN           19
    #define AUX_PIN          20
    #define MSG_SEND_LED_PIN 0
    #define MSG_RCV_LED_PIN  1
    #define SEND_INTERVAL_MS 1000
#elif IS_PING == 0 && BOARD == 1
    #define LoraSerial       Serial8
    #define RX_PIN           34
    #define TX_PIN           35
    #define M0_PIN           33
    #define M1_PIN           37
    #define AUX_PIN          36
    #define AWAKE_LED_PIN    6
    #define MSG_RCV_LED_PIN  LED_BUILTIN
    #define AWAKE_BLINK_INTERVAL_MS 1000
#else
    #error "Unsupported configuration"
#endif

#if IS_PING == 1
    LED sendLed(MSG_SEND_LED_PIN);
#else
    LED awakeLed(AWAKE_LED_PIN);
#endif

LED rcvLed(MSG_RCV_LED_PIN);
E32Driver e32Driver(LoraSerial, AUX_PIN, M0_PIN, M1_PIN);

void setup() {
    LoraSerial.setRX(RX_PIN);
    LoraSerial.setTX(TX_PIN);

    LoraSerial.begin(9600);
    Serial.begin(9600);

    e32Driver.putToSleep();
    bool success = e32Driver.setParams();
    if (success) {
        Serial.println("Successfully set params");
    } else {
        Serial.println("Failed to set params");
    }
    e32Driver.wakeUp();

    #if IS_PING == 0
        awakeLed.turnOn();
    #endif
}

#if IS_PING == 1

unsigned long previousMillis = millis();
void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= SEND_INTERVAL_MS) {
        previousMillis = currentMillis;

        const char *msg = "ping";
        e32Driver.sendMessage((uint8_t*)msg, strlen(msg) + 1);
        sendLed.blink();
    }

    if (e32Driver.available()) {
        std::vector<uint8_t> msg;
        e32Driver.readMessage(msg);

        String s = "";
        for (auto c : msg) {
            s += (char)c;
        }

        if (s.startsWith("pong")) {
            rcvLed.blink();
        }
    }

    rcvLed.update();
    sendLed.update();
}

#else // pong

void loop() {
    if (e32Driver.available()) {
        std::vector<uint8_t> msg;
        e32Driver.readMessage(msg);

        char* charPtr = reinterpret_cast<char*>(msg.data());
        String s(charPtr);

        if (s.startsWith("ping")) {
            const char *response = "pong";
            e32Driver.sendMessage((uint8_t*)response, strlen(response) + 1);
            rcvLed.blink();
        }
    }

    awakeLed.update();
    rcvLed.update();
}

#endif
