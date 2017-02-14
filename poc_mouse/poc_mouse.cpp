
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
		mouseClick.set_event(wp, lp);
		mouseClick.write_data();
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

		std::cout << "POC Mouse\n" << "Cette application intercepte les messages Windows \n";
		std::cout << "emis lors de la munipulation de la souris. \nA partir de ces messages, l'application recuppere le code evenement";
		std::cout << " et son timestamp.\n Elle stocke les donnees dans un fichier. Ces informations seront pseudo-anonymees." << std::endl;
		MSG msg;
		running = true;
		while (running)
		{
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
