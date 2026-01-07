#include "interfaces.h"
#include "../../includes.h"
#include "../utils/utils.h"
#include "../classes/i_res_manager.h"
#include "../../features/visuals/main/helperchams.h"

void interfaces::initialize()
{
	client = find_interface<c_client*>(g_modules.client, XOR_STR("Source2Client002"));
	m_network_client_service = find_interface<i_network_client_service*>(g_modules.engine2, XOR_STR("NetworkClientService_001"));

	input_system = find_interface<void*>(g_modules.input_system, XOR_STR("InputSystemVersion001"));
	engine_client = find_interface<c_engine_client*>(g_modules.engine2, XOR_STR("Source2EngineToClient001"));
	cvar = find_interface<c_cvar*>(g_modules.tier0, XOR_STR("VEngineCvar007"));
	localize = find_interface<c_localize*>(g_modules.localize, XOR_STR("Localize_001"));
	schema_system = find_interface<c_schema_system*>(g_modules.schemasystem, XOR_STR("SchemaSystem_001"));
	m_res_manager = find_interface<i_res_manager*>(g_modules.resourcesystem, XOR_STR("ResourceSystem013"));
	m_legacy_game_ui = find_interface<i_legacy_game_ui*>(g_modules.client, XOR_STR("LegacyGameUI001"));

	entity_system = *reinterpret_cast<c_entity_system**>(utils::find_pattern_rel(g_modules.client, "48 8B 3D ? ? ? ? 48 89 3D", 0x3));
	global_vars = *reinterpret_cast<c_global_vars**>(utils::find_pattern_rel(g_modules.client, "48 8B 0D ? ? ? ? 4C 8D 05 ? ? ? ? 48 85 D2", 0x3));

	typedef c_csgo_input* (*csgo_input228)();
	csgo_input228 get_input = reinterpret_cast<csgo_input228>( utils::find_pattern( g_modules.client, "48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 33 C0" ) );
	csgo_input = get_input();

	engine_trace = *reinterpret_cast<c_engine_trace**>(utils::find_pattern_rel(g_modules.client, "4C 8B 35 ? ? ? ? 24 ? 0C ? 66 0F 7F 45 ? 88 45 ? 48 8B CB 48 8D 05 ? ? ? ? 89 7D ? 48 89 45 ? 89 7D ? C7 45 ? ? ? ? ? 44 88 7D", 0x3));
	m_game_event_manager = *reinterpret_cast<cs_i_game_event_manager**>(g_opcodes->resolve_relative_address(vmt::get_v_method<std::uint8_t*>(client, 14U) + 0x3E, 0x3, 0x7));
	mem_alloc = *reinterpret_cast<c_mem_alloc**>(utils::export_fn(reinterpret_cast<std::size_t>(GetModuleHandleA("tier0.dll")), XOR_STR("g_pMemAlloc")));

	random_float = reinterpret_cast<decltype(random_float)>(utils::export_fn(reinterpret_cast<std::size_t>(GetModuleHandleA("tier0.dll")), XOR_STR("RandomFloat")));
	random_seed = reinterpret_cast<decltype(random_seed)>(utils::export_fn(reinterpret_cast<std::size_t>(GetModuleHandleA("tier0.dll")), XOR_STR("RandomSeed")));


}