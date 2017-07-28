#ifndef _json_stream_writer_hpp_
#define _json_stream_writer_hpp_

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

/// \file stream_writer.hpp The stream_writer class.

#include <ostream>

#include "common.hpp"
#include "writer.hpp"

namespace NAMESPACE
{
    /// A class to write JSON messages to a C++ istream.
    class stream_writer : public writer
    {
    public:

        /**
         * Constructor. Does not take over owenership of the stream,
         * the caller is required to close it and deallocate (if needed)
         * when done.
         */
        stream_writer(std::ostream *s);

        /**
         * Write a string to the stream.
         */
        virtual void write(const std::string &s);

    private:

        std::ostream *m_stream;

    };

}

#endif
