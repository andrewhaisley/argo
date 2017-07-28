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

/// \file reader.cpp The reader class implementation.

#include "common.hpp"
#include "reader.hpp"
#include "json_io_exception.hpp"

using namespace NAMESPACE;

reader::reader(size_t max_message_length, bool block_read) : 
            m_put_back(reader::no_put_back), 
            m_byte_index(0),
            m_max_message_length(max_message_length),
            m_block_read(block_read),
            m_block_num_bytes(0),
            m_block_index(-1)
{
}

int reader::read_next_char_from_block()
{
    if (m_block_index == -1 || m_block_index >= m_block_num_bytes)
    {
        if (!read_next_block())
        {
            return EOF;
        }
    }

    return m_block[m_block_index++];
}

int reader::next()
{
    if (m_put_back == no_put_back)
    {
        int res = m_block_read ?  read_next_char_from_block() : read_next_char();

        if (++m_byte_index > m_max_message_length)
        {
            throw json_io_exception(json_exception::message_too_long_e, m_max_message_length);
        }

        return res;
    }
    else
    {
        int c = m_put_back;
        m_put_back = no_put_back;
        return c;
    }
}

void reader::put_back(int c)
{
    if (m_put_back == no_put_back)
    {
        m_put_back = c;
    }
    else
    {
        throw json_io_exception(json_io_exception::too_many_put_back_e);
    }
}

size_t reader::get_byte_index() const
{
    return m_byte_index;
}

void reader::reset_byte_index()
{
    m_byte_index = 0;
}
