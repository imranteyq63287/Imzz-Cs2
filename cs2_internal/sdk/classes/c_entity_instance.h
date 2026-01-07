#pragma once

#include "../utils/schema/schema.h"
#include "../utils/vector.h"
#include "../utils/typedefs/c_handle.h"
#include "c_entity_indentity.h"

#include "../interfaces/interfaces.h"

#define GET_CLASS_INFO(varName, dllName, className)                                                                  \
    static c_schema_type_scope* typeScope = interfaces::schema_system->find_type_scope_for_module(dllName);          \
    if (!typeScope) {                                                                                                \
        printf("Failed to get type scope: %s", dllName);                                                             \
                                                                                                                     \
        return false;                                                                                                \
    }                                                                                                                \
                                                                                                                     \
    static c_schema_class_info* varName = typeScope->find_declared_class(className);                                 \
    if (!classInfo) {                                                                                                \
        printf("Failed to get class info: %s", className);                                                           \
                                                                                                                     \
        return false;                                                                                                \
    }

class c_entity_instance
{
public:
    template <typename T>
    T* as() 
    {
        return reinterpret_cast<T*>(this);
    }

    c_base_handle get_handle()
    {
        c_entity_identity* identity = p_entity();

        if (identity == nullptr)
            return c_base_handle();

        return c_base_handle(identity->m_index(), identity->get_serial_number() - (identity->m_flags() & 1));
    }

    c_schema_class_info* get_schema_class_info() 
    {
        c_schema_class_info* classInfo = nullptr;

        vfunc(this, void, 42, &classInfo);

        return classInfo;
    }

    const char* get_entity_class_name()
    {
        c_schema_class_info* classInfo = get_schema_class_info();

        return classInfo->get_name();
    }

    c_base_handle get_ref_ehandle()
    {
        c_entity_identity* identity = p_entity();

        if (identity == nullptr)
            return c_base_handle();

        return c_base_handle(identity->m_index(), identity->get_serial_number() - (identity->m_flags() & 1));
    }

    bool is_player() 
    {
        return is_player_pawn() || is_player_controller();
    }

    bool is_player_pawn()
    {
        GET_CLASS_INFO(classInfo, "client.dll", "C_CSPlayerPawn");

        return get_schema_class_info() == classInfo;
    }

    bool is_player_controller() 
    {
        GET_CLASS_INFO(classInfo, "client.dll", "CCSPlayerController");

        return get_schema_class_info() == classInfo;
    }

    SCHEMA(isz_private_vscripts, const char*, "CEntityInstance", "m_iszPrivateVScripts"); // CUtlSymbolLarge
    SCHEMA(p_entity, c_entity_identity*, "CEntityInstance", "m_pEntity"); // CEntityIdentity*
    SCHEMA(c_script_component, void*, "CEntityInstance", "m_CScriptComponent"); // CScriptComponent*
    SCHEMA(visible_in_pvs, bool, "CEntityInstance", "m_bVisibleinPVS");
};