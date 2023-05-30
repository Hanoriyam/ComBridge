#pragma once
class LogMgr
{
public:
	static void Init();
	static void SetShow();
	static void SetHide();
	static void LogDebug(const WCHAR* str, ...);
	static void LogInfo(const WCHAR* str, ...);
	static void LogError(const WCHAR* str, ...);

private:
	static void Log(const WCHAR* level, const WCHAR* str, va_list args);

private:
	static WCHAR m_strPath[MAX_PATH];
	static BOOL m_bShow;
};