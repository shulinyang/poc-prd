#include "MouseEvent.h"
#include <iostream>

MouseEvent::MouseEvent()
	:file("clicks.data", std::ios::ate)
{
}

MouseEvent::~MouseEvent()
{
	file.close();
}

void MouseEvent::setDownEvent(LPARAM & lp)
{
	MSLLHOOKSTRUCT* st_hook = (MSLLHOOKSTRUCT*)lp;
	currentEvent.typeClick = st_hook->mouseData;
	currentEvent.keyDown = st_hook->time;
}

void MouseEvent::setUpEvent(LPARAM & lp)
{
	MSLLHOOKSTRUCT* st_hook = (MSLLHOOKSTRUCT*)lp;
	if (currentEvent.typeClick == st_hook->mouseData)
	{
		currentEvent.keyUp = st_hook->time;
#ifdef DEBUG
		std::cout << "event: " << currentEvent.typeClick << " ";
		std::cout << "duration: " << currentEvent.keyUp - currentEvent.keyDown << "\n";
#endif // DEBUG
	}

}
void MouseEvent::writeData()
{
	if (file.is_open())
	{
		file << currentEvent.typeClick << ";" << currentEvent.keyDown << ";" << currentEvent.keyUp << std::endl;
	}
	else
		std::cerr << "File Failed to open" << std::endl;
}