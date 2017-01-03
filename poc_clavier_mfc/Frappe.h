#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <Windows.h>

class Keystroke
{
public:
	Keystroke();
	~Keystroke();

	inline const std::vector<std::pair<LARGE_INTEGER, LARGE_INTEGER>> getKeystockes() { return keystrokes; }
	inline const long long getLastKeyDuration() { return (keystrokes.back().second.QuadPart - keystrokes.back().first.QuadPart); }
	inline const long long getCurrentKeyDuration() { return (currentKeyUp.QuadPart - currentKeyDown.QuadPart); }
	// counter
	inline BOOL setDown() { return (oneDown) ? QueryPerformanceCounter(&currentKeyDown) : FALSE; }
	inline BOOL setUp() { oneDown = true; return QueryPerformanceCounter(&currentKeyUp); }
	// timestamp message
	void setTSDown(LPARAM& lp);
	void setTSUp(LPARAM& lp);
	inline long getDiffTS() { return currentTimestampUp - currentTimestampDown; }

private:
	bool oneDown = true;
	long currentTimestampDown, currentTimestampUp;
	LARGE_INTEGER currentKeyDown, currentKeyUp;
	std::pair<LARGE_INTEGER, LARGE_INTEGER> currentKey;
	std::vector<std::pair<LARGE_INTEGER, LARGE_INTEGER>> keystrokes;
	inline std::pair<LARGE_INTEGER, LARGE_INTEGER> makePair() { currentKey = std::make_pair(currentKeyDown, currentKeyUp); return currentKey; }
	inline void add_keystroke(std::pair<LARGE_INTEGER, LARGE_INTEGER> keys) { keystrokes.push_back(keys); }
};

