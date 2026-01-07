#include "directx.h"
#include "hooks.h"
#include "../utils/utils.h"
#include "../modules/modules.h"
#include "../utils/virual.h"
#include "../utils/render/render.h"
#include "../interfaces/interfaces.h"
#include "../../features/menu/menu.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")


IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND wnd, UINT msg, WPARAM param, LPARAM l_param);

LRESULT hk_wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Сначала своя обработка клавиши — до ImGui
	static bool insert_was_down = false;

	if (msg == WM_KEYDOWN && wparam == VK_INSERT)
	{
		if (!insert_was_down)
		{
			menu::was_opened = !menu::was_opened;

			// Полезно в CS2: сразу обновляем состояние курсора ImGui
			ImGui::GetIO().MouseDrawCursor = menu::was_opened;
		}
		insert_was_down = true;
	}
	else if (msg == WM_KEYUP && wparam == VK_INSERT)
	{
		insert_was_down = false;
	}

	// Передаём в ImGui
	bool imgui_handled = ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);

	// Когда меню открыто — блокируем ввод в игру, если ImGui его обработал
	if (menu::was_opened && imgui_handled)
		return 1;  // ← Очень помогает в Source 2

	// Управление курсором игры
	static auto original = hooks::enable_cursor::m_enable_cursor.get_original<decltype(&hooks::enable_cursor::hook)>();
	original(interfaces::input_system, menu::was_opened ? false : hooks::enable_cursor::m_enable_cursor_input);

	return CallWindowProc(directx::m_window_proc_original, hwnd, msg, wparam, lparam);
}

void directx::initialize()
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = GetForegroundWindow();
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;

	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] =
	{
		D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0
	};

	if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		featureLevelArray, 2, D3D11_SDK_VERSION,
		&sd, &pSwapChain, &pDevice, &featureLevel, &pContext)))
	{
		void** vTable = *reinterpret_cast<void***>(pSwapChain);
		m_present_address = vTable[8]; // индекс 8 = IDXGISwapChain::Present

		pSwapChain->Release();
		pDevice->Release();
		pContext->Release();
	}
}

void directx::unitialize()
{
	if (!m_window_proc_original)
		return;

	SetWindowLongPtr(m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_window_proc_original));
}

std::once_flag init_flag;

void directx::start_frame(IDXGISwapChain* swap_chain)
{
	ID3D11Texture2D* back_buffer;
	DXGI_SWAP_CHAIN_DESC desc;
	swap_chain->GetDesc(&desc);

	m_window = desc.OutputWindow;

	std::call_once(init_flag, [&]()
		{
			swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&m_device));
			m_device->GetImmediateContext(&m_device_context);
			swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));

			m_device->CreateRenderTargetView(back_buffer, nullptr, &m_render_target);

			if (back_buffer)
				back_buffer->Release();

			m_window_proc_original = reinterpret_cast<WNDPROC>(SetWindowLongPtr(m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hk_wnd_proc)));

			ImGui::CreateContext();

			ImGui_ImplWin32_Init(m_window);
			ImGui_ImplDX11_Init(m_device, m_device_context);

			g_render->initialize();
		});
}

void directx::new_frame()
{
	ImGuiIO io = ImGui::GetIO();

	g_render->update_screen_size(io);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
}

void directx::end_frame()
{
	ImGui::Render();

	m_device_context->OMSetRenderTargets(1, &m_render_target, nullptr);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}