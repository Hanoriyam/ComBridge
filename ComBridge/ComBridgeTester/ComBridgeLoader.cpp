#include "pch.h"
#include "ComBridgeLoader.h"

ComBridgeLoader::ComBridgeLoader()
{
	WCHAR pwPath[MAX_PATH]{ 0, };
	ZeroMemory(pwPath, sizeof(pwPath));
	GetCurrentDirectoryW(MAX_PATH, pwPath);
	m_strPath = pwPath;

	m_hDll = nullptr;

	m_InitBridge = nullptr;
	m_StartBridge = nullptr;
	m_StopBridge = nullptr;
	m_AllowBridge = nullptr;
	m_BlockBridge = nullptr;
	m_SetSerialBufferSize = nullptr;
	m_RegisterCallback = nullptr;
}

ComBridgeLoader::~ComBridgeLoader()
{
	if (m_hDll) FreeLibrary(m_hDll);
	m_hDll = nullptr;
}

BOOL ComBridgeLoader::InitLoader()
{
	if (nullptr != m_hDll)
	{
		return FALSE;
	}

#ifdef _WIN64
	std::wstring strDllpath{ m_strPath + L"\\ComBridge_x64.dll" };
#else
	std::wstring strDllpath{ m_strPath + L"\\ComBridge_x86.dll" };
#endif

	m_hDll = LoadLibraryW(strDllpath.c_str());
	if (nullptr == m_hDll)
	{
		return FALSE;
	}

	m_InitBridge = reinterpret_cast<fnInitBridge>(GetProcAddress(m_hDll, "fnInitBridge"));
	m_StartBridge = reinterpret_cast<fnStartBridge>(GetProcAddress(m_hDll, "fnStartBridge"));
	m_StopBridge = reinterpret_cast<fnStopBridge>(GetProcAddress(m_hDll, "fnStopBridge"));
	m_AllowBridge = reinterpret_cast<fnAllowBridge>(GetProcAddress(m_hDll, "fnAllowBridge"));
	m_BlockBridge = reinterpret_cast<fnBlockBridge>(GetProcAddress(m_hDll, "fnBlockBridge"));
	m_SetSerialBufferSize = reinterpret_cast<fnSetSerialBufferSize>(GetProcAddress(m_hDll, "fnSetSerialBufferSize"));
	m_RegisterCallback = reinterpret_cast<fnRegisterCallback>(GetProcAddress(m_hDll, "fnRegisterCallback"));

	if (nullptr == m_InitBridge ||
		nullptr == m_StartBridge ||
		nullptr == m_StopBridge ||
		nullptr == m_AllowBridge ||
		nullptr == m_BlockBridge ||
		nullptr == m_SetSerialBufferSize ||
		nullptr == m_RegisterCallback)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL ComBridgeLoader::InitBridge(LPCWSTR pwReadPort, LPCWSTR pwWritePort, DWORD dwBaudRate, BYTE bByteSize)
{
	if (nullptr == m_hDll)
	{
		return FALSE;
	}

	return m_InitBridge(pwReadPort, pwWritePort, dwBaudRate, bByteSize);
}

BOOL ComBridgeLoader::StartBridge()
{
	if (nullptr == m_hDll)
	{
		return FALSE;
	}

	return m_StartBridge();
}

BOOL ComBridgeLoader::StopBridge()
{
	if (nullptr == m_hDll)
	{
		return FALSE;
	}

	return m_StopBridge();
}

void ComBridgeLoader::AllowBridge()
{
	if (nullptr == m_hDll)
	{
		return;
	}

	m_AllowBridge();
}

void ComBridgeLoader::BlockBridge()
{
	if (nullptr == m_hDll)
	{
		return;
	}

	m_BlockBridge();
}

void ComBridgeLoader::SetSerialBufferSize(DWORD dwSize)
{
	if (nullptr == m_hDll)
	{
		return;
	}

	m_SetSerialBufferSize(dwSize);
}

BOOL ComBridgeLoader::RegisterCallback(BRIDGE_CALLBACK_FUNC func)
{
	if (nullptr == m_hDll)
	{
		return FALSE;
	}

	m_RegisterCallback(func);
}