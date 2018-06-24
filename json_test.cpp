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

/// \file json_test.cpp Argo test code.

#include <iostream>
#include <fstream>
#include <sstream>
#include "argo.hpp"

#ifndef _ARGO_WINDOWS_
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

using namespace std;
using namespace argo;

ofstream jlog;

void test_scalar_constructors()
{
    jlog << "test_scalar_constructors()\n";
    json null;
    jlog << null << endl;
    json t(true);
    jlog << t << endl;
    json f(false);
    jlog << f << endl;
    json i(123);
    jlog << i << endl;
    json d(123.4);
    jlog << d << endl;
    json s("asdasd");
    jlog << s << endl;
}

void test_scalar_casts()
{
    jlog << "test_scalar_casts()\n";

    json t(true);
    bool tc = t;
    jlog << tc << endl;
    json f(false);
    bool fc = f;
    jlog << fc << endl;
    json i(123);
    int ic = i;
    double icd = i;
    bool icb = i;
    jlog << ic << endl;
    jlog << icd << endl;
    jlog << (icb ? "true" : "false") << endl;
    json d(123.4);
    double dc = d;
    int dci = d;
    jlog << dc << endl;
    jlog << dci << endl;
    json s("asdasd");
    string sc = s;
    jlog << sc << endl;
}

void test_scalar_equality()
{
    jlog << "test_scalar_equality()\n";

    json j_null1;
    json j_null2;

    if (j_null1 == j_null2)
    {
        jlog << "PASS: null pass\n";
    }
    if (j_null1 != j_null2)
    {
        jlog << "FAIL: null fail\n";
    }

    json j_int1(100);
    json j_int2(100);
    json j_int3(200);

    if (j_int1 == j_int2)
    {
        jlog << "PASS: int pass\n";
    }
    if (j_int1 == j_int3)
    {
        jlog << "FAIL: int fail\n";
    }
    if (j_int1 != j_int2)
    {
        jlog << "FAIL: int fail\n";
    }

    json j_double1(100.1);
    json j_double2(100.1);
    json j_double3(200.1);

    if (j_double1 == j_double2)
    {
        jlog << "PASS: double pass\n";
    }
    if (j_double1 == j_double3)
    {
        jlog << "FAIL: double fail\n";
    }
    if (j_double1 != j_double2)
    {
        jlog << "FAIL: double fail\n";
    }

    json j_string1("asdf");
    json j_string2("asdf");
    json j_string3("dfoiuoiuoiusdf");

    if (j_string1 == j_string2)
    {
        jlog << "PASS: string pass\n";
    }
    if (j_string1 == j_string3)
    {
        jlog << "FAIL: string fail\n";
    }
    if (j_string1 != j_string2)
    {
        jlog << "FAIL: string fail\n";
    }

    json dt1(1);
    json dt2(1.0);
    json dt3(true);

    if (dt1 == dt2)
    {
        jlog << "PASS: int/double pass\n";
    }

    if (dt2 == dt1)
    {
        jlog << "PASS: double/int pass\n";
    }

    if (dt1 != dt3)
    {
        jlog << "PASS: missmatched types pass\n";
    }
}

void test_scalar_assign()
{
    jlog << "test_scalar_assign()\n";

    json null;
    json null1 = null;
    jlog << null1 << endl;

    json t(true);
    json t1 = t;
    jlog << t1 << endl;

    json f(false);
    json f1 = f;
    jlog << f1 << endl;

    json i(123);
    json i1 = i;
    jlog << i1 << endl;

    json d(123.4);
    json d1 = d;
    jlog << d1 << endl;

    json s("asdasd");
    json s1 = s;
    jlog << s1 << endl;
}

void test_scalar_copy()
{
    jlog << "test_scalar_copy()\n";

    json null;
    json null1(null);
    jlog << null1 << endl;

    json t(true);
    json t1(t);
    jlog << t1 << endl;

    json f(false);
    json f1(f);
    jlog << f1 << endl;

    json i(123);
    json i1(i);
    jlog << i1 << endl;

    json d(123.4);
    json d1(d);
    jlog << d1 << endl;

    json s("asdasd");
    json s1(s);
    jlog << s1 << endl;
}

void test_scalar_move()
{
    vector<json> vj;

    vj.push_back(json());
    vj.push_back(true);
    vj.push_back(false);
    vj.push_back(1);
    vj.push_back(1.1);
    vj.push_back("asdasd");

    for (const auto &i : vj)
    {
        jlog << i << endl;
    }
}

void test_get_type()
{
    json j1;

    if (j1.get_instance_type() != json::null_e)
    {
        jlog << "FAIL: get type null failed" << endl;
    }
    else
    {
        jlog << j1.get_instance_type_name() << endl;
    }

    json j2(true);

    if (j2.get_instance_type() != json::boolean_e)
    {
        jlog << "FAIL: get type boolean failed" << endl;
    }
    else
    {
        jlog << j2.get_instance_type_name() << endl;
    }

    json j3(100);

    if (j3.get_instance_type() != json::number_int_e)
    {
        jlog << "FAIL: get type number_int failed" << endl;
    }
    else
    {
        jlog << j3.get_instance_type_name() << endl;
    }

    json j4(100.1);

    if (j4.get_instance_type() != json::number_double_e)
    {
        jlog << "FAIL: get type number_double failed" << endl;
    }
    else
    {
        jlog << j4.get_instance_type_name() << endl;
    }

    json j5("sdfsdf");

    if (j5.get_instance_type() != json::string_e)
    {
        jlog << "FAIL: get type number_double failed" << endl;
    }
    else
    {
        jlog << j5.get_instance_type_name() << endl;
    }

    json j6(json::array_e);

    if (j6.get_instance_type() != json::array_e)
    {
        jlog << "FAIL: get type array failed" << endl;
    }
    else
    {
        jlog << j6.get_instance_type_name() << endl;
    }

    json j7(json::object_e);

    if (j7.get_instance_type() != json::object_e)
    {
        jlog << "FAIL: get type object failed" << endl;
    }
    else
    {
        jlog << j7.get_instance_type_name() << endl;
    }
}

void test_raw_values()
{
    json raw_int(json::string_e, "123");
    json raw_double(json::string_e, "123.456");
    json raw_string(json::string_e, "asdf");

    jlog << raw_int << endl;
    jlog << raw_double << endl;
    jlog << raw_string << endl;
}

void smoke_test()
{
    {
        auto j1 = parser::load("test_files/test2.json");
        unparser::save(*j1, "test_files/tmp.json");
        auto j2 = parser::load("test_files/test2.json");
        if (*j1 == *j2)
        {
            jlog << "PASS: load and dump and load again produced equal results\n";
        }
        else
        {
            jlog << "FAIL: load and dump and load again produced unequal results\n";
        }
    }

    // now with conversions and an extre dump/load cycle to get everything into the same state
    {
        auto j1 = parser::load("test_files/test2.json");
        unparser::save(*j1, "test_files/tmp.json");

        j1 = parser::load("test_files/tmp.json");
        unparser::save(*j1, "test_files/tmp.json");

        auto j2 = parser::load("test_files/tmp.json");

        if (*j1 == *j2)
        {
            jlog << "PASS: load and dump and load again produced equal results\n";
        }
        else
        {
            jlog << "FAIL: load and dump and load again produced unequal results\n";
        }
    }
}

void test_square_brackets_const_array(const json &j)
{
    jlog << "const array access " << j[0] << endl;
}

void test_square_brackets_const_object(const json &j)
{
    jlog << "const object access " << j["c"] << endl;
}

void test_square_brackets()
{
    json a(json::array_e);
    a.append(10);
    a.append(20.0);
    a.append("asdf");
    a.append(true);

    test_square_brackets_const_array(a);

    if ((a[0] != 10) ||
        (a[1] != 20.0) ||
        (a[2] != "asdf") ||
         !a[3])
    {
        jlog << "FAIL: array access failed\n";
    }

    json o(json::object_e);
    if (o["a"] != json())
    {
        jlog << "FAIL: empty element isn't null - failed\n";
    }
    o["b"] = 1;
    o["c"] = 1.1;
    o["d"] = "asdf";
    o["e"] = true;

    test_square_brackets_const_object(o);

    if ((o["b"] != 1) ||
        (o["c"] != 1.1) ||
        (o["d"] != "asdf") ||
        !o["e"])
    {
        jlog << "FAIL: object access failed\n";
    }
}

void test_comparison_operators_same(json &v, json &same, json &greater)
{
    if (v == same)
    {
        jlog << "PASS: == same worked\n";
    }
    else
    {
        jlog << "FAIL: == same failed\n";
    }

    if (v != same)
    {
        jlog << "FAIL: != same failed\n";
    }
    else
    {
        jlog << "PASS: != same worked\n";
    }

    if (v == greater)
    {
        jlog << "FAIL: == greater failed\n";
    }
    else
    {
        jlog << "PASS: == greater worked\n";
    }

    if (v != greater)
    {
        jlog << "PASS: == greater worked\n";
    }
    else
    {
        jlog << "FAIL: == greater failed\n";
    }

    if (v != same)
    {
        jlog << "FAIL: != same failed\n";
    }
    else
    {
        jlog << "PASS: != same worked\n";
    }

    if (v <= same)
    {
        jlog << "PASS: <= same worked\n";
    }
    else
    {
        jlog << "FAIL: <= same failed\n";
    }

    if (v <= greater)
    {
        jlog << "PASS: <= greater worked\n";
    }
    else
    {
        jlog << "FAIL: <= greater failed\n";
    }

    if (v >= same)
    {
        jlog << "PASS: >= same worked\n";
    }
    else
    {
        jlog << "FAIL: >= same failed\n";
    }

    if (v >= greater)
    {
        jlog << "FAIL: >= greater failed\n";
    }
    else
    {
        jlog << "PASS: >= greater worked \n";
    }

    if (v < same)
    {
        jlog << "FAIL: < same failed\n";
    }
    else
    {
        jlog << "PASS: < same worked\n";
    }

    if (v < greater)
    {
        jlog << "PASS: < greater worked\n";
    }
    else
    {
        jlog << "FAIL: < greater failed\n";
    }

    if (v > same)
    {
        jlog << "FAIL: > same failed\n";
    }
    else
    {
        jlog << "PASS: > same worked\n";
    }

    if (v > greater)
    {
        jlog << "FAIL: > greater failed\n";
    }
    else
    {
        jlog << "PASS: > greater worked\n";
    }
}

void test_comparison_operators()
{
    // test < > <= >= == != where both sides are the same type

    json i1(1);
    json i2(1);
    json i3(2);
    jlog << "testing int comparisons\n";
    test_comparison_operators_same(i1, i2, i3);
    json d1(1.0);
    json d2(1.0);
    json d3(2.0);
    jlog << "testing double comparisons\n";
    test_comparison_operators_same(d1, d2, d3);
    json s1("a");
    json s2("a");
    json s3("b");
    jlog << "testing string comparisons\n";
    test_comparison_operators_same(d1, d2, d3);

    // test < > <= >= == != where one side is an int and the other is a double
    jlog << "testing int/double comparisons\n";
    test_comparison_operators_same(i1, i2, i3);
    test_comparison_operators_same(i1, i2, d3);
    test_comparison_operators_same(i1, d2, i3);
    test_comparison_operators_same(i1, d2, d3);
    test_comparison_operators_same(d1, i2, i3);
    test_comparison_operators_same(d1, i2, d3);
    test_comparison_operators_same(d1, d2, i3);
    test_comparison_operators_same(d1, d2, d3);
}
    
void test_append()
{
    json a(json::array_e);
    const json &ae1 = a.append(1);
    jlog << "appended was " << ae1 << " len is " << a.get_array().size() << endl;
    const json &ae2 = a.append("asdf");
    jlog << "appended was " << ae2 << " len is " << a.get_array().size() << endl;
    unique_ptr<json> ae3(new json(1.23));
    a.append(move(ae3));
    jlog << " len is " << a.get_array().size() << endl;
    jlog << a << endl;
}

void test_insert()
{
    json o(json::object_e);

    o.insert("one", 1);
    o.insert("two", 2.123);
    o.insert("three", "asfdsad");
    unique_ptr<json> x(new json(1.23));
    o.insert("four", move(x));

    jlog << o << endl;
}

void test_const_exception(const json &j)
{
    try
    {
        (void)j["aaa"];
        jlog << "FAIL: attempt to access non-existing element of const object failed to throw exception\n";
    }
    catch (json_invalid_key_exception &e)
    {
        if (e.get_type() != json_exception::invalid_key_e)
        {
            jlog << "FAIL: access non-existing element of const object threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: access non-existing element of const object threw correct exception type\n";
        }
    }
}

void test_json_class_exceptions()
{
    try
    {
        json j(static_cast<json::type>(100));
        jlog << "FAIL: construct invalid type failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::invalid_json_type_e)
        {
            jlog << "FAIL: construct invalid type threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: construct invalid type threw correct exception type\n";
        }
    }

    try
    {
        json j(json::array_e, "asdadasd");
        jlog << "FAIL: construct invalid type with raw value failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::not_number_or_string_e)
        {
            jlog << "FAIL: construct invalid type with raw value threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: construct invalid type with raw value threw correct exception type\n";
        }
    }

    try
    {
        json j(json::number_int_e, "100");
        (void)static_cast<int>(j);
        jlog << "FAIL: cast raw to int failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::cant_cast_raw_e)
        {
            jlog << "FAIL: cast raw to int threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: cast raw to int threw correct exception type\n";
        }
    }

    try
    {
        json j("100");
        (void)static_cast<int>(j);
        jlog << "FAIL: cast string to int failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::not_number_e)
        {
            jlog << "FAIL: cast string to int threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: cast string to int threw correct exception type\n";
        }
    }

    try
    {
        json j(json::number_double_e, "100");
        (void)static_cast<double>(j);
        jlog << "FAIL: cast raw double to double failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::cant_cast_raw_e)
        {
            jlog << "FAIL: cast raw double to double threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: cast raw double to double threw correct exception type\n";
        }
    }

    try
    {
        json j("100");
        (void)static_cast<double>(j);
        jlog << "FAIL: cast string to double failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::not_number_e)
        {
            jlog << "FAIL: cast string to double threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: cast string to double threw correct exception type\n";
        }
    }

    try
    {
        json j(json::string_e, "100");
        (void)static_cast<string>(j);
        jlog << "FAIL: cast raw string to string failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::cant_cast_raw_e)
        {
            jlog << "FAIL: cast raw string to string threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: cast raw string to string threw correct exception type\n";
        }
    }

    try
    {
        json j(100);
        (void)static_cast<string>(j);
        jlog << "FAIL: cast int to string failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::not_string_e)
        {
            jlog << "FAIL: cast int to string threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: cast int to string threw correct exception type\n";
        }
    }

    try
    {
        json j("asdasd");
        (void)static_cast<bool>(j);
        jlog << "FAIL: cast string to bool failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::not_number_int_or_boolean_e)
        {
            jlog << "FAIL: cast string to bool threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: cast string to bool threw correct exception type\n";
        }
    }

    try
    {
        json j("asdasd");
        (void)j["asdasd"];
        jlog << "FAIL: attempt to access string via [\"name\"] failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::not_an_object_e)
        {
            jlog << "FAIL: access string via [\"name\"] threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: access string via [\"name\"] threw correct exception type\n";
        }
    }

    try
    {
        json j("asdasd");
        (void)j[1];
        jlog << "FAIL: attempt to access string via [1] failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::not_an_array_e)
        {
            jlog << "FAIL: access string via [1] threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: access string via [1] threw correct exception type\n";
        }
    }

    try
    {
        json j(json::array_e);
        (void)j[1];
        jlog << "FAIL: attempt to access item 1 of zero length array failed to throw exception\n";
    }
    catch (json_array_index_range_exception &e)
    {
        if (e.get_type() != json_exception::array_index_range_e)
        {
            jlog << "FAIL: access empty array via [1] threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: access empty array via [1] threw correct exception type\n";
        }
    }

    try
    {
        json j(json::array_e);
        (void)j[-1];
        jlog << "FAIL: attempt to access item -1 of zero length array failed to throw exception\n";
    }
    catch (json_array_index_range_exception &e)
    {
        if (e.get_type() != json_exception::array_index_range_e)
        {
            jlog << "FAIL: access empty array via [-1] threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: access empty array via [-1] threw correct exception type\n";
        }
    }

    json j(json::object_e);
    test_const_exception(j);

    try
    {
        json j(1);
        j.append(json(1));
        jlog << "FAIL: attempt to append to non-array failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::not_an_array_e)
        {
            jlog << "FAIL: append to non-array threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: append to non-array threw correct exception type\n";
        }
    }

    try
    {
        json j(1);
        j.insert("asdasd", json(1));
        jlog << "FAIL: attempt insert into non-object failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::not_an_object_e)
        {
            jlog << "FAIL: insert into non-object threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: insert into non-object threw correct exception type\n";
        }
    }

    try
    {
        json r1(json::number_int_e, "123");
        json r2(json::number_int_e, "123");
        if (r1 == r2)
        {
            jlog << "PASS: attempt to compare raw ints failed to throw exception\n";
        }
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::cant_compare_raw_e)
        {
            jlog << "FAIL: compare raw ints threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: compare raw ints threw correct exception type\n";
        }
    }

    try
    {
        json r1(json::number_int_e, "123");
        if (r1 == 123)
        {
            jlog << "FAIL: attempt to compare raw int to int failed to throw exception\n";
        }
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::cant_cast_raw_e)
        {
            jlog << "FAIL: compare raw int with int threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: compare raw int with int threw correct exception type\n";
        }
    }

    try
    {
        json r1(json::string_e, "123");
        json r2(json::string_e, "123");
        if (r1 == r2)
        {
            jlog << "FAIL: attempt to compare raw strings failed to throw exception\n";
        }
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::cant_compare_raw_e)
        {
            jlog << "FAIL: compare raw strings threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: compare raw strings threw correct exception type\n";
        }
    }

    try
    {
        json r1(json::string_e, "123");
        if (r1 == "123")
        {
            jlog << "FAIL: attempt to compare raw strings failed to throw exception\n";
        }
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::cant_cast_raw_e)
        {
            jlog << "FAIL: compare raw strings threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: compare raw strings threw correct exception type\n";
        }
    }

    try
    {
        json j1(100);
        json j2("asdasd");
        if (j1 < j2) {};
        jlog << "FAIL: < compare incomparable types failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::types_not_comparable_e)
        {
            jlog << "FAIL: < compare incomparable types threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: < compare incomparable types threw correct exception type\n";
        }
    }

    try
    {
        json j1(100);
        json j2("asdasd");
        if (j1 <= j2) {};
        jlog << "FAIL: <= compare incomparable types failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::types_not_comparable_e)
        {
            jlog << "FAIL: <= compare incomparable types threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: <= compare incomparable types threw correct exception type\n";
        }
    }

    try
    {
        json j1(100);
        json j2("asdasd");
        if (j1 > j2) {};
        jlog << "FAIL: > compare incomparable types failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::types_not_comparable_e)
        {
            jlog << "FAIL: > compare incomparable types threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: > compare incomparable types threw correct exception type\n";
        }
    }

    try
    {
        json j1(100);
        json j2("asdasd");
        if (j1 <= j2) {};
        jlog << "FAIL: >= compare incomparable types failed to throw exception\n";
    }
    catch (json_exception &e)
    {
        if (e.get_type() != json_exception::types_not_comparable_e)
        {
            jlog << "FAIL: >= compare incomparable types threw wrong exception type\n";
            jlog << e.what() << endl;
        }
        else
        {
            jlog << "PASS: >= compare incomparable types threw correct exception type\n";
        }
    }
}

void test_strings()
{
    ifstream f("test_files/strings/tests.txt");

    string line;

    while (getline(f, line))
    {
        auto x = line.find_first_of(" ");
        auto file_name = line.substr(0, x);
        auto valid = line.substr(x + 1, line.size() - x -1);

        try
        {
            auto j = parser::load("test_files/strings/" + file_name);
            if (valid == "valid")
            {
                unparser::save(*j, "test_files/tmp.json");
                auto j1 = parser::load("test_files/tmp.json");
                if (*j1 == *j)
                {
                    jlog << "PASS: UTF8 test passed, valid file " << file_name << " was read, written, reread and compared\n";
                }
                else
                {
                    jlog << "FAIL: UTF8 test failed, valid file " << file_name << " was read, written, reread and came out different\n";
                }
            }
            else
            {
                jlog << "FAIL: UTF8 test failed, invalid file " << file_name << " didn't cause an exception\n";
            }
        }
        catch (exception &e)
        {
            if (valid == "valid")
            {
                jlog << "FAIL: UTF8 test failed, valid file " << file_name << " caused an exception\n";
                jlog << e.what() << endl;
            }
            else
            {
                jlog << "PASS: UTF8 test passed, invalid file " << file_name << " caused an exception:" << e.what() << endl;
            }
        }
    }
}

void test_parse()
{
    ifstream f("test_files/general/tests.txt");

    string line;

    while (getline(f, line))
    {
        auto x = line.find_first_of(" ");
        auto file_name = line.substr(0, x);
        auto valid = line.substr(x + 1, line.size() - x -1);

        try
        {
            auto j = parser::load("test_files/general/" + file_name);
            if (valid == "valid")
            {
                unparser::save(*j, "test_files/tmp.json");
                auto j1 = parser::load("test_files/tmp.json");
                if (*j1 == *j)
                {
                    jlog << "PASS: parse test passed, valid file " << file_name << " was read, written, reread and compared\n";
                }
                else
                {
                    jlog << "FAIL: parse test failed, valid file " << file_name << " was read, written, reread and came out different\n";
                }
            }
            else
            {
                jlog << "FAIL: parse test failed, invalid file " << file_name << " didn't cause an exception\n";
            }
        }
        catch (exception &e)
        {
            if (valid == "valid")
            {
                jlog << "FAIL: parse test failed, valid file " << file_name << " caused an exception\n";
                jlog << e.what() << endl;
            }
            else
            {
                jlog << "PASS: parse test passed, invalid file " << file_name << " caused an exception:" << e.what() << endl;
            }
        }
    }
}

void test_numbers()
{
    string s = "123";
    auto j = argo::parser::parse(s);
    jlog << static_cast<int>(*j) << endl;
    s = "-123";
    j = argo::parser::parse(s);
    jlog << static_cast<int>(*j) << endl;

    s = "123.456";
    j = argo::parser::parse(s);
    jlog << static_cast<int>(*j) << endl;

    s = "-123.456";
    j = argo::parser::parse(s);
    jlog << static_cast<double>(*j) << endl;

    s = "123.456e+3";
    j = argo::parser::parse(s);
    jlog << static_cast<double>(*j) << endl;

    s = "123.456e-3";
    j = argo::parser::parse(s);
    jlog << static_cast<double>(*j) << endl;

    s = "123.456e-2000";
    j = argo::parser::parse(s);
    jlog << static_cast<double>(*j) << endl;

    try
    {
        s = "123.456e+2000";
        j = argo::parser::parse(s);
        jlog << static_cast<double>(*j) << endl;
        jlog << "FAIL: range test 1 failed, no exception thrown " << s << endl;
    }
    catch (json_exception &e)
    {
        if (e.get_type() == json_exception::number_out_of_range_e)
        {
            jlog << "PASS: range test 1 passed, correct exception type thrown:" << e.what() << endl;
        }
        else
        {
            jlog << "FAIL: range test 1 failed, incorrect exception type thrown:" << e.what() << endl;
        }
    }

    try
    {
        s = "9999999999999999999999999999999999999999";
        for (int i = 0; i < 5; i++)
        {
            s = s + s;
        }
        j = argo::parser::parse(s);
        jlog << static_cast<int>(*j) << endl;
        jlog << "FAIL: range test 2 failed, no exception thrown " << static_cast<double>(*j) << endl;
    }
    catch (json_exception &e)
    {
        if (e.get_type() == json_exception::number_out_of_range_e)
        {
            jlog << "PASS: range 2 test passed, correct exception type thrown:" << e.what() << endl;
        }
        else
        {
            jlog << "FAIL: range 2 test failed, incorrect exception type thrown:" << e.what() << endl;
        }
    }

    try
    {
        s = "1aaaa";
        j = argo::parser::parse(s);
        jlog << static_cast<double>(*j) << endl;
        jlog << "FAIL: bad format test 1 failed, no exception thrown " << s << endl;
    }
    catch (json_exception &e)
    {
        if (e.get_type() == json_exception::invalid_number_e ||
            e.get_type() == json_exception::invalid_character_e)
        {
            jlog << "PASS: bad format test 1 passed, correct exception type thrown:" << e.what() << endl;
        }
        else
        {
            jlog << "FAIL: bad format test 1 failed, incorrect exception type thrown:" << e.what() << endl;
        }
    }

    try
    {
        s = "1.1aaaa";
        j = argo::parser::parse(s);
        jlog << static_cast<double>(*j) << endl;
        jlog << "FAIL: bad format test 2 failed, no exception thrown " << s << endl;
    }
    catch (json_exception &e)
    {
        if (e.get_type() == json_exception::invalid_number_e ||
            e.get_type() == json_exception::invalid_character_e)
        {
            jlog << "PASS: bad format test 2 passed, correct exception type thrown:" << e.what() << endl;
        }
        else
        {
            jlog << "FAIL: bad format test 2 failed, incorrect exception type thrown:" << e.what() << endl;
        }
    }

    try
    {
        s = "1.1e1";
        j = argo::parser::parse(s);
        jlog << static_cast<double>(*j) << endl;
        jlog << "FAIL: bad format test 3 failed, no exception thrown " << s << endl;
    }
    catch (json_exception &e)
    {
        if (e.get_type() == json_exception::invalid_number_e ||
            e.get_type() == json_exception::invalid_character_e)
        {
            jlog << "PASS: bad format test 3 passed, correct exception type thrown:" << e.what() << endl;
        }
        else
        {
            jlog << "FAIL: bad format test 3 failed, incorrect exception type thrown:" << e.what() << endl;
        }
    }
}

void test_reader(reader &r)
{
    int sum = 0;
    int c = r.next();
    sum += c;
    jlog << "FIRST CHAR : " << c << endl;
    int n = 1;
    while ((c = r.next() != EOF)) 
    {
        sum += c;
        n++;
    }
    jlog << "LENGTH : " << n << endl;
    jlog << "SUM    : " << sum << endl;
}

void test_readers_on_file(const string &file_name)
{

#ifndef _ARGO_WINDOWS_
    {
        jlog << "TESTING FD READER - BLOCK READER\n";

        int fd = open(file_name.c_str(), O_RDONLY);
        fd_reader r(fd, 20000, true);
        test_reader(r);
        close(fd);
    }

    {
        jlog << "TESTING FD READER - BYTE READER\n";

        int fd = open(file_name.c_str(), O_RDONLY);
        fd_reader r(fd, 20000, false);
        test_reader(r);
        close(fd);
    }
#endif

    {
        jlog << "TESTING IOSTREAM READER - BLOCK READER\n";

        ifstream f(file_name);
        stream_reader r(&f, 20000, true);
        test_reader(r);
    }

    {
        jlog << "TESTING IOSTREAM READER - BYTE READER\n";

        ifstream f(file_name);
        stream_reader r(&f, 20000, false);
        test_reader(r);
    }

    {
        jlog << "TESTING FILE READER - BLOCK READER\n";

        FILE *f = fopen(file_name.c_str(), "r");
        file_reader r(f, 20000, true);
        test_reader(r);
        fclose(f);
    }

    {
        jlog << "TESTING FILE READER - BYTE READER\n";

        FILE *f = fopen(file_name.c_str(), "r");
        file_reader r(f, 20000, false);
        test_reader(r);
        fclose(f);
    }

}

void test_readers()
{
    test_readers_on_file("test_files/test3.json");
    test_readers_on_file("test_files/8192bytes.test");
    test_readers_on_file("test_files/8193bytes.test");
    test_readers_on_file("test_files/8293bytes.test");
    test_readers_on_file("test_files/16384bytes.test");
    test_readers_on_file("test_files/16385bytes.test");
}

void test_writer(string name)
{
    FILE *f = fopen("test_files/tmp.json", "r");
    if (fseek(f, 0, SEEK_END) != 13)
    {
        jlog << "FAIL: " << name << " writer file length is not 13 : " << endl;
    }
    else
    {
        fclose(f);
        auto j = parser::load("test_files/tmp.json");
        if ((*j)["one"] == 1)
        {
            jlog << "PASS: " << name << " value of field one is 1" << endl;
        }
        else
        {
            jlog << "FAIL: " << name << " value of field one is not 1" << endl;
        }
    }
}

void test_writers()
{
    {
        ofstream f("test_files/tmp.json");
        stream_writer w(&f);
        w << "{ \"one\" : 1 }";
        f.close();
        test_writer("stream");
    }

    {
        FILE *f = fopen("test_files/tmp.json", "w");
        file_writer w(f);
        w << "{ \"one\" : 1 }";
        fclose(f);
        test_writer("file");
    }

#ifndef _ARGO_WINDOWS_
    {
        int f = open("test_files/tmp.json", O_CREAT | O_WRONLY, 0);
        fd_writer w(f);
        w << "{ \"one\" : 1 }";
        close(f);
        test_writer("fd");
    }
#endif
}
void test_options()
{
    // test message length limit
    {
        try
        {
            ifstream f("test_files/test2.json");
            stream_reader r(&f, 100, true);
            parser p(r);
            auto j = p.parse();
            jlog << "FAIL: message too long but parsed anyway\n";
        }
        catch (json_io_exception &e)
        {
            if (e.get_type() == json_exception::message_too_long_e)
            {
                jlog << "PASS: message too long threw correct exception type\n";
            }
            else
            {
                jlog << "FAIL: message too long threw incorrect exception type\n";
            }
        }
    }

    // test reading more than one message from file (in byte read mode)
    {
        try
        {
            ifstream f("test_files/test6.json");
            stream_reader r(&f, 1000, false);
            parser p(r, false);
            auto j1 = p.parse();
            if ((*j1)["one"] != 1)
            {
                jlog << "FAIL: first message broken\n";
            }
            auto j2 = p.parse();
            if ((*j2)["two"] != 2)
            {
                jlog << "FAIL: second message broken\n";
            }
            auto j3 = p.parse();
            if ((*j3)["three"] != 3)
            {
                jlog << "FAIL: third message broken\n";
            }
            jlog << "PASS: parsed three messages from file\n";
        }
        catch (exception &e)
        {
            jlog << "FAIL: failed to parse three messages from file : " << e.what() << endl;
        }
    }

    // test don't fallback to doubles for large ints
    {
        try
        {
            istringstream is("99999999999999999999");
            stream_reader r(&is, 1000, false);
            parser p(r, true, 1000, 1000, true, false, true);
            auto j1 = p.parse();
            jlog << "FAIL: int that was too large to be an int didn't cause an exception\n";
        }
        catch (json_parser_exception &e)
        {
            if (e.get_type() == json_exception::number_out_of_range_e)  
            {
                jlog << "PASS: int that was too large to be an int threw correct exception type\n";
            }
            else
            {
                jlog << "FAIL: int that was too large to be an int threw incorrect exception type\n";
            }
        }
    }

    // test don't convert numbers
    {
        try
        {
            istringstream is("99999999999999999999.99");
            stream_reader r(&is, 1000, false);
            parser p(r, true, 1000, 1000, false, true, true);
            auto j1 = p.parse();
            if (j1->get_raw_value() == "99999999999999999999.99")
            {
                jlog << "PASS: don't convert number returned the correct raw value\n";
            }
            else
            {
                jlog << "FAIL: don't convert number returned an incorrect raw value\n";
            }
        }
        catch (json_parser_exception)
        {
            jlog << "FAIL: don't convert number threw an exception\n";
        }
    }

    // test don't convert strings
    {
        try
        {
            istringstream is("\" lkjlkj lkjlkj \\x \\y \"");
            stream_reader r(&is, 1000, false);
            parser p(r, true, 1000, 1000, true, true, false);
            auto j1 = p.parse();
            if (j1->get_raw_value() == " lkjlkj lkjlkj \\x \\y ")
            {
                jlog << "PASS: don't convert string returned the correct raw value\n";
            }
            else
            {
                jlog << "FAIL: don't convert string returned an incorrect raw value:'" << j1->get_raw_value() << "'" << endl;
            }
        }
        catch (json_parser_exception)
        {
            jlog << "FAIL: don't convert string threw an exception\n";
        }
    }

    // test nesting depth limit
    {
        try
        {
            istringstream is("[[[[[[[[[[]]]]]]]]]]");
            stream_reader r(&is, 1000, false);
            parser p(r, true, 1000, 9, true, true, true);
            auto j1 = p.parse();
            jlog << "FAIL: nesting depth exceeded didn't cause exception\n";
        }
        catch (json_parser_exception &e)
        {
            if (e.get_type() == json_exception::maximum_nesting_depth_exceeded_e)
            {
                jlog << "PASS: message nested too deeply threw correct exception type\n";
            }
            else
            {
                jlog << "FAIL: message nested too deeply threw incorrect exception type\n";
            }
        }
    }
}

int check_results()
{
    ifstream f1("test_files/known_good_jlog.txt");
    ifstream f2("test_files/jlog.txt");

    string line1;
    string line2;

    while (getline(f1, line1))
    {
        if (getline(f2, line2))
        {
            if (line1 == line2)
            {
                continue;
            }
        }
        cerr << "TESTS FAILED - compare test_files/jlog.txt with test_files/known_good_jlog.txt\n";
        return 1;
    }

    (void)getline(f2, line2);

    if (f2.eof())
    {
        cout << "TESTS PASSED\n";
    }
    else
    {
        cerr << "TESTS FAILED - compare test_files/jlog.txt with test_files/known_good_jlog.txt\n";
        return 1;
    }

    return 0;
}

void test_pointer()
{
    // RFC tests
    {
        auto j = parser::load("test_files/test7.json");

        jlog << j->find(pointer("")) << endl;
        jlog << j->find(pointer("/foo")) << endl;
        jlog << j->find(pointer("/foo/0")) << endl;
        jlog << j->find(pointer("/")) << endl;
        jlog << j->find(pointer("/a~1b")) << endl;
        jlog << j->find(pointer("/c%d")) << endl;
        jlog << j->find(pointer("/e^f")) << endl;
        jlog << j->find(pointer("/g|h")) << endl;
        jlog << j->find(pointer("/i\\\\j")) << endl;
        jlog << j->find(pointer("/k\"l")) << endl;
        jlog << j->find(pointer("/ ")) << endl;
        jlog << j->find(pointer("/m~0n")) << endl;

        jlog << j->find(pointer("#")) << endl;
        jlog << j->find(pointer("#/foo")) << endl;
        jlog << j->find(pointer("#/foo/0")) << endl;
        jlog << j->find(pointer("#/")) << endl;
        jlog << j->find(pointer("#/a~1b")) << endl;
        jlog << j->find(pointer("#/c%25d")) << endl;
        jlog << j->find(pointer("#/e%5Ef")) << endl;
        jlog << j->find(pointer("#/g%7Ch")) << endl;
        jlog << j->find(pointer("#/i%5Cj")) << endl;
        jlog << j->find(pointer("#/k%22l")) << endl;
        jlog << j->find(pointer("#/%20")) << endl;
        jlog << j->find(pointer("#/m~0n")) << endl;
    }

    ifstream f("test_files/pointer_tests.txt");

    string line;

    while (getline(f, line))
    {
        auto x = line.find_first_of(":");
        auto pointer_text = line.substr(0, x);
        auto valid = line.substr(x + 1, line.size() - x -1);

        try
        {
            pointer p(pointer_text);
            if (valid == "valid")
            {
                jlog << "PASS: pointer " << pointer_text << " compiled successfully" << endl;
            }
            else
            {
                jlog << "FAIL: invalid pointer " << pointer_text << " compiled successfully" << endl;
            }
        }
        catch (exception &e)
        {
            if (valid == "valid")
            {
                jlog << "FAIL: pointer " << pointer_text << " failed to compile" << endl;
                jlog << e.what() << endl;
            }
            else
            {
                jlog << "PASS: pointer " << pointer_text << " caused an exception" << endl;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    jlog.open("test_files/jlog.txt");
    try
    {
        smoke_test();
        test_scalar_constructors();
        test_scalar_assign();
        test_scalar_copy();
        test_scalar_equality();
        test_scalar_move();
        test_get_type();
        test_raw_values();
        test_comparison_operators();
        test_append();
        test_insert();
        test_square_brackets();
        test_scalar_casts();
        test_json_class_exceptions();
        test_numbers();
        test_readers();
        test_writers();
        test_strings();
        test_parse();
        test_options();
        test_pointer();
    }
    catch (json_exception &e)
    {
        jlog << e.what() << endl;
    }

    jlog.close();
    exit(check_results());
}
