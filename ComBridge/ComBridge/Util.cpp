#include "pch.h"
#include "Util.h"

std::wstring Util::ConvertToUnicode(BYTE* pBuffer, UINT uiCodePage)
{
	int iSize;
	std::unique_ptr<WCHAR[]> pwBuffer;
	std::wstring strRet;

	iSize = MultiByteToWideChar(uiCodePage, 0, (LPCSTR)pBuffer, -1, NULL, 0);
	if (iSize < 1) return L"";

	pwBuffer = std::make_unique<WCHAR[]>(iSize + 1);
	if (pwBuffer == nullptr) return L"";

	MultiByteToWideChar(uiCodePage, 0, (LPSTR)pBuffer, -1, pwBuffer.get(), iSize + 1);
	strRet = pwBuffer.get();

	return strRet;
}

std::string Util::ConvertToMultiByte(WCHAR* pwBuffer, UINT uiCodePage)
{
	int iSize;
	std::unique_ptr<CHAR[]> pBuffer;
	std::string strRet;

	iSize = WideCharToMultiByte(uiCodePage, 0, pwBuffer, -1, NULL, 0, NULL, NULL);
	if (iSize < 1) return "";

	pBuffer = std::make_unique<CHAR[]>(iSize + 1);
	if (pBuffer == NULL) return "";

	WideCharToMultiByte(uiCodePage, 0, pwBuffer, -1, pBuffer.get(), iSize + 1, NULL, NULL);
	strRet = pBuffer.get();

	return strRet;
}