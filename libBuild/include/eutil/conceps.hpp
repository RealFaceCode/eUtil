#pragma once

#include <cstdint>

namespace util
{
    template<typename T> concept IsString_v = std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>;
    template<typename T> concept IsCharVector_v = std::is_same_v<T, std::vector<uint8_t>> || std::is_same_v<T, std::vector<std::byte>> || std::is_same_v<T, std::vector<char>>;
    template<typename T> concept IsChar_v = std::is_same_v<T, char> || std::is_same_v<T, wchar_t> || std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t>;
    template<typename T> concept IsArithmetic_v = std::is_arithmetic_v<T> || std::is_enum_v<T>;
    template<typename T> concept IsArithmeticVector_v = std::is_same_v<T, std::vector<uint8_t>> || std::is_same_v<T, std::vector<std::byte>> || std::is_same_v<T, std::vector<char>> || std::is_same_v<T, std::vector<int>> || std::is_same_v<T, std::vector<float>> || std::is_same_v<T, std::vector<double>>;
    template<typename T> concept IsStringVector_v = std::is_same_v<T, std::vector<std::string>> || std::is_same_v<T, std::vector<std::wstring>>;
    template<typename T> concept IsCharPtr_v = std::is_pointer_v<T> && (std::is_same_v<typename std::remove_pointer_t<T>, char> || std::is_same_v<typename std::remove_pointer_t<T>, wchar_t> || std::is_same_v<typename std::remove_pointer_t<T>, char16_t> || std::is_same_v<typename std::remove_pointer_t<T>, char32_t>);
    template<typename T> concept IsConstCharPtr_v = std::is_pointer_v<T> && (std::is_same_v<typename std::remove_pointer_t<T>, const char> || std::is_same_v<typename std::remove_pointer_t<T>, const wchar_t> || std::is_same_v<typename std::remove_pointer_t<T>, const char16_t> || std::is_same_v<typename std::remove_pointer_t<T>, const char32_t>);
}