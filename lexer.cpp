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

/// \file lexer.cpp The lexer class implementation.

#include "common.hpp"
#include "lexer.hpp"
#include "json_parser_exception.hpp"

using namespace std;
using namespace NAMESPACE;

lexer::lexer(reader &r, size_t max_token_length) :
                            m_reader(r),
                            m_last_put_back(false),
                            m_max_token_length(max_token_length),
                            m_buffer(new char[max_token_length])
{
}

lexer::~lexer()
{
    delete [] m_buffer;
}

void lexer::consume_white_space()
{
    while (true)
    {
        int c = m_reader.next();

        if ((c == 0x20) || (c == 0x09) || (c == 0x0A) || (c == 0x0D))
        {
            continue;
        }
        else
        {
            m_reader.put_back(c);
            return;
        }
    }
}

void lexer::read_token()
{
    consume_white_space();
    int c = m_reader.next();

    if (isdigit(c))
    {
        m_reader.put_back(c);
        read_number();
    }
    else
    {
        switch (c)
        {
        case '[':
            m_token = token(token::begin_array_e);
            break;
        case ']':
            m_token = token(token::end_array_e);
            break;
        case '{':
            m_token = token(token::begin_object_e);
            break;
        case '}':
            m_token = token(token::end_object_e);
            break;
        case ':':
            m_token = token(token::name_separator_e);
            break;
        case ',':
            m_token = token(token::value_separator_e);
            break;
        case '-':
            m_reader.put_back(c);
            read_number();
            break;
        case '"':
            read_string();
            break;
        case 'f':
            read_false();
            break;
        case 't':
            read_true();
            break;
        case 'n':
            read_null();
            break;
        case EOF:
            throw json_parser_exception(json_parser_exception::unexpected_eof_e, m_reader.get_byte_index());
        default:
            throw json_parser_exception(json_parser_exception::invalid_character_e, m_reader.get_byte_index());
        }
    }
}

void lexer::append_to_number_buffer(char *s, size_t &index, int c)
{
    if (index >= m_max_token_length)
    {
        throw json_parser_exception(
                json_parser_exception::number_too_long_e,
                m_max_token_length,
                m_reader.get_byte_index());
    }

    s[index++] = c;
}

void lexer::throw_number_exception(int c)
{
    if (c == EOF)
    {
        throw json_parser_exception(json_parser_exception::unexpected_eof_e, m_reader.get_byte_index());
    }
    else
    {
        throw json_parser_exception(
                json_parser_exception::invalid_number_e,
                static_cast<char>(c),
                m_reader.get_byte_index());
    }
}

size_t lexer::read_digits(size_t &index)
{
    size_t num_digits = 0;

    while (true)
    {
        int c = m_reader.next();
        if (isdigit(c))
        {
            append_to_number_buffer(m_buffer, index, c);
            num_digits++;
        }
        else
        {
            m_reader.put_back(c);
            return num_digits;
        }
    }
}

void lexer::read_number()
{
    int c = m_reader.next();

    size_t n = 0;
    bool is_double = false;
    size_t num_int_digits = 0;

    // - or digit
    if (isdigit(c))
    {
        append_to_number_buffer(m_buffer, n, c);
        num_int_digits++;
    }
    else if (c == '-')
    {
        append_to_number_buffer(m_buffer, n, c);
    }
    else
    {
        throw_number_exception(c);
    }

    // some digits
    while (true)
    {
        c = m_reader.next();

        if (isdigit(c))
        {
            append_to_number_buffer(m_buffer, n, c);
            num_int_digits++;
        }
        else
        {
            m_reader.put_back(c);
            break;
        }
    }

    if (num_int_digits == 0)
    {
        // Must be at least one digit in the buffer
        throw_number_exception(c);
    }
    else if (num_int_digits > 1 && m_buffer[n - num_int_digits] == '0')
    {
        // If there's more than one digit, the first one must not be zero. No leading zeroes
        // are allowed by the standard.
        throw_number_exception('0');
    }

    c = m_reader.next();

    // maybe followed by a point
    if (c == '.')
    {
        append_to_number_buffer(m_buffer, n, c);
        is_double = true;

        // followed by more digits
        if (read_digits(n) == 0)
        {
            throw_number_exception(c);
        }

        c = m_reader.next();
    }

    // e/E
    if (c == 'e' || c == 'E')
    {
        append_to_number_buffer(m_buffer, n, c);
        is_double = true;

        // +/-
        c = m_reader.next();

        if (c == '+' || c == '-')
        {
            append_to_number_buffer(m_buffer, n, c);

            // some digits
            if (read_digits(n) == 0)
            {
                throw_number_exception(c);
            }
        }
        else 
        {
            throw_number_exception(c);
        }
    }
    else
    {
        m_reader.put_back(c);
    }

    m_token = token(is_double ? token::number_double_e : token::number_int_e, m_buffer, n);
}

void lexer::read_string()
{
    size_t n = 0;
    bool in_escape = false;

    while (true)
    {
        int c;

        c = m_reader.next();
        if (c <= 0xf)
        {
            throw json_parser_exception(
                        json_parser_exception::invalid_character_e,
                        static_cast<char>(c),
                        m_reader.get_byte_index());
        }
        else if (c == '"' && !in_escape)
        {
            m_token = token(token::string_e, m_buffer, n);
            return;
        }
        else
        {
            if (n >= m_max_token_length)
            {
                throw json_parser_exception(
                            json_parser_exception::string_too_long_e,
                            m_max_token_length,
                            m_reader.get_byte_index());
            }
            else
            {
                m_buffer[n++] = c;
            }
        }
        in_escape = (c == '\\');
    }
}

void lexer::read_false()
{
    read_matching("alse");
    m_token = token(token::false_e);
}

void lexer::read_true()
{
    read_matching("rue");
    m_token = token(token::true_e);
}

void lexer::read_null()
{
    read_matching("ull");
    m_token = token(token::null_e);
}

void lexer::read_matching(const char *s)
{
    int i = 0;
    while (s[i] != 0)
    {
        char c = m_reader.next();

        if (c != s[i++])
        {
            throw json_parser_exception(json_parser_exception::invalid_character_e, c, m_reader.get_byte_index());
        }
    }
}

const token &lexer::next()
{
    if (m_last_put_back)
    {
        m_last_put_back = false;
    }
    else
    {
        read_token();
    }

    return m_token;
}

void lexer::put_back_last()
{
    m_last_put_back = true;
}
