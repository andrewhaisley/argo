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

/// \file json_parser_exception.cpp The json_parser_exception class implementation.

#include "common.hpp"
#include "json_parser_exception.hpp"

using namespace std;
using namespace NAMESPACE;

const char *json_parser_exception::get_main_message()
{
    switch (m_type)
    {
    case invalid_character_e:
        return "invalid character";
    case invalid_number_e:
        return "invalid number format";
    case number_out_of_range_e:
        return "number out of range";
    case number_too_long_e:
        return "number too long";
    case string_too_long_e:
        return "string too long";
    case unexpected_eof_e:
        return "unexpected end of file";
    case unexpected_token_e:
        return "unexpected token";
    default:
        return "generic";
    }

}

json_parser_exception::json_parser_exception(exception_type et, size_t byte_index) noexcept:
                    json_exception(et),
                    m_byte_index(byte_index)
{
    snprintf(m_message, max_message_length, "parser exception, %s, at or near byte %lu", get_main_message(), m_byte_index);
}

json_parser_exception::json_parser_exception(exception_type et, char c, size_t byte_index) noexcept:
                    json_exception(et),
                    m_byte_index(byte_index)
{
    snprintf(m_message, max_message_length, "parser exception, %s, at or near byte %lu : 0x%02x", get_main_message(), m_byte_index, c);
}
 
json_parser_exception::json_parser_exception(exception_type et, const string &s, size_t byte_index) noexcept:
                    json_exception(et),
                    m_byte_index(byte_index)
{
    snprintf(m_message, max_message_length, "parser exception, %s, at or near byte %lu : %s", get_main_message(), m_byte_index, s.c_str());
}

json_parser_exception::json_parser_exception(exception_type et, size_t st, size_t byte_index) noexcept:
                    json_exception(et),
                    m_byte_index(byte_index)
{
    snprintf(m_message, max_message_length, "parser exception, %s, at or near byte %lu : %ld", get_main_message(), m_byte_index, st);
}

size_t json_parser_exception::get_byte_index() const noexcept
{
    return m_byte_index;
}
