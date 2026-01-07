#include "modules.h"
#include "../../includes.h"

void c_modules::parse()
{
	client = GetModuleHandleA(XOR_STR("client.dll"));
	engine2 = GetModuleHandleA(XOR_STR("engine2.dll"));
	schemasystem = GetModuleHandleA(XOR_STR("schemasystem.dll"));
	tier0 = GetModuleHandleA(XOR_STR("tier0.dll"));
	navsystem = GetModuleHandleA(XOR_STR("navsystem.dll"));
	rendersystem = GetModuleHandleA(XOR_STR("rendersystemdx11.dll"));
	localize = GetModuleHandleA(XOR_STR("localize.dll"));
	scenesystem = GetModuleHandleA(XOR_STR("scenesystem.dll"));
	materialsystem2 = GetModuleHandleA(XOR_STR("materialsystem2.dll"));
	resourcesystem = GetModuleHandleA(XOR_STR("resourcesystem.dll"));
	input_system = GetModuleHandleA(XOR_STR("inputsystem.dll"));
	animation_system = GetModuleHandleA(XOR_STR("animationsystem.dll"));
	particle = GetModuleHandleA(XOR_STR("particles.dll"));
	game_overlay_renderer64_dll = GetModuleHandleA(XOR_STR("GameOverlayRenderer64.dll"));
}