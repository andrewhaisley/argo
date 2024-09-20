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

/// \file stream_reader.cpp The stream_reader class implementation.

#include "common.hpp"
#include "stream_reader.hpp"
#include "json_io_exception.hpp"

using namespace NAMESPACE;


stream_reader::stream_reader(std::istream *s, int max_message_length, bool block_read) :
                                reader(max_message_length, block_read),
                                m_stream(s)
{
}

int stream_reader::read_next_char()
{
    return m_stream->get();
}

bool stream_reader::read_next_block()
{
    m_stream->read(reinterpret_cast<char *>(m_block), block_size);

    std::streamsize n = m_stream->gcount();

    if (n > 0)
    {
        m_block_num_bytes = static_cast<int>(n);
        m_block_index = 0;
        return true;
    }
    else
    {
        if (m_stream->eof())
        {
            return false;
        }
        else
        {
            throw json_io_exception(json_io_exception::read_failed_e, errno);
        }
    }
}
