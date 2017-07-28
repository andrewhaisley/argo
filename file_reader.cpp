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

/// \file file_reader.cpp The file_reader class implementation.

#include <errno.h>

#include "common.hpp"
#include "file_reader.hpp"
#include "json_io_exception.hpp"

using namespace std;
using namespace NAMESPACE;


file_reader::file_reader(FILE *f, int max_message_length, bool block_read) : 
                            reader(max_message_length, block_read), 
                            m_file(f)
{
}

int file_reader::read_next_char()
{
    int c = fgetc(m_file);

    if (c == EOF && ferror(m_file))
    {
        throw json_io_exception(json_io_exception::fgetc_failed_e, errno);
    }
    else
    {
        return c;
    }
}

bool file_reader::read_next_block()
{
    size_t n = fread(m_block, 1, block_size, m_file);

    if (n > 0)
    {
        m_block_num_bytes = static_cast<int>(n);
        m_block_index = 0;
        return true;
    }
    else
    {
        if (ferror(m_file))
        {
            throw json_io_exception(json_io_exception::read_failed_e, errno);
        }
        else
        {
            return false;
        }
    }
}
