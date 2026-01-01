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

/// \file json_io_exception.cpp The json_io_exception class implementation.

#include <string.h>
#include <stdio.h>

#include "common.hpp"
#include "json_io_exception.hpp"

using namespace NAMESPACE;

const char *json_io_exception::get_main_message()
{
    switch (m_type)
    {
    case fgetc_failed_e:
        return "fgetc() failed";
    case read_failed_e:
        return "read() failed";
    case unexpected_result_from_read_e:
        return "unexpected result from read()";
    case write_fd_failed_e:
        return "write() failed";
    case write_file_failed_e:
        return "fwrite() failed";
    case too_many_put_back_e:
        return "internal error, too many characters put back into a reader";
    case message_too_long_e:
        return "maximum configured message length exceeded";
    case file_open_failed_e:
        return "file open failed";
    default:
        return "generic";
    }
}

json_io_exception::json_io_exception(exception_type et) noexcept : json_exception(et)
{
    strncpy(m_message, get_main_message(), max_message_length - 1);
}

json_io_exception::json_io_exception(exception_type et, int posix_errno) noexcept : json_exception(et)
{
    // strerror_r may not always write to the provided buffer, it may return a pointer to
    // a static string instead.
    char buffer[max_message_length - 3];
    const char *errmsg = strerror_r(posix_errno, buffer, max_message_length - 3);

    snprintf(m_message, max_message_length, "%s : %s", get_main_message(), errmsg);
}

json_io_exception::json_io_exception(exception_type et, size_t s) noexcept : json_exception(et)
{
    snprintf(m_message, max_message_length, "%s : %zu", get_main_message(), s);
}
