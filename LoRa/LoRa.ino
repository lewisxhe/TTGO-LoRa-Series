#include <SPI.h>
#include <LoRa.h>
#include "ds3231.h"
#include <WiFi.h>
#include <SD.h>
#include <Button2.h>

OLED_CLASS_OBJ display(OLED_ADDRESS, OLED_SDA, OLED_SCL);

#define WIFI_SSID       ""
#define WIFI_PASSWORD   ""

#if defined(BUTTON_1)
Button2 btn(BUTTON_1);
#endif

#if defined(BUTTON_1)
void pressHander(Button2 &b)
{
    Serial.println("press BUTTON 0 goto sleep\n");
    display.displayOff();
    LoRa.sleep();
    delay(1000);
    esp_sleep_enable_ext0_wakeup((gpio_num_t )BUTTON_1, LOW);
    esp_deep_sleep_start();
}
#endif

#include <Ticker.h>
Ticker tick;

void btmloop()
{
    btn.loop();
}

void setup()
{
    Serial.begin(115200);
    while (!Serial);

#if defined(BUTTON_1)
    btn.setLongClickHandler(pressHander);
    tick.attach_ms(30,btmloop);
#endif

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
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, LORA_SENDER ? "LoRa Sender" : "LoRa Receiver");
    display.display();
    delay(2000);

    if (SDCARD_CS >  0) {
        display.clear();
        SPIClass sdSPI(VSPI);
        sdSPI.begin(SDCARD_SCLK, SDCARD_MISO, SDCARD_MOSI, SDCARD_CS);
        if (!SD.begin(SDCARD_CS, sdSPI)) {
            display.drawString(display.getWidth() / 2, display.getHeight() / 2, "SDCard  FAIL");
        } else {
            display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 16, "SDCard  PASS");
            uint32_t cardSize = SD.cardSize() / (1024 * 1024);
            display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Size: " + String(cardSize) + "MB");
        }
        display.display();
        delay(2000);
    }


    String info = ds3231_test();
    if (info != "") {
        display.clear();
        display.setFont(ArialMT_Plain_16);
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, info);
        display.display();
        delay(2000);
    }

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        display.clear();
        Serial.println("WiFi Connect Fail");
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "WiFi Connect Fail");
        display.display();
        delay(2000);
        esp_restart();
    }
    Serial.print("Connected : ");
    Serial.println(WiFi.SSID());
    Serial.print("IP:");
    Serial.println(WiFi.localIP().toString());
    display.clear();
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, "IP:" + WiFi.localIP().toString());
    display.display();
    delay(2000);

    SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
    LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
    if (!LoRa.begin(BAND)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
    if (!LORA_SENDER) {
        display.clear();
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "LoraRecv Ready");
        display.display();
    }
}

int count = 0;

void loop()
{
    static uint64_t timestamp = 0;
// #if defined(BUTTON_1)
//     btn.loop();
// #endif
#if LORA_SENDER
    if (millis() - timestamp > 2500) {
        timestamp = millis();
        int32_t rssi;
        if (WiFi.status() == WL_CONNECTED) {
            rssi = WiFi.RSSI();
            display.clear();
            display.setTextAlignment(TEXT_ALIGN_CENTER);
            display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Send RSSI:" + String(rssi));
            display.display();
            LoRa.beginPacket();
            LoRa.print("WiFi RSSI: ");
            LoRa.print(rssi);
            LoRa.endPacket();
        } else {
            Serial.println("WiFi Connect lost ...");
        }
    }
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
