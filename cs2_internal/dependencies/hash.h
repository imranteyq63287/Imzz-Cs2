#pragma once
#include <cstdint>

namespace fnv1a
{
	constexpr uint32_t val_32_const = 0x811c9dc5;
	constexpr uint32_t prime_32_const = 0x1000193;
	constexpr uint64_t val_64_const = 0xcbf29ce484222325;
	constexpr uint64_t prime_64_const = 0x100000001b3;

	inline constexpr uint32_t hash_32(const char* const str, const uint32_t value = val_32_const) noexcept
	{
		return (str[0] == '\0') ? value : hash_32(&str[1], (value ^ uint32_t((uint8_t)str[0])) * prime_32_const);
	}

	inline constexpr uint64_t hash_64(const char* const str, const uint64_t value = val_64_const) noexcept
	{
		return (str[0] == '\0') ? value : hash_64(&str[1], (value ^ uint64_t((uint8_t)str[0])) * prime_64_const);
	}
}