#ifndef _json_utf8_exception_hpp_
#define _json_utf8_exception_hpp_

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

/// \file json_utf8_exception.hpp The json_utf8_exception class.

#include <string>

#include "json_exception.hpp"

namespace NAMESPACE
{
    /// Exception class for errors translating to and from UTF-8 strings.
    class json_utf8_exception : public json_exception
    {
    public:

        /**
         * Constructor
         * \param et Type of the exeption.
         * \param s  Extra infomation to add to the message.
         */
        json_utf8_exception(exception_type et, const std::string &s) noexcept;

        /**
         * Constructor
         * \param et Type of the exeption.
         * \param c  Extra infomation to add to the message.
         */
        json_utf8_exception(exception_type et, char c) noexcept;

        /**
         * Constructor
         * \param et Type of the exeption.
         * \param i  Extra infomation to add to the message.
         */
        json_utf8_exception(exception_type et, unsigned int i) noexcept;

        /// Add the byte index where the error occured to the exception.
        void add_byte_index(size_t byte_index) noexcept;

        /// Get the (approximate) byte index at which the error occurred.
        size_t get_byte_index() const noexcept;

        /// Get the (approximate) column at which the error occurred.
        size_t get_column() const noexcept;

    private:

        const char *get_main_message();

        size_t m_byte_index;
    };
}

#endif
