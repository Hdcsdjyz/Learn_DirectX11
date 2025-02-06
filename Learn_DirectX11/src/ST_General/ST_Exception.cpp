/**
 * @file ST_Exception.cpp
 * @author Lhxl
 * @date 2025-2-3
 * @version build4
 */

#include <sstream>

#include"ST_Exception.h"

ST_Exception::ST_Exception(int line, LPCWSTR file) noexcept : _line(line), _file(file) {

}

const char* ST_Exception::what() const noexcept {
	std::wostringstream woss;
	woss << GetType() << std::endl
		 << GetOriginString();
	_whatBuffer = woss.str();
	LPCWSTR w_str = _whatBuffer.c_str();
	return lpwstr2lpcstr(w_str);
}

const char* ST_Exception::GetType() const noexcept {
	return "ST_Exception";
}

int ST_Exception::GetLine() const noexcept {
	return _line;
}

const std::wstring& ST_Exception::GetFile() const noexcept {
	return _file;
}

std::wstring ST_Exception::GetOriginString() const noexcept {
	std::wostringstream woss;
	woss << L"[文件]: " << _file << std::endl
		 << L"[行号]: " << _line;
	std::wstring w_str = woss.str();
	return w_str;
}
