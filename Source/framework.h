#pragma once
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
//Windows Header Files
#pragma disable(warning : 4996)
#include <windows.h>
#include <vadefs.h>
#include <stack>
#include <winsdkver.h>
#include <vector>
#include <string>
#include <ostream>
#include <istream>
#include <fstream>
#include <iosfwd>
#include <set>
#include <map>
#include <unordered_map>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <filesystem>
#include <format>
//Namespace Defines
using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;
//ImGui Includes
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
//ImGui Defines
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//DirectX Includes
#include <dxgi.h>
#include <d3d11.h>
//Renderer Includes
#include "kiero.h"
#include "render.h"
//MinHook Includes
#include "minhook/include/minhook.h"