#pragma once
#ifdef COMBRIDGE_EXPORTS
#define COMBRIDGE_API __declspec(dllexport)
#else
#define COMBRIDGE_API	__declspec(dllimport)
#endif

extern "C"
{
	COMBRIDGE_API BOOL fnInitBridge(LPCWSTR pwReadPort, LPCWSTR pwWritePort, DWORD dwBaudRate, BYTE bByteSize);
	COMBRIDGE_API BOOL fnStartBridge();
	COMBRIDGE_API BOOL fnStopBridge();
	COMBRIDGE_API void fnAllowBridge();
	COMBRIDGE_API void fnBlockBridge();
	COMBRIDGE_API void fnSetSerialBufferSize(DWORD dwSize);
	COMBRIDGE_API BOOL fnRegisterCallback(BRIDGE_CALLBACK_FUNC func);
}