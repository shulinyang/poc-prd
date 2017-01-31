#include "MouseEvent.h"
#include <iostream>

MouseEvent::MouseEvent():file("clicks.data", std::ios::app){}

MouseEvent::~MouseEvent()
{
	file.close();
}

void MouseEvent::setEvent(WPARAM & wp, LPARAM & lp)
{
	currentEvent.typeEvent = wp;
	MSLLHOOKSTRUCT* st_hook = (MSLLHOOKSTRUCT*)lp;
	currentEvent.typeClick = st_hook->mouseData;
	currentEvent.key = st_hook->time;
	currentEvent.flag = st_hook->flags;
	currentEvent.pt = st_hook->pt;
}

void MouseEvent::writeData()
{

#ifdef DEBUG
	std::cout << currentEvent.typeEvent << ";" << currentEvent.typeClick << ";" << currentEvent.key
		<< ";" << currentEvent.pt.x << ";" << currentEvent.pt.y << ";" << currentEvent.flag << std::endl;
#else
	if (file.is_open())
	{
		file << currentEvent.typeEvent << ";" << currentEvent.typeClick << ";" << currentEvent.key
			<< ";" << currentEvent.pt.x << ";" << currentEvent.pt.y << ";" << currentEvent.flag << std::endl;
	}
	else
		std::cerr << "File Failed to open" << std::endl;

#endif // DEBUG

}