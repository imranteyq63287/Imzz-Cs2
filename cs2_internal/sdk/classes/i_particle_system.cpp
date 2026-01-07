#include "i_particle_system.h"
#include <unordered_map>
#include <iostream>
#include "../modules/c_modules.hpp"
#include "../../dependencies/hash.h"

class c_signature final
{
	std::unordered_map< std::size_t, c_address > m_signature_list{ };

public:
	void add(const std::string name, c_address address)
	{
		if (address)
		{
			std::cout << "[SIG] find " << name << " -> " << std::to_string(address.as<int>()) << '\n';
		}
		else
		{
			std::cout << "[SIG] failed to find " << name << '\n';
			return;
		}

		m_signature_list.emplace(fnv1a::hash_64(name.c_str()), address);
	}

	c_address get(const std::string name)
	{
		return m_signature_list[fnv1a::hash_64(name.c_str())];
	}
};

inline const std::unique_ptr< c_signature > g_sig{ new c_signature() };

void i_particle_system::create_effect_index(unsigned int* effect_index, particle_effect * effect_data)
{
    g_sig->get("create_effect_index").as<void(__fastcall*)(i_particle_system*, unsigned int*, particle_effect*)>()(this, effect_index, effect_data);
}

void i_particle_system::create_effect(unsigned int effect_index, int unk, void* clr, int unk2)
{
    g_sig->get("create_effect").as<void(__fastcall*)(i_particle_system*, unsigned int, int, void*, int)>()(this, effect_index, unk, clr, unk2);
}

void i_particle_system::unk_function(int effect_index, unsigned int unk, const c_strong_handle<c_particle_snapshot>* particle_snapshot)
{
    g_sig->get("unk_function").as<bool(__fastcall*)(i_particle_system*, int, unsigned int, const c_strong_handle<c_particle_snapshot>*)>()(this, effect_index, unk, particle_snapshot);
}