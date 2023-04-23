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


template<typename T>
T getFromProgMem(const T *_address)
{
    return T();
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


template<typename T>
T getFromEeprom(const T *_address)
{
    return T();
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

template<typename T>
class ValueGetter
{
public:
    ValueGetter() : _address{0}, _location{ValueLocation::Invalid} {}
    ValueGetter(const T *address, ValueLocation location = ValueLocation::RAM)
        : _address{address}, _location{location}
    {}

    operator T() const
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
                return T();
        }
    }

    T operator [](size_t index) const
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
                return T();
        }
    }

    template <typename MemberType>
    void load(T &object, MemberType &member) const
    {
        size_t offset = reinterpret_cast<size_t>(&member) - reinterpret_cast<size_t>(&object);

        if (offset + sizeof(MemberType) > sizeof(T))
        {
            return;
        }

        const MemberType * p = reinterpret_cast<const MemberType *>(
            reinterpret_cast<const uint8_t *>(_address) + offset
        );

        switch (_location)
        {
            case ValueLocation::RAM:
                member =  *p;
                break;
            case ValueLocation::ProgMem:
                member = getFromProgMem(p);
                break;
            case ValueLocation::EEPROM:
                member = getFromEeprom(p);
                break;
            default:
                break;
        }
    }

private:
    const T *_address;
    ValueLocation _location;
};


#endif