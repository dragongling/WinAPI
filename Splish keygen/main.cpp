#pragma comment(linker, "/MERGE:.data=.text")
#pragma comment(linker, "/MERGE:.rdata=.text")
//#pragma comment(linker,"/merge:.reloc=.text")
#pragma comment(linker, "/SECTION:.text,EWR")
/*#pragma comment(linker,"/merge:.rdata=.elf")
#pragma comment(linker,"/merge:.text=.elf")
#pragma comment(linker,"/merge:.reloc=.elf")
#pragma comment(linker,"/merge:.data=.elf")
#pragma comment(linker,"/SECTION:.elf,EWRX")*/

#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

const BYTE nameFieldID = 0, serialFieldID = 1;
HWND hwnds[2];
LPCSTR strings[] = {"Name:", "Serial:"};

int APIENTRY WinMain(HINSTANCE hInstance,
             HINSTANCE         hPrevInstance,
             LPSTR             lpCmdLine,
             int               nCmdShow)
{

  LPCTSTR lpzClass = "0";
  WNDCLASS wcWindowClass = {0};
  wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
  wcWindowClass.style = CS_HREDRAW|CS_VREDRAW;
  wcWindowClass.hInstance = hInstance;
  wcWindowClass.lpszClassName = lpzClass;
  RegisterClassA(&wcWindowClass);

  int x = (GetSystemMetrics(SM_CXSCREEN) - 140) / 2;
  int y = (GetSystemMetrics(SM_CYSCREEN) - 110) / 2;
  /*for(int n = 0; n < 2; ++n){
	  wcoords[n] = (GetSystemMetrics(SM_CYSCREEN) - wsize[n]) >> 1;
  }
  HWND hWnd = CreateWindowA(lpzClass, "SPLISH KEYGEN", 
    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, wcoords[0], wcoords[1], wsize[0], wsize[1], NULL, NULL, 
    hInstance, NULL);*/

  HWND hWnd = CreateWindowA(lpzClass, "SPLISH KEYGEN", 
    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, x, y, 150, 110, NULL, NULL, 
    hInstance, NULL);

  for(int n = 0; n < 2; ++n){
	  SetWindowText(CreateWindow("STATIC",
        NULL, WS_CHILD | WS_VISIBLE,
		50, 35 * n, 45, 15,  hWnd, (HMENU)(n+2), hInstance,NULL), strings[n]);
	  hwnds[n] = CreateWindowExA(WS_EX_CLIENTEDGE, "edit", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | (n? ES_READONLY: n),
        10, 35 * n + 15, 120, 20, hWnd, (HMENU)n, hInstance, NULL);
  }

  MSG msg = {0};
  while (GetMessageA(&msg, NULL, 0, 0 ))
  {
    TranslateMessage(&msg);    
    DispatchMessageA(&msg);
  }

  return msg.wParam;
}
/*
inline LPCSTR genSplishKey(const LPSTR name)
{
	for(int n = 0; n < lstrlenA(name); ++n){
	  name[n] = ((name[n] % 10) ^ n) + 52;
	  while(name[n] > '9')
		  name[n] -= 10;
	}
	return (LPCSTR)name;
}*/
/*
////////////////// CrueHead's Crackme

inline bool checkForLetters( const LPSTR name ){
    for(int i = 0; i < lstrlenA(name); ++i){
        if(name[i] > 'Z')
            name[i] -= 0x20;
        if(name[i] < 'a' || name[i] > 'z')
            return false;
    }
    return true;
}

inline void toUpper(LPSTR string){
    for(int n = 0; n < lstrlenA(string); ++n){
        if(string[n] > 'Z')
			string[n] -= 0x20;
    }
}

inline LPCSTR genCrueHeadKey(LPSTR name)
{
    if(!checkForLetters(name))
        return "Letters only!";
    toUpper(name);
    int cname = 0;
    for(int n = 0; n < lstrlenA(name); ++n){
        cname += name[n];
    }
	//wvsprintf();
    cname ^= 0x444C;
	LPSTR serial;
	return (LPCSTR)serial;
}

//////////////// CrueHead's Crackme end
*/

LRESULT CALLBACK WndProc(
  HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if(message == WM_DESTROY)
	  ExitProcess(0);
  if(message == WM_COMMAND){
	  if( LOWORD(wParam) == nameFieldID && HIWORD(wParam) == EN_CHANGE ){
		  LPSTR name = "";
		  GetWindowText(hwnds[0], name, 1000);
		  for(int n = 0; n < lstrlenA(name); ++n){
			name[n] = ((name[n] % 10) ^ n) + 52;
			while(name[n] > '9')
			  name[n] -= 10;
		  }
		  SetWindowText(hwnds[1], name);
	  } else if(LOWORD(wParam) == serialFieldID && HIWORD(wParam) == EN_SETFOCUS){
		  PostMessage(hwnds[1], EM_SETSEL, 0, 1000);
	  }
      return 0;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}