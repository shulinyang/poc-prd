#include "Keystrokes.h"
#include <algorithm>
#include <iostream>

Keystrokes::Keystrokes():file("keys.data", std::ios::app){}

Keystrokes::~Keystrokes()
{
	file.close();
}

void Keystrokes::set_timestamp_press(LPARAM& lp)
{
	KBDLLHOOKSTRUCT* st_hook = ((KBDLLHOOKSTRUCT*)lp);

	if (current_key.one_down && st_hook->flags!=16)
	{
			current_key.vk_code = st_hook->vkCode;
			current_key.key_down = st_hook->time;
			current_key.one_down = false;
#ifdef _DEBUG
			std::cout << "flag: " << st_hook->flags << " ";
			std::cout << "vkcode: " << st_hook->vk_code << " ";
#endif // DEBUG

	}
	else if (current_key.vk_code != st_hook->vkCode && st_hook->flags!=16)
	{
		waiting_key.push_back(current_key);
		current_key = key(st_hook->time, st_hook->vkCode);
	}
}

void Keystrokes::set_timestamp_release(LPARAM& lp)
{
	KBDLLHOOKSTRUCT* st_hook = ((KBDLLHOOKSTRUCT*)lp);
	
	if (current_key.vk_code == st_hook->vkCode)
	{
		current_key.key_up = st_hook->time;
		current_key.one_down = true;
		add_current_key();
	}
	else if (st_hook->flags != 16)
		for (size_t i = 0; i < waiting_key.size(); i++)
			if (waiting_key[i].vk_code == st_hook->vkCode)
			{
				waiting_key[i].key_up = st_hook->time;
				keystrokes.push_back(waiting_key[i]);
				waiting_key.erase(waiting_key.begin() + i);
				break;
			}
}

void Keystrokes::write_data()
{
	if (file.is_open())
	{
		file << keystrokes.back().vk_code << ";" << keystrokes.back().key_down << ";" << keystrokes.back().key_up << std::endl;
	}
	else
		std::cerr << "File Failed to open" << std::endl;
}
