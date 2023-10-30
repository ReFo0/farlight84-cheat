#include <Windows.h>
#include "offsets.h"
#include "func.h"
#include "draw.h"


void initcheat()
{
	auto u_world = read<uintptr_t>(uworld_sig + *(int*)(uworld_sig + 0x3) + 0x7);
	auto persistent_level = read<uintptr_t>(u_world + offset_persistent_level);
	auto game_instance = read<uintptr_t>(u_world + offset_game_instance);
	auto local_player_array = read<uintptr_t>(game_instance + offset_local_players_array);
	auto local_player = read<uintptr_t>(local_player_array);
	auto local_player_controller = read<uintptr_t>(local_player + offset_player_controller);
	auto local_player_camera = read<uintptr_t>(local_player_controller + offset_camera_manager);
	auto local_player_pawn = read<uintptr_t>(local_player_controller + offset_apawn);
	auto actors = read<uintptr_t>(persistent_level + offset_actor_array);
	auto actors_count = read<int>(persistent_level + offset_actor_count);
	screensize(local_player_controller, width, height);
	for (int i = 0; i < actors_count; i++)
	{
		auto actor_pawn = read<uintptr_t>(actors + i * 0x8);
		if (!actor_pawn) continue;
		auto instigator = read<uintptr_t>(actor_pawn + offsets_instigator_actor);
		if (!instigator) continue;
		auto mesh = read<uintptr_t>(instigator + offset_actor_mesh);
		if (!mesh) continue;
		auto root = read<uintptr_t>(instigator + offset_root_component);
		if (!root) continue;
		auto _health = read<float>(actor_pawn + offset_currenthealth);
		if (!_health) continue;
		auto maxhealth = read<float>(actor_pawn + offset_maxhealth);
		if (!maxhealth) continue;
		auto health = _health * 100 / maxhealth;
		if (health == 0) continue;

		if (draw_line)
		{
			vec2 screen;
			if (worldtoscreen(local_player_controller, read<vec3>(root + offset_relative_location), &screen, false))
			{
				ImVec4 snaplines = ImColor(255, 255, 255);
				DrawLine(ImVec2(width / 2, height), ImVec2(screen.x, screen.y), ImGui::ColorConvertFloat4ToU32(snaplines));
			}
		}

		if (!box && cornerbox) {
			ImVec4 boxcolor = ImColor(255, 255, 255);
			vec2 basew2s;
			auto vBaseBone = getbone(mesh, 0);
			worldtoscreen(local_player_controller, vBaseBone, &basew2s, false);
			vec3 headPos = vBaseBone; headPos.z = vBaseBone.z + (52.f * 3);
			vec2 headw2s;
			worldtoscreen(local_player_controller, headPos, &headw2s, false);
			auto CornerHeight = abs(headw2s.y - basew2s.y);
			auto CornerWidth = CornerHeight * 0.65;
			DrawCornerBox(headw2s.x - (CornerWidth / 2), headw2s.y, CornerWidth, CornerHeight, ImGui::ColorConvertFloat4ToU32(boxcolor), 1.f);
		}

		if (!cornerbox && box) {
			ImVec4 boxcolor = ImColor(255, 255, 255);
			vec2 basew2s;
			auto vBaseBone = getbone(mesh, 0);
			worldtoscreen(local_player_controller, vBaseBone, &basew2s, false);
			vec3 headPos = vBaseBone; headPos.z = vBaseBone.z + (52.f * 3);
			vec2 headw2s;
			worldtoscreen(local_player_controller, headPos, &headw2s, false);
			auto CornerHeight = abs(headw2s.y - basew2s.y);
			auto CornerWidth = CornerHeight * 0.65;
			DrawBox(headw2s.x - (CornerWidth / 2), headw2s.y, CornerWidth, CornerHeight, ImGui::ColorConvertFloat4ToU32(boxcolor), 1.f);
		}

		if (aimbot)
		{
			if (!steam_keybind(VK_RBUTTON)) continue;
			if (!visible_check(local_player_controller, instigator)) continue;
			vec3 boneid;
			if (boneidselect == 1) {
				boneid = getbone(mesh, head);

			}
			else if (boneidselect == 2) {
				boneid = getbone(mesh, neck_01);
			}
			else if (boneidselect == 3) {
				boneid = getbone(mesh, middle_01_l);
			}
			aimbot_function(local_player_controller, width, height, boneid, mesh);
		}

	}
}