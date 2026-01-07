#pragma once

#include <memory>
#include <Windows.h>
#include <vector>

namespace utils
{
	uint8_t* find_pattern(HMODULE module_name, const char* pattern);
	uint8_t* find_pattern_rel(HMODULE module_name, const char* pattern, int pre_offset);
	uint8_t* find_pattern_relevant(HMODULE module, const char* pattern, int rel_offset, int add_offset);
    uint8_t* find_pattern_relevant3x(HMODULE module_name, const char* pattern, int offset, int rva_offset, int rip_offset);

	unsigned __int64 export_fn(unsigned __int64 base, const char* procedure_name);
 
    template <typename T = std::uint8_t>
    T* get_absolute_addresss(T* rel_addres, int pre_fffset = 0x0, int post_offset = 0x0)
    {
        rel_addres += pre_fffset;
        rel_addres += sizeof(std::int32_t) + *reinterpret_cast<std::int32_t*>(rel_addres);
        rel_addres += post_offset;
        return rel_addres;
    }
}