#include "Frappe.h"

Keystroke::Keystroke()
{
}

Keystroke::~Keystroke()
{
}

void Keystroke::setTSDown(LPARAM& lp)
{
	if (oneDown)
	{
		KBDLLHOOKSTRUCT st_hook = *((KBDLLHOOKSTRUCT*)lp);
		currentTimestampDown = st_hook.time;
		oneDown = false;
	}
}

void Keystroke::setTSUp(LPARAM& lp)
{
	KBDLLHOOKSTRUCT st_hook = *((KBDLLHOOKSTRUCT*)lp);
	currentTimestampUp = st_hook.time;
	oneDown = true;
}