#include "Frappe.h"
#include <algorithm>
#include <iostream>

Keystrokes::Keystrokes()
	:file("keys.data", std::ios::ate)
{
}

Keystrokes::~Keystrokes()
{
	file.close();
}

void Keystrokes::setTSDown(LPARAM& lp)
{
	KBDLLHOOKSTRUCT st_hook = *((KBDLLHOOKSTRUCT*)lp);

	if (currentKey.oneDown)
	{
			currentKey.vkCode = st_hook.vkCode;
			currentKey.keyDown = st_hook.time;
			currentKey.oneDown = false;
	}
	else if (currentKey.vkCode != st_hook.vkCode)
	{
		waitingKey.push_back(currentKey);
		currentKey = key(st_hook.time, st_hook.vkCode);
	}
}

void Keystrokes::setTSUp(LPARAM& lp)
{
	KBDLLHOOKSTRUCT st_hook = *((KBDLLHOOKSTRUCT*)lp);
	
	if (currentKey.vkCode == st_hook.vkCode)
	{
		currentKey.keyUp = st_hook.time;
		currentKey.oneDown = true;
		addCurrentKey();
	}
	else
		for (size_t i = 0; i < waitingKey.size(); i++)
			if (waitingKey[i].vkCode == st_hook.vkCode)
			{
				waitingKey[i].keyUp = st_hook.time;
				keystrokes.push_back(waitingKey[i]);
				waitingKey.erase(waitingKey.begin() + i);
				break;
			}
}

void Keystrokes::writeData()
{
	if (file.is_open())
	{
		file << keystrokes.back().vkCode << ";" << keystrokes.back().keyDown << ";" << keystrokes.back().keyUp << std::endl;
	}
	else
		std::cerr << "File Failed to open" << std::endl;
}
