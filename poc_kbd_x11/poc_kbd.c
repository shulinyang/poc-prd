#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>


int handlerXorg(Display* display, XErrorEvent* error)
{
	return 0;
}

int main()
{
	Display* d = XOpenDisplay(":0");
	if (d == NULL)
	{
		printf("Failed to create context.\n");
		return 1;
	}
	Window curFocus;
	char buf[17];
	KeySym ks;
	XComposeStatus comp;
	int revert;

	XGetInputFocus(d, &curFocus, &revert);
	XSelectInput(d, curFocus, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);


	FILE *f = fopen("keys.csv", "a");
	FILE* fclick = fopen("clicks.csv", "a");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		return 1;
	}
	if (fclick == NULL)
		return 1;

	XSetErrorHandler(handlerXorg);
	while (1)
	{
		XEvent ev;
		XNextEvent(d, &ev);
		XGetInputFocus(d, &curFocus, &revert);
		XSelectInput(d, curFocus, KeyPressMask | KeyReleaseMask);
		switch (ev.type)
		{
		case KeyPress:
			XLookupString(&ev.xkey, buf, 16, &ks, &comp);
			fprintf(f, "%d;%d;%d;%d\n", ev.xkey.type, (int)ks, (int)ev.xkey.keycode, (int)ev.xkey.time);
			break;
		case KeyRelease:
			XLookupString(&ev.xkey, buf, 16, &ks, &comp);
			fprintf(f, "%d;%d;%d;%d\n", ev.xkey.type, (int)ks, (int)ev.xkey.keycode, (int)ev.xkey.time);
			break;
		case ButtonReleaseMask:
			fprintf(fclick, "%d;%d\n", ev.xbutton.type, (int)ev.xbutton.time);
		case ButtonPressMask:
			fprintf(fclick, "%d;%d\n", ev.xbutton.type, (int)ev.xbutton.time);
		}
	}
	fclose(f);
}