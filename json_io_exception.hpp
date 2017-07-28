#ifndef _json_io_exception_hpp_
#define _json_io_exception_hpp_

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

/// \file json_io_exception.hpp The json_io_exception class.

#include "json_exception.hpp"

namespace NAMESPACE
{
    /// Specific class of exceptions for IO errors of various types.
    class json_io_exception : public json_exception
    {
    public:

        /// Constructor for generic error.
        json_io_exception(exception_type et) noexcept;

        /// Constructor with a POSIX errno.
        json_io_exception(exception_type et, int posix_errno) noexcept;

        /// Constructor with a size_t
        json_io_exception(exception_type et, size_t s) noexcept;

    private:

        /// Get the main part of the message based on the exception type
        const char *get_main_message();

    };
}

#endif
