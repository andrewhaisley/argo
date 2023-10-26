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

/// \file json_utf8_exception.cpp The json_utf8_exception class implementation.

#include <string>

#include "common.hpp"
#include "json_utf8_exception.hpp"

using namespace std;
using namespace NAMESPACE;

json_utf8_exception::json_utf8_exception(exception_type et, const string &s) noexcept :
                    json_exception(et), m_byte_index(0)
{
    snprintf(m_message, max_message_length, "%s : %s", get_main_message(), s.c_str());
}

json_utf8_exception::json_utf8_exception(exception_type et, char c) noexcept :
                    json_exception(et), m_byte_index(0)
{
    snprintf(m_message, max_message_length, "%s : 0x%02x", get_main_message(), c);
}

json_utf8_exception::json_utf8_exception(exception_type et, unsigned int i) noexcept :
                    json_exception(et), m_byte_index(0)
{
    snprintf(m_message, max_message_length, "%s : %d", get_main_message(), i);
}

void json_utf8_exception::add_byte_index(size_t byte_index) noexcept
{
    snprintf(m_message, max_message_length, "%s at or near byte %zu", get_main_message(), byte_index);
}

size_t json_utf8_exception::get_byte_index() const noexcept
{
    return m_byte_index;
}

const char *json_utf8_exception::get_main_message()
{
    switch (m_type)
    {
    case invalid_string_escape_e:
        return "invalid escape character found in string";
    case invalid_string_encoding_e:
        return "string encoding is invalid";
    case invalid_unicode_e:
        return "invalid unicode value";
    case invalid_utf8_sequence_length_e:
        return "invalid UTF-8 sequence length";
    case invalid_hex_char_e:
        return "invalid hex character";
    case invalid_hex_number_e:
        return "invalid hex number (must be 0-15)";
    case invalid_utf8_char_e:
        return "0 byte detected in UTF-8 sequence other than code point 0";
    default:
        return "generic";
    }
}
