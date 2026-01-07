#pragma once

#include "../utils/schema/schema.h"
#include "../utils/typedefs/c_handle.h"

class c_entity_identity
{
public:
	SCHEMA(name_stringable_index, int, "CEntityIdentity", "m_nameStringableIndex"); // int32
	SCHEMA(name, const char*, "CEntityIdentity", "m_name"); // CUtlSymbolLarge
	SCHEMA(designer_name, const char*, "CEntityIdentity", "m_designerName"); // CUtlSymbolLarge
	SCHEMA(m_flags, uint32_t, "CEntityIdentity", "m_flags"); // uint32
	SCHEMA(world_group_id, void*, "CEntityIdentity", "m_worldGroupId"); // WorldGroupId_t
	SCHEMA(data_object_types, uint32_t, "CEntityIdentity", "m_fDataObjectTypes"); // uint32
	SCHEMA(path_index, void*, "CEntityIdentity", "m_PathIndex"); // ChangeAccessorFieldPathIndex_t
	SCHEMA(prev, c_entity_identity*, "CEntityIdentity", "m_pPrev"); // CEntityIdentity*
	SCHEMA(next, c_entity_identity*, "CEntityIdentity", "m_pNext"); // CEntityIdentity*
	SCHEMA(prev_by_class, c_entity_identity*, "CEntityIdentity", "m_pPrevByClass"); // CEntityIdentity*
	SCHEMA(next_by_class, c_entity_identity*, "CEntityIdentity", "m_pNextByClass"); // CEntityIdentity*

	bool is_valid()
	{
		return m_index() != INVALID_EHANDLE_INDEX;
	}

	int get_entry_index()
	{
		if (!is_valid())
			return ENT_ENTRY_MASK;

		return m_index() & ENT_ENTRY_MASK;
	}

	int get_serial_number()
	{
		return m_index() >> NUM_SERIAL_NUM_SHIFT_BITS;
	}

	OFFSET(int, m_index, 0x10);
};