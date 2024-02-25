#include <iostream>

#include "binaryarraybuffer.hpp"
#include "../test_util.hpp"

int main()
{
    eUtil::BinaryArrayBuffer buffer;

    buffer.reserve(10);
    ::compare(10, buffer.capacity(), "Capacity after reserve");

    buffer.resize(5);
    ::compare(5, buffer.size(), "Size after resize");

    buffer.clear();
    ::compare(0, buffer.size(), "Size after clear");
    ::compare(10, buffer.capacity(), "Capacity after clear");

    buffer.push_back(10);
    buffer.push_back(20);
    buffer.push_back(30);

    ::compare(3 * sizeof(int), buffer.size(), "Size after push_back");
    ::compare(12, buffer.capacity(), "Capacity after push_back");

    ::compare(10, buffer.getNextElement<int>(), "First element");
    ::compare(20, buffer.getNextElement<int>(), "Second element");
    ::compare(30, buffer.getNextElement<int>(), "Third element");

    buffer.clear();

    buffer.push_back(std::string("Hello"));
    buffer.push_back(std::string("World"));

    ::compare(10 + (2 * sizeof(uint64_t)), buffer.size(), "Size after push_back");
    ::compare(10 + (2 * sizeof(uint64_t)), buffer.capacity(), "Capacity after push_back");

    ::compare(std::string("Hello"), buffer.getNextElement<std::string>(), "First string");
    ::compare(std::string("World"), buffer.getNextElement<std::string>(), "Second string");

    buffer.clear();
    
    std::string str = "Hello";
    std::wstring wstr = L"World";

    buffer.push_back(str);
    buffer.push_back(wstr);

    ::compare(15 + (2 * sizeof(uint64_t)), buffer.size(), "Size after push_back");
    ::compare(15 + (2 * sizeof(uint64_t)), buffer.capacity(), "Capacity after push_back");

    ::compare(std::string("Hello"), buffer.getNextElement<std::string>(), "First string");
    ::compare(std::wstring(L"World"), buffer.getNextElement<std::wstring>(), "Second string");

    return ::FAILED;
}