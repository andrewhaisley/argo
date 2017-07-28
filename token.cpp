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

/// \file token.cpp The token class implementation.

#include "common.hpp"
#include "token.hpp"

using namespace std;
using namespace NAMESPACE;

token::token() noexcept : m_type(null_e)
{
}

token::token(const token &other)
{
    *this = other;
}

token::token(token &&other)
{
    *this = other;
}

token &token::operator=(const token &other)
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_raw_value = other.m_raw_value;
    }
    return *this;
}

token &token::operator=(token &&other)
{
    if (this != &other)
    {
        m_type = other.m_type;
        other.m_type = null_e;
        m_raw_value = move(other.m_raw_value);
    }
    return *this;
}

token::token(token_type t) : m_type(t)
{
}

token::token(token_type t, const char *raw_value, size_t len) : m_raw_value(raw_value, len), m_type(t)
{
}

token::~token()
{
}

const string &token::get_raw_value() const
{
    return m_raw_value;
}

token::token_type token::get_type() const
{
    return m_type;
}
