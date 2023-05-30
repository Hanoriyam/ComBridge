#pragma once

typedef void(*BRIDGE_CALLBACK_FUNC)(BYTE* pData, DWORD dwSize);
typedef BOOL(*fnInitBridge)(LPCWSTR pwReadPort, LPCWSTR pwWritePort, DWORD dwBaudRate, BYTE bByteSize);
typedef BOOL(*fnStartBridge)();
typedef BOOL(*fnStopBridge)();
typedef void (*fnAllowBridge)();
typedef void (*fnBlockBridge)();
typedef void (*fnSetSerialBufferSize)(DWORD dwSize);
typedef BOOL(*fnRegisterCallback)(BRIDGE_CALLBACK_FUNC func);

class ComBridgeLoader
{
public:
	ComBridgeLoader();
	~ComBridgeLoader();

public:
	BOOL InitLoader();

public:
	BOOL InitBridge(LPCWSTR pwReadPort, LPCWSTR pwWritePort, DWORD dwBaudRate, BYTE bByteSize);
	BOOL StartBridge();
	BOOL StopBridge();
	void AllowBridge();
	void BlockBridge();
	void SetSerialBufferSize(DWORD dwSize);
	BOOL RegisterCallback(BRIDGE_CALLBACK_FUNC func);

private:
	std::wstring m_strPath;
	HMODULE m_hDll;

	fnInitBridge m_InitBridge;
	fnStartBridge m_StartBridge;
	fnStopBridge m_StopBridge;
	fnAllowBridge m_AllowBridge;
	fnBlockBridge m_BlockBridge;
	fnSetSerialBufferSize m_SetSerialBufferSize;
	fnRegisterCallback m_RegisterCallback;
};