#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>

// Error handler
int handlerXorg(Display* display, XErrorEvent* error)
{
	return 0;
}

int main()
{
	Display* x11_connection = XOpenDisplay(":0");
	
	if (x11_connection == NULL)
	{
		printf("Failed to create context.\n");
		return 1;
	}

	Window current_focus;
	char buf[17];
	KeySym ks;
	XComposeStatus comp;
	int revert;

	// Files
	FILE *f = fopen("keys.csv", "a");
	FILE* fclick = fopen("clicks.csv", "a");

	XGetInputFocus(x11_connection, &current_focus, &revert);
	XSelectInput(x11_connection, current_focus, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);

	// Open file
	if (f == NULL)
	{
		printf("Error opening file!\n");
		return 1;
	}
	if (fclick == NULL)
		return 1;

	// Set handler
	XSetErrorHandler(handlerXorg);
	
	// infinite loop
	while (1)
	{
		XEvent ev;
		XNextEvent(x11_connection, &ev);
		XGetInputFocus(x11_connection, &current_focus, &revert);
		XSelectInput(x11_connection, current_focus, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);
		// Event handler
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
			break;
		case ButtonPressMask:
			fprintf(fclick, "%d;%d\n", ev.xbutton.type, (int)ev.xbutton.time);
			break;
		}
	}
	fclose(f);
}