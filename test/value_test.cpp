#include <catch.hpp>
#include <qmlbind.h>
#include <array>

TEST_CASE("value")
{
    auto engine = qmlbind_engine_new();
    auto global = qmlbind_engine_get_global_object(engine);

    SECTION("comparison")
    {
        auto one1 = qmlbind_value_new_number(1);
        auto one2 = qmlbind_value_new_number(1);
        auto strOne = qmlbind_value_new_string("1");

        SECTION("is_equal")
        {
            REQUIRE(qmlbind_value_is_equal(one1, strOne));
        }

        SECTION("is_identical")
        {
            REQUIRE(!qmlbind_value_is_identical(one1, strOne));
            REQUIRE(qmlbind_value_is_identical(one1, one2));
        }

        qmlbind_value_delete(one1);
        qmlbind_value_delete(one2);
        qmlbind_value_delete(strOne);
    }

    SECTION("undefined")
    {
        auto value = qmlbind_value_new_undefined();
        REQUIRE(qmlbind_value_is_undefined(value));
        qmlbind_value_delete(value);
    }

    SECTION("null")
    {
        auto value = qmlbind_value_new_null();
        REQUIRE(qmlbind_value_is_null(value));
        qmlbind_value_delete(value);
    }

    SECTION("number")
    {
        auto value = qmlbind_value_new_number(3.14);
        REQUIRE(qmlbind_value_is_number(value));
        REQUIRE(qmlbind_value_get_number(value) == 3.14);
        qmlbind_value_delete(value);
    }

    SECTION("string")
    {
        auto str = "ほげ";
        auto value = qmlbind_value_new_string(str);
        auto result = qmlbind_value_get_string(value);

        REQUIRE(qmlbind_value_is_string(value));
        REQUIRE(strcmp(qmlbind_string_get(result), str) == 0);

        qmlbind_string_delete(result);
        qmlbind_value_delete(value);
    }

    SECTION("object")
    {
        auto value = qmlbind_engine_new_object(engine);
        REQUIRE(qmlbind_value_is_object(value));

        auto prop = qmlbind_value_new_number(123);
        qmlbind_value_set_property(value, "prop", prop);
        qmlbind_value_delete(prop);

        prop = qmlbind_value_get_property(value, "prop");
        REQUIRE(qmlbind_value_get_number(prop) == 123);
        qmlbind_value_delete(prop);

        qmlbind_value_delete(value);
    }

    SECTION("function")
    {
        auto parseInt = qmlbind_value_get_property(global, "parseInt");

        REQUIRE(qmlbind_value_is_function(parseInt));

        auto str = qmlbind_value_new_string("123");
        auto parsed = qmlbind_value_call(parseInt, 1, &str);

        REQUIRE(qmlbind_value_get_number(parsed) == 123);

        qmlbind_value_delete(parseInt);
        qmlbind_value_delete(str);
        qmlbind_value_delete(parsed);
    }

    SECTION("construtor")
    {
        auto arrayConstructor = qmlbind_value_get_property(global, "Array");
        auto len = qmlbind_value_new_number(10);

        auto array = qmlbind_value_call_constructor(arrayConstructor, 1, &len);
        qmlbind_value_delete(len);

        REQUIRE(qmlbind_value_is_array(array));

        len = qmlbind_value_get_property(array, "length");
        REQUIRE(qmlbind_value_get_number(len) == 10);

        qmlbind_value_delete(array);
        qmlbind_value_delete(len);
        qmlbind_value_delete(arrayConstructor);
    }

    SECTION("array")
    {
        constexpr auto count = 10;

        auto array = qmlbind_engine_new_array(engine, count);

        for (auto i = 0; i < count; ++i) {
            auto value =  qmlbind_value_new_number(i);
            qmlbind_value_set_array_item(array, i, value);
            qmlbind_value_delete(value);
        }

        REQUIRE(qmlbind_value_is_array(array));

        for (auto i = 0; i < count; ++i) {
            auto value = qmlbind_value_get_array_item(array, i);
            REQUIRE(qmlbind_value_get_number(value) == i);
            qmlbind_value_delete(value);
        }

        qmlbind_value_delete(array);
    }

    SECTION("set / get prototype")
    {
        auto proto = qmlbind_engine_new_object(engine);

        {
            auto prop = qmlbind_value_new_number(123);
            qmlbind_value_set_property(proto, "prop", prop);
            qmlbind_value_delete(prop);
        }

        auto obj = qmlbind_engine_new_object(engine);
        qmlbind_value_set_prototype(obj, proto);

        {
            auto prop = qmlbind_value_get_property(obj, "prop");
            REQUIRE(qmlbind_value_get_number(prop) == 123);
            qmlbind_value_delete(prop);
        }

        auto gotProto = qmlbind_value_get_prototype(obj);
        REQUIRE(qmlbind_value_is_identical(proto, gotProto));

        qmlbind_value_delete(obj);
        qmlbind_value_delete(proto);
        qmlbind_value_delete(gotProto);
    }

    SECTION("error")
    {
        auto value = qmlbind_engine_eval(engine, "throw new Error('hoge')", "file", 1);
        REQUIRE(qmlbind_value_is_error(value));
        qmlbind_value_delete(value);
    }

    qmlbind_value_delete(global);
    qmlbind_engine_delete(engine);
}