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

#define MAX_MESSAGE_LEN 255

char message[MAX_MESSAGE_LEN];


void setUp(void) {
}

void tearDown(void) {
// clean stuff up here
}


void test_memberGetter_RAM()
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

  MemberGetter<M> mg = MemberGetter<M>(&expected);

  mg.load(actual, actual.a);
  mg.load(actual, actual.b);
  mg.load(actual, actual.c);

  TEST_ASSERT_EQUAL(expected.a, actual.a);
  TEST_ASSERT_EQUAL(expected.b, actual.b);
  TEST_ASSERT_EQUAL(expected.c, actual.c);
}


void test_memberGetter_PROGMEM()
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

  MemberGetter<M> mg = MemberGetter<M>(&m, ValueLocation::ProgMem);

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

  RUN_TEST(test_memberGetter_RAM);
  RUN_TEST(test_memberGetter_PROGMEM);

  UNITY_END(); // stop unit testing
}

void loop() {
}

