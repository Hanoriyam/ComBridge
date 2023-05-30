#pragma once

typedef void(*BRIDGE_CALLBACK_FUNC)(BYTE* pData, DWORD dwSize);

enum class BRIDGE_STATUS
{
	ALLOW,
	BLOCK
};

class ComBridge
{
public:
	ComBridge();
	~ComBridge();

	BOOL InitBridge(LPCWSTR pwReadPort, LPCWSTR pwWritePort, DWORD dwBaudRate, BYTE bByteSize);
	BOOL StartBridge();
	BOOL StopBridge();
	void AllowBridge();
	void BlockBridge();
	void SetSerialBufferSize(DWORD dwSize);
	BOOL RegisterCallback(BRIDGE_CALLBACK_FUNC func);

private:
	void BridgeThread();

private:
	std::wstring m_strReadPortName;
	std::wstring m_strWritePortName;
	DWORD m_dwBaudRate;
	BYTE m_bByteSize;

	HANDLE m_hReadPort;
	HANDLE m_hWritePort;

	std::unique_ptr<std::thread> m_pThread;
	BOOL m_bStop;
	DWORD m_dwBufferSize;

	BRIDGE_STATUS m_status;
	BRIDGE_CALLBACK_FUNC m_callback;
};