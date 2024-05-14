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
  }

  select_sys_i2c();

  badge_io.analogWrite(rabbit_red_pin, 4);
  badge_io.analogWrite(duck_blue_pin, 4);

  select_port_e_i2c();
}

void loop() {
  set_m_intensity(0);
  m_breathe_effect();

  set_m_intensity(0);
  m_scroll_effect();
}

// clang-format off
std::array<std::tuple<int, int>, 29> segments = {
    std::make_tuple(0, 7),
    std::make_tuple(0, 12),
    std::make_tuple(0, 13),
    std::make_tuple(0, 14),
    std::make_tuple(0, 15),
    std::make_tuple(0, 6),
    std::make_tuple(0, 5),
    std::make_tuple(0, 4),
    std::make_tuple(0, 3),
    std::make_tuple(0, 2),
    std::make_tuple(0, 1),
    std::make_tuple(0, 0),
    std::make_tuple(0, 11),
    std::make_tuple(0, 10),
    std::make_tuple(0, 9),
    std::make_tuple(0, 8),
    std::make_tuple(1, 7),
    std::make_tuple(1, 12),
    std::make_tuple(1, 6),
    std::make_tuple(1, 5),
    std::make_tuple(1, 4),
    std::make_tuple(1, 3),
    std::make_tuple(1, 2),
    std::make_tuple(1, 1),
    std::make_tuple(1, 0),
    std::make_tuple(1, 8),
    std::make_tuple(1, 9),
    std::make_tuple(1, 10),
    std::make_tuple(1, 11),
};
// clang-format on

void m_scroll_effect() {
  for (const auto &segment : segments) {
    auto expander = std::get<0>(segment);
    auto pin = std::get<1>(segment);

    set_m_intensity(0);
    if (expander == 0) {
      ms_hexpansion_io_1.analogWrite(pin, 12);
    } else if (expander == 1) {
      ms_hexpansion_io_2.analogWrite(pin, 12);
    }

    Serial.println("-");
    delay(50);
  }
}

void m_breathe_effect() {
  for (int i = 0; i < 24; i++) {
    set_m_intensity(i);
    delay(10);
  }
  for (int i = 22; i > 0; i--) {
    set_m_intensity(i);
    delay(10);
  }
}

void set_m_intensity(int intensity) {
  for (int i = 0; i < 16; i++) {
    ms_hexpansion_io_1.analogWrite(i, intensity);
    ms_hexpansion_io_2.analogWrite(i, intensity);
  }
}
