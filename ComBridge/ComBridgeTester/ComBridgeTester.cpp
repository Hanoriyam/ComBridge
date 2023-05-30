#include "pch.h"
#include "ComBridgeLoader.h"

void DataCallback(BYTE* arrData, DWORD dwSize)
{
	printf("%s\n", arrData);
}

int wmain(int argc, const wchar_t* argv[])
{
	setlocale(LC_ALL, "korean");
	if (__argc < 5)
	{
		wprintf(L"Usage: %s [ReadPort] [WritePort] [BaudRate] [DataBits]\n", __wargv[0]);
		return 0;
	}

	ComBridgeLoader ccLoader;
	if (!ccLoader.InitLoader())
	{
		wprintf(L"InitLoader() error.\n");
		return 0;
	}

	if (!ccLoader.InitBridge(__wargv[1], __wargv[2], _wtoi(__wargv[3]), _wtoi(__wargv[4])))
	{
		wprintf(L"Init() error.\n");
		return 0;
	}

	if (!ccLoader.RegisterCallback(&DataCallback))
	{
		wprintf(L"RegisterCallback() error.\n");
		return 0;
	}

	if (!ccLoader.StartBridge())
	{
		wprintf(L"StartBridge() error.\n");
		return 0;
	}

	while (true);

	return 0;
}