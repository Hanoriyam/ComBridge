#include "pch.h"
#include "ComBridge.h"
#include "ComBridgeAPI.h"

ComBridge g_cc;

BOOL fnInitBridge(LPCWSTR pwReadPort, LPCWSTR pwWritePort, DWORD dwBaudRate, BYTE bByteSize)
{
	return g_cc.InitBridge(pwReadPort, pwWritePort, dwBaudRate, bByteSize);
}

BOOL fnStartBridge()
{
	return g_cc.StartBridge();
}

BOOL fnStopBridge()
{
	return g_cc.StopBridge();
}

void fnAllowBridge()
{
	g_cc.AllowBridge();
}

void fnBlockBridge()
{
	g_cc.BlockBridge();
}

void fnSetSerialBufferSize(DWORD dwSize)
{
	g_cc.SetSerialBufferSize(dwSize);
}

BOOL fnRegisterCallback(BRIDGE_CALLBACK_FUNC func)
{
	return g_cc.RegisterCallback(func);
}

void fnShowLog()
{
	LogMgr::SetShow();
}

void fnHideLog()
{
	LogMgr::SetHide();
}