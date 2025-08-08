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

/// \file unparser.cpp The unparser class implementation.

#include <ostream>
#include <fstream>
#include <sstream>

#include "common.hpp"
#include "unparser.hpp"
#include "utf8.hpp"
#include "json_exception.hpp"
#include "json_io_exception.hpp"
#include "file_writer.hpp"
#include "stream_writer.hpp"

#ifndef _ARGO_WINDOWS_
#include "fd_writer.hpp"
#endif

using namespace NAMESPACE;

unparser::unparser(
         writer     &w,
         const char *space,
         const char *newline,
         const char *indent,
         int        indent_inc) :
                        m_writer(w),
                        m_space(space),
                        m_newline(newline),
                        m_indent(indent),
                        m_indent_inc(indent_inc)
{
}

void unparser::print_indent(int indent_level)
{
    for (int i = 0; i < indent_level; i++)
    {
        m_writer << m_indent;
    }
}


void unparser::unparse_object(const json &j, int indent_level)
{
    print_indent(indent_level);
    m_writer << '{' << m_space << m_newline;
    int n = j.get_object().size();
    for (const auto &p : j.get_object())
    {
        print_indent(indent_level + m_indent_inc);
        m_writer << '"' << p.first << '"' << m_space << ':' << m_space;
        if (p.second.get_instance_type() == json::object_e ||
            p.second.get_instance_type() == json::array_e)
        {
            m_writer << m_newline;
            unparse((p.second), indent_level + (m_indent_inc * 2));
        }
        else
        {
            unparse((p.second), indent_level + m_indent_inc);
        }
        if (n-- > 1)
        {
            m_writer << ',';
        }
        m_writer << m_newline;
    }
    print_indent(indent_level);
    m_writer << m_space << '}';
}

void unparser::unparse_array(const json &j, int indent_level)
{
    print_indent(indent_level);
    m_writer << '[' << m_space << m_newline;
    int n = j.get_array().size();
    for (const auto &i : j.get_array())
    {
        if (i.get_instance_type() != json::object_e &&
            i.get_instance_type() != json::array_e)
        {
            print_indent(indent_level + m_indent_inc);
        }
        unparse(i, indent_level + m_indent_inc);
        if (n-- > 1)
        {
            m_writer << ',' << m_space;
        }
        m_writer << m_newline;
    }
    print_indent(indent_level);
    m_writer << m_space << ']';
}

void unparser::unparse(const json &j, int indent_level)
{
    if (j.get_raw_value() == "")
    {
        switch (j.get_instance_type())
        {
        case json::object_e:
            unparse_object(j, indent_level);
            break;
        case json::array_e:
            unparse_array(j, indent_level);
            break;
        case json::boolean_e:
            m_writer << (static_cast<bool>(j) ? "true" : "false");
            break;
        case json::null_e:
            m_writer << "null";
            break;
        case json::number_int_e:
            m_writer << static_cast<int>(j);
            break;
        case json::number_double_e:
            m_writer << static_cast<double>(j);
            break;
        case json::string_e:
            m_writer << '"' << *(utf8::utf8_to_json_string(j)) << '"';
            break;
        default:
            throw json_exception(json_exception::invalid_json_type_e);
        }
    }
    else
    {
        if (j.get_instance_type() == json::string_e)
        {
            m_writer << '"' << j.get_raw_value() << '"';
        }
        else
        {
            m_writer << j.get_raw_value();
        }
    }
}

void unparser::unparse(
        std::ostream &o,
        const json   &j,
        const char   *space,
        const char   *newline,
        const char   *indent,
        int           indent_inc)
{
    stream_writer w(&o);
    unparser u(w, space, newline, indent, indent_inc);
    u.unparse(j, 0);
}

#ifndef _ARGO_WINDOWS_
void unparser::unparse(
        int        fd,
        const json &j,
        const char *space,
        const char *newline,
        const char *indent,
        int        indent_inc)
{
    fd_writer w(fd);
    unparser u(w, space, newline, indent, indent_inc);
    u.unparse(j, 0);
}
#endif

void unparser::unparse(
        FILE       *s,
        const json &j,
        const char *space,
        const char *newline,
        const char *indent,
        int        indent_inc)
{
    file_writer w(s);
    unparser u(w, space, newline, indent, indent_inc);
    u.unparse(j, 0);
}

void unparser::save(
        const json        &j,
        const std::string &file_name,
        const char        *space,
        const char        *newline,
        const char        *indent,
        int                indent_inc)
{
    std::ofstream os(file_name);

    if (os)
    {
        stream_writer w(&os);
        unparser u(w, space, newline, indent, indent_inc);
        u.unparse(j, 0);
    }
    else
    {
        throw json_io_exception(json_io_exception::file_open_failed_e, errno);
    }
}

std::ostream &NAMESPACE::operator<<(std::ostream &stream, const json &j)
{
    unparser::unparse(stream, j);
    return stream;
}

void NAMESPACE::operator<<(std::string &s, const json &j)
{
    std::stringstream ss;
    unparser::unparse(ss, j);
    s = ss.str();
}
