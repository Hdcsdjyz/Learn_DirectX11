/**
 * @file ST_Exception.h
 * @author Lhxl
 * @date 2025-2-3
 * @version build4
 */

#ifndef ST_EXCEPTION_H
#define ST_EXCEPTION_H

#include <exception>
#include <string>

#include "ST_Window.h"
#include "function.h"

class ST_Exception : public std::exception {
public:
	ST_Exception(int line, LPCWSTR file) noexcept;
public:
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::wstring& GetFile() const noexcept;
	std::wstring GetOriginString() const noexcept;
protected:
	mutable std::wstring _whatBuffer;
private:
	int _line;
	std::wstring _file;
};

#endif