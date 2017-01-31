
#include <windows.h>
#include <iostream>
#include "MouseEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define DEBUG
#endif

HINSTANCE modulehandle;
HHOOK mouseHook;
bool running = true;
MouseEvent mouseClick;


__declspec(dllexport) LRESULT CALLBACK handlerMouse(int code, WPARAM wp, LPARAM lp)
{
	if (code == HC_ACTION && wp != WM_MOUSEMOVE) {
		mouseClick.setEvent(wp, lp);
		mouseClick.writeData();
	}
	
	return CallNextHookEx(mouseHook, code, wp, lp);
}



LRESULT CALLBACK windowprocedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_CLOSE: case WM_DESTROY:
		running = false;
		break;
	default:
		/* Call default message handler */
		return DefWindowProc(hwnd, msg, wp, lp);
	}

	return 0;
}

int main()
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(nullptr);
	mouseHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)handlerMouse, modulehandle, NULL);

	if (hModule != nullptr)
	{

		std::cout << "Ok, let's work !" << std::endl;
		MSG msg;
		running = true;
		while (running) {
			/*
			* Get messages, dispatch to window procedure
			*/
			if (!GetMessage(&msg, NULL, 0, 0))
				running = false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		system("Pause");
		return nRetCode;
	}
	else
	{
		// TODO: modifiez le code d'erreur selon les besoins
		wprintf(L"Erreur irrécupérable : échec de GetModuleHandle\n");
		nRetCode = 1;
	}

	return nRetCode;
}
