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
 * @brief ½«×Ö·û´®×ª»»Îª¿í×Ö·û´®Ö¸Õë
 * @param[in] str ×Ö·û´®
 * @return ×ª»»ºóµÄ¿í×Ö·û´®Ö¸Õë
 */
LPWSTR str2lpwstr(const std::string& str) noexcept;

/**
 * @brief ½«¿í×Ö·û´®Ö¸Õë×ª»»Îª×Ö·û´®Ö¸Õë
 * @param w_str ¿í×Ö·û´®Ö¸Õë
 * @return ×ª»»ºóµÄ×Ö·û´®Ö¸Õë
 */
const char* lpwstr2lpcstr(LPCWSTR& w_str) noexcept;

void delptr(void* ptr);

#endif