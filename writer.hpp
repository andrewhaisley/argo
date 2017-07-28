#ifndef _json_writer_hpp_
#define _json_writer_hpp_

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

/// \file writer.hpp The writer class.

#include <ostream>

#include "common.hpp"

namespace NAMESPACE
{
    /**
     * \brief Simple proxy for write operations on various types of stream.
     *
     * A class that acts as a proxy for the various places that JSON messages
     * can be written to (or to which they can be written even).
     * Actually writing to somewhere is in in derived classes.
     */
    class writer
    {
    public:

        /// Constructor.
        writer();

        /// Write a string - implemented by derived classes.
        virtual void write(const std::string &s) = 0;

    };

    /// Write a string to a writer.
    writer &operator<<(writer& w, const std::string &s);
    /// Write a single character.
    writer &operator<<(writer& w, char c);
    /// Write an int (formatted as a string. i.e. 123 is written as "123").
    writer &operator<<(writer& w, int i);
    /// Write a double (formatted as a string. i.e. 123e23 is written as "123e23").
    writer &operator<<(writer& w, double d);
}

#endif
