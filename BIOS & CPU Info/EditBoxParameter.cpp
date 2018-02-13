#include "EditBoxParameter.h"

void EditBoxParameter::createFormElements(HWND & parentHWnd, HINSTANCE & hInstance, int & x, int & y, int & hMenu, std::string editBoxText)
{
	const int elementHeight = 25;
	CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", labelName.c_str(),
		WS_CHILD | WS_VISIBLE | SS_RIGHT,
		x, y + 4, labelWidth, elementHeight,
		parentHWnd, (HMENU)hMenu,
		hInstance, NULL);
	x += labelWidth + 3;
	labelHMenu = hMenu;
	hMenu++;
	editBoxHWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", editBoxText.c_str(),
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_READONLY,
		x, y, editBoxWidth, elementHeight,
		parentHWnd, (HMENU)hMenu,
		hInstance, NULL);
	x += editBoxWidth;
	labelHMenu = hMenu;
	hMenu++;
}

std::string EditBoxParameter::toKilobytesString(unsigned long long dwMemoryParameter) {
	const unsigned long long kilobyte = 1024ull;
	return std::to_string(dwMemoryParameter) + " MB";
}

std::string EditBoxParameter::getRegistryValue(std::string path, std::string keyName) {
	TCHAR stringBuf[200];
	DWORD bufLen;
	HKEY hKey;
	bufLen = 200 * sizeof(TCHAR);
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, path.c_str(), NULL, KEY_READ, &hKey);
	RegQueryValueEx(hKey, keyName.c_str(), NULL, NULL, (LPBYTE)stringBuf, &bufLen);
	RegCloseKey(hKey);
	return (std::string)stringBuf;
}

std::string EditBoxParameter::getString(MEMORYSTATUSEX & memoryStatus) {
	TCHAR stringBuf[50];
	DWORD bufLen = 50 * sizeof(TCHAR);
	std::string videoPath;
	switch (paramType) {
	case EditBoxParameter::RegParameter:
		return getRegistryValue(regParamPtr->regPath, regParamPtr->regParamName);
		break;
	case EditBoxParameter::ComputerName:
		GetComputerName(stringBuf, &bufLen);
		return (std::string)stringBuf;
		break;
	case EditBoxParameter::UserName:
		GetUserName(stringBuf, &bufLen);
		return (std::string)stringBuf;
		break;
	case EditBoxParameter::VideoCardName:
		if (IsWindows8OrGreater()) {
			videoPath = getRegistryValue("HARDWARE\\DEVICEMAP\\VIDEO", "\\Device\\Video0");
			videoPath.erase(0, 18);
			videoPath += "\\Settings";
			return getRegistryValue(videoPath, "Device Description");
		}
		else {
			return getRegistryValue("HARDWARE\\DESCRIPTION\\System", "VideoBiosVersion");
		}
		break;
	case EditBoxParameter::TotalPhys:
		return std::to_string(memoryStatus.ullTotalPhys);
		break;
	case EditBoxParameter::MemoryLoad:
		return std::to_string(memoryStatus.dwMemoryLoad) + "%";
		break;
	case EditBoxParameter::TotalPageFile:
		return std::to_string(memoryStatus.ullTotalPageFile);
		break;
	case EditBoxParameter::AvailPageFile:
		return std::to_string(memoryStatus.ullAvailPageFile);
		break;
	case EditBoxParameter::TotalVirtual:
		return std::to_string(memoryStatus.ullTotalVirtual);
		break;
	case EditBoxParameter::AvailVirtual:
		return std::to_string(memoryStatus.ullAvailVirtual);
		break;
	default:
		break;
	}
}
