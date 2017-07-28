#ifndef _json_parser_exception_hpp_
#define _json_parser_exception_hpp_

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

/// \file json_parser_exception.hpp The json_parser_exception class.

#include "json_exception.hpp"

namespace NAMESPACE
{
    /// Exception class for parser errors.
    class json_parser_exception : public json_exception
    {
    public:

        /**
         * Constructor.
         * \param et         Type of the exeption.
         * \param byte_index Place in the input the error occurred.
         */
        json_parser_exception(exception_type et, size_t byte_index) noexcept;

        /**
         * Constructor for a character based parser exception.
         * \param et         Type of the exeption.
         * \param c          The character that caused the exception.
         * \param byte_index Place in the input the error occurred.
         */
        json_parser_exception(exception_type et, char c, size_t byte_index) noexcept;

        /**
         * Constructor for a parser exception where a string needs to be reported back 
         * (e.g. unexpected token).
         * \param et         Type of the exeption.
         * \param s          The string related to the exception.
         * \param byte_index Place in the input the error occurred.
         */
        json_parser_exception(exception_type et, const std::string &s, size_t byte_index) noexcept;

        /**
         * Constructor for a parser exception where a size_t needs to be reported back
         * (e.g. too many characters in a string).
         * \param et         Type of the exeption.
         * \param s          The size_t related to the exception.
         * \param byte_index Place in the input the error occurred.
         */
        json_parser_exception(exception_type et, size_t s, size_t byte_index) noexcept;

        /// Get the (approximate) byte index at which the error occurred.
        size_t get_byte_index() const noexcept;

    private:

        const char *get_main_message();

        size_t m_byte_index;
    };
}

#endif
