#include "utilities.h"
#include <iostream>


void processKey(LPARAM lp)
{
	DWORD msg = 1;
	KBDLLHOOKSTRUCT st_hook = *((KBDLLHOOKSTRUCT*)lp);

	msg += (st_hook.scanCode << 16);
	msg += (st_hook.flags << 24);
	wchar_t key[255];
	GetKeyNameText(msg, key, 0xFF);

	static bool capslock = false;
	static bool shift = false;
	std::wstring tmpStr(key);
	bool printable = (tmpStr.length() <= 1) ? true : false;

	if (tmpStr == L"ESPACE")
	{
		tmpStr = L" ";
		printable = true;
	}

	if (!printable)
	{
		if (tmpStr == L"VERR.MAJ")
			capslock = !capslock;
		else if (tmpStr == L"MAJ")
			shift = true;
	}
	else
	{
		if (shift == capslock)
		{
			for (size_t i = 0; i < tmpStr.length(); ++i)
				tmpStr[i] = tolower(tmpStr[i]);
		}
		else
		{
			for (size_t i = 0; i < tmpStr.length(); ++i)
				if (tmpStr[i] >= 'A' && tmpStr[i] <= 'Z')
					tmpStr[i] = toupper(tmpStr[i]);
		}
		std::cout << "converted key: " << ConvertEncodingBack(tmpStr) << " ";
	}
}

inline std::string ConvertEncodingBack(const std::wstring& wideString)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
	return convert.to_bytes(wideString);
}