
#include "Frappe.h"
#include "utilities.h"

#include <windows.h>
#include <iostream>


#ifdef _DEBUG
#define new DEBUG_NEW
#define DEBUG
#endif

HINSTANCE modulehandle;
HHOOK kbdhook;
bool running = true;
Keystrokes keystrokes;


__declspec(dllexport) LRESULT CALLBACK handlerKeys(int code, WPARAM wp, LPARAM lp)
{
	if (code == HC_ACTION &&  wp == WM_KEYDOWN) {
		keystrokes.setTSDown(lp);
	}
	else if (code == HC_ACTION &&  wp == WM_KEYUP)
	{
		keystrokes.setTSUp(lp);
#ifdef DEBUG
		processKey(lp);
		std::cout << " ["<< keystrokes.getLastTimePressed()<<"] \n";
#endif
	}
	
	return CallNextHookEx(kbdhook, code, wp, lp);
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
	kbdhook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)handlerKeys, modulehandle, NULL);
	
    if (hModule != nullptr)
    {

			std::cout << "POC Keyboard\n" << "Cette application intercepte les messages Windows \n";
			std::cout << "emis lors de la pression sur une touche. \nA partir de ces messages, l'application recuppere le code touche";
			std::cout << " et son timestamp.\n Elle stocke les donnees dans un fichier. Ces informations seront pseudo-anonymees."<<std::endl;
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
