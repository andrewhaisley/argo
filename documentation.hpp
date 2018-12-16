/// \file documentation.hpp A file containing just documentation for doxygen to pick up and put on the main page.

/*! \mainpage Argo JSON Library Main Page
 * 
 * \section first Yet Another JSON Library?!
 *
 * Well, coders always think we can do better, don't we? I orginally wrote this a few years back
 * as part of another project when there weren't any C++ JSON libraries I could
 * find that didn't leave me banging my head on the desk in frustration. Since then
 * many good options have come along and <a href="https://github.com/miloyip">Milo Yip</a> has 
 * done an outstanding job of <a href="https://github.com/miloyip/nativejson-benchmark">testing and cataloging</a>
 * many of them.
 *
 * Anyway, this one is, I think, nice to use and the combination of performance and
 * error checking is strong so I think it's worth publishing.
 *
 * \section examples Examples
 *
 * \subsection hello Hello World
 *
 * All of these examples can be found in json_example.cpp. json_test.cpp also includes a lot of
 * example code.
 *
 * \code{.cpp}
 * #include <iostream>
 * #include "argo.hpp"
 * 
 * int main(int argc, char *argv[])
 * {
 *     std::string s = "{\"one\" : \"hello world\"}";
 *     argo::json j;
 *     s >> j;
 *     std::cout << j["one"] << std::endl;
 * }
 * \endcode
 *
 * \subsection file Read From File & Write To File
 *
 * Read a JSON message from one file and write it back out to another.
 *
 * \code{.cpp}
 * #include "argo.hpp"
 * 
 * int main(int argc, char *argv[])
 * {
 *     auto j = argo::parser::load("test_files/test2.json");
 *     argo::unparser::save(*j, "test_files/tmp.json");
 * }
 * \endcode
 * 
 * \subsection construct Construct A Message From Scratch
 *
 * Create an object with some scalar values in it:
 *
 * \code{.cpp}
 *    // create an object
 *    argo::json j(argo::json::object_e);
 *
 *    // add some values
 *    j["one"] = 1;
 *    j["two"] = 2.0;
 *    j["three"] = "three";
 *    j["four"] = true;
 *    j["five"] = false;
 *    j["six"] = nullptr; // null - can also use json()
 *    
 *    // print it
 *    std::cout << j << std::endl;
 * \endcode
 *
 * \subsection pointers Extract a value using a JSON pointer.
 *
 * Given the following JSON in a file named foo.json:
 * \code{.javascript}
 *
 * {
 *    "foo": ["bar", "baz"],
 *    "": 0,
 *    "a/b": 1,
 *    "c%d": 2,
 *    "e^f": 3,
 *    "g|h": 4,
 *    "i\\j": 5,
 *    "k\"l": 6,
 *    " ": 7,
 *    "m~n": 8
 * }
 *
 * \endcode
 * Extract the value of the zeroeth element of the foo array:
 *
 * \code{.cpp}
 *     auto j = argo::parser::load("foo.json");
 *     std::cout << j->find(argo::pointer("/foo/0")) << std::endl;
 * \endcode
 *
 * \section overview Overview
 *
 * The Argo library provides a set of C++ classes for manipulating JSON
 * structures as defined by <a href="https://tools.ietf.org/html/rfc7159">RFC7159</a>.
 * It was designed with the following goals in mind:
 *     - Ease of use. C++'s type system doesn't map naturally onto the dynamic
 *                    type systems used by Javascript, Python and others. The aim
 *                    is to provide an interface that is brief and intuitive.
 *     - Performance. If you're using C++ then you probably care about performance.
 *                    Argo provides efficient ways to manipulate large JSON 
 *                    structures whilst still providing coding simplicity for 
 *                    smaller ones.
 *     - Flexibility. Not all JSON messages follow the standard. Argo allows
 *                    calling code to decide how to handle some of those situations
 *                    should it choose to.
 *     - Conformant.  Every attempt has been made to conform to the 
 *                    <a href="https://tools.ietf.org/html/rfc7159">RFC</a>.
 *
 * \section featured Features
 *
 * Argo has a number of features:
 *     - Concise API.
 *     - Direct handling of multiple IO styles (streams, FILEs, file descriptors, strings).
 *     - DOM style representation of JSON messages.
 *     - JSON Pointer access as per <a href="https://tools.ietf.org/html/rfc6901">RFC6901</a>.
 *     - <a href="https://tools.ietf.org/html/rfc7159">RFC7159</a> compliance.
 *     - Full unicode support.
 *     - Good performance in the context of the amount of error checking carried out.
 *     - No dependence on Boost or any other set of external libraries.
 *     - Lots of built in error and limit checking.
 *     - Reasonably comprehensive documentation.
 *
 * \section dependencies Dependencies
 * 
 * The library has no external dependencies beyond the C++ standard libraries. It
 * does however make use of several C++11 features and therefore needs a compiler
 * that supports them. The code was developed using GCC 4.8 but also built and 
 * tested with Visual Studio 2017. 
 *
 * If you want to build the documentation you'll also need doxygen installed. 
 *
 * \section unicode Unicode Strings, Unicode and UTF-8
 *
 * JSON messages can contain unicode characters (encoded as UTF-8) directly or in
 * the form of \\u sequences. E.g.
 *
 * \code{.unparsed}
 * {
 *     "one"   : "\u2100",       // Unicode character for A/C
 *     "two"   : "\uD834\uDD1E", // Unicode character for the G (or treble) cleff
 *     "three" : "😂 ",          // Directly embedded unicode emoji
 *     "three" : "\ud83d\ude02"  // The same emoji in \u form
 * }
 * \endcode
 *
 * When reading, Argo handles translates everything into 
 * <a href="https://en.wikipedia.org/wiki/UTF-8">UTF-8</a>. When writing
 * it encodes all unicode characaters using \\u sequences.
 *
 * \section numbers Numbers, Ints and Doubles
 *
 * JSON has just numbers. C++, on the other hand, has ints and doubles. When parsing
 * messages Argo handles this as follows:
 *     - if the number is an integer (i.e. if it consists of a sequence of 0-9 digits
 *       and an options leading '-' sign) then it is parsed as an integer. If it is out
 *       of range for the int POD type, then, if the parser fallback_to_double option is true (the
 *       default) an attempt is then made to convert it to a double instead, otherwise parsing fails 
 *       and an exception is thrown.
 *     - if the number is floating point, (e.g. 1.2, 3e+10 etc.) then Argo attempts
 *       to convert it to a double. If it is out of range, then parsing fails and an
 *       exception is thrown.
 *
 * An option is provided for programs that need to handle larger numbers using code of
 * their own whereby numbers are syntax checked only and returned as a string for conversion 
 * by the caller.
 * 
 * \subsection precision Floating Point Precision
 *
 * Turning floating point numbers into strings is 
 * <a href="http://www.serpentine.com/blog/2011/06/29/here-be-dragons-advances-in-problems-you-didnt-even-know-you-had/">harder
 * than it looks</a>. Argo take the approach of writing doubles to the full available precision (17 digits) which means
 * that, in string terms, what comes out will not always match what comes out of a Python or Javascript (or other) program but,
 * in numeric terms once parsed and held in an IEEE floating point number, will. If you care about the exact formatting 
 * of floating point numbers in your output file then use the json(json::number_double_e, "123.5678") constructor to control
 * the exact output format. To produce optimal number strings (broadly the shortest string that produces the exact same
 * floating point number when converted back) from C++ code I recommend 
 * <a href="https://github.com/google/double-conversion/blob/master/double-conversion/strtod.h">Google's double conversion library</a>.
 *
 * \subsection raw Raw Values
 *
 * Argo offers the option to parse JSON messages without converting strings into UTF-8 and 
 * without converting numbers into ints or doubles. This allows calling code to handle its
 * own coversions in the case where string values may be invalid (e.g. formatting issues or
 * invalid unicode code points) or where numbers may be too large to fit into the standard
 * types. String and number raw values (created using the json(type, raw_value) constructor)
 * behave differently to normally constructed objects in the following ways:
 *
 *     - Cast operators will throw an exception.
 *     - Comparison operators (== != < > <= >=) will throw an exception.
 *
 * If you need to compare them then you must handle your own conversion on the string
 * returned by get_raw_value().
 *
 * \subsection casting Casting Values
 *
 * Doubles and ints may be cast to each other and the usual C/C++ conventions of type
 * promotion are followed by all the relevant operators. Ints, but not doubles, may be
 * cast to booleans. 
 *
 * \section sizes Maximum Lengths
 *
 * The default maximum length of a lexical token (e.g. a string, a number, a keyword) is 
 * defined in parser.hpp and is set to 100K. This is almost certainly enough for 99.99% of
 * all known applications but the option exists to specify a different number at run
 * time via parameters to the parse class constructor.
 *
 * \section threads Thread Safety
 *
 * The library uses only thread safe system calls and has no global variables that are not 
 * also constant. This means that multiple threads can safely parse and uparse messages at 
 * the same time and that multiple threads can safely call const methods on a shared json 
 * instance. Simultaneous update/read or update/update  operations on a json instance are 
 * not thread safe. This is the same thread safety model implemented by STL containers.
 * 
 * \section performance Efficiency Peformance vs. Convenience
 *
 * The Argo json class comes with a copy constructor and an assignment operator. For
 * small objects they're convient to use and don't come with too much of a performance hit.
 * E.g. this is probably fine unless you're doing a very large number of times in a tight
 * loop:
 * 
 * \code{.cpp}
 * json j1;
 * j1["a"] = 1;
 * j1["b"] = "a string";
 * j1["c"] = true;
 * json j2(j1);
 * \endcode
 *
 * However, if you're handling large structures then you need to put a more effort in to 
 * ensure efficiency. The parser() class returns unique_ptrs to instances for this reason
 * and there are specific versions of the json.append and json.insert methods that also
 * work with pointers. In addition there is a json move constructor and a json move
 * assignment operator which can remove the need for explicit pointer operations in many
 * cases.
 *
 * \section installing Installation
 *
 * \subsection all All Operating Systems & Compilers
 * 
 * The simplest way to install Argo is to include the code in your own project and
 * then build it along with everything else. All the .hpp files and all the .cpp
 * files are needed except for documentation.hpp, json_test.cpp and json_example.cpp.
 *
 * \subsection linux Linux
 *
 * The -std=c++11 g++ flag is required for compilation with gcc.
 *
 * Alternatively, use the included cmake config file to build a static library and link to that:
 *
 * <pre>
 * cd directory-where-you-downloaded-argo
 * cmake .
 * make
 * </pre>
 * 
 * Then in your own build config on the compile line:
 * 
 * <pre>
 * -Idirectory-where-you-downloaded-argo
 * </pre>
 *
 * And on the link line:
 * <pre>
 * -Ldirectory-where-you-downloaded-argo-largo
 * </pre>
 *
 * \subsection windoze Windows
 *
 * Open Argo.vcxproj using Visual Studio 2017 (community edition works fine) and 
 * build it. If you want to incorporate it into an existing project you'll need to 
 * configure your build not to use precompiled headers for the Argo files.
 *
 * Note that the Windows version does not provide the file description based 
 * read and write options that the Unix version does. On Windows you are limited to
 * string, stdio and iostreams.
 *
 * \section stuff Bits & Pieces
 *
 * JSON vs json - I've tried to stick to the convention of using JSON to refer
 * to bits of text in the JSON format and json to refer to the Argo json
 * class.
 *
 * AVRO - the library doesn't have support for this right now
 * but I do plan to add it at some point as I need it for another project.
 *
 * SAX style parser - I'm not sure about this one. Does anyone really use
 * JSON to represent anything so large and/or complex that it's a genuinely 
 * useful approach? I get why the monster than XML has become needs such a 
 * thing but I'm not convinced of its usefulness for JSON, at least not from 
 * the use cases I've observed or considered. Please tell me otherwise if 
 * you disagree.
 *
 * \section licence License
 *
 * This software is distributed under the MIT open source license.
 *
 * <pre>
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
 * </pre>
 * 
 */
