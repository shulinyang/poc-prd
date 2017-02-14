#pragma once

#include <Windows.h>
#include <fstream>
typedef struct MEvent MEvent;

/*
	struct MEvent
	represent mouse event
*/

struct MEvent
{
	long long key;
	long type_click;
	long type_event;
	long long flag;
	POINT pt;
};

/*
	class MouseEvent
	manage MEvent
*/


class MouseEvent
{
	MEvent current_event;	// current event
	std::ofstream file;	// output file

public:
	MouseEvent();
	~MouseEvent();

	// Event handler
	void set_event(WPARAM & wp, LPARAM& lp);
	// output function
	void write_data();
};