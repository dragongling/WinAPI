#pragma comment(linker, "/MERGE:.data=.text")
#pragma comment(linker, "/MERGE:.rdata=.text")
#pragma comment(linker, "/SECTION:.text,EWR")

#include <Windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int windowWidth = 500;
int windowHeight = 500;

class PinGroup {
public:
	int firstCoord;
	int step;
	unsigned int count;
	std::wstring text;
	bool inversedText;
	bool inversedPin;
};

class ComponentSide {
public:
	std::vector<PinGroup> pinGroups;
	std::vector<int> delimiters;
	int currentCoord = 0;
};

class LabelString {
public:
	int x, y;
	Gdiplus::REAL width, height;
	Gdiplus::Font* font;
	const WCHAR *string;
	Gdiplus::Brush* brush;

	LabelString(const WCHAR *string, Gdiplus::Font* font, Gdiplus::Brush* brush, const int x, const int y, Gdiplus::Graphics &graphics) :
		string(string), font(font), brush(brush), x(x), y(y) {
		RectF layoutRect(0, 0, 100, 50);
		RectF boundRect;
		graphics.MeasureString(string, -1, font, layoutRect, &boundRect);
		width = boundRect.Width;
		height = boundRect.Height;
	}

	void changeText(const WCHAR *string, Graphics &graphics) {
		this->string = string;
		RectF layoutRect(0, 0, 100, 50);
		RectF boundRect;
		graphics.MeasureString(string, -1, font, layoutRect, &boundRect);
		width = boundRect.Width;
		height = boundRect.Height;
	}

	void draw(Gdiplus::Graphics &graphics) {
		graphics.DrawString(string, -1, font,
			Gdiplus::PointF(static_cast<Gdiplus::REAL>(x), static_cast<Gdiplus::REAL>(y)), brush);
	}

	~LabelString() {
		brush = nullptr;
		string = nullptr;
		font = nullptr;
	}
};

class Component {
public:
	ComponentSide leftSide;
	ComponentSide rightSide;
	int x, y, width, height, innerWidth;
	std::wstring name;
	float labelY;

	void draw(Gdiplus::Graphics &graphics, Gdiplus::Pen &pen) {
		graphics.DrawRectangle(&pen, x, y, width, height);
		graphics.DrawLine(&pen, x + innerWidth, y, x + innerWidth, y + height);
		graphics.DrawLine(&pen, x + width - innerWidth, y, x + width - innerWidth, y + height);

		Font font(&FontFamily(L"Arial"), 12);
		Font subFont(&FontFamily(L"Arial"), 6);
		SolidBrush blackBrush(Color(255, 0, 0, 0));

		LabelString text{ name.c_str(), &font, &blackBrush, 0, 0, graphics };
		text.x = static_cast<int>(x + width / 2 - text.width / 2);
		text.y = static_cast<int>(y + height * labelY);
		text.draw(graphics);

		for (auto &group : leftSide.pinGroups) {
			text.changeText(group.text.c_str(), graphics);
			text.x = static_cast<int>(x + innerWidth / 2 - text.width / 2);
			text.y = y + leftSide.currentCoord + group.firstCoord;
			if (group.count == 0) {
				text.draw(graphics);
				if (group.inversedText) {
					graphics.DrawLine(&pen, text.x + 2 /*- int(text.width)*/, text.y, text.x + int(text.width) - 2, text.y);
				}
				graphics.DrawLine(&pen, x - 20, int(text.y + text.height / 2), x, int(text.y + text.height / 2));
			}
			for (int i = 0; i < group.count; ++i) {
				text.draw(graphics);
				if (group.inversedText) {
					graphics.DrawLine(&pen, text.x + 2 /*- int(text.width)*/, text.y, text.x + int(text.width) - 2, text.y);
				}
				graphics.DrawLine(&pen, x - 20, int(text.y + text.height / 2), x, int(text.y + text.height / 2));
				graphics.DrawString(std::to_wstring(i).c_str(), -1, &subFont,
					PointF(x + innerWidth / 2 + text.width / 2 - 2, y + i * 20 + text.height - 5), &blackBrush);
				text.y += group.step;
			}
			leftSide.currentCoord += group.firstCoord + group.count * group.step;
		}
		for (auto &group : rightSide.pinGroups) {
			text.changeText(group.text.c_str(), graphics);
			text.x = static_cast<int>(x + width - innerWidth / 2 - text.width / 2);
			text.y = y + rightSide.currentCoord + group.firstCoord;
			if (group.count == 0)
				text.draw(graphics);
			for (int i = 0; i < group.count; ++i) {
				text.draw(graphics);
				graphics.DrawLine(&pen, x + width, int(text.y + text.height / 2), x + width + 20, int(text.y + text.height / 2));
				graphics.DrawString(std::to_wstring(i).c_str(), -1, &subFont,
					PointF(x + width - innerWidth / 2 + text.width / 2 - 2, y + i * 20 + text.height - 5), &blackBrush);
				text.y += group.step;
			}
			rightSide.currentCoord += group.firstCoord + group.count * group.step;
		}
	}
};



float center(float width1, float width2) {
	return width1 / 2.0f - width2 / 2.0f;
}

void drawComponent(Graphics &graphics, Pen &pen, const int x, const int y) {
	const int width = 120;
	const int height = 250;
	const int innerWidth = 30;
	const double labelY = 0.1;
	const std::wstring label = L"RAM";

	Component ram;
	ram.name = L"RAM";
	ram.x = 50;
	ram.y = 20;
	ram.width = 120;
	ram.height = 250;
	ram.innerWidth = 30;
	ram.labelY = 0.1f;

	ram.leftSide.pinGroups.push_back(PinGroup());
	PinGroup &address = ram.leftSide.pinGroups.back();
	address.count = 9;
	address.text = L"A";
	address.firstCoord = 5;
	address.step = 20;
	address.inversedPin = false;
	address.inversedText = false;

	ram.rightSide.pinGroups.push_back(PinGroup());
	PinGroup &data = ram.rightSide.pinGroups.back();
	data.count = 9;
	data.text = L"D";
	data.firstCoord = 5;
	data.step = 20;
	data.inversedPin = false;
	data.inversedText = false;

	ram.leftSide.pinGroups.push_back(PinGroup());
	PinGroup &W = ram.leftSide.pinGroups.back();
	W.count = 0;
	W.text = L"W";
	W.firstCoord = 10;
	W.inversedPin = false;
	W.inversedText = true;

	ram.leftSide.pinGroups.push_back(PinGroup());
	PinGroup &CS = ram.leftSide.pinGroups.back();
	CS.count = 0;
	CS.text = L"CS";
	CS.firstCoord = 30;
	CS.inversedPin = false;
	CS.inversedText = true;

	ram.draw(graphics, pen);
}

class Lamp {
	int x, y, size;
	SolidBrush *enabledColor;
	bool enabled;

	Lamp(int x, int y) : x(x), y(y) {
		//enabledColor.SetColor(Color::Red);
		enabled = false;
	}
};

VOID OnPaint(HDC hdc)
{
	Graphics graphics{ hdc };
	Pen      pen{ Color::Black };
	SolidBrush whiteBrush{ Color::White };

	graphics.FillRectangle(&whiteBrush, 0, 0, 220, 300);
	graphics.DrawRectangle(&pen, 0, 0, 220, 300);
	const int x = 100;
	const int y = 100;
	
	drawComponent(graphics, pen, x, y);
}

HWND cs;

WNDPROC wpOrigEditProc;

/*LRESULT APIENTRY EditBoxProc(
	HWND hwndDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	HWND hwndEdit;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		// Retrieve the handle to the edit control. 
		hwndEdit = GetDlgItem(hwndDlg, ID_EDIT);

		// Subclass the edit control. 
		wpOrigEditProc = (WNDPROC)SetWindowLong(hwndEdit,
			GWL_WNDPROC, (LONG)EditSubclassProc);
		// 
		// Continue the initialization procedure. 
		// 
		return TRUE;

	case WM_DESTROY:
		// Remove the subclass from the edit control. 
		SetWindowLong(hwndEdit, GWL_WNDPROC,
			(LONG)wpOrigEditProc);
		// 
		// Continue the cleanup procedure. 
		// 
		break;
	}
	return FALSE;
	UNREFERENCED_PARAMETER(lParam);
}*/

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE         hPrevInstance,
	LPSTR             lpCmdLine,
	int               nCmdShow)
{
	LPCTSTR lpzClass{ L"0" };
	WNDCLASS wcWindowClass{ 0 };
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	wcWindowClass.hInstance = hInstance;
	wcWindowClass.lpszClassName = lpzClass;
	wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	RegisterClass(&wcWindowClass);

	RECT windowRect{ 0, 0, windowWidth, windowHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, false);

	int x{ (GetSystemMetrics(SM_CXSCREEN) - (windowRect.right - windowRect.left)) / 2 };
	int y{ (GetSystemMetrics(SM_CYSCREEN) - (windowRect.bottom - windowRect.top)) / 2 };

	HWND hWnd{ CreateWindow(lpzClass, L"RAM",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, x, y, windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top, NULL, NULL,
		hInstance, NULL) };

	SetWindowText(CreateWindow(L"STATIC",
		NULL, WS_CHILD | WS_VISIBLE,
		50, 35, 45, 15, hWnd, (HMENU)(2), hInstance, NULL), L"Hello");

	cs = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY, 300, 50,
		53, 20, hWnd, NULL, hInstance, NULL);
	
	SetWindowText(cs, L"C\u0305S\u0305 = 0");
	//EnableWindow(cs, false);

	HFONT consoleFont = CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_EMBEDDED, CLEARTYPE_QUALITY, FF_MODERN, TEXT("Consolas"));
	SendMessage(cs, WM_SETFONT, (int)consoleFont, NULL);

	MSG msg{ 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}



LRESULT CALLBACK WndProc(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		if(LOWORD(lParam) > 300 && LOWORD(lParam) < 352 
			&& HIWORD(lParam) > 50 && HIWORD(lParam) < 70)
			SetWindowText(cs, L"C\u0305S\u0305 = 1");
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}