#include "pch.h"
#include "ComBridge.h"

ComBridge::ComBridge()
{
	m_hReadPort = NULL;
	m_hWritePort = NULL;
	m_pThread = nullptr;
	m_bStop = FALSE;

	m_status = BRIDGE_STATUS::BLOCK;
	m_dwBufferSize = 4096;
}

ComBridge::~ComBridge()
{
	if (m_pThread != nullptr) StopBridge();

	if (m_hReadPort != NULL) CloseHandle(m_hReadPort);
	m_hReadPort = NULL;

	if (m_hWritePort != NULL) CloseHandle(m_hWritePort);
	m_hWritePort = NULL;
}

BOOL ComBridge::InitBridge(LPCWSTR pwReadPort, LPCWSTR pwWritePort, DWORD dwBaudRate, BYTE bByteSize)
{
	if (m_pThread)
	{
		LogMgr::LogInfo(L"ComBridge::Init: Already Init.\n");
		return FALSE;
	}

	WCHAR pwReadPortName[256] = { 0, };
	WCHAR pwWritePortName[256] = { 0, };

	swprintf_s(pwReadPortName, L"\\\\.\\%s", pwReadPort);
	swprintf_s(pwWritePortName, L"\\\\.\\%s", pwWritePort);

	m_hReadPort =
		CreateFileW(pwReadPort,
			GENERIC_READ | GENERIC_WRITE,
			NULL,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
	if (m_hReadPort == INVALID_HANDLE_VALUE)
	{
		LogMgr::LogInfo(L"ComBridge::Init: CreateFile() error <1>. (%d)\n", GetLastError());
		return FALSE;
	}

	DCB comReadStateConfig = { 0, };
	if (!GetCommState(m_hReadPort, &comReadStateConfig))
	{
		LogMgr::LogInfo(L"ComBridge::Init: GetCommState() error <1>. (%d)\n", GetLastError());
		return FALSE;
	}
	comReadStateConfig.BaudRate = dwBaudRate;
	comReadStateConfig.ByteSize = bByteSize;

	if (!SetCommState(m_hReadPort, &comReadStateConfig))
	{
		LogMgr::LogInfo(L"ComBridge::Init: SetCommState() error <1>. (%d)\n", GetLastError());
		return FALSE;
	}

	COMMTIMEOUTS comReadTimeouts = { 0, };
	comReadTimeouts.ReadIntervalTimeout = 0;
	comReadTimeouts.ReadTotalTimeoutMultiplier = 0;
	comReadTimeouts.ReadTotalTimeoutConstant = 1;
	comReadTimeouts.WriteTotalTimeoutMultiplier = 0;
	comReadTimeouts.WriteTotalTimeoutConstant = 0;

	if (!SetCommTimeouts(m_hReadPort, &comReadTimeouts))
	{
		LogMgr::LogInfo(L"ComBridge::Init: SetCommTimeouts() error <1>. (%d)\n", GetLastError());
		return FALSE;
	}

	m_hWritePort =
		CreateFileW(
			pwWritePort,
			GENERIC_READ | GENERIC_WRITE,
			NULL,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
	if (m_hWritePort == INVALID_HANDLE_VALUE)
	{
		LogMgr::LogInfo(L"ComBridge::Init: CreateFile() error <2>. (%d)\n", GetLastError());
		return FALSE;
	}

	DCB comWriteStateConfig = { 0, };
	if (!GetCommState(m_hWritePort, &comWriteStateConfig))
	{
		LogMgr::LogInfo(L"ComBridge::Init: GetCommState() error <2>. (%d)\n", GetLastError());
		return FALSE;
	}
	comWriteStateConfig.BaudRate = dwBaudRate;
	comWriteStateConfig.ByteSize = bByteSize;

	if (!SetCommState(m_hWritePort, &comReadStateConfig))
	{
		LogMgr::LogInfo(L"ComBridge::Init: SetCommState() error <2>. (%d)\n", GetLastError());
		return FALSE;
	}

	COMMTIMEOUTS comWriteTimeouts{ 0, };
	comWriteTimeouts.ReadIntervalTimeout = 0;
	comWriteTimeouts.ReadTotalTimeoutMultiplier = 0;
	comWriteTimeouts.ReadTotalTimeoutConstant = 1;
	comWriteTimeouts.WriteTotalTimeoutMultiplier = 0;
	comWriteTimeouts.WriteTotalTimeoutConstant = 0;

	if (!SetCommTimeouts(m_hWritePort, &comWriteTimeouts))
	{
		LogMgr::LogInfo(L"ComBridge::Init: SetCommTimeouts() error <2>. (%d)\n", GetLastError());
		return FALSE;
	}

	m_strReadPortName = pwReadPort;
	m_strWritePortName = pwWritePort;
	m_dwBaudRate = dwBaudRate;
	m_bByteSize = bByteSize;

	return TRUE;
}

BOOL ComBridge::StartBridge()
{
	if (m_pThread != nullptr)
	{
		LogMgr::LogInfo(L"ComBridge::StartBridge: Already running.\n");
		return FALSE;
	}

	if (m_hReadPort == NULL || m_hWritePort == NULL)
	{
		LogMgr::LogInfo(L"ComBridge::StartBridge: Port is null\n");
		return FALSE;
	}

	m_pThread = std::make_unique<std::thread>(&ComBridge::BridgeThread, this);
	if (m_pThread == nullptr)
	{
		LogMgr::LogInfo(L"ComBridge::StartBridge: make_unique() error.\n");
		return FALSE;
	}

	return TRUE;
}

BOOL ComBridge::StopBridge()
{
	if (m_pThread == nullptr)
	{
		LogMgr::LogInfo(L"ComBridge::StopBridge: thread is not working.\n");
		return FALSE;
	}

	m_bStop = true;

	if (m_pThread->joinable())
	{
		m_pThread->join();
	}

	if (m_hReadPort != NULL) CloseHandle(m_hReadPort);
	m_hReadPort = NULL;

	if (m_hWritePort != NULL) CloseHandle(m_hWritePort);
	m_hWritePort = NULL;

	m_pThread = nullptr;

	return TRUE;
}

void ComBridge::AllowBridge()
{
	m_status = BRIDGE_STATUS::ALLOW;
}

void ComBridge::BlockBridge()
{
	m_status = BRIDGE_STATUS::BLOCK;
}

void ComBridge::SetSerialBufferSize(DWORD dwSize)
{
	m_dwBufferSize = dwSize;
}

BOOL ComBridge::RegisterCallback(BRIDGE_CALLBACK_FUNC func)
{
	if (nullptr == func)
		return FALSE;

	m_callback = func;

	return TRUE;
}

void ComBridge::BridgeThread()
{
	LogMgr::LogInfo(L"ComBridge::StartBridge: Bridge Start. [%s => %s] (BaudRate: %d, ByteSize: %d)\n",
		m_strReadPortName.c_str(),
		m_strWritePortName.c_str(),
		m_dwBaudRate,
		m_bByteSize);
	while (!m_bStop)
	{
		std::unique_ptr<BYTE[]> readData{ std::make_unique<BYTE[]>(m_dwBufferSize) };
		DWORD dwNumberOfBytesRead;
		if (!ReadFile(m_hReadPort, readData.get(), m_dwBufferSize, &dwNumberOfBytesRead, NULL))
		{
			LogMgr::LogInfo(L"ComBridge::StartBridge: ReadFile() error. (%d)\n", GetLastError());
			continue;
		}

		if (dwNumberOfBytesRead == 0)
		{
			continue;
		}

		LogMgr::LogInfo(L"%s\n", Util::ConvertToUnicode(readData.get(), CP_ACP).c_str());

		m_callback(readData.get(), dwNumberOfBytesRead);

		if (BRIDGE_STATUS::BLOCK == m_status)
		{
			continue;
		}

		DWORD dwNumberOfBytesWrite;
		if (!WriteFile(m_hWritePort, readData.get(), dwNumberOfBytesRead, &dwNumberOfBytesWrite, NULL))
		{
			LogMgr::LogInfo(L"ComBridge::StartBridge: WriteFile() error. (%d)\n", GetLastError());
			continue;
		}

		if (dwNumberOfBytesRead != dwNumberOfBytesRead)
		{
			LogMgr::LogInfo(L"dwNumberOfBytesRead: %d, dwNumberOfBytesWrite: %d not matched.\n",
				dwNumberOfBytesRead, dwNumberOfBytesWrite);
		}
	}

	m_bStop = false;
}