#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG

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
	XEvent ev;
	Window root = DefaultRootWindow(x11_connection);

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
	
	//XAllowEvents(x11_connection, AsyncBoth, CurrentTime);
	//XGrabPointer(x11_connection,current_focus, 1, PointerMotionMask | ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);

	// infinite loop
	while (1)
	{
		if (current_focus != root)
			XSelectInput(x11_connection, current_focus, 0);
		XGetInputFocus(x11_connection, &current_focus, &revert);
		if (current_focus == PointerRoot)
			current_focus = root;
		XSelectInput(x11_connection, current_focus, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | FocusChangeMask);
		XNextEvent(x11_connection, &ev);

		// Event handler
		switch (ev.type)
		{
		case FocusOut:
			if (current_focus != root)
				XSelectInput(x11_connection, current_focus, 0);
			XGetInputFocus(x11_connection, &current_focus, &revert);
			if (current_focus == PointerRoot)
				current_focus = root;
			XSelectInput(x11_connection, current_focus, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | FocusChangeMask);
			break;

		case KeyPress:
			XLookupString(&ev.xkey, buf, 16, &ks, &comp);
			fprintf(f, "%d;%d;%d;%d\n", ev.xkey.type, (int)ks, (int)ev.xkey.keycode, (int)ev.xkey.time);
#ifdef DEBUG
			printf("%d;%d;%d;%d\n", ev.xkey.type, (int)ks, (int)ev.xkey.keycode, (int)ev.xkey.time);
#endif // DEBUG
			break;
		case KeyRelease:
			XLookupString(&ev.xkey, buf, 16, &ks, &comp);
			fprintf(f, "%d;%d;%d;%d\n", ev.xkey.type, (int)ks, (int)ev.xkey.keycode, (int)ev.xkey.time);
#ifdef DEBUG
			printf("%d;%d;%d;%d\n", ev.xkey.type, (int)ks, (int)ev.xkey.keycode, (int)ev.xkey.time);
#endif // DEBUG
			break;
		case ButtonRelease:
			fprintf(fclick, "%d;%d\n", ev.xbutton.type, (int)ev.xbutton.time);
#ifdef DEBUG
			printf("%d;%d\n", ev.xbutton.type, (int)ev.xbutton.time);
#endif // DEBUG
			break;
		case ButtonPress:
			fprintf(fclick, "%d;%d\n", ev.xbutton.type, (int)ev.xbutton.time);
#ifdef DEBUG
			printf("%d;%d\n", ev.xbutton.type, (int)ev.xbutton.time);
#endif // DEBUG
			break;
		}
	}
	fclose(f);
}