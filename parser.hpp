#ifndef _json_parser_hpp_
#define _json_parser_hpp_

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

/// \file parser.hpp The parser class.

#include <istream>

#include "common.hpp"
#include "json.hpp"
#include "reader.hpp"
#include "lexer.hpp"

namespace NAMESPACE
{
    /**
     * Read a JSON object from a stream using default options.
     * See parser class static methods for more options.
     */
    std::istream &operator>>(std::istream &stream, json &e);

    /**
     * Read a JSON object from a string using default options.
     * See parser class static methods for more options.
     */
    void operator>>(std::string &stream, json &e);

    /**
     * \brief A recursive decent parser for JSON messages. 
     *
     * A class to parse JSON messages from an input source and store
     * the results in a json object instance.
     */
    class parser
    {
    public:

        /// The default maximum length of a message that can be parsed. 
        static const size_t max_message_length = 10*1024*1024;

        /**
         * The default maximum length of any individual lexical token 
         * (string and number being the relevant ones in this context).
         */
        static const size_t max_token_length = 100*1024;

        /**
         * The default maximum nesting depth (objects and arrays) of a message 
         * that can be parsed. 
         */
        static const size_t max_nesting_depth = 1000;

        /**
         * Convenience method. Parse a JSON message read from an istream. If you
         * need more detailed control of the options, create a reader and parser object
         * directly.
         * \param i                   Where to read the message from.
         * \throw json_io_exception   Thrown when something goes wrong with reading.
         * \throw json_utf_exception  Thrown when an invalid string is found in the
         *                            message.
         */
        static std::unique_ptr<json> parse(std::istream &i);

#ifndef _ARGO_WINDOWS_
        /**
         * Convenience method. Parse a JSON message read from a POSIX file descriptor. If you
         * need more detailed control of the options, create a reader and parser object
         * directly.
         * \param fd                  Where to read the message from.
         * \throw json_io_exception   Thrown when something goes wrong with reading.
         * \throw json_utf_exception  Thrown when an invalid string is found in the
         *                            message.
         */
        static std::unique_ptr<json> parse(int fd);
#endif

        /**
         * Convenience method. Parse a JSON message read from a stdio file. If you
         * need more detailed control of the options, create a reader and parser object
         * directly.
         * \param f                   Where to read the message from.
         * \throw json_io_exception   Thrown when something goes wrong with reading.
         * \throw json_utf_exception  Thrown when an invalid string is found in the
         *                            message.
         */
        static std::unique_ptr<json> parse(FILE *f);

        /**
         * Convenience method. Parse a JSON message read from a string. If you
         * need more detailed control of the options, create a reader and parser object
         * directly.
         * \param s                   Where to read the message from.
         * \throw json_io_exception   Thrown when something goes wrong with reading.
         * \throw json_utf_exception  Thrown when an invalid string is found in the
         *                            message.
         */
        static std::unique_ptr<json> parse(const char *s);

        /**
         * Convenience method. Parse a JSON message read from a string. If you
         * need more detailed control of the options, create a reader and parser object
         * directly.
         * \param s                   Where to read the message from.
         * \throw json_io_exception   Thrown when something goes wrong with reading.
         * \throw json_utf_exception  Thrown when an invalid string is found in the
         *                            message.
         */
        static std::unique_ptr<json> parse(const std::string &s);

        /**
         * Convenience method. Open a file, parse the entire contents, then
         * close it. The file_name parameter is the name of the file.
         *
         * \param file_name           File name to read the message from.
         * \throw json_io_exception   Thrown when something goes wrong with reading.
         * \throw json_utf_exception  Thrown when an invalid string is found in the
         *                            message.
         */
        static std::unique_ptr<json> load(const std::string &file_name);

        /**
         * Constructor. Create a parser that will read from the given reader.
         * If you need to read from some other type of source, then create an 
         * appropriate derived class of reader and pass that in here.
         *
         * \param r                    Where to read the message from.
         * \param read_all             If true, read the whole file to the end and 
         *                             throw an exception if there is anything other
         *                             than white space following a parseable JSON
         *                             message. If false, read until the end of the 
         *                             first parseable JSON message and leave it at that.
         * \param p_max_nesting_depth  The maximum total depth of nested objects and 
         *                             arrays allowed.
         * \param p_max_token_length    The maximum length of any individual token,
         *                              You can change this if you have a strangely
         *                              huge string in a message or you can make it     
         *                              smaller if you have very constrained memory
         *                              availability.
         * \param p_convert_numbers     If true, convert numbers to ints or doubles
         *                              as appropriate. If false, create the json
         *                              instance using the json(type, raw_string) method
         *                              but don't do the numeric conversion. This allows
         *                              the caller to write custom code to handle numbers
         *                              outside of the normal int & double ranges.
         * \param p_fallback_to_double  If true, convert integers that are too large to be
         *                              stored as an int into doubles instead.
         * \param p_convert_strings     If true, convert strings into UTF-8 encoded
         *                              STL strings. If false, create the json instance
         *                              using the json(type, raw_string) method but
         *                              leave the caller to do any conversion. This is
         *                              useful when dealing with messages with broken
         *                              Unicode characters and the like in them. 
         * \throw json_parser_exception Thrown when there is something syntactically 
         *                              wrong with the message.
         * \throw json_io_exception     Thrown when something goes wrong with reading.
         * \throw json_utf_exception    Thrown when an invalid string is found in the
         *                              message.
         */
        parser(
            reader &r,
            bool   read_all = true,
            size_t p_max_token_length = max_token_length,
            size_t p_max_nesting_depth = max_nesting_depth,
            bool   p_convert_numbers = true,
            bool   p_fallback_to_double = true,
            bool   p_convert_strings = true);

        /**
         * Parse a single json object from the stream. The object
         * is dynamically allocated to prevent the need to make a
         * deep copy at any point.
         */
        std::unique_ptr<json> parse();

    private:

        std::unique_ptr<json> parse_number_int(const token &t);
        std::unique_ptr<json> parse_number_double(const token &t);
        std::unique_ptr<json> parse_string(const token &t);
        std::unique_ptr<json> parse_value(lexer &l, size_t nesting_depth);
        std::unique_ptr<json> parse_array(lexer &l, size_t nesting_depth);
        void parse_name_value_pair(lexer &l, std::unique_ptr<json> &object, size_t nesting_depth);
        std::unique_ptr<json> parse_object(lexer &l, size_t nesting_depth);

        /// Reader to get characters from.
        reader &m_reader;

        /**
         * Whether to read all input at once or just to read up until the end of
         * the message.
         */
        bool m_read_all;

        /// Maximum allowed length of any token (primarily strings and numbers).
        size_t m_max_token_length;

        /// Maximum depth of object/array nesting allowed.
        size_t m_max_nesting_depth;

        /// Whether to convert numbers to ints/double or leave them in their raw form.
        bool m_convert_numbers;

        /// Whether to fallback to storing a number as a double if it's too large to be an int.
        bool m_fallback_to_double;

        /// Whether to convert string to UTF-8 strings or leave them in their raw form.
        bool m_convert_strings;

    };
}

#endif
