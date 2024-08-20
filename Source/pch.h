#pragma once
#ifndef PCH_H
#define PCH_H

#define _HAS_STD_BYTE 0

#include "framework.h"

namespace Defines {
	inline bool g_Running = true;
	inline HMODULE g_hModule = 0;
	inline HANDLE g_hModuleHandle = 0;
	inline DWORD g_dThreadID = 0;
}

using namespace Defines;

#endif //PCH_H