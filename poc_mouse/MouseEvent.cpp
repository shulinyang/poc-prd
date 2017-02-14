#include "MouseEvent.h"
#include <iostream>

MouseEvent::MouseEvent():file("clicks.data", std::ios::app){}

MouseEvent::~MouseEvent()
{
	file.close();
}

void MouseEvent::set_event(WPARAM & wp, LPARAM & lp)
{
	current_event.type_event = wp;
	MSLLHOOKSTRUCT* st_hook = (MSLLHOOKSTRUCT*)lp;
	current_event.type_click = st_hook->mouseData;
	current_event.key = st_hook->time;
	current_event.flag = st_hook->flags;
	current_event.pt = st_hook->pt;
}

void MouseEvent::write_data()
{

#ifdef DEBUG
	std::cout << current_event.type_event << ";" << current_event.type_click << ";" << current_event.key
		<< ";" << current_event.pt.x << ";" << current_event.pt.y << ";" << current_event.flag << std::endl;
#else
	if (file.is_open())
	{
		file << current_event.type_event << ";" << current_event.type_click << ";" << current_event.key
			<< ";" << current_event.pt.x << ";" << current_event.pt.y << ";" << current_event.flag << std::endl;
	}
	else
		std::cerr << "File Failed to open" << std::endl;

#endif // DEBUG

}