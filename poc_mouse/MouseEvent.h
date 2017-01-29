#pragma once

#include <Windows.h>
#include <fstream>
typedef struct MEvent MEvent;

struct MEvent
{
	long long key;
	long typeClick;
	long typeEvent;
	long long flag;
	POINT pt;
};

class MouseEvent
{
	MEvent currentEvent;
	std::ofstream file;

public:
	void setEvent(WPARAM & wp, LPARAM& lp);
	void writeData();
	MouseEvent();
	~MouseEvent();
};