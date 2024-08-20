#pragma once
#include "kiero.h"

class simpleTimer {
public:
	void start(uint64_t ticks) {
		if (m_tick) {
			m_readyAt = GetTickCount64() + ticks;
			m_tick = false;
		}
	}
	bool isReady() { return GetTickCount64() > m_readyAt; }
	void reset() { m_tick = true; }
public:
	uint64_t m_readyAt;
	bool m_tick;
};
inline void check_if_pressed(bool& value, int key, size_t delay) {
	static simpleTimer t;
	if (GetForegroundWindow() == FindWindowA("grcWindow", NULL)) {
		if (GetAsyncKeyState(key) & 1) { value = true; }
		else if (GetAsyncKeyState(key) & 0x8000) {
			t.start(delay * 100);
			if (t.isReady())
				value = true;
		}
		else { t.reset(); }
	}
}
class render {
public:
	using directx_present_hook_t = long(__stdcall*)(IDXGISwapChain*, UINT, UINT);
	directx_present_hook_t directx_present_hook;
public:
	render() = default;
	~render() = default;
public:
	void start_render();
	void stop_render();
public:
	static LRESULT wndproc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	WNDPROC wnd_proc_hk;
public:
	bool initdx = true;
	static long __stdcall present_hk(IDXGISwapChain* swapchain, UINT syncinterval, UINT flags);
public:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	IDXGISwapChain* m_swapchain;
	ID3D11RenderTargetView* m_targetview;
	DXGI_SWAP_CHAIN_DESC m_desc;
public:
	ImFont* m_font;
	ImFont* m_subtitle_font;
	ImFont* m_header_font;
	ImFont* m_infobox_font;
	ImFont* m_icon_font;
	ImFont* m_big_icon_font;
	ImFont* m_default_font;
private:
	ImFontConfig m_font_config;
};
inline render g_render;