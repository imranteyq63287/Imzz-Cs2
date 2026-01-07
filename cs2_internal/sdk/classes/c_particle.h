#pragma once

#include "../../../sdk/utils/typedefs/c_strong_handle.h"
#include <mutex>
#include "../utils/vector.h"

class cs_particle_effect
{
public:
	const char* name{};
	char pad[0x30]{};
};

class cs_particle_information
{
public:
	float time{};
	float width{};
	float unk2{};
};

class cs_particle_data
{
public:
	vec3_t* positions{};
	char pad[0x74]{};
	float* times{};
	void* unk_ptr{};
	char pad2[0x28];
	float* times2{};
	char pad3[0x98];
	void* unk_ptr2{};
};

struct cs_particle_color
{
	float r;
	float g;
	float b;
};

class cs_particle_snapshot
{
public:
	void draw(int count, void* data);
};

class cs_game_particle_manager_system
{
public:
	void create_effect_index(unsigned int* effect_index, cs_particle_effect* effect_data);
	void set_effect(unsigned int effect_index, int unk, void* clr, int unk2);
	void fn_init_effect(int effect_index, unsigned int unk, const c_strong_handle<cs_particle_snapshot>* particle_snapshot);
};

class cs_particle_manager
{
public:
	void create_snapshot(c_strong_handle<cs_particle_snapshot>* out_particle_snapshot);
	void draw(c_strong_handle<cs_particle_snapshot>* particle_snapshot, int count, void* data);
};

class cs_tracer_info
{
public:
	unsigned int effect_index = -1;

	vec3_t* positions = nullptr;

	float* times = nullptr;

	c_strong_handle<cs_particle_snapshot> handle_snapshot_particle{};
	cs_particle_data particle_data_;
};
