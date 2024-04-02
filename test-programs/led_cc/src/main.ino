#include "Wire.h"
#include <Adafruit_AW9523.h>

static constexpr auto TCAADDR = 0x77;

void tcaselect(uint8_t i) {
  if (i > 7)
    return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

Adafruit_AW9523 aw;

uint8_t led_pin = 6; // Port E, LS 5

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit AW9523 GPIO Expander test!");

  Wire.setPins(26, 27);
  Wire.begin();

  tcaselect(7);

  if (!aw.begin(0x58)) {
    Serial.println("AW9523 not found? Check wiring!");
    while (1)
      delay(10); // halt forever
  }
  Serial.println("AW9523 found!");

  aw.pinMode(led_pin, AW9523_LED_MODE);
}

uint8_t x = 0;

void loop() {
  aw.analogWrite(led_pin, x++);
  Serial.println(x);
  delay(10);
}
