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

/// \file json_invalid_key_exception.cpp The json_invalid_key_exception class implementation.

#include <string.h>

#include "common.hpp"
#include "json_invalid_key_exception.hpp"

using namespace std;
using namespace NAMESPACE;

json_invalid_key_exception::json_invalid_key_exception(exception_type et, const string &key) noexcept : json_exception(et)
{
    if (et == invalid_key_e)
    {
        strncpy(m_message, "invalid object key: ", max_message_length);
        strncpy(m_message, key.c_str(), max_message_length);
    }
    else
    {
        strncpy(m_message, "generic", max_message_length);
    }
}
