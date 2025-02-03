/**
 * @file function.cpp
 * @auther Lhxl
 * @date 2025-2-3
 * @version build2
 */

#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <windows.h>

/**
 * @brief ½«×Ö·û´®×ª»»Îª¿í×Ö·û´®
 * @param[in] str ×Ö·û´®
 * @return ×ª»»ºóµÄ¿í×Ö·û´®
 */
LPWSTR str2lpwstr(const std::string& str) noexcept;

/**
 * @brief ½«¿í×Ö·û´®×ª»»Îª×Ö·û´®
 * @param w_str ¿í×Ö·û´®
 * @return ×ª»»ºóµÄ×Ö·û´®
 */
const char* lpwstr2lpcstr(LPCWSTR& w_str) noexcept;

void delptr(void* ptr);

#endif