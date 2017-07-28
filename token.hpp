#ifndef _json_token_hpp_
#define _json_token_hpp_

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

/// \file token.hpp The token class.

#include <ostream>

#include "common.hpp"

namespace NAMESPACE
{
    /**
     * \brief Lexical tokens read from a JSON message. 
     *
     * A lexical token read from a JSON message. The underlying string
     * and number values are held in non-converted form to allow a caller
     * further up the stack to decide how to interpret them.
     */
    class token
    {
    public:

        /// Type of the token. Matches the RFC except that ints and floats are separated out.
        typedef enum 
        {
            /// [
            begin_array_e,
            /// {
            begin_object_e,
            /// ]
            end_array_e,
            /// }
            end_object_e,
            /// :
            name_separator_e,
            /// ,
            value_separator_e,
            /// whole number
            number_int_e,
            /// floating point number
            number_double_e,
            /// sting in JSON format
            string_e,
            /// false
            false_e,
            /// true
            true_e,
            /// null
            null_e
        } 
        token_type;

        /// Constructor. null token.
        token() noexcept;

        /// Copy constructor. Deep copy.
        token(const token &other);

        /// Move constructor. Shallow copy.
        token(token &&other);

        /**
         * Constructor given a type - used for the various structural tokens
         * where the type implies the textual representation.
         */
        token(token_type t);

        /**
         * Constructor given a type and a raw string value.
         * A copy is taken of the string data.
         */
        token(token_type t, const char *raw_value, size_t len);

        /**
         * Destructor.
         */
        virtual ~token();

        /// Assignment. Deep copy.
        token &operator=(const token &other);

        /// Move. Shallow copy.
        token &operator=(token &&other);

        /// Get the raw untranslated JSON value.
        const std::string &get_raw_value() const;

        /// Get the token type.
        token_type get_type() const;

    private:

        /// The raw untranslated value. E.g. "\u1234"
        std::string m_raw_value;

        /// Token type.
        token_type m_type;
    };
}

#endif
