#pragma once

#include "../utils/schema/schema.h"
#include "../utils/vector.h"
#include "../utils/utils.h"
#include "../utils/virual.h"

class c_skeleton_instace;
class c_skeleton_instace_legit;

class c_game_scene_node
{
public:
	SCHEMA(p_owner, c_game_scene_node*, "CGameSceneNode", "m_pOwner"); // CEntityInstance*
	SCHEMA(p_parent, c_game_scene_node*, "CGameSceneNode", "m_pParent"); // CGameSceneNode*
	SCHEMA(p_child, c_game_scene_node*, "CGameSceneNode", "m_pChild"); // CGameSceneNode*
	SCHEMA(p_next_sibling, c_game_scene_node*, "CGameSceneNode", "m_pNextSibling"); // CGameSceneNode*
	SCHEMA(h_parent, void*, "CGameSceneNode", "m_hParent"); // CGameSceneNodeHandle
	SCHEMA(vec_origin, vec3_t, "CGameSceneNode", "m_vecOrigin"); // CNetworkOriginCellCoordQuantizedVector
	SCHEMA(ang_rotation, vec3_t, "CGameSceneNode", "m_angRotation"); // QAngle
	SCHEMA(fl_scale, float, "CGameSceneNode", "m_flScale");
	SCHEMA(vec_abs_origin, vec3_t, "CGameSceneNode", "m_vecAbsOrigin"); // Vector
	SCHEMA(ang_abs_rotation, vec3_t, "CGameSceneNode", "m_angAbsRotation"); // QAngle
	SCHEMA(fl_abs_scale, float, "CGameSceneNode", "m_flAbsScale");
	SCHEMA(n_parent_attachment_or_bone, int16_t, "CGameSceneNode", "m_nParentAttachmentOrBone");
	SCHEMA(b_debug_abs_origin_changes, bool, "CGameSceneNode", "m_bDebugAbsOriginChanges");
	SCHEMA(b_dormant, bool, "CGameSceneNode", "m_bDormant");
	SCHEMA(b_force_parent_to_be_networked, bool, "CGameSceneNode", "m_bForceParentToBeNetworked");
	SCHEMA(n_hierarchical_depth, uint8_t, "CGameSceneNode", "m_nHierarchicalDepth");
	SCHEMA(n_hierarchy_type, uint8_t, "CGameSceneNode", "m_nHierarchyType");
	SCHEMA(n_do_not_set_anim_time_in_invalidate_physics_count, uint8_t, "CGameSceneNode", "m_nDoNotSetAnimTimeInInvalidatePhysicsCount");
	SCHEMA(name, const char*, "CGameSceneNode", "m_name"); // CUtlStringToken
	SCHEMA(hierarchy_attach_name, const char*, "CGameSceneNode", "m_hierarchyAttachName"); // CUtlStringToken
	SCHEMA(fl_z_offset, float, "CGameSceneNode", "m_flZOffset");
	SCHEMA(fl_client_local_scale, float, "CGameSceneNode", "m_flClientLocalScale");
	SCHEMA(v_render_origin, vec3_t, "CGameSceneNode", "m_vRenderOrigin"); // Vector


	c_skeleton_instace* get_skeleton_instance() 
	{
		return vmt::call_virtual<c_skeleton_instace*>(this, 8);
	}

	c_skeleton_instace_legit* get_skeleton_instance_legit()
	{
		return vmt::call_virtual<c_skeleton_instace_legit*>(this, 8);
	}

	void set_mesh_group_mask(uint64_t mask)
	{
		static auto fn = reinterpret_cast<void(__thiscall*) (void*, uint64_t)>(utils::find_pattern_rel(g_modules.client, "E8 ? ? ? ? 48 8B 5C 24 ? 4C 8B 7C 24 ? 4C 8B 74 24 ?", 0x1));
		
		return fn(this, mask);
	}
};