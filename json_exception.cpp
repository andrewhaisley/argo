/*
 * Copyright (c) 2017 Andrew Haisley
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/// \file json_exception.cpp The json_exception class implementation.

#include <string.h>
#include <ostream>

#include "common.hpp"
#include "json_exception.hpp"

using namespace NAMESPACE;

json_exception::json_exception(exception_type et) noexcept : exception(), m_type(et)
{
    switch (et)
    {
    case invalid_json_type_e:
        strncpy(m_message, "internal error, invalid json instance type", max_message_length);
        break;
    case not_number_e:
        strncpy(m_message, "instance is not a number", max_message_length);
        break;
    case cant_compare_raw_e:
        strncpy(m_message, "raw values can't be compared", max_message_length);
        break;
    case cant_cast_raw_e:
        strncpy(m_message, "raw values can't be cast", max_message_length);
        break;
    case pointer_not_matched_e:
        strncpy(m_message, "pointer doesn't match a location in the instance", max_message_length);
        break;
    case pointer_token_type_invalid_e:
        strncpy(m_message, "pointer token type is invalid", max_message_length);
        break;
    default:
        strncpy(m_message, "generic", max_message_length);
        break;
    }
}

json_exception::json_exception(exception_type et, const char *json_type_name) noexcept : exception(), m_type(et)
{
    switch (et)
    {
    case not_an_array_e:
        snprintf(m_message, max_message_length, "instance type is %s not array", json_type_name);
        break;
    case not_an_object_e:
        snprintf(m_message, max_message_length, "instance type is %s not object", json_type_name);
        break;
    case not_number_int_or_boolean_e:
        snprintf(m_message, max_message_length, "instance type is %s not integer number or boolean", json_type_name);
        break;
    case not_string_e:
        snprintf(m_message, max_message_length, "instance type is %s not string", json_type_name);
        break;
    case not_number_or_string_e:
        snprintf(m_message, max_message_length, "attempt to set a raw value of an instance of type %s (only works for number and string)", json_type_name);
        break;
    case not_number_e:
        snprintf(m_message, max_message_length, "instance type is %s not number", json_type_name);
        break;
    default:
        strncpy(m_message, "generic", max_message_length);
        break;
    }
}

json_exception::json_exception(
                    exception_type et,
                    const char     *first_json_type_name,
                    const char     *second_json_type_name) noexcept : exception(), m_type(et)
{
    if (et == types_not_comparable_e)
    {
        snprintf(m_message, max_message_length, "attempt to compare %s with %s", first_json_type_name, second_json_type_name);
    }
    else
    {
        strncpy(m_message, "generic", max_message_length);
    }
}

const char *json_exception::what() const noexcept
{
    return m_message;
}

json_exception::exception_type json_exception::get_type() const noexcept
{
    return m_type;
}

std::ostream &NAMESPACE::operator<<(std::ostream& stream, const NAMESPACE::json_exception &e)
{
    return stream << e.what();
}
