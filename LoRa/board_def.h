#include <Arduino.h>



#define LORA_V1_0_OLED  0
#define LORA_V1_2_OLED  1



#define LORA_SENDER 0

#if LORA_V1_0_OLED
#include <Wire.h> // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"
#define OLED_CLASS_OBJ  SSD1306Wire
#define OLED_ADDRESS    0x3C
#define OLED_SDA    4
#define OLED_SCL    15
#define OLED_RST    16
//Lora V1.0 sx1276
#define CONFIG_MOSI 27
#define CONFIG_MISO 19
#define CONFIG_CLK  5
#define CONFIG_NSS  18
#define CONFIG_RST  14
#define CONFIG_DIO0 26
#define LORA_PERIOD 868E6

#elif LORA_V1_2_OLED
//Lora V1.2 ds3231
#include <Wire.h> // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"
#define OLED_CLASS_OBJ  SSD1306Wire
#define OLED_ADDRESS    0x3C
#define OLED_SDA    21
#define OLED_SCL    22
#define OLED_RST    -1
#define CONFIG_MOSI 27
#define CONFIG_MISO 19
#define CONFIG_CLK  5
#define CONFIG_NSS  18
#define CONFIG_RST  23
#define CONFIG_DIO0 26
#define LORA_PERIOD 433E6

#define ENABLE_DS3231

#else
//T3_V1.1
#define CONFIG_MOSI 27
#define CONFIG_MISO 19
#define CONFIG_CLK  5
#define CONFIG_NSS  18
#define CONFIG_RST  -1
#define CONFIG_DIO0 26
#define LORA_PERIOD 433E6

#endif