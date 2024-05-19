#include <iostream>

#include "binarybuffer.hpp"
#include "../test_util.hpp"

struct test_struct
{
    int a;
    float b;
    double c;
};

int main()
{
    eutil::BinaryBuffer bb;
    bb.push<std::string>("Hello, World!");
    bb.push<std::wstring>(L"Hello, World!");

    auto str = bb.get<std::string>();
    auto wstr = bb.get<std::wstring>();

    compare("Hello, World!", str, "String test");
    compare(L"Hello, World!", wstr, "WString test");

    bb.push<uint64_t>(1234567890);
    auto num = bb.get<uint64_t>();
    compare(1234567890, num, "Number test");

    test_struct ts = { 1, 2.0f, 3.0 };
    bb.push<test_struct>(ts);
    auto ts2 = bb.get<test_struct>();
    compare(ts.a, ts2.a, "Struct test: a");
    compare(ts.b, ts2.b, "Struct test: b");
    compare(ts.c, ts2.c, "Struct test: c");

    return ::FAILED;
}