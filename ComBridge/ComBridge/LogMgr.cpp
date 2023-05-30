#include "pch.h"
#include "LogMgr.h"

#define LOG_FILE_NAME L"log.txt"

WCHAR LogMgr::m_strPath[MAX_PATH] = { 0, };
BOOL LogMgr::m_bShow = FALSE;

void LogMgr::Init()
{
	ZeroMemory(m_strPath, sizeof(m_strPath));
	GetCurrentDirectoryW(MAX_PATH, m_strPath);
	swprintf_s(m_strPath, L"%s\\%s", m_strPath, LOG_FILE_NAME);
}

void LogMgr::SetShow()
{
	m_bShow = TRUE;
}

void LogMgr::SetHide()
{
	m_bShow = FALSE;
}

void LogMgr::Log(const WCHAR* level, const WCHAR* str, va_list args)
{
	if (!m_bShow)
		return;

	FILE* fp = NULL;
	SYSTEMTIME systemTime;
	WCHAR strTime[200] = { 0, };

	_wfopen_s(&fp, m_strPath, L"ab");
	GetLocalTime(&systemTime);
	swprintf_s(strTime,
		L"=== %4.4d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d %s ===\n",
		systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, level);

	std::wstring strEndLine(wcslen(strTime), '=');

	fwprintf(fp, L"%s", strTime);
	vfwprintf(fp, str, args);
	fwprintf(fp, L"%s\n", strEndLine.c_str());
	fclose(fp);

	wprintf(L"%s", strTime);
	vwprintf(str, args);
	wprintf(L"%s\n", strEndLine.c_str());
}

void LogMgr::LogInfo(const WCHAR* str, ...)
{
	va_list args;

	va_start(args, str);
	Log(L"[INFO]", str, args);
	va_end(args);
}

void LogMgr::LogError(const WCHAR* str, ...)
{
	va_list args;

	va_start(args, str);
	Log(L"[ERROR]", str, args);
	va_end(args);
}

void LogMgr::LogDebug(const WCHAR* str, ...)
{
#ifdef _DEBUG
	va_list args;

	va_start(args, str);
	Log(L"[DEBUG]", str, args);
	va_end(args);
#endif
}