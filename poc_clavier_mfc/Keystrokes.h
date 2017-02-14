#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <fstream>
#include <Windows.h>
typedef struct key key;

/*
	struct key
	represent key event
*/

struct key
{
	bool one_down;
	long long key_up, key_down;
	long vk_code;
	key::key(DWORD key_down, DWORD vkcode) :key_down(key_down), one_down(false), vk_code(vkcode){}
	key::key() :one_down(true), vk_code(-1){}
};

/*
	class Keystrokes
	Manage keys
*/

class Keystrokes
{
public:
	Keystrokes();
	~Keystrokes();

	// Event handler
	void set_timestamp_press(LPARAM& lp);
	void set_timestamp_release(LPARAM& lp);
	// Getter
	inline long long get_current_key_duration() { return current_key.key_up - current_key.key_down; }
	inline long long get_last_key_duration() { return keystrokes.back().key_up - keystrokes.back().key_down; }
	inline const key get_last_key() { return keystrokes.back(); }
	// Output function
	void write_data();

private:
	std::ofstream file;	// output file
	key current_key;	// current key
	std::vector<key> waiting_key;	// waiting keys
	std::vector<key> keystrokes;	// past keys
	inline void add_current_key() { keystrokes.push_back(current_key); write_data(); }
	
};

