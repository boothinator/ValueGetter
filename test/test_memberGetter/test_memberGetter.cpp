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


void test_memberGetter()
{
  struct M {
    int a;
    float b;
    uint8_t c;
  };

  struct M2 {
    M2(const M * const address, const ValueLocation location)
      : _address{address}, _location{location} {}

    const M * _address;
    ValueLocation _location;

public:
    const OffsetGetter<int, M, offsetof(M, a), M2> a = OffsetGetter<int, M, offsetof(M, a), M2>(this);
  };

  M m = {
    1, 2, 3
  };

  M2 m2 = M2(&m, ValueLocation::RAM);

  Serial.println(sizeof(M2));
  Serial.println(sizeof(m2.a));

  TEST_ASSERT_EQUAL(m.a, m2.a);
  //TEST_ASSERT_EQUAL(m.b, m2.b);
  //TEST_ASSERT_EQUAL(m.c, m2.c);
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();    // IMPORTANT LINE!

  RUN_TEST(test_memberGetter);

  UNITY_END(); // stop unit testing
}

void loop() {
}

