// poc_clavier_mfc.cpp�: d�finit le point d'entr�e pour l'application console.
//

//#include "stdafx.h"
#include "poc_clavier.h"
#include "Frappe.h"
#include "utilities.h"

#include <windows.h>
#include <iostream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Seul et unique objet application

//CWinApp theApp;

HINSTANCE modulehandle;
HHOOK kbdhook;
bool running = true;
Keystroke keystrokes;


__declspec(dllexport) LRESULT CALLBACK handlerKeys(int code, WPARAM wp, LPARAM lp)
{
	if (code == HC_ACTION &&  wp == WM_KEYDOWN) {
		keystrokes.setDown();
	}
	else if (code == HC_ACTION &&  wp == WM_KEYUP)
	{
		keystrokes.setUp();
#ifdef _DEBUG
		processKey(lp);
#endif
		std::cout << " [" << keystrokes.getCurrentKeyDuration() << "] \n";
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
        // Initialise MFC et affiche un message d'erreur en cas d'�chec
        if (false)//!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: modifiez le code d'erreur selon les besoins
            wprintf(L"Erreur irr�cup�rable�: l'initialisation MFC a �chou�\n");
            nRetCode = 1;
        }
        else
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
    }
    else
    {
        // TODO: modifiez le code d'erreur selon les besoins
        wprintf(L"Erreur irr�cup�rable : �chec de GetModuleHandle\n");
        nRetCode = 1;
    }

    return nRetCode;
}
