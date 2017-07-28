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

/// \file writer.cpp The writer class implementation.

#include <sstream>
#include <iomanip>

#include "common.hpp"
#include "writer.hpp"

using namespace std;
using namespace NAMESPACE;

writer::writer()
{
}

writer &NAMESPACE::operator<<(writer& w, const std::string &s)
{
    w.write(s);
    return w;
}

writer &NAMESPACE::operator<<(writer& w, char c)
{
    char s[2];
    s[0] = c;
    s[1] = '\0';
    w.write(s);
    return w;
}

writer &NAMESPACE::operator<<(writer& w, int i)
{
    ostringstream ss;
    ss << i;
    w.write(ss.str());
    return w;
}

writer &NAMESPACE::operator<<(writer& w, double d)
{
    ostringstream ss;
    ss << setprecision(17) << fixed << d;
    w.write(ss.str());
    return w;
}
