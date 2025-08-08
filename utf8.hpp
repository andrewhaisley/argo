#ifndef _json_utf8_hpp_
#define _json_utf8_hpp_

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

/// \file utf8.hpp The utf8 class.

#include <memory>
#include <string>

#include "common.hpp"

namespace NAMESPACE
{
    /**
     * \brief Methods to convert JSON format string to UTF-8 and vice-versa.
     *
     * This only really exists because the C++ standard libraries for this are
     * not widely implemented yet.
     */
    class utf8
    {
    public:

        /**
         * Create a UTF-8 encoded string from a JSON string. Allocated dynamically
         * to reduce copying.
         * \param src   A string in JSON format. E.g. "jkhjkh\uAABB\t".
         */
        static std::unique_ptr<std::string> json_string_to_utf8(const std::string &src);

        /**
         * Create a JSON encoded string from a UTF-8 string. Allocated dynamically
         * to reduce copying.
         * \param src   A UTF-8 string.
         */
        static std::unique_ptr<std::string> utf8_to_json_string(const std::string &src);
    };
}

#endif
