// Value Getter
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

#include "ValueGetter.h"

template<>
uint8_t getFromProgMem(const uint8_t *_address)
{
    return pgm_read_byte(_address);
}

template<>
int8_t getFromProgMem(const int8_t *_address)
{
    uint8_t val = pgm_read_byte(_address);
    return reinterpret_cast<int8_t &>(val);
}

template<>
uint16_t getFromProgMem(const uint16_t *_address)
{
    return pgm_read_word(_address);
}

template<>
int16_t getFromProgMem(const int16_t *_address)
{
    uint16_t val = pgm_read_word(_address);
    return reinterpret_cast<int16_t &>(val);
}

template<>
uint32_t getFromProgMem(const uint32_t *_address)
{
    return pgm_read_dword(_address);
}

template<>
int32_t getFromProgMem(const int32_t *_address)
{
    uint32_t val = pgm_read_dword(_address);
    return reinterpret_cast<int32_t &>(val);
}

template<>
float getFromProgMem(const float *_address)
{
    return pgm_read_float(_address);
}
