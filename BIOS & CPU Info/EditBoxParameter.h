#pragma once
#include <Windows.h>
#include <VersionHelpers.h>
#include <string>

struct RegistryParameter {
	std::string regPath;
	std::string regParamName;

	RegistryParameter(std::string regPath, std::string regParamName) : regPath(regPath), regParamName(regParamName) {}
};

class EditBoxParameter {
public:
	const enum Type { RegParameter, ComputerName, UserName, VideoCardName, TotalPhys, MemoryLoad, TotalPageFile, AvailPageFile, TotalVirtual, AvailVirtual };
	Type paramType;
	HWND editBoxHWnd;
	int labelWidth;
	int editBoxWidth;
	int afterWidth;
	int labelHMenu;
	int editBoxHMenu;
	std::string labelName;
	RegistryParameter *regParamPtr;

	EditBoxParameter(std::string labelName, int labelWidth, int editBoxWidth, Type paramType)
		: labelName(labelName), regParamPtr(nullptr), labelWidth(labelWidth), editBoxWidth(editBoxWidth), paramType(paramType) {}

	EditBoxParameter(std::string labelName, RegistryParameter* regParamPtr, int labelWidth, int editBoxWidth, Type paramType)
		: labelName(labelName), regParamPtr(regParamPtr), labelWidth(labelWidth), editBoxWidth(editBoxWidth), paramType(paramType) {}

	void createFormElements(HWND &parentHWnd, HINSTANCE &hInstance, int &x, int &y, int &hMenu, std::string editBoxText);
	std::string getString(MEMORYSTATUSEX &memoryStatus);

private:
	std::string toKilobytesString(DWORDLONG dwMemoryParameter);
	std::string getRegistryValue(std::string path, std::string keyName);
};