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

void select_sys_i2c() { tcaselect(7); }

void select_port_d_i2c() { tcaselect(4); }

void select_port_e_i2c() { tcaselect(5); }

void select_port_f_i2c() { tcaselect(6); }

Adafruit_AW9523 badge_io;

Adafruit_AW9523 ms_hexpansion_io_1;
Adafruit_AW9523 ms_hexpansion_io_2;

uint8_t rabbit_red_pin = 0;    // Port D, LS E
uint8_t rabbit_green_pin = 10; // Port D, LS C
uint8_t rabbit_blue_pin = 11;  // Port D, LS D

uint8_t duck_red_pin = 15;   // Port F, LS E
uint8_t duck_green_pin = 14; // Port F, LS D
uint8_t duck_blue_pin = 13;  // Port F, LS C

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit AW9523 GPIO Expander test!");

  Wire.setPins(26, 27);
  Wire.begin();

  select_sys_i2c();

  Serial.print("Badge AW9523 good? : ");
  Serial.println(badge_io.begin(0x58));

  badge_io.pinMode(rabbit_red_pin, AW9523_LED_MODE);
  badge_io.pinMode(rabbit_green_pin, AW9523_LED_MODE);
  badge_io.pinMode(rabbit_blue_pin, AW9523_LED_MODE);

  badge_io.pinMode(duck_red_pin, AW9523_LED_MODE);
  badge_io.pinMode(duck_green_pin, AW9523_LED_MODE);
  badge_io.pinMode(duck_blue_pin, AW9523_LED_MODE);

  select_port_e_i2c();

  Serial.print("MS Hexpansion AW9523 1 good? : ");
  Serial.println(ms_hexpansion_io_1.begin(0x5A));

  Serial.print("MS Hexpansion AW9523 2 good? : ");
  Serial.println(ms_hexpansion_io_2.begin(0x5B));

  for (int i = 0; i < 16; i++) {
    ms_hexpansion_io_1.pinMode(i, AW9523_LED_MODE);
    ms_hexpansion_io_2.pinMode(i, AW9523_LED_MODE);

    ms_hexpansion_io_1.analogWrite(i, 50);
    ms_hexpansion_io_2.analogWrite(i, 50);
  }
}

void loop() {
  select_sys_i2c();
  flash_led(rabbit_red_pin);
  flash_led(rabbit_green_pin);
  flash_led(rabbit_blue_pin);
  flash_led(duck_red_pin);
  flash_led(duck_green_pin);
  flash_led(duck_blue_pin);

  // TODO
}

void flash_led(int led) {
  Serial.print("Flashing LED: ");
  Serial.println(led);

  badge_io.analogWrite(led, 50);
  delay(500);
  badge_io.analogWrite(led, 0);
}
