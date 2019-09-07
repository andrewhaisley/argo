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

#include "common.hpp"

#ifdef _ARGO_WINDOWS_
#   include <io.h>
#else
#   include <unistd.h>
#endif

#include <errno.h>

#include "fd_reader.hpp"
#include "json_io_exception.hpp"

using namespace std;
using namespace NAMESPACE;

/// \file fd_reader.cpp The fd_reader class implementation.

fd_reader::fd_reader(int fd, int max_message_length, bool block_read) :
                            reader(max_message_length, block_read),
                            m_fd(fd)
{
}

int fd_reader::read_next_char()
{
    char buf[1];
    size_t n = read(m_fd, buf, 1);
    if (n == 0)
    {
        return EOF;
    }
    else if (n == 1)
    {
        return buf[0];
    }
    else if (n == -1)
    {
        throw json_io_exception(json_io_exception::read_failed_e, errno);
    }
    else
    {
        throw json_io_exception(json_io_exception::unexpected_result_from_read_e);
    }
}

bool fd_reader::read_next_block()
{
    size_t n = read(m_fd, m_block, block_size);

    if (n == 0)
    {
        return false;
    }
    else if (n == -1)
    {
        throw json_io_exception(json_io_exception::read_failed_e, errno);
    }
    else
    {
        m_block_num_bytes = static_cast<int>(n);
        m_block_index = 0;
        return true;
    }
}
