#include "utils.h"
#include <string_view>
#include "../../includes.h"

namespace utils
{
    template <typename T = std::uint8_t>
    T* get_absolute_address(T* rel_addres, int pre_fffset = 0x0, int post_offset = 0x0)
    {
        rel_addres += pre_fffset;
        rel_addres += sizeof(std::int32_t) + *reinterpret_cast<std::int32_t*>(rel_addres);
        rel_addres += post_offset;

        return rel_addres;
    }

    static std::vector<int> ida_to_bytes(const std::string_view& pattern)
    {
        std::vector<int> bytes{};

        char* start = const_cast<char*>(pattern.data());
        char* end = const_cast<char*>(pattern.data()) + pattern.size();

        for (char* current = start; current < end; ++current)
        {
            if (*current == '?')
            {

                ++current;

                if (*current == '?')
                    ++current;

                bytes.push_back(-1);
            }
            else
                bytes.push_back(strtoul(current, &current, 16));
        }

        return bytes;
    }

    uint8_t* find_pattern(HMODULE module_name, const char* pattern)
    {
        if (!module_name || !pattern || !*pattern)
        {
            debug_text(WHITE_COLOR, "find_pattern: Invalid arguments (module or pattern is null)\n");
            return nullptr;
        }

        PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(module_name);
        if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
        {
            debug_text(WHITE_COLOR, "find_pattern: Invalid DOS header for module 0x%p\n", module_name);
            return nullptr;
        }

        PIMAGE_NT_HEADERS ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(
            reinterpret_cast<uint8_t*>(module_name) + dosHeader->e_lfanew);

        if (ntHeaders->Signature != IMAGE_NT_SIGNATURE)
        {
            debug_text(WHITE_COLOR, "find_pattern: Invalid NT header for module 0x%p\n", module_name);
            return nullptr;
        }

        auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
        auto patternBytes = ida_to_bytes(pattern);
        auto scanBytes = reinterpret_cast<uint8_t*>(module_name);

        auto patternSize = patternBytes.size();
        auto patternData = patternBytes.data();

        for (unsigned int i = 0; i < sizeOfImage - patternSize; i++)
        {
            bool found = true;

            for (unsigned int j = 0; j < patternSize; ++j)
            {
                if (patternData[j] != -1 && scanBytes[i + j] != patternData[j])
                {
                    found = false;
                    break;
                }
            }

            if (found)
            {
                debug_text(GREEN_COLOR, "find_pattern: Pattern \"%s\" found at 0x%p in module 0x%p\n", pattern, &scanBytes[i], module_name);
                return &scanBytes[i];
            }
        }

        debug_text(RED_COLOR, "find_pattern: Pattern \"%s\" NOT found in module 0x%p\n", pattern, module_name);
        return nullptr;
    }

    uint8_t* find_pattern_rel(HMODULE module_name, const char* pattern, int offset)
    {
        uint8_t* address = find_pattern(module_name, pattern);
        if (address == nullptr)
            return nullptr;

        return get_absolute_address(address, offset);
    }

    uint8_t* find_pattern_relevant(HMODULE module, const char* pattern, int rel_offset, int add_offset)
    {
        uint8_t* address = find_pattern(module, pattern);
        if (!address)
            return nullptr;

        int32_t relative = *reinterpret_cast<int32_t*>(address + rel_offset);
        uint8_t* final_address = address + rel_offset + 4 + relative + add_offset;

#ifdef _DEBUG
        char debug_msg[256];
        sprintf_s(debug_msg, "find_pattern_rel: %p (relative: 0x%X, final: %p)", address, relative, final_address);
        debug_text(WHITE_COLOR, debug_msg);
#endif

        return final_address;
    }

    uint8_t* find_pattern_relevant3x(HMODULE module_name, const char* pattern, int offset, int rva_offset, int rip_offset)
    {
        uint8_t* address = find_pattern(module_name, pattern);
        if (!address)
            return nullptr;

        int32_t relative = *reinterpret_cast<int32_t*>(address + rva_offset);
        uint8_t* final_address = address + rva_offset + 4 + relative + rip_offset + offset;

#ifdef _DEBUG
        char debug_msg[256];
        sprintf_s(debug_msg,
            "find_pattern_relevant: base %p | rel_offset %d | relative 0x%X | rip_offset %d | final %p",
            address, rva_offset, relative, rip_offset, final_address);
        debug_text(WHITE_COLOR, debug_msg);
#endif

        return final_address;
    }

    unsigned __int64 export_fn(unsigned __int64 base, const char* procedure_name)
    {
        const auto dos_headers = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
        if (dos_headers->e_magic != IMAGE_DOS_SIGNATURE)
            return 0;

        const auto nt_headers = reinterpret_cast<const IMAGE_NT_HEADERS64*>(base + dos_headers->e_lfanew);
        if (nt_headers->Signature != IMAGE_NT_SIGNATURE)
            return 0;

        const std::uintptr_t export_directory_size = nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
        const std::uintptr_t export_directory_address = nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

        if (export_directory_size == 0u || export_directory_address == 0u)
            return 0;

        const auto exports = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(base + export_directory_address);
        const auto names_rva = reinterpret_cast<const std::uint32_t*>(base + exports->AddressOfNames);
        const auto name_ordinals_rva = reinterpret_cast<const std::uint16_t*>(base + exports->AddressOfNameOrdinals);
        const auto functions_rva = reinterpret_cast<const std::uint32_t*>(base + exports->AddressOfFunctions);

        std::size_t right = exports->NumberOfNames, left = 0u;
        while (right != left)
        {
            const std::size_t middle = left + ((right - left) >> 1u);
            const int result = strcmp(procedure_name, reinterpret_cast<const char*>(base + names_rva[middle]));

            if (result == 0)
            {
                const std::uint32_t function_rva = functions_rva[name_ordinals_rva[middle]];

                if (function_rva >= export_directory_address && function_rva - export_directory_address < export_directory_size)
                    break;

                return base + function_rva;
            }

            if (result > 0)
                left = middle + 1;
            else
                right = middle;
        }

        return 0;
    }
}