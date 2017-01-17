#pragma once

#include <Windows.h>
#include <fstream>
typedef struct MEvent MEvent;

struct MEvent
{
	long long keyUp, keyDown;
	long typeClick;
	MEvent::MEvent(DWORD keyDown, DWORD vkcode)
		:keyDown(keyDown), typeClick(vkcode){}
	MEvent::MEvent():typeClick(-1){}
};

class MouseEvent
{
	MEvent currentEvent;
	std::ofstream file;

public:
	void setDownEvent(LPARAM& lp);
	void setUpEvent(LPARAM& lp);
	void writeData();
	inline long long getCurrentPressedTime() { return currentEvent.keyUp - currentEvent.keyDown; }
	MouseEvent();
	~MouseEvent();
};