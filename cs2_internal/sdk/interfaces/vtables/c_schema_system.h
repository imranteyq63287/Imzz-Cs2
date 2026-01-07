#pragma once
#include "../../utils/virual.h"

struct c_schema_class_field
{
    const char* m_name;
    char pad0[0x8];
    short m_offset;
    char pad1[0xE];
};

class c_schema_class_info 
{
public:
    const char* get_name() 
    {
        return *reinterpret_cast<const char**>((unsigned __int64)(this) + 0x8);
    };

    short get_fields_size() 
    {
        return *reinterpret_cast<short*>((unsigned __int64)(this) + 0x1C);
    };

    c_schema_class_field* get_fields() 
    {
        return *reinterpret_cast<c_schema_class_field**>((unsigned __int64)(this) + 0x28);
    };
};

class c_schema_type_scope
{
public:
    c_schema_class_info* find_declared_class(const char* class_name)
    {
        c_schema_class_info* rv = nullptr;

        vfunc(this, void, 2, &rv, class_name);

        return rv;
    };
};

class c_schema_system 
{
public:
    c_schema_type_scope* find_type_scope_for_module(const char* module_name) 
    {
        return vfunc(this, c_schema_type_scope*, 13, module_name, nullptr);
    };
};