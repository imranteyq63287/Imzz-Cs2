#pragma once
#include "../../includes.h"
#include "../../features/visuals/main/chams.h"
#include "../../features/visuals/other/event.hpp"
#include "../../features/visuals/main/chams.h"
class c_entity_instance;
class c_csgo_input;
class c_view_setup;
class c_aggregate_scene_object;
class c_base_scene_data;
class c_CMeshData;

class c_hook
{
	void* function;
	void* detour;
	void* trampoline;

public:
	void hook(void* target, void* dtr)
	{
		function = target;
		detour = dtr;

		MH_CreateHook(function, detour, &trampoline);
		MH_EnableHook(function);
	}

	void hook(uintptr_t target, void* dtr)
	{
		hook((void*)target, dtr);
	}

	template <typename t_original>
	t_original get_original()
	{
		return reinterpret_cast<t_original>(trampoline);
	}

	void unhook()
	{
		if (!function || !detour)
			return;

		MH_DisableHook(function);
		MH_RemoveHook(function);
	}

	bool is_valid() const
	{
		return function != nullptr && trampoline != nullptr;
	}
};

namespace hooks
{
	namespace create_move
	{
		inline c_hook m_create_move;

		void __fastcall hook(c_csgo_input* rcx, int slot, bool active);
	}
	namespace remove_intro
	{
		inline c_hook m_remove_intro;

		void hook(std::uintptr_t a1, std::uintptr_t a2, char* a3);
	}
	namespace auto_accept
	{
		inline c_hook m_auto_accept;

		void hook(void* unk, const char* event_name, void* unk1, float unk2);
	}
	namespace draw_sniper_crosshair
	{
		inline c_hook m_draw_sniper_crosshair;

		bool hook(c_cs_weapon_base* pWeaponBaseGun);
	}
	namespace is_gloving
	{
		inline c_hook m_dis_gloving;

		bool hook(c_glow_property* glowprop);
	}
	namespace glow_obj
	{
		inline c_hook m_glow_obj;

		void hook(c_glow_property* glowProperty, float* color);
	}
	namespace remove_legs
	{
		inline c_hook m_remove_legs;

		void hook(void* a1, void* a2, void* a3, void* a4, void* a5);
	}
	namespace draw_viewmodel
	{
		inline c_hook m_draw_viewmodel;
		void hook(float* a1, float* offsets, float* fov);
	}
	namespace get_field_of_view
	{
		inline c_hook m_get_field_of_view;
		float hook(void* a1);
	}
	namespace chams
	{
		inline c_hook m_chams;
		void hook(void* a1, void* a2, c_CMeshData* pMeshScene, int nMeshCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2);
	}
	namespace chamslocal
	{
		inline c_hook m_chams;
		void hook(void* a1, void* a2, c_CMeshData* pMeshScene, int nMeshCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2);
	}
	namespace present
	{
		inline c_hook m_present;

		HRESULT hook(IDXGISwapChain* swap_chain, unsigned int sync_interval, unsigned int flags);
	}

	namespace enable_cursor
	{
		inline c_hook m_enable_cursor;
		inline bool m_enable_cursor_input;

		void* hook(void* rcx, bool active);
		void unhook();
	}

	namespace validate_view_angles
	{
		inline c_hook m_validate_view_angles;

		void hook(c_csgo_input* input, void* a2);
	}

	namespace mouse_input_enabled
	{
		inline c_hook m_mouse_input_enabled;

		bool hook(void* ptr);
	}

	namespace frame_stage_notify
	{
		inline c_hook m_frame_stage_notify;

		void hook(void* source_to_client, int stage);
	}

	namespace override_view
	{
		inline c_hook m_override_view;

		void hook(void* source_to_client, c_view_setup* view_setup);
	}
	namespace removescope
	{
		inline c_hook m_removescope;

		void hook(void* a1, void* a2);
	}
	namespace on_add_entity
	{
		inline c_hook m_on_add_entity;

		void* hook(void* a1, c_entity_instance* entity_instance, int handle);
	}

	namespace on_remove_entity
	{
		inline c_hook m_on_remove_entity;

		void* hook(void* a1, c_entity_instance* entity_instance, int handle);
	}

	namespace on_level_init
	{
		inline c_hook m_on_level_init;

		void* hook(void* a1, const char* map_name);
	}

	namespace on_level_shutdown
	{
		inline c_hook m_on_level_shutdown;

		void* hook(void* a1, const char* map_name);
	}

	namespace draw_skybox_array
	{
		inline c_hook m_draw_skybox_array;

		void hook(void* a1, void* a2, void* a3, int a4, int a5, void* a6, void* a7);
	}

	namespace draw_light_scene
	{
		inline c_hook m_draw_light_scene;

		void hook(__int64 a1, __int64 a2);
	}

	namespace update_aggregate_scene_object
	{
		inline c_hook m_update_aggregate_scene_object;

		void* hook(c_aggregate_scene_object* a1, void* a2);
	}

	namespace draw_aggregate_scene_object
	{
		inline c_hook m_draw_aggregate_scene_object;

		void hook(void* a1, void* a2, c_base_scene_data* a3, int a4, int a5, void* a6, void* a7, void* a8);
	}

	namespace smoke_voxel_draw
	{
		inline c_hook m_smoke_voxel_draw;

		void hook(__int64 a1, __int64* a2, int a3, int a4, __int64 a5, __int64 a6);
	}
	
	bool initialize();

	void destroy();
}