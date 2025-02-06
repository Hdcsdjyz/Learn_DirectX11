/**
 * @file function.cpp
 * @author Lhxl
 * @date 2025-2-3
 * @version build2
 */

#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <windows.h>

/**
 * @brief 将字符串转换为宽字符串指针
 * @param[in] str 字符串
 * @return 转换后的宽字符串指针
 */
LPWSTR str2lpwstr(const std::string& str) noexcept;

/**
 * @brief 将宽字符串指针转换为字符串指针
 * @param w_str 宽字符串指针
 * @return 转换后的字符串指针
 */
const char* lpwstr2lpcstr(LPCWSTR& w_str) noexcept;

void delptr(void* ptr);

#endif