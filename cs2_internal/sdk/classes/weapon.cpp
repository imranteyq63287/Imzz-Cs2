#include "c_base_player_pawn.h"
#include "c_base_player_weapon.h"

bool c_player_weapon_services::weapon_owns_this_type(const char* name)
{
	if (this->my_weapons().m_size <= 0)
		return false;

	for (int i = 0; i < this->my_weapons().m_size; ++i)
	{
		auto handle = this->my_weapons().m_elements[i];
		
		if (!handle.is_valid())
			continue;

		auto entity = reinterpret_cast<c_base_player_weapon*>(interfaces::entity_system->get_base_entity(handle.get_entry_index()));
	
		if (!entity)
			continue;

		auto class_info = entity->get_class_information();

		if (!class_info || !class_info->first_name)
			continue;

		if (strcmp(class_info->first_name, name) == 0)
			return true;
	}

	return false;
}