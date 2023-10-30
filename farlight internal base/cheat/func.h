#pragma once
#include "../math/xor.h"
#include "../memory/memory.h"
#include "../math/math.h"
#include "settings.h"

auto uworld_sig = sig_scan(0, _("48 8B 1D ? ? ? ? 48 85 DB 74 3B 41 B0 01 33 D2 48 8B CB E8"));
auto bonematrix_sig = sig_scan(0, _("E8 ? ? ? ? 48 8B 8C 24 ? ? ? ? 0F 28 00 0F 29 01 0F 28 48 10 0F 29 49 10 0F 28 40 20 0F 29 41"));
auto addrsLineOfSightTo = sig_scan(0, _("40 ? 53 56 57 48 8D ? ? ? 48 81 EC ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? 49 8B ? 48 8B ? 48 8B ? 48 85"));
auto w2s_sig = sig_scan(0, _("48 89 ? ? ? 48 89 ? ? ? 48 89 ? ? ? 57 48 81 EC ? ? ? ? 41 0F ? ? 49 8B ? 48 8B ? 48 8B ? 48 85 ? 0F 84"));
auto screen_sig = sig_scan(0, _("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 33 C0 49 8B F8 89 02 48 8B F2 41 89 00 48 8B 99"));

typedef bool(__thiscall* this_ProjectWorldLocationToScreens)(uintptr_t playerController, vec3 pos, vec2* screen, bool bPlayerViewportRelative);
this_ProjectWorldLocationToScreens m_ProjectWorldLocationToScreens;

typedef void(__thiscall* this_GetViewPortSize)(uintptr_t playerController, int32_t& ScreenWidth, int32_t& ScreenHeight);
this_GetViewPortSize m_GetViewPortSize;


vec3 getbone(uintptr_t mesh, int id)
{
	auto array = read<uintptr_t>(mesh + offset_bonearray);
	if (!array)
		array = read<uintptr_t>(mesh + offset_bonearray + offset_bonecount);
	FTransform bone = read<FTransform>(array + (id * 0x30));
	FTransform ComponentToWorld = read<FTransform>(mesh + offset_compent);
	D3DMATRIX Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
	return vec3(Matrix._41, Matrix._42, Matrix._43);
}


bool worldtoscreen(uintptr_t playerController, vec3 pos, vec2* screen, bool bPlayerViewportRelative)
{
	this_ProjectWorldLocationToScreens m_projworld = (this_ProjectWorldLocationToScreens)(DWORD64)w2s_sig;
	return m_projworld(playerController, pos, screen, bPlayerViewportRelative);
}

void screensize(uintptr_t playerController, int32_t& ScreenWidth, int32_t& ScreenHeight)
{
	this_GetViewPortSize m_getviewportsize = (this_GetViewPortSize)((DWORD64)screen_sig);
	return m_getviewportsize(playerController, ScreenWidth, ScreenHeight);
}

bool visible_check(uintptr_t playerController, uintptr_t Entity)
{
	vec3 rot = { 0,0,0 };
	auto LineCast = reinterpret_cast<bool(__fastcall*)(uintptr_t, uintptr_t, vec3*)>(addrsLineOfSightTo);
	return LineCast(playerController, Entity, &rot);
}

void aimbot_function(uintptr_t local_player_controller, int32_t width, int32_t height, vec3 boneid, uintptr_t mesh) {

	auto get_distance = [](double x1, double y1, double x2, double y2) {
		return sqrtf(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
	};
	vec2 screen_;
	if (worldtoscreen(local_player_controller, boneid, &screen_, false))
	{
		float aimspeedon = smooth_ + 1;
		auto screen = screen_;

		auto center_x = width / 2;
		auto center_y = height / 2;

		auto fov = get_distance(center_x, center_y, screen.x, screen.y);
		float TargetX = 0;
		float TargetY = 0;

		if (screen.x != 0) {
			if (screen.x > center_x) {
				TargetX = -(center_x - screen.x);
				TargetX /= aimspeedon;
				if (TargetX + center_x > center_x * 2) TargetX = 0;
			}

			if (screen.x < center_x) {
				TargetX = screen.x - center_x;
				TargetX /= aimspeedon;
				if (TargetX + center_x < 0) TargetX = 0;
			}
		}

		if (screen.y != 0) {
			if (screen.y > center_y) {
				TargetY = -(center_y - screen.y);
				TargetY /= aimspeedon;
				if (TargetY + center_y > center_y * 2) TargetY = 0;
			}

			if (screen.y < center_y) {
				TargetY = screen.y - center_y;
				TargetY /= aimspeedon;
				if (TargetY + center_y < 0) TargetY = 0;
			}
		}
		if (fov < aimfov)
		{

			mouse_event(MOUSEEVENTF_MOVE, float(TargetX), float(TargetY / 1.2), 0, 0);
		}
	}
}
