/***
 * @file function.cpp
 * @auther Lhxl
 * @date 2025-2-3
 * @version build2
 ***/

#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <windows.h>

LPWSTR str2lpwstr(const std::string& str) noexcept;
const char* lpwstr2lpcstr(LPCWSTR& w_str) noexcept;
void delptr(void* ptr);

#endif