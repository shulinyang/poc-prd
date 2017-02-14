#pragma once

#include <locale>
#include <codecvt>
#include <Windows.h>

inline std::string ConvertEncodingBack(const std::wstring& wideString);
void processKey(LPARAM lp);