// Copyright (C) 2023  Joshua Booth

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <Arduino.h>
#include <unity.h>

#include "ValueGetter.h"
#include "avr/eeprom.h"

#define MAX_MESSAGE_LEN 255

char message[MAX_MESSAGE_LEN];

uint32_t *p_eeprom_test_data = reinterpret_cast<uint32_t *>(256);
const uint32_t eeprom_test_expected_value = 0x12345678;

void setUp(void) {
  // Configure test value
  eeprom_update_dword(p_eeprom_test_data, eeprom_test_expected_value);
}

void tearDown(void) {
// clean stuff up here
}

struct DummyType { uint8_t data; };

template<typename T>
void test_ValueGetter_RAM()
{
  ValueGetter<T> vg;

  T expected = static_cast<T>(5.0 * pow(2, (sizeof(T) - 1) * 8));

  vg = ValueGetter<T>(&expected);

  T actual = vg;

  TEST_ASSERT_EQUAL(expected, actual);
}

template<typename T>
void test_ValueGetter_Array_RAM()
{
  ValueGetter<T> vg;

  T data[2] = {
    static_cast<T>(1.0 * pow(2, (sizeof(T) - 1) * 8)),
    static_cast<T>(2.0 * pow(2, (sizeof(T) - 1) * 8))
  };

  vg = ValueGetter<T>(data);

  T expected = data[1];
  T actual = vg[1];

  TEST_ASSERT_EQUAL(expected, actual);
}

template<typename T>
void test_ValueGetter_PROGMEM()
{
  ValueGetter<T> vg;

  static const PROGMEM T expected = static_cast<T>(5.0 * pow(2, (sizeof(T) - 1) * 8));

  vg = ValueGetter<T>(&expected, ValueLocation::ProgMem);

  T actual = vg;

  TEST_ASSERT_EQUAL(expected, actual);
}

template<typename T>
void test_ValueGetter_Array_PROGMEM()
{
  ValueGetter<T> vg;

  static const PROGMEM T data[2] = {
    static_cast<T>(1.0 * pow(2, (sizeof(T) - 1) * 8)),
    static_cast<T>(2.0 * pow(2, (sizeof(T) - 1) * 8))
  };

  vg = ValueGetter<T>(data, ValueLocation::ProgMem);

  T expected = static_cast<T>(2.0 * pow(2, (sizeof(T) - 1) * 8));
  T actual = vg[1];

  TEST_ASSERT_EQUAL(expected, actual);
}

void test_ValueGetter_PROGMEM_DummyType()
{
  DummyType expected;

  ValueGetter<DummyType> vg;

  static const PROGMEM DummyType stored = { 20 };

  vg = ValueGetter<DummyType>(&stored, ValueLocation::ProgMem);

  DummyType actual = vg;

  TEST_ASSERT_EQUAL(expected.data, actual.data);
  TEST_ASSERT_NOT_EQUAL(20, actual.data);
}

template<typename T>
void test_ValueGetter_EEPROM()
{
  ValueGetter<T> vg;

  vg = ValueGetter<T>(reinterpret_cast<T *>(p_eeprom_test_data), ValueLocation::EEPROM);

  T actual = vg;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
  TEST_ASSERT_EQUAL(reinterpret_cast<const T&>(eeprom_test_expected_value), actual);
#pragma GCC diagnostic pop
}

void test_ValueGetter_EEPROM_DummyType()
{
  DummyType expected;

  ValueGetter<DummyType> vg;

  vg = ValueGetter<DummyType>(reinterpret_cast<DummyType *>(p_eeprom_test_data), ValueLocation::EEPROM);

  DummyType actual = vg;

  TEST_ASSERT_EQUAL(expected.data, actual.data);
  TEST_ASSERT_NOT_EQUAL(eeprom_test_expected_value, actual.data);
}
void test_load_RAM()
{
  struct M {
    int a;
    float b;
    uint8_t c;
  };

  M expected = {
    1, 2.75, 255
  };

  M actual = {
    0, 0, 0
  };

  ValueGetter<M> mg = ValueGetter<M>(&expected);

  mg.load(actual, actual.a);
  mg.load(actual, actual.b);
  mg.load(actual, actual.c);

  TEST_ASSERT_EQUAL(expected.a, actual.a);
  TEST_ASSERT_EQUAL(expected.b, actual.b);
  TEST_ASSERT_EQUAL(expected.c, actual.c);
}


void test_load_PROGMEM()
{
  struct M {
    int a;
    float b;
    uint8_t c;
  };

  M expected = {
    1, 2.75, 255
  };

  static const PROGMEM M m = {
    1, 2.75, 255
  };

  M actual = {
    0, 0, 0
  };

  ValueGetter<M> mg = ValueGetter<M>(&m, ValueLocation::ProgMem);

  mg.load(actual, actual.a);
  mg.load(actual, actual.b);
  mg.load(actual, actual.c);

  TEST_ASSERT_EQUAL(expected.a, actual.a);
  TEST_ASSERT_EQUAL(expected.b, actual.b);
  TEST_ASSERT_EQUAL(expected.c, actual.c);
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();    // IMPORTANT LINE!

  RUN_TEST((test_ValueGetter_RAM<uint8_t>));
  RUN_TEST((test_ValueGetter_RAM<int8_t>));
  RUN_TEST((test_ValueGetter_RAM<uint16_t>));
  RUN_TEST((test_ValueGetter_RAM<int16_t>));
  RUN_TEST((test_ValueGetter_RAM<uint32_t>));
  RUN_TEST((test_ValueGetter_RAM<int32_t>));
  RUN_TEST((test_ValueGetter_RAM<float>));

  RUN_TEST((test_ValueGetter_Array_RAM<uint8_t>));
  RUN_TEST((test_ValueGetter_Array_RAM<int8_t>));
  RUN_TEST((test_ValueGetter_Array_RAM<uint16_t>));
  RUN_TEST((test_ValueGetter_Array_RAM<int16_t>));
  RUN_TEST((test_ValueGetter_Array_RAM<uint32_t>));
  RUN_TEST((test_ValueGetter_Array_RAM<int32_t>));
  RUN_TEST((test_ValueGetter_Array_RAM<float>));

  RUN_TEST((test_ValueGetter_PROGMEM<uint8_t>));
  RUN_TEST((test_ValueGetter_PROGMEM<int8_t>));
  RUN_TEST((test_ValueGetter_PROGMEM<uint16_t>));
  RUN_TEST((test_ValueGetter_PROGMEM<int16_t>));
  RUN_TEST((test_ValueGetter_PROGMEM<uint32_t>));
  RUN_TEST((test_ValueGetter_PROGMEM<int32_t>));
  RUN_TEST((test_ValueGetter_PROGMEM<float>));
  RUN_TEST(test_ValueGetter_PROGMEM_DummyType);

  RUN_TEST((test_ValueGetter_Array_PROGMEM<uint8_t>));
  RUN_TEST((test_ValueGetter_Array_PROGMEM<int8_t>));
  RUN_TEST((test_ValueGetter_Array_PROGMEM<uint16_t>));
  RUN_TEST((test_ValueGetter_Array_PROGMEM<int16_t>));
  RUN_TEST((test_ValueGetter_Array_PROGMEM<uint32_t>));
  RUN_TEST((test_ValueGetter_Array_PROGMEM<int32_t>));
  RUN_TEST((test_ValueGetter_Array_PROGMEM<float>));

  RUN_TEST((test_ValueGetter_EEPROM<uint8_t>));
  RUN_TEST((test_ValueGetter_EEPROM<int8_t>));
  RUN_TEST((test_ValueGetter_EEPROM<uint16_t>));
  RUN_TEST((test_ValueGetter_EEPROM<int16_t>));
  RUN_TEST((test_ValueGetter_EEPROM<uint32_t>));
  RUN_TEST((test_ValueGetter_EEPROM<int32_t>));
  RUN_TEST((test_ValueGetter_EEPROM<float>));
  RUN_TEST(test_ValueGetter_EEPROM_DummyType);

  RUN_TEST(test_load_RAM);
  RUN_TEST(test_load_PROGMEM);

  UNITY_END(); // stop unit testing
}

void loop() {
}

