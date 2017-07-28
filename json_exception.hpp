#ifndef _json_exception_hpp_
#define _json_exception_hpp_

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

/// \file json_exception.hpp The json_exception class.

#include <stdexcept>

#include "common.hpp"

namespace NAMESPACE
{
    /**
     * \brief Base class for all exceptions thrown by the library. 
     *
     * Base class for all exceptions thrown by the library. All exeption
     * types are defined here but subclasses are defined to provide
     * specialised error message and extra parameters (e.g. the byte
     * index where a JSON parsing error occured). For code that doesn't
     * need to know anything about the specific exception and just wants to 
     * log an error message before exiting or continuing, catch exceptions
     * of this class and call what() to get some human readable English
     * text.
     */
    class json_exception : public std::exception
    {
    public:

        /// Maximum length of the message that may be returned by the what() method.
        static const int max_message_length = 200;

        /**
         * The type of the exception. It's a bit clunky but this also includes
         * all the types for derived classes. That keeps the amount of code to a
         * minimum and provides a single place to look up exception types. This
         * exception class is really not intended to be subclassed outside of the
         * library so that's not so bad.
         */
        typedef enum
        {
            /// Internal error - the m_type member variable has an invalid value.
            invalid_json_type_e, 
            /// Attempt to access a non-existent slot in a json instance.
            invalid_key_e,
            /// Attempt to call a method that assumes the json instance is an array.
            not_an_array_e,
            /// Attempt to call a method that assumes the json instance is an object.
            not_an_object_e,
            /// Attempt to call a method that assumes the json instance is a boolean or castable to one.
            not_number_int_or_boolean_e,
            /// Attempt to call a method that assumes the json instance is a string.
            not_string_e,
            /// Attempt to call a method that assumes the json instance is an int or double.
            not_number_e,
            /// Attempt to to access an index in an array object that is out of range.
            array_index_range_e,
            /// Attempt to construct an instance with a raw value that isn't a number or a string
            not_number_or_string_e,
            /// Raw values cannot be directly compared.
            cant_compare_raw_e,
            /// Raw values cannot be cast.
            cant_cast_raw_e,
            /// Tried to compare two different types (e.g. int vs string)
            types_not_comparable_e,

            /// The stdio fgetc call failed in an unexpected way.
            fgetc_failed_e,
            /// The POSIX read() system call failed.
            read_failed_e,
            /// The POSIX read() system call returned an unexpected value.
            unexpected_result_from_read_e,
            /// The POSIX write() system call failed.
            write_fd_failed_e,
            /// The stdio fwrite() system call failed.
            write_file_failed_e,
            /// Attempt to put back more than one character onto a reader.
            too_many_put_back_e,
            /// Attempt to open file failed
            file_open_failed_e,

            /// An invalid character was found during parsing (e.g. ! outside of a string).
            invalid_character_e,
            /// Something that looked like a number ended up not being (e.g. 1234AAAA).
            invalid_number_e,
            /// A number was too large or small to convert accrurately to an int or double.
            number_out_of_range_e,
            /// A number had too many characters in it.
            number_too_long_e,
            /// A string had too many characters in it.
            string_too_long_e,
            /// The file ended where more tokens were expected.
            unexpected_eof_e,
            /// A valid, but unexpected, lexical token was found (e.g. { {).
            unexpected_token_e,
            /// Maximum message length was exceeded
            message_too_long_e,
            /// Maximum nesting depth exceeded.
            maximum_nesting_depth_exceeded_e,

            /// An invalid JSON character escape was found in a string. e.g. "\P"
            invalid_string_escape_e,
            /// A string encoding of some sort was invalid (e.g. "\uAA").
            invalid_string_encoding_e,
            /// An invalid unicode code point was found when parsing a JSON string.
            invalid_unicode_e,
            /// An invalid UTF-8 character was found decoding or encoding a string.
            invalid_utf8_char_e,
            /// Internal error, couldn't determine the length of a UTF-8 sequence.
            invalid_utf8_sequence_length_e,
            /// Invalid hex character was found. (e.g. "\uAA8Z").
            invalid_hex_char_e,
            /// Internal error, invalid hex number was found (< 0 or > 15).
            invalid_hex_number_e
        }
        exception_type;

        /**
         * Constructor.
         */
        json_exception(exception_type et) noexcept;

        /**
         * Constructor when the error is a type related problem.
         */
        json_exception(exception_type et, const char *json_type_name) noexcept;

        /**
         * Constructor when the error is a type related problem involving
         * two objects.
         */
        json_exception(
                exception_type et, 
                const char     *first_json_type_name,
                const char     *second_json_type_name) noexcept;

        /**
         * Get the type of the exception. More details can be obtained
         * for some sub-class exception types by calling methods specific
         * to them.
         */
        exception_type get_type() const noexcept;

        /**
         * Get a human readable English error message that can be used
         * for logging or debugging purposes.
         */
        virtual const char *what() const noexcept;

    protected:

        /**
         * Error message. Held this way so as to allow the what() method to
         * be declared noexcept.
         */
        char m_message[max_message_length];

        /**
         * Specific type of the exception. Note that derived classes
         * use this one too and don't define their own types.
         */
        exception_type m_type;

    };

    /**
     * Write the exception object to a stream. This is primarily a shortcut for 
     * logging and debugging purposes and is a simple wrapper around the
     * what() method.
     */
    std::ostream &operator<<(std::ostream& stream, const json_exception &e);
}

#endif
