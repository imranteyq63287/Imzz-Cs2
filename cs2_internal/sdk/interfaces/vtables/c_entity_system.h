#pragma once

#include "../../utils/utils.h"
#include "../../modules/modules.h"
#include <string>
#include <iostream>

class c_base_entity;
class c_cs_player_pawn;
class c_cs_player_controller;
class c_main_class_information
{
public:
	const char* designer_class;
	const char* class_;
	const char* name_class;
	void* unk;
	c_main_class_information* next_class;
};

class c_second_class_information
{
public:
	char pad[0x28];
	c_main_class_information* main_class_information;
};

class c_unkown_information
{
public:
	char pad[0x8];
	c_second_class_information* second_class_information;
	char pad2[0x10];
	const char* first_name;
	char pad3[0x10];
	int number;
};

class c_entity_system
{
public:
	template <class C = c_base_entity>
	C* get_base_entity(int index)
	{
		static auto get_client_entity = reinterpret_cast<C * (__fastcall*)(void*, int)>(utils::find_pattern(g_modules.client, "4C 8D 49 ? 81 FA"));

		return get_client_entity(this, index);
	}

	c_cs_player_pawn* get_local_pawn()
	{
		static auto fn = reinterpret_cast<c_cs_player_pawn * (__fastcall*)(int)>(utils::find_pattern_rel(g_modules.client, "E8 ? ? ? ? 48 8B F0 48 85 C0 74 ? 48 8D 15 ? ? ? ? B9", 0x1));

		return fn(0);
	}

	c_cs_player_controller* get_local_controller() 
	{
		static auto local_controller = reinterpret_cast<c_cs_player_controller**>(utils::find_pattern_rel(g_modules.client, "E8 ? ? ? ? 48 85 C0 74 ? 8B CB E8", 0x1));

		return local_controller[0];
	}

	int get_highest_entiy_index() 
	{
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x20F0);
	}
};