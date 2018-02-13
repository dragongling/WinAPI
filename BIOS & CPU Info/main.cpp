#ifndef _DEBUG
#pragma comment(linker, "/MERGE:.data=.text")
#pragma comment(linker, "/MERGE:.rdata=.text")
#pragma comment(linker, "/SECTION:.text,EWR")
#endif //_DEBUG

#include <Windows.h>
#include <CommCtrl.h>
#include <stdio.h>
#include <ShellAPI.h>
#include "EditBoxParameter.h"
//#include "RegistryEditBoxParameter.h"

#include <vector>
#include <string>

std::vector<RegistryParameter> regParams{
	{ "HARDWARE\\DESCRIPTION\\System\\BIOS", "BIOSReleaseDate" },
	{ "HARDWARE\\DESCRIPTION\\System\\BIOS", "SystemVersion" },
	{ "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "Identifier" },
	{ "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString" },
	{ "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "VendorIdentifier" }
};

std::vector<EditBoxParameter> params{
	{ "Processor ID:", &regParams[2], 120, 400, EditBoxParameter::RegParameter },
	{ "Processor name:", &regParams[3], 120, 400, EditBoxParameter::RegParameter },
	{ "Processor vendor:", &regParams[4], 120, 120, EditBoxParameter::RegParameter },
	{ "Video card name:", 120, 160, EditBoxParameter::VideoCardName },
	{ "BIOS date:", &regParams[0], 120, 120, EditBoxParameter::RegParameter },
	{ "BIOS version:", &regParams[1], 120, 160, EditBoxParameter::RegParameter },
	{ "Computer name:", 120, 160, EditBoxParameter::ComputerName },
	{ "User name:", 80, 160, EditBoxParameter::UserName },
	{ "Used memory percentage:", 240, 40, EditBoxParameter::MemoryLoad },
	{ "Total memory:", 120, 120, EditBoxParameter::TotalPhys },
	{ "Total pagefile size:", 144, 120, EditBoxParameter::TotalPageFile },
	{ "Pagefile available:", 144, 120, EditBoxParameter::AvailPageFile },
	{ "Total virtual:", 144, 120, EditBoxParameter::TotalVirtual },
	{ "Available virtual:", 144, 120, EditBoxParameter::AvailVirtual }
};

LRESULT CALLBACK HelloWorldWndProc(HWND, UINT, UINT, LONG);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	WNDCLASS WndClass;
	MSG Msg;
	TCHAR *szClassName = "Lab2BIOS";
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = HelloWorldWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	WndClass.hCursor = LoadCursor(NULL, IDC_ICON);
	WndClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = szClassName;
	RegisterClass(&WndClass);
	hWnd = CreateWindow(szClassName, "BIOS & CPU information", WS_TILEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, 600, 300, NULL, NULL, hInstance, NULL);

	int y = 20;
	int x = 20;
	int hMenu = 501;

	MEMORYSTATUSEX memoryStatus = {sizeof(memoryStatus)};
	GlobalMemoryStatusEx(&memoryStatus);

	for (auto &param : params) {
		if (x + param.labelWidth + param.editBoxWidth > 580) {
			y += 30;
			x = 20;
		}
		param.createFormElements(hWnd, hInstance, x, y, hMenu, param.getString(memoryStatus));
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

LRESULT CALLBACK HelloWorldWndProc(HWND hWnd, UINT Message, UINT wParam, LONG lParam){
	if (Message == WM_DESTROY)
		PostQuitMessage(0);
	return DefWindowProc(hWnd, Message, wParam, lParam);
}