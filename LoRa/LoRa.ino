#include <SPI.h>
#include <LoRa.h>
#include "ds3231.h"

OLED_CLASS_OBJ display(OLED_ADDRESS, OLED_SDA, OLED_SCL);

void setup()
{
    Serial.begin(115200);
    while (!Serial);

    if (OLED_RST > 0) {
        pinMode(OLED_RST, OUTPUT);
        digitalWrite(OLED_RST, HIGH);
        delay(100);
        digitalWrite(OLED_RST, LOW);
        delay(100);
        digitalWrite(OLED_RST, HIGH);
    }

    display.init();
    display.flipScreenVertically();
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, "LoRa Receiver");
    display.display();

    String info = ds3231_test();
    if (info != "") {
        display.clear();
        display.setFont(ArialMT_Plain_16);
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, info);
        display.display();
        delay(2000);
    }

    SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
    LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
    Serial.println("LoRa Receiver");
    if (!LoRa.begin(LORA_PERIOD)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}

int count = 0;

void loop()
{
#if LORA_SENDER
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Send Count:" + String(count));
    display.display();
    LoRa.beginPacket();
    LoRa.print("lora: ");
    LoRa.print(count);
    LoRa.endPacket();
    ++count;
    delay(2500);
#else
    if (LoRa.parsePacket()) {
        String recv = "";
        while (LoRa.available()) {
            recv += (char)LoRa.read();
        }
        count++;
        display.clear();
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, recv);
        String info = "[" + String(count) + "]" + "RSSI " + String(LoRa.packetRssi());
        display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 16, info);
        display.display();
    }
#endif
}
