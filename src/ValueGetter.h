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

#ifndef VALUEGETTER_H
#define VALUEGETTER_H

#pragma once

#include <avr/eeprom.h>
#include <avr/pgmspace.h>


template<typename OutputType>
OutputType getFromProgMem(const OutputType *_address)
{
    return OutputType();
}

template<>
uint8_t getFromProgMem(const uint8_t *_address);

template<>
int8_t getFromProgMem(const int8_t *_address);

template<>
uint16_t getFromProgMem(const uint16_t *_address);

template<>
int16_t getFromProgMem(const int16_t *_address);

template<>
uint32_t getFromProgMem(const uint32_t *_address);

template<>
int32_t getFromProgMem(const int32_t *_address);

template<>
float getFromProgMem(const float *_address);


template<typename OutputType>
OutputType getFromEeprom(const OutputType *_address)
{
    return OutputType();
}

template<>
uint8_t getFromEeprom(const uint8_t *_address);

template<>
int8_t getFromEeprom(const int8_t *_address);

template<>
uint16_t getFromEeprom(const uint16_t *_address);

template<>
int16_t getFromEeprom(const int16_t *_address);

template<>
uint32_t getFromEeprom(const uint32_t *_address);

template<>
int32_t getFromEeprom(const int32_t *_address);

template<>
float getFromEeprom(const float *_address);


enum class ValueLocation : uint8_t {Invalid, RAM, ProgMem, EEPROM};

template<typename OutputType>
class ValueGetter
{
public:
    ValueGetter() : _address{0}, _location{ValueLocation::Invalid} {}
    ValueGetter(const OutputType *address, ValueLocation location = ValueLocation::RAM)
        : _address{address}, _location{location}
    {}

    operator OutputType()
    {
        switch (_location)
        {
            case ValueLocation::RAM:
                return *_address;
            case ValueLocation::ProgMem:
                return getFromProgMem(_address);
            case ValueLocation::EEPROM:
                return getFromEeprom(_address);
            default:
                return OutputType();
        }
    }

    OutputType operator [](size_t index)
    {
        switch (_location)
        {
            case ValueLocation::RAM:
                return _address[index];
            case ValueLocation::ProgMem:
                return getFromProgMem(&_address[index]);
            case ValueLocation::EEPROM:
                return getFromEeprom(&_address[index]);
            default:
                return OutputType();
        }
    }

private:
    const OutputType *_address;
    ValueLocation _location;
};

#endif