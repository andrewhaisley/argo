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

/// \file parser.cpp The parser class implementation.

#include <math.h>

#include <sstream>
#include <fstream>

#include "common.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include "utf8.hpp"
#include "json_parser_exception.hpp"
#include "stream_reader.hpp"
#include "file_reader.hpp"
#include "json_utf8_exception.hpp"
#include "json_io_exception.hpp"

#ifndef _ARGO_WINDOWS_
#include "fd_reader.hpp"
#endif

using namespace std;
using namespace NAMESPACE;

parser::parser(
            reader &r,
            bool   read_all,
            size_t p_max_token_length,
            size_t p_max_nesting_depth,
            bool   p_convert_numbers, 
            bool   p_fallback_to_double, 
            bool   p_convert_strings) :
                        m_reader(r),
                        m_read_all(read_all),
                        m_max_token_length(p_max_token_length),
                        m_max_nesting_depth(p_max_nesting_depth),
                        m_convert_numbers(p_convert_numbers),
                        m_fallback_to_double(p_fallback_to_double), 
                        m_convert_strings(p_convert_strings)
{
}

unique_ptr<json> parser::parse_number_int(const token &t)
{
    istringstream is(t.get_raw_value());

    int i;

    if (is >> i)
    {
        return unique_ptr<json>(new json(i));
    }
    else
    {
        // We know the int is syntactically correct so this has to be a range error.
        if (m_fallback_to_double)
        {
            return parse_number_double(t);
        }
        else
        {
            throw json_parser_exception(
                        json_parser_exception::number_out_of_range_e,
                        t.get_raw_value(),
                        m_reader.get_byte_index());
        }
    }
}

unique_ptr<json> parser::parse_number_double(const token &t)
{
    istringstream is(t.get_raw_value());
    double d;

    if (is >> d && isfinite(d))
    {
        return unique_ptr<json>(new json(d));
    }
    else
    {
        // We know the float is syntactically correct so this has to be a range error.
        throw json_parser_exception(
                            json_parser_exception::number_out_of_range_e,
                            t.get_raw_value(),
                            m_reader.get_byte_index());
    }
}

unique_ptr<json> parser::parse_value(lexer &l, size_t nesting_depth)
{
    const token &t = l.next();

    switch (t.get_type())
    {
    case token::begin_object_e:
        return parse_object(l, nesting_depth + 1);
    case token::begin_array_e:
        return parse_array(l, nesting_depth + 1);
    case token::number_int_e:
        if (m_convert_numbers)
        {
            return parse_number_int(t);
        }
        else
        {
            return unique_ptr<json>(new json(json::number_int_e, t.get_raw_value()));
        }
    case token::number_double_e:
        if (m_convert_numbers)
        {
            return parse_number_double(t);
        }
        else
        {
            return unique_ptr<json>(new json(json::number_double_e, t.get_raw_value()));
        }
    case token::string_e:
        if (m_convert_strings)
        {
            try
            {
                return unique_ptr<json>(new json(utf8::json_string_to_utf8(t.get_raw_value())));
            }
            catch (json_utf8_exception &e)
            {
                e.add_byte_index(m_reader.get_byte_index());
                throw e;
            }
        }
        else
        {
            return unique_ptr<json>(new json(json::string_e, t.get_raw_value()));
        }
    case token::false_e:
        return unique_ptr<json>(new json(false));
    case token::true_e:
        return unique_ptr<json>(new json(true));
    case token::null_e:
        return unique_ptr<json>(new json);
    default:
        throw json_parser_exception(
                            json_parser_exception::unexpected_token_e,
                            t.get_raw_value(),
                            m_reader.get_byte_index());
    }
}

unique_ptr<json> parser::parse_array(lexer &l, size_t nesting_depth)
{
    if (nesting_depth > m_max_nesting_depth)
    {
        throw json_parser_exception(
                            json_parser_exception::maximum_nesting_depth_exceeded_e,
                            m_max_nesting_depth,
                            m_reader.get_byte_index());
    }

    unique_ptr<json> array = unique_ptr<json>(new json(json::array_e));

    const token &t1 = l.next();

    if (t1.get_type() == token::end_array_e)
    {
        return array;
    }
    else
    {
        l.put_back_last();
        array->get_array().push_back(parse_value(l, nesting_depth));
    }

    while (true)
    {
        const token &t2 = l.next();

        if (t2.get_type() == token::value_separator_e)
        {
            array->get_array().push_back(parse_value(l, nesting_depth));
        }
        else if (t2.get_type() == token::end_array_e)
        {
            return array;
        }
        else
        {
            throw json_parser_exception(
                            json_parser_exception::unexpected_token_e,
                            t2.get_raw_value(),
                            m_reader.get_byte_index());
        }
    }
    
    return array;
}

void parser::parse_name_value_pair(lexer &l, unique_ptr<json> &object, size_t nesting_depth)
{
    const token &t1 = l.next();

    string name;

    if (t1.get_type() == token::string_e)
    {
        if (m_convert_strings)
        {
            name = *utf8::json_string_to_utf8(t1.get_raw_value());
        }
        else
        {
            name = t1.get_raw_value();
        }
    }
    else
    {
        throw json_parser_exception(
                        json_parser_exception::unexpected_token_e,
                        t1.get_raw_value(),
                        m_reader.get_byte_index());
    }

    const token &t2 = l.next();

    if (t2.get_type() != token::name_separator_e)
    {
        throw json_parser_exception(
                            json_parser_exception::unexpected_token_e,
                            t2.get_raw_value(),
                            m_reader.get_byte_index());
    }

    object->get_object()[name] = parse_value(l, nesting_depth);
}

unique_ptr<json> parser::parse_object(lexer &l, size_t nesting_depth)
{
    if (nesting_depth > m_max_nesting_depth)
    {
        throw json_parser_exception(
                            json_parser_exception::maximum_nesting_depth_exceeded_e,
                            m_max_nesting_depth,
                            m_reader.get_byte_index());
    }

    unique_ptr<json> object = unique_ptr<json>(new json(json::object_e));

    const token &t1 = l.next();

    // check for empty object
    if (t1.get_type() == token::end_object_e)
    {
        return object;
    }
    else
    {
        l.put_back_last();
    }

    while (true)
    {
        parse_name_value_pair(l, object, nesting_depth);

        const token &t2 = l.next();

        if (t2.get_type() == token::end_object_e)
        {
            return object;
        }
        else if (t2.get_type() == token::value_separator_e)
        {
            continue;
        }
        else
        {
            throw json_parser_exception(
                        json_parser_exception::unexpected_token_e,
                        t2.get_raw_value(),
                        m_reader.get_byte_index());
        }
    }

    return object;
}

unique_ptr<json> parser::parse()
{
    m_reader.reset_byte_index();

    lexer l(m_reader, m_max_token_length);

    auto res = parse_value(l, 0);

    if (m_read_all)
    {
        // check that there isn't anything other than whitespace left
        int c;

        while ((c = m_reader.next()) != EOF)
        {
            if ((c != 0x20) && (c != 0x09) && (c != 0x0A) && (c != 0x0D))
            {
                throw json_parser_exception(json_parser_exception::invalid_character_e, m_reader.get_byte_index());
            }
        }
    }
    return res;
}

unique_ptr<json> parser::parse(istream &i)
{
    stream_reader r(&i, max_message_length, true);
    parser p(r);
    return p.parse();
}

#ifndef _ARGO_WINDOWS_
unique_ptr<json> parser::parse(int fd)
{
    fd_reader r(fd, max_message_length, true);
    parser p(r);
    return p.parse();
}
#endif

unique_ptr<json> parser::parse(FILE *s)
{
    file_reader r(s, max_message_length, true);
    parser p(r);
    return p.parse();
}

unique_ptr<json> parser::parse(const char *s)
{
    istringstream i(s);
    return parse(i);
}

unique_ptr<json> parser::parse(const string &s)
{
    istringstream i(s);
    return parse(i);
}

unique_ptr<json> parser::load(const string &file_name)
{
    ifstream is(file_name);

    if (is)
    {
        return parse(is);
    }
    else
    {
        throw json_io_exception(json_io_exception::file_open_failed_e, errno);
    }
}

istream &NAMESPACE::operator>>(istream &stream, json &j)
{
    j = move(*parser::parse(stream));
    return stream;
}

void NAMESPACE::operator>>(string &s, json &j)
{
    j = move(*parser::parse(s));
}
