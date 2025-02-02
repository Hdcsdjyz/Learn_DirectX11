/***
 * @file ST_Window_MessageMap.h
 * @auther Lhxl
 * @date 2025-2-3
 * @version build2
 ***/

#ifndef MESSAGEMAP_H
#define MESSAGEMAP_H

#include <Windows.h>
#include <unordered_map>
#include <string>

class MessageMap
{
public:
	MessageMap();
	LPWSTR operator()(DWORD msg, LPARAM lp, WPARAM wp) const;
private:
	std::unordered_map<DWORD, std::string> map;
};

#endif