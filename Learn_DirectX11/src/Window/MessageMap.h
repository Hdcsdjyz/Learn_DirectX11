/**
 * @file MessageMap.h
 * @author Lhxl
 * @date 2025-2-5
 * @version build7
 */

#ifndef MESSAGEMAP_H
#define MESSAGEMAP_H

#include <Windows.h>
#include <unordered_map>
#include <string>

/**
 * @brief 窗口信息
 */
class MessageMap
{
public:
	MessageMap();
	LPWSTR operator()(DWORD msg, LPARAM lp, WPARAM wp) const;
private:
	std::unordered_map<DWORD, std::string> _map;
};

#endif