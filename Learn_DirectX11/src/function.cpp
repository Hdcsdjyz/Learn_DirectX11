/***
 * @file function.cpp
 * @auther Lhxl
 * @date 2025-2-3
 * @version build2
 ***/

#include "function.h"

LPWSTR str2lpwstr(const std::string& str) noexcept {
	const char* ctr = str.c_str();
	int wsize = MultiByteToWideChar(CP_OEMCP, 0, ctr, strlen(ctr) + 1, NULL, 0);
	LPWSTR wctr = new WCHAR[wsize];
	MultiByteToWideChar(CP_OEMCP, 0, ctr, strlen(ctr) + 1, wctr, wsize);
	ctr = nullptr;
	return wctr;
}

const char* lpwstr2lpcstr(LPCWSTR& w_str) noexcept {
	char* c_str = nullptr;
	int len = WideCharToMultiByte(CP_ACP, 0, w_str, wcslen(w_str), NULL, 0, NULL, NULL);
	c_str = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, w_str, wcslen(w_str), c_str, len, NULL, NULL);
	c_str[len] = '\0';
	return c_str;
}

void delptr(void* ptr) {
	delete ptr;
	ptr = nullptr;
}