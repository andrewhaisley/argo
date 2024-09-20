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

/// \file fd_writer.cpp The fd_writer class implementation.

#include "common.hpp"

#ifdef _ARGO_WINDOWS_
#   include <io.h>
#else
#   include <unistd.h>
#endif

#include <errno.h>

#include "fd_writer.hpp"
#include "json_io_exception.hpp"

using namespace NAMESPACE;

fd_writer::fd_writer(int fd) : writer(), m_fd(fd)
{
}

void fd_writer::write(const std::string &s)
{
    if (::write(m_fd, s.c_str(), s.size()) == -1)
    {
        throw json_io_exception(json_io_exception::write_fd_failed_e, errno);
    }
}
