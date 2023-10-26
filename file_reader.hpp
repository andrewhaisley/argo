#ifndef _json_file_reader_hpp_
#define _json_file_reader_hpp_

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

/// \file file_reader.hpp The file_reader class.

#include <stdio.h>

#include "reader.hpp"

namespace NAMESPACE
{
    /// A derived class of reader that reads from stdio FILEs.
    class file_reader : public reader
    {
    public:

        virtual ~file_reader() {}

        /**
         * Constructor.
         * \param   f                   STDIO file. The instance does not take
         *                              ownership of the open file and it is up to the calling 
         *                              code to close it when done.
         * \param   max_message_length  Longest message that can be read.
         * \param   block_read          Whether to read in blocks or one byte at a time.
         */
        file_reader(FILE *f, int max_message_length, bool block_read);

    protected:

        /**
         * Read a character from the file using fread()
         * \throw json_io_exception Thrown if fread fails in some way.
         */
        virtual int read_next_char();

        /**
         * Read the next block of data up to block_size bytes.
         * \return  true if some data was read, false if EOF.
         * \throw   json_io_error if the read failed for some reason other than EOF.
         */
        virtual bool read_next_block();

        /// File to read from
        FILE *m_file;
    };
}

#endif
