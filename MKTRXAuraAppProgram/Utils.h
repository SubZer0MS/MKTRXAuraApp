#pragma once

namespace utils
{
	CONST ULONGLONG GetDllVersion(CONST LPCTSTR&);
	CONST PTCHAR GetStringResource(UINT, SIZE_T);
	VOID PrintError(CONST LPCTSTR&);
	VOID PrintInfo(CONST LPCTSTR, ...);
}