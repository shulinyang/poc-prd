#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <fstream>
#include <Windows.h>
typedef struct key key;

struct key
{
	bool oneDown;
	long long keyUp, keyDown;
	long vkCode;
	key::key(DWORD keyDown, DWORD vkcode)
		:keyDown(keyDown), oneDown(false), vkCode(vkcode)
	{
	}
	key::key()
		: oneDown(true), vkCode(-1)
	{}
};

class Keystrokes
{
public:
	Keystrokes();
	~Keystrokes();

	void setTSDown(LPARAM& lp);
	void setTSUp(LPARAM& lp);
	inline long long getCurrentTimePressed() { return currentKey.keyUp - currentKey.keyDown; }
	inline long long getLastTimePressed() { return keystrokes.back().keyUp - keystrokes.back().keyDown; }
	inline const key getLastKey() { return keystrokes.back(); }
	void writeData();

private:
	std::ofstream file;
	key currentKey;
	std::vector<key> waitingKey;
	std::vector<key> keystrokes;
	inline void addCurrentKey() { keystrokes.push_back(currentKey); writeData(); }
	
};

