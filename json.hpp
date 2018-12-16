#ifndef _json_json_hpp_
#define _json_json_hpp_

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

/// \file json.hpp The json class.

#include <memory>
#include <map>
#include <vector>

#include "common.hpp"
#include "pointer.hpp"

namespace NAMESPACE
{
    /**
     * \brief All json things are represented by instances of this class.
     *
     * All JSON things are represented by instaces of this class. Due to the nature of
     * JSON, which is fundamentally a construct of dynamically typed languages, there
     * are inevitably a few compromises and edge cases to be handled.<br>
     *
     * Refer to <a href="https://tools.ietf.org/html/rfc7159">RFC7159</a> for more
     * details.
     *
     * Notes on comparison operators: == and != are provided as the implementation
     * is complex and the semantics in the case of comparing one type with another are
     * simple - they're always not equal. <, <=, > & >= are not provided as a deliberate
     * policy as it's complex and not obvious what would be wanted when the types of the
     * two sides didn't match or were arrays/objects. The calling code is always free
     * to cast scalar types to numbers or strings where appropriate and compare those.
     *
     * E.g.
     *
     * \code{.cpp}
     * if (static_cast<int>(j1) < static_cast<int>(j2))
     * {
     *     // do something...
     * }
     * \endcode
     */
    class json
    {
    public:

        // Convenience definition for creating null instances
        typedef decltype(nullptr) null_t;

        /**
         * JSON types as per RFC 4627 but with numbers split into int and double.
         */
        typedef enum 
        { 
            object_e, 
            array_e, 
            boolean_e, 
            null_e, 
            number_int_e, 
            number_double_e, 
            string_e 
        } 
        type;

        /**
         * New json null instance.
         */
        json() noexcept;

        /**
         * Destructor.
         */
        ~json();

        /**
         * Copy constructor. Note that this is a full deep copy so use
         * carefully to avoid performance problems.
         */
        json(const json &other);

        /**
         * Move constructor - shallow copy.
         */
        json(json &&other) noexcept;

        /**
         * New json instance of the given type. The value is initialised according to the
         * type as follows:<br>
         * object_e  : empty      <br>
         * array_e   : empty      <br>
         * boolean_e : false      <br>
         * null_e    : null       <br>
         * number_e  : 0          <br>
         * string_e  : ""         <br>
         * \throw json_exception if t isn't a valid type.
         */
        json(type t);

        /**
         * New json instance of int, double or string type in the specific
         * situation where the underlying text representation couldn't be
         * converted to an stl string, an int or a double. This might arise
         * when the number is out of range for an int or double variable or
         * contains invalid unicode or escape characters. Storing it in an
         * unconverted form allows code using this library to process it anyway
         * if desired.
         *
         * \param t         number_int_e, number_double_e or string_e
         * \param raw_value e.g. 999989999999999999 (too big to be an int)
         * \throw json_exception if t is not one of the allowed types.
         */
        json(type t, const std::string &raw_value);

        /**
         * New json instance of number int type.
         */
        json(int i);

        /**
         * New json instance of number double type.
         */
        json(double d);

        /**
         * New json instance of bool type.
         */
        json(bool b);

        /**
         * New json instance of string type. In this case the string is copied.
         */
        json(const std::string &s);

        /**
         * New json instance of string type. In this case the string is copied.
         */
        json(const char *s);

        /**
         * New json instance of a null type
         */
        json(null_t) noexcept;

        /**
         * New json instance of string type. In this case ownership of the string
         * is taken over by the json object. The caller will need to move() the 
         * unique_ptr to call this - which makes the transfer of ownership
         * explicit.
         */
        json(std::unique_ptr<std::string> s);

        /**
         * Assignment. Full deep copy.
         */
        json &operator=(const json &other);

        /**
         * Move assignment. Shallow copy therefore much more efficient than a 
         * deep copy.
         */
        json &operator=(json &&other) noexcept;

        /**
         * Assign the instance an int value. All previous values are erased and/or
         * freed.
         */
        json &operator=(int i);

        /**
         * Assign the instance a double value. All previous values are erased and/or
         * freed.
         */
        json &operator=(double d);

        /**
         * Assign the instance a bool value. All previous values are erased and/or
         * freed.
         */
        json &operator=(bool b);

        /**
         * Assign the instance a string value. All previous values are erased and/or
         * freed. In this case, a copy of the string is made.
         */
        json &operator=(const char *s);

        /**
         * Assign the instance a string value. All previous values are erased and/or
         * freed. In this case, a copy of the string is made.
         */
        json &operator=(const std::string &s);

        /**
         * Assign the instance a string value. All previous values are erased and/or
         * freed. In this case ownership of the string is taken over by the json object. 
         * The caller will need to move() the unique_ptr to call this - which makes 
         * the transfer of ownership explicit.
         */
        json &operator=(std::unique_ptr<std::string> s);

        /**
         * Assign the instance an object value. All previous values are erased and/or
         * freed. All values are deep copied so this can be inefficient for large
         * objects.
         */
        json &operator=(const std::map<std::string, std::unique_ptr<json>> &o);

        /**
         * Assign the instance an array value. All previous values are erased and/or
         * freed. All values are deep copied so this can be inefficient for large
         * arrays.
         */
        json &operator=(const std::vector<std::unique_ptr<json>> &a);

        /**
         * Assign the instance a null value. All previous values are erased and/or
         * freed. All values are deep copied so this can be inefficient for large
         * arrays.
         */
        json &operator=(null_t);

        /**
         * Get the type of a JSON instance.
         */
        type get_instance_type() const;

        /**
         * Get the type name of a JSON instance - mainly for logging etc.
         */
        const char *get_instance_type_name() const;

        /**
         * Get the underlying vector of JSON instances in an array JSON instance.
         * \throw json_exception if the instance isn't an array.
         */
        std::vector<std::unique_ptr<json>> &get_array();

        /**
         * Get the underlying const vector of JSON instances in a const array of JSON instances.
         * \throw json_exception if the instance isn't an array.
         */
        const std::vector<std::unique_ptr<json>> &get_array() const;

        /**
         * Get the underlying map of JSON instances in an object JSON instance.
         * \throw json_exception if the instance isn't an object.
         */
        std::map<std::string, std::unique_ptr<json>> &get_object();

        /**
         * Get the underlying const map of JSON instances in a const object JSON instance.
         * \throw json_exception if the instance isn't an object.
         */
        const std::map<std::string, std::unique_ptr<json>> &get_object() const;

        /**
         * Cast the object to an int.
         * \throw json_exception if the instance isn't an int or a double.
         */
        operator int() const;

        /**
         * Cast the object to a double.
         * \throw json_exception if the instance isn't a double or an int.
         */
        operator double() const;

        /**
         * Cast the object to a string reference.
         * \throw json_exception if the instance isn't a string.
         */
        operator const std::string&() const;

        /**
         * Cast the object to a bool.
         * \throw json_exception if the instance isn't a bool.
         */
        operator bool() const;

        /**
         * Find an entry in an object instance by name. If the entry
         * doesn't exist, a new one is created with a json(null_e) 
         * value. Note, this is the same semantics as the STL map []
         * operator.
         * \throw json_exception if the instance isn't an object.
         */
        json &operator[](const std::string &name);

        /**
         * Find an entry in an object instance by name. If the entry
         * doesn't exist, a new one is created with a json(null_e) 
         * value. Note, this is the same semantics as the STL map []
         * operator.
         * \throw json_exception if the instance isn't an object.
         */
        json &operator[](const char *name);

        /**
         * Find an entry in an object instance by name. If the entry
         * doesn't exist, an exception is thrown.
         * \throw json_exception if the instance isn't an object.
         * \throw json_invalid_key_exception if the slot doesn't exist.
         */
        const json &operator[](const std::string &name) const;

        /**
         * Find an entry in an object instance by name. If the entry
         * doesn't exist, an exception is thrown.
         * \throw json_exception if the instance isn't an object.
         * \throw json_invalid_key_exception if the slot doesn't exist.
         */
        const json &operator[](const char *name) const;

        /**
         * Find an entry in an array instance by index. If the index
         * is out of range, an exception is thrown.
         * \throw json_exception if the instance isn't an array.
         * \throw json_array_index_range_exception if the index is out of range.
         */
        json &operator[](size_t index);

        /**
         * Find an entry in an array instance by index. If the index
         * is out of range, an exception is thrown. This method is
         * here to provide disambiguation from char * when a 0 literal is 
         * used.
         * \throw json_exception if the instance isn't an array.
         * \throw json_array_index_range_exception if the index is out of range.
         */
        json &operator[](int index);

        /**
         * Find an entry in an array instance by index. If the index
         * is out of range, an exception is thrown.
         * \throw json_exception if the instance isn't an array.
         * \throw json_array_index_range_exception if the index is out of range.
         */
        const json &operator[](size_t index) const;

        /**
         * Find an entry in an array instance by index. If the index
         * is out of range, an exception is thrown. This method is
         * here to provide disambiguation from char * when a 0 literal is 
         * \throw json_exception if the instance isn't an array.
         * \throw json_array_index_range_exception if the index is out of range.
         */
        const json &operator[](int index) const;

        /**
         * Append a value to an array instance. A copy is made of the value
         * so this can be inefficient for large structures. In that case use
         * the append method that takes a unique_ptr.
         * \throw json_exception if the instance isn't an array.
         */
        const json &append(const json &j);

        /**
         * Append a value to an array instance. Ownership of the passed instance
         * is taken over. E.g.:
         * \code
         * json array(json::array_e);
         * unique_ptr<json> i(new json(1000));
         * array.append(move(i));
         * \endcode
         * \throw json_exception if the instance isn't an array.
         */
        const json &append(std::unique_ptr<json> j);

        /**
         * Insert a value into an object instance. A copy is made of the value
         * so this can be inefficient for large structures. In that case use
         * the insert method that takes a unique_ptr. Any existing slot with
         * the same name will be overwitten.
         * \throw json_exception if the instance isn't an object.
         */
        const json &insert(const std::string &name, const json &j);

        /**
         * Insert a value into an object instance. Ownership of the passed
         * instance is taken over.
         * \code
         * json object(json::object_e);
         * unique_ptr<json> i(new json(1000));
         * object.insert("bob", move(i));
         * \endcode
         * \throw json_exception if the instance isn't an object.
         */
        const json &insert(const std::string &name, std::unique_ptr<json> j);

        /**
         * Gets the uninterpretted raw text for a scalar value from the JSON file or string that was parsed.
         * For objects and arrays, this will be empty.
         */
        const std::string &get_raw_value() const;

        /**
         * Equality operator. This will not work if either side is a raw value.
         * \throw   json_exception  if the instance is a raw value (constructed with json(type, raw_value).
         */
        bool operator==(const json &other) const;

        /**
         * Inequality operator. This will not work if either side is a raw value.
         * \throw   json_exception  if the instance is a raw value (constructed with json(type, raw_value).
         */
        bool operator!=(const json &other) const;

        /**
         * < operator. This will not work if either side is a raw value and only works for numbers
         * and strings. Strings can only be compared with strings but ints and double may be    
         * compared with each other and will be cast to doubles first.
         * \throw   json_exception  if either instance is a raw value (constructed with json(type, raw_value)
         *                          or if the types aren't comparable.
         */
        bool operator<(const json &other) const;

        /**
         * < operator. This will not work if either side is a raw value and only works for numbers
         * and strings. Strings can only be compared with strings but ints and double may be    
         * compared with each other and will be cast to doubles first.
         * \throw   json_exception  if either instance is a raw value (constructed with json(type, raw_value)
         *                          or if the types aren't comparable.
         */
        bool operator<=(const json &other) const;

        /**
         * > operator. This will not work if either side is a raw value and only works for numbers
         * and strings. Strings can only be compared with strings but ints and double may be    
         * compared with each other and will be cast to doubles first.
         * \throw   json_exception  if either instance is a raw value (constructed with json(type, raw_value)
         *                          or if the types aren't comparable.
         */
        bool operator>(const json &other) const;

        /**
         * > operator. This will not work if either side is a raw value and only works for numbers
         * and strings. Strings can only be compared with strings but ints and double may be    
         * compared with each other and will be cast to doubles first.
         * \throw   json_exception  if either instance is a raw value (constructed with json(type, raw_value)
         *                          or if the types aren't comparable.
         */
        bool operator>=(const json &other) const;

        /**
         * Full set of relevant comparison operators. 
         */

        /// == operator - throws for raw values 
        bool operator==(int i) const;
        /// == operator - throws for raw values 
        bool operator==(double d) const;
        /// == operator - throws for raw values 
        bool operator==(const std::string &s) const;
        /// == operator - throws for raw values 
        bool operator==(const char *s) const;
        /// != operator - throws for raw values 
        bool operator!=(int i) const;
        /// != operator - throws for raw values 
        bool operator!=(double d) const;
        /// != operator - throws for raw values 
        bool operator!=(const std::string &s) const;
        /// != operator - throws for raw values 
        bool operator!=(const char *s) const;
        /// < operator - throws for raw values 
        bool operator<(int i) const;
        /// < operator - throws for raw values 
        bool operator<(double d) const;
        /// < operator - throws for raw values 
        bool operator<(const std::string &s) const;
        /// < operator - throws for raw values 
        bool operator<(const char *s) const;
        /// <= operator - throws for raw values 
        bool operator<=(int i) const;
        /// <= operator - throws for raw values 
        bool operator<=(double d) const;
        /// <= operator - throws for raw values 
        bool operator<=(const std::string &s) const;
        /// <= operator - throws for raw values 
        bool operator<=(const char *s) const;
        /// > operator - throws for raw values 
        bool operator>(int i) const;
        /// > operator - throws for raw values 
        bool operator>(double d) const;
        /// > operator - throws for raw values 
        bool operator>(const std::string &s) const;
        /// > operator - throws for raw values 
        bool operator>(const char *s) const;
        /// >= operator - throws for raw values 
        bool operator>=(int i) const;
        /// >= operator - throws for raw values 
        bool operator>=(double d) const;
        /// >= operator - throws for raw values 
        bool operator>=(const std::string &s) const;
        /// >= operator - throws for raw values 
        bool operator>=(const char *s) const;

        /**
         * Find the thing pointed at by a pointer.
         * \throw   json_exception  If the pointer didn't match, a json_exception is throw.
         */
        const json &find(const pointer &p) const;

    private:

        /**
         * A union to hold the value of the json instance. Done as a union so as to
         * minimise space usage although that complicates constructors, destructors
         * etc.
         */
        typedef union
        {
            /// Objects - represented as an STL map of name -> json instance.
            std::map<std::string, std::unique_ptr<json>> *u_object;
            /// Arrays - STL vector of json instances.
            std::vector<std::unique_ptr<json>> *u_array;
            /// Bool value.
            bool u_boolean;
            /// int representation of a number (not set if the raw option is used).
            int u_number_int;
            /// double representation of a number (not set if the raw option is used).
            double u_number_double;
            /// UTF-8 string representation of tha value (not set if the raw option is used).
            std::string *u_string;
        }
        json_value;

        /**
         * The type of the specific instance. Note that, in order to get a sensible
         * mapping onto C++, the generic JSON number type is split into ints and
         * doubles.
         */
        type m_type;

        /// Value for the instance.
        json_value m_value;

        /**
         * Raw string value of int, double and string types. This is held in the case
         * where the value can't be represented as an int, double or STL string. This can 
         * happen for a variety of reasons:<br>
         *   An integer is too large to be held in the available int size.<br>
         *   An floating point number is too large to be held in the available double size.<br>
         *   A string contains characters and/or escape sequences outside those allowed by
         *   RFC7159. E.g. an invalid unicode sequence or an invalid \ escape sequence.<br>
         */
        std::string m_raw_value;

        /**
         * Reset the instance back to a null object. All memory is freed and all
         * values are overwritten.
         */
        void reset();

        /// copy the other object to this one
        void copy_json(const json &other);

        /// move the other object to this one
        void move_json(json &other);

        /// operator == helper method for numbers
        bool number_equal(const json &other) const;

        /// operator == helper method for strings
        bool string_equal(const json &other) const;

        /// operator == helper method for objects
        bool object_equal(const json &other) const;

        /// operator == helper method for arrays
        bool array_equal(const json &other) const;

    };
}

#endif
