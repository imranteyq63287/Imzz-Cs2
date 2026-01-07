#pragma once

#include "c_baseentity.h"
#include "c_cs_weapon_base_vdata.h"

class c_attribute_container;
class c_econ_item_view;

class c_econ_entity : public c_base_entity
{
public:
	SCHEMA(flex_delay_time, float, "C_EconEntity", "m_flFlexDelayTime"); // float32
	SCHEMA(flex_delayed_weight, float*, "C_EconEntity", "m_flFlexDelayedWeight"); // float32*
	SCHEMA(attributes_initialized, bool, "C_EconEntity", "m_bAttributesInitialized");
	SCHEMA_ARRAY(attribute_manager, c_attribute_container, "C_EconEntity", "m_AttributeManager"); 
	SCHEMA(original_owner_xuid_low, uint32_t, "C_EconEntity", "m_OriginalOwnerXuidLow");
	SCHEMA(original_owner_xuid_high, uint32_t, "C_EconEntity", "m_OriginalOwnerXuidHigh");
	SCHEMA(fallback_paint_kit, int, "C_EconEntity", "m_nFallbackPaintKit");
	SCHEMA(fallback_seed, int, "C_EconEntity", "m_nFallbackSeed");
	SCHEMA(fallback_wear, float, "C_EconEntity", "m_flFallbackWear");
	SCHEMA(fallback_stat_trak, int, "C_EconEntity", "m_nFallbackStatTrak");
	SCHEMA(clientside, bool, "C_EconEntity", "m_bClientside");
	SCHEMA(particle_systems_created, bool, "C_EconEntity", "m_bParticleSystemsCreated");
	SCHEMA(attached_particles, void*, "C_EconEntity", "m_vecAttachedParticles"); // CUtlVector<int32> - указать подходящий контейнер
	SCHEMA(viewmodel_attachment, void*, "C_EconEntity", "m_hViewmodelAttachment"); // CHandle<CBaseAnimGraph>
	SCHEMA(old_team, int, "C_EconEntity", "m_iOldTeam");
	SCHEMA(attachment_dirty, bool, "C_EconEntity", "m_bAttachmentDirty");
	SCHEMA(unloaded_model_index, int, "C_EconEntity", "m_nUnloadedModelIndex");
	SCHEMA(num_owner_validation_retries, int, "C_EconEntity", "m_iNumOwnerValidationRetries");
	SCHEMA(old_providee, void*, "C_EconEntity", "m_hOldProvidee"); // CHandle<C_BaseEntity>
	SCHEMA(attached_models, void*, "C_EconEntity", "m_vecAttachedModels"); // CUtlVector<C_EconEntity::AttachedModelData_t>
};