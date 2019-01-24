#include <SPI.h>
#include <LoRa.h>
#include "board_def.h"

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
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, "LoRa Sender");
    display.display();

    SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
    LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
    Serial.println("LoRa Sender");
    if (!LoRa.begin(LORA_PERIOD)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}

int count = 0;

void loop()
{
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
}
