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

/// \file json.cpp The json class.

#include <iterator>
#include <algorithm>

#include "common.hpp"
#include "json.hpp"
#include "json_invalid_key_exception.hpp"
#include "json_array_index_range_exception.hpp"

using namespace NAMESPACE;

json::json() noexcept : m_type(null_e)
{
}

json::~json() noexcept
{
    reset();
}

// objects

void json::destroy_object() noexcept
{
    m_value.u_object.~json_object();
}

void json::construct_object()
{
    new (&m_value.u_object) json::json_object;
}

void json::move_construct_object(json_object&& o)
{
    construct_object();
    std::move(o.begin(), o.end(), std::inserter(m_value.u_object, m_value.u_object.begin()));
}

void json::copy_construct_object(const json_object &o)
{
    construct_object();
    std::transform(o.begin(), o.end(), std::inserter(m_value.u_object, m_value.u_object.begin()),
                   [] (const std::pair<const std::string, std::unique_ptr<json>> &p)
                        { return std::pair<const std::string, std::unique_ptr<json>>(p.first, std::unique_ptr<json>(new json(*p.second))); });
}

// arrays

void json::destroy_array() noexcept
{
    m_value.u_array.~json_array();
}

void json::construct_array()
{
    new (&m_value.u_array) json::json_array;
}

void json::move_construct_array(json_array&& a)
{
    construct_array();
    std::move(a.begin(), a.end(), std::back_inserter(m_value.u_array));
}

void json::copy_construct_array(const json_array &a)
{
    construct_array();
    std::copy(a.begin(), a.end(), std::back_inserter(m_value.u_array));
}

// strings

void json::become_string(std::string s)
{
    m_type = string_e;
    construct_string(std::move(s));
}

void json::destroy_string() noexcept
{
    m_value.u_string.~basic_string();
}

void json::construct_string()
{
    new (&m_value.u_string) std::string;
}

void json::construct_string(std::string s)
{
    new (&m_value.u_string) std::string(std::move(s));
}

void json::reset() noexcept
{
    switch (m_type)
    {
    case object_e:
        destroy_object();
        break;
    case array_e:
        destroy_array();
        break;
    case string_e:
        destroy_string();
        break;
    default:
        break;
    }

    m_type = null_e;
    m_raw_value.clear();
}

void json::copy_json(const json &other)
{
    if (this == &other) return;

    reset();
    m_type = other.m_type;
    m_raw_value = other.m_raw_value;

    if (m_type == object_e)
    {
        copy_construct_object(other.m_value.u_object);
    }
    else if (m_type == array_e)
    {
        copy_construct_array(other.m_value.u_array);
    }
    else if (m_type == string_e)
    {
        construct_string(other.m_value.u_string);
    }
    else if (m_type == boolean_e)
    {
        m_value.u_boolean = other.m_value.u_boolean;
    }
    else if (m_type == number_int_e)
    {
        m_value.u_number_int = other.m_value.u_number_int;
    }
    else if (m_type == number_double_e)
    {
        m_value.u_number_double = other.m_value.u_number_double;
    }
}

json::json(const json &other) : m_type(null_e)
{
    copy_json(other);
}

json &json::operator=(const json &other)
{
    copy_json(other);
    return *this;
}

void json::move_json(json &other)
{
    if (this == &other) return;

    reset();
    m_type = other.m_type;
    if (m_type == object_e)
    {
        move_construct_object(std::move(other.m_value.u_object));
    }
    else if (m_type == array_e)
    {
        move_construct_array(std::move(other.m_value.u_array));
    }
    else if (m_type == string_e)
    {
        construct_string(other.m_value.u_string);
    }
    else if (m_type == boolean_e)
    {
        m_value.u_boolean = other.m_value.u_boolean;
    }
    else if (m_type == number_int_e)
    {
        m_value.u_number_int = other.m_value.u_number_int;
    }
    else if (m_type == number_double_e)
    {
        m_value.u_number_double = other.m_value.u_number_double;
    }

    m_raw_value = std::move(other.m_raw_value);

    other.m_type = null_e;
}

json &json::operator=(json &&other) noexcept
{
    move_json(other);
    return *this;
}

json::json(json &&other) noexcept : m_type(null_e)
{
    move_json(other);
}

json::json(type t) : m_type(t)
{
    switch (m_type)
    {
    case object_e:
        construct_object();
        break;
    case array_e:
        construct_array();
        break;
    case string_e:
        construct_string();
        break;
    case boolean_e:
        m_value.u_boolean = false;
        break;
    case number_int_e:
        m_value.u_number_int = 0;
        break;
    case number_double_e:
        m_value.u_number_double = 0;
        break;
    case null_e:
        break;
    default:
        throw json_exception(json_exception::invalid_json_type_e);
    }
}

json::json(type t, std::string raw_value) : m_type(t), m_raw_value(std::move(raw_value))
{
    if (t != string_e && t != number_int_e && t != number_double_e)
    {
        throw json_exception(json_exception::not_number_or_string_e, get_instance_type_name());
    }

    if (t == string_e)
    {
        construct_string();
    }
}

json::json(int i) noexcept
{
    m_type = number_int_e;
    m_value.u_number_int = i;
}

json::json(double d) noexcept
{
    m_type = number_double_e;
    m_value.u_number_double = d;
}

json::json(bool b) noexcept
{
    m_type = boolean_e;
    m_value.u_boolean = b;
}

json::json(std::string s)
{
    become_string(std::move(s));
}

json::json(const char *s)
{
    become_string(s);
}

json::json(null_t) noexcept : m_type(null_e)
{
}

json::json(std::unique_ptr<std::string> s)
{
    become_string(*s.release());
}

json &json::operator=(int i)
{
    reset();
    m_type = number_int_e;
    m_value.u_number_int = i;
    return *this;
}

json &json::operator=(double d)
{
    reset();
    m_type = number_double_e;
    m_value.u_number_double = d;
    return *this;
}

json &json::operator=(bool b)
{
    reset();
    m_type = boolean_e;
    m_value.u_boolean = b;
    return *this;
}

json &json::operator=(std::string s)
{
    reset();
    become_string(std::move(s));
    return *this;
}

json &json::operator=(const char *s)
{
    reset();
    become_string(s);
    return *this;
}

json &json::operator=(std::unique_ptr<std::string> s)
{
    reset();
    become_string(*s);
    return *this;
}

json &json::operator=(const json::json_object &o)
{
    reset();
    m_type = object_e;
    copy_construct_object(o);
    return *this;
}

json &json::operator=(const json_array &a)
{
    reset();
    m_type = array_e;
    copy_construct_array(a);
    return *this;
}

json &json::operator=(null_t)
{
    reset();
    m_type = null_e;
    return *this;
}

json::type json::get_instance_type() const
{
    return m_type;
}

json::json_array &json::get_array()
{
    ensure_type(array_e, json_exception::not_an_array_e);
    return m_value.u_array;
}

const json::json_array &json::get_array() const
{
    ensure_type(array_e, json_exception::not_an_array_e);
    return m_value.u_array;
}

json::json_object &json::get_object()
{
    ensure_type(object_e, json_exception::not_an_object_e);
    return m_value.u_object;
}

const json::json_object &json::get_object() const
{
    ensure_type(object_e, json_exception::not_an_object_e);
    return m_value.u_object;
}

bool json::has(const std::string &name) const
{
    const json_object& o = get_object();
    return o.find(name) != o.end();
}

const std::string &json::get_raw_value() const
{
    return m_raw_value;
}

json::operator int() const
{
    if (m_raw_value.size() > 0)
    {
        throw json_exception(json_exception::cant_cast_raw_e);
    }
    else if (m_type == number_int_e)
    {
        return m_value.u_number_int;
    }
    else if (m_type == number_double_e)
    {
        return static_cast<int>(m_value.u_number_double);
    }
    else
    {
        throw json_exception(json_exception::not_number_e);
    }
}

json::operator double() const
{
    if (m_raw_value.size() > 0)
    {
        throw json_exception(json_exception::cant_cast_raw_e);
    }
    else if (m_type == number_double_e)
    {
        return m_value.u_number_double;
    }
    else if (m_type == number_int_e)
    {
        return static_cast<double>(m_value.u_number_int);
    }
    else
    {
        throw json_exception(json_exception::not_number_e);
    }
}

json::operator const std::string&() const
{
    if (m_raw_value.size() > 0)
    {
        throw json_exception(json_exception::cant_cast_raw_e);
    }
    else if (m_type == string_e)
    {
        return m_value.u_string;
    }
    else
    {
        throw json_exception(json_exception::not_string_e, get_instance_type_name());
    }
}

json::operator bool() const
{
    if (m_type == boolean_e)
    {
        return m_value.u_boolean;
    }
    else if (m_type == number_int_e)
    {
        return m_value.u_number_int != 0;
    }
    else
    {
        throw json_exception(json_exception::not_number_int_or_boolean_e, get_instance_type_name());
    }
}

json &json::operator[](const std::string &name)
{
    json_object& o = get_object();
    auto i = o.find(name);
    if (i == o.end())
    {
        return *((o)[name] = std::unique_ptr<json>(new json));
    }
    else
    {
        return *(i->second);
    }
}

json &json::operator[](const char *name)
{
    return (*this)[std::string(name)];
}

json &json::operator[](size_t index)
{
    json_array& a = get_array();
    if (index < a.size())
    {
        return (a)[index];
    }
    else
    {
        throw json_array_index_range_exception(json_exception::array_index_range_e, index);
    }
}

json &json::operator[](int index)
{
    if (index >= 0)
    {
        return (*this)[static_cast<size_t>(index)];
    }
    else
    {
        throw json_array_index_range_exception(json_exception::array_index_range_e, index);
    }
}

const json &json::operator[](size_t index) const
{
    const json_array& a = get_array();
    if (index < a.size())
    {
        return (a)[index];
    }
    else
    {
        throw json_array_index_range_exception(json_exception::array_index_range_e, index);
    }
}

const json &json::operator[](int index) const
{
    if (index >= 0)
    {
        return (*this)[static_cast<size_t>(index)];
    }
    else
    {
        throw json_array_index_range_exception(json_exception::array_index_range_e, index);
    }
}

const json &json::operator[](const std::string &name) const
{
    const json_object& o = get_object();
    auto i = o.find(name);
    if (i == o.end())
    {
        throw json_invalid_key_exception(json_exception::invalid_key_e, name);
    }
    else
    {
        return *(i->second);
    }
}

const json &json::operator[](const char *name) const
{
    return (*this)[std::string(name)];
}

const json &json::append(const json &j)
{
    json_array& a = get_array();
    a.push_back(j);
    return a.back();
}

const json &json::append(std::unique_ptr<json> j)
{
    return append(*j.release());
}

const json &json::insert(const std::string &name, const json &j)
{
    return insert(name, std::unique_ptr<json>(new json(j)));
}

const json &json::insert(const std::string &name, std::unique_ptr<json> j)
{
    json_object& o = get_object();
    json *r;
    o.emplace(name, std::unique_ptr<json>(r = j.release()));
    return *r;
}

const char *json::get_instance_type_name() const
{
    switch (m_type)
    {
    case object_e:
        return "object";
    case array_e:
        return "array";
    case boolean_e:
        return "boolean";
    case null_e:
        return "null";
    case number_int_e:
        return "number (int)";
    case number_double_e:
        return "number (double)";
    case string_e:
        return "string";
    default:
        return "corrupted";
    }
}

bool json::number_equal(const json &other) const
{
    if (m_raw_value.size() == 0 && other.m_raw_value.size() == 0)
    {
        if (m_type == number_int_e)
        {
            return m_value.u_number_int == other.m_value.u_number_int;
        }
        else
        {
            // Will only work if they are exactly the same in all bits.
            return m_value.u_number_double == other.m_value.u_number_double;
        }
    }
    else
    {
        throw json_exception(json_exception::cant_compare_raw_e);
    }
}

bool json::string_equal(const json &other) const
{
    if (m_raw_value.size() == 0 && other.m_raw_value.size() == 0)
    {
        return m_value.u_string == other.m_value.u_string;
    }
    else
    {
        throw json_exception(json_exception::cant_compare_raw_e);
    }
}

bool json::object_equal(const json &other) const
{
    return m_value.u_object.size() == other.m_value.u_object.size() &&
            equal(
                m_value.u_object.begin(),
                m_value.u_object.end(),
                other.m_value.u_object.begin(),
                [] (const std::pair<const std::string, std::unique_ptr<json>> &a, const std::pair<const std::string, std::unique_ptr<json>> &b)
                        { return a.first == b.first && *(a.second) == *(b.second); });
}

bool json::array_equal(const json &other) const
{
    return m_value.u_array.size() == other.m_value.u_array.size() &&
            equal(
                m_value.u_array.begin(),
                m_value.u_array.end(),
                other.m_value.u_array.begin(),
                [] (const json &a, const json &b)
                        { return a == b; });
}

bool json::operator==(const json &other) const
{
    if (m_type == other.m_type)
    {
        switch (m_type)
        {
        case object_e:
            return object_equal(other);
        case array_e:
            return array_equal(other);
        case boolean_e:
            return m_value.u_boolean == other.m_value.u_boolean;
        case null_e:
            return true;
        case number_int_e:
            return number_equal(other);
        case number_double_e:
            return number_equal(other);
        case string_e:
            return string_equal(other);
        default:
            throw json_exception(json_exception::invalid_json_type_e);
        }
    }
    else if ((m_type == number_int_e && other.m_type == number_double_e) ||
             (m_type == number_double_e && other.m_type == number_int_e))
    {
        return static_cast<double>(*this) == static_cast<double>(other);
    }
    else
    {
        return false;
    }
}


bool json::operator!=(const json &other) const
{
    return !(*this == other);
}

bool json::operator==(int i) const
{
    return static_cast<int>(*this) == i;
}

bool json::operator==(double d) const
{
    return static_cast<double>(*this) == d;
}

bool json::operator==(const std::string &s) const
{
    return static_cast<std::string>(*this) == s;
}

bool json::operator==(const char *s) const
{
    return static_cast<std::string>(*this) == s;
}

bool json::operator!=(int i) const
{
    return static_cast<int>(*this) != i;
}

bool json::operator!=(double d) const
{
    return static_cast<double>(*this) != d;
}

bool json::operator!=(const std::string &s) const
{
    return static_cast<std::string>(*this) != s;
}

bool json::operator!=(const char *s) const
{
    return static_cast<std::string>(*this) != s;
}

bool json::operator<(int i) const
{
    return static_cast<int>(*this) < i;
}

bool json::operator<(double d) const
{
    return static_cast<double>(*this) < d;
}

bool json::operator<(const std::string &s) const
{
    return static_cast<std::string>(*this) < s;
}

bool json::operator<(const char *s) const
{
    return static_cast<std::string>(*this) < s;
}

bool json::operator<=(int i) const
{
    return static_cast<int>(*this) <= i;
}

bool json::operator<=(double d) const
{
    return static_cast<double>(*this) <= d;
}

bool json::operator<=(const std::string &s) const
{
    return static_cast<std::string>(*this) < s;
}

bool json::operator<=(const char *s) const
{
    return static_cast<std::string>(*this) <= s;
}

bool json::operator>(int i) const
{
    return static_cast<int>(*this) > i;
}

bool json::operator>(double d) const
{
    return static_cast<double>(*this) > d;
}

bool json::operator>(const std::string &s) const
{
    return static_cast<std::string>(*this) > s;
}

bool json::operator>(const char *s) const
{
    return static_cast<std::string>(*this) > s;
}

bool json::operator>=(int i) const
{
    return static_cast<int>(*this) >= i;
}

bool json::operator>=(double d) const
{
    return static_cast<double>(*this) >= d;
}

bool json::operator>=(const std::string &s) const
{
    return static_cast<std::string>(*this) >= s;
}

bool json::operator>=(const char *s) const
{
    return static_cast<std::string>(*this) >= s;
}

bool json::operator<(const json &other) const
{
    if (m_type == string_e && other.m_type == string_e)
    {
        return static_cast<std::string>(*this) < static_cast<std::string>(other);
    }
    else if (m_type == number_int_e && other.m_type == number_int_e)
    {
        return static_cast<int>(*this) < static_cast<int>(other);
    }
    else if ((m_type == number_double_e && other.m_type == number_double_e) ||
             (m_type == number_double_e && other.m_type == number_int_e) ||
             (m_type == number_int_e && other.m_type == number_double_e))
    {
        return static_cast<double>(*this) < static_cast<double>(other);
    }
    else
    {
        throw json_exception(
                        json_exception::types_not_comparable_e,
                        get_instance_type_name(),
                        other.get_instance_type_name());
    }
}

bool json::operator<=(const json &other) const
{
    if (m_type == string_e && other.m_type == string_e)
    {
        return static_cast<std::string>(*this) <= static_cast<std::string>(other);
    }
    else if (m_type == number_int_e && other.m_type == number_int_e)
    {
        return static_cast<int>(*this) <= static_cast<int>(other);
    }
    else if ((m_type == number_double_e && other.m_type == number_double_e) ||
             (m_type == number_double_e && other.m_type == number_int_e) ||
             (m_type == number_int_e && other.m_type == number_double_e))
    {
        return static_cast<double>(*this) <= static_cast<double>(other);
    }
    else
    {
        throw json_exception(
                        json_exception::types_not_comparable_e,
                        get_instance_type_name(),
                        other.get_instance_type_name());
    }
}

bool json::operator>(const json &other) const
{
    return !(*this <= other);
}

bool json::operator>=(const json &other) const
{
    return !(*this < other);
}

const json &json::find(const pointer &p) const
{
    const json *res = this;

    for (auto t : p.get_path())
    {
        switch (t.get_type())
        {
        case pointer::token::all_e:
            break;

        case pointer::token::object_e:
            if (res->m_type == object_e)
            {
                auto i = res->m_value.u_object.find(t.get_name());
                if (i == res->m_value.u_object.end())
                {
                    throw json_exception(json_exception::pointer_not_matched_e);
                }
                else
                {
                    res = i->second.get();
                }
            }
            else
            {
                throw json_exception(json_exception::pointer_not_matched_e);
            }
            break;

        case pointer::token::array_e:
            if (res->m_type == array_e)
            {
                if (t.get_index() < res->m_value.u_array.size())
                {
                    res = &(res->m_value.u_array)[t.get_index()];
                }
                else
                {
                    throw json_exception(json_exception::pointer_not_matched_e);
                }
            }
            else
            {
                throw json_exception(json_exception::pointer_not_matched_e);
            }
            break;

        default:
            throw json_exception(json_exception::pointer_token_type_invalid_e);
        }
    }

    return *res;
}

void json::ensure_type(type t, int ex) const
{
    if (m_type != t)
    {
        throw json_exception(json_exception::exception_type(ex), get_instance_type_name());
    }
}
