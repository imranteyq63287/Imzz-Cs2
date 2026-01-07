#pragma once

#include <memory>

short schema_get_offset(const char* class_name, const char* key_name);

#define SCHEMA(varName, type, className, keyName)																		\
	type& varName() {																									\
		static const short offset = schema_get_offset(className, keyName);												\
																														\
		return *reinterpret_cast<type*>(reinterpret_cast<unsigned __int64>(this) + offset);								\
	}

#define SCHEMA_WITH_OFFSET(varName, type, className, keyName, offset2)																		\
	type& varName() {																									\
		static const short offset = schema_get_offset(className, keyName);												\
																														\
		return *reinterpret_cast<type*>(reinterpret_cast<unsigned __int64>(this) + (offset + offset2));								\
	}

#define SCHEMA_ARRAY(varName, type, className, keyName)																	\
	type* varName() {																									\
		static const short offset = schema_get_offset(className, keyName);												\
																														\
		return reinterpret_cast<type*>(reinterpret_cast<unsigned __int64>(this) + offset);								\
	}

#define OFFSET( type, name, offset )																					\
    __forceinline std::add_lvalue_reference_t< type > name( ) const {													\
        return *reinterpret_cast< type* >( reinterpret_cast< std::uintptr_t >( this ) + offset );						\
    }