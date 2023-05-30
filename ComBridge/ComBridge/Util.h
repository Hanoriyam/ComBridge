#pragma once

class Util
{
public:	
	static std::wstring ConvertToUnicode(BYTE* pBuffer, UINT uiCodePage);
	static std::string ConvertToMultiByte(WCHAR* pwBuffer, UINT uiCodePage);
};