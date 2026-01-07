#pragma once

#include "../../includes.h"

namespace directx
{
	inline IDXGISwapChain* m_swap_chain;
	inline ID3D11DeviceContext* m_device_context;
	inline ID3D11RenderTargetView* m_render_target;
	inline ID3D11Device* m_device;

	void initialize();
	void unitialize();
	void start_frame(IDXGISwapChain* swap_chain);
	void new_frame();
	void end_frame();

	inline void* m_present_address;

	inline WNDPROC m_window_proc_original = 0;
	inline HWND m_window;
}
