#include "stdafx.h"

SETTINGS Settings = { 0 };

namespace SettingsHelper 
{
	VOID SaveConfig()
	{
		CHAR path[0xFF];
		GetTempPathA(sizeof(path) / sizeof(path[0]), path);
		strcat(path, xorstr("fn.cfg"));

		auto file = fopen(path, xorstr("wb"));
		if (file) {
			fwrite(&Settings, sizeof(Settings), 1, file);
			fclose(file);
		}
	}
	VOID LoadDefaultConfig()
	{
		Settings.Aimbot = true;
		Settings.Memory = true;
		Settings.AutoAimbot = false;
		Settings.VisibleCheck = true;
		Settings.SilentAimbot = false;
		Settings.AimbotFOV = 360.f;
		Settings.AimKey = VK_RBUTTON;
		Settings.InstantReload = false;
		Settings.ESP.Tracked = true;
		Settings.FOV = 110.f;
		Settings.SilentTP = false;
		Settings.AimbotSlow = 0.0f;
		Settings.NoRecoil = false;
		Settings.SilentPickaxe = false;
		Settings.RapidFire1 = false;
		Settings.Crosshair2 = false;
		Settings.MemoryV2 = false;
		Settings.BoatTeleport = false;
		Settings.WaterMark = false;
		Settings.ChestDist = 100.f;
		Settings.LootDist = 100.f;
		Settings.Head = false;
		Settings.EnemyTeleportKey = VK_MENU;
		Settings.CrosshairThickness = 2.0;
		Settings.CrosshairScale = 12.0;
		Settings.ESP.Containers = true;
		Settings.NoSpreadValue = 0.0f;
		Settings.ESP.Weapons = true;
		Settings.ESP.AimbotFOV = true;
		Settings.ESP.PlayerNames = false;
		Settings.ESP.Vehicles = true;
		Settings.FirstPerson = false;
		Settings.Freecam = false;
		Settings.ESP.RadarDistance = 4000;
		Settings.ESP.SupplyDrops = true;
		Settings.ESP.Distance = true;
		Settings.vehiclespeed = 2.f;
		Settings.FPS = true;
		Settings.NoSpread = false;
		Settings.SpinSpeed = 11.25f;
		Settings.RestoreRotation = true;
		Settings.NoClip = false;
		Settings.TeleportToEnemies = false;
		Settings.DrawSelf = false;
		Settings.RapidFire = false;
		Settings.ESP.PlayerWeapons = true;
		Settings.OutlineText = true; //true
		Settings.ESP.Ammo = false;
		Settings.ESP.Skeletons = true;
		Settings.ESP.WeakSpots = false;
		Settings.MemesTest = false;
		Settings.ESP.Memes = false;
		Settings.ColorAdjuster = false;
		Settings.PlayerFly = false;
		Settings.CustomSpeedValue = 2;
		Settings.ESP.PlayerLines = false;
		Settings.ESP.Boxes = false;
		Settings.FovChanger = false;
		Settings.RFMod = 0.02;
		Settings.FovValue = 80;
		Settings.PlayerTP = false;
		Settings.TargetLine = true;
		Settings.ESP.PlayerAmmo = true;
		Settings.ESP.LLamas = true;
		Settings.Crosshair = true;
		Settings.Test = false;
		Settings.OverallDistance = 250;
		Settings.HitBoxPos = 0;
		Settings.Rainbow = false;
		Settings.ESP.MinWeaponTier = 2;
		Settings.BoatFly = false;
		Settings.BoatFlyKey = VK_SHIFT;
		Settings.BoatFlySpeed = 50;
		Settings.ESP.Chams = false;
		Settings.Spinbot = false;
		Settings.ESP.cornerbox = false;
		Settings.BigPlayers = false;

		Settings.SniperAimbotSlow = 30.f;
		Settings.ESP.PlayerNameColor[0] = 210;
		Settings.ESP.PlayerNameColor[1] = 210;
		Settings.ESP.PlayerNameColor[2] = 210;
		Settings.ESP.PlayerNameColor[3] = 255;

		Settings.ESP.FovColor[0] = 255;
		Settings.ESP.FovColor[1] = 255;
		Settings.ESP.FovColor[2] = 255;
		Settings.ESP.FovColor[3] = 255;

		Settings.ESP.PlayerNameVisibleColor[0] = 210;
		Settings.ESP.PlayerNameVisibleColor[1] = 210;
		Settings.ESP.PlayerNameVisibleColor[2] = 210;
		Settings.ESP.PlayerNameVisibleColor[3] = 255;

		Settings.ESP.PlayerNameNotVisibleColor[0] = 210;
		Settings.ESP.PlayerNameNotVisibleColor[1] = 0;
		Settings.ESP.PlayerNameNotVisibleColor[2] = 0;
		Settings.ESP.PlayerNameNotVisibleColor[3] = 255;

		Settings.ESP.BoxVisibleColor[0] = 255;
		Settings.ESP.BoxVisibleColor[1] = 174;
		Settings.ESP.BoxVisibleColor[2] = 0;
		Settings.ESP.BoxVisibleColor[3] = 174;

		Settings.ESP.BoxNotVisibleColor[0] = 210;
		Settings.ESP.BoxNotVisibleColor[1] = 0;
		Settings.ESP.BoxNotVisibleColor[2] = 0;
		Settings.ESP.BoxNotVisibleColor[3] = 255;

		Settings.ESP.SnaplineVisibleColor[0] = 210;
		Settings.ESP.SnaplineVisibleColor[1] = 210;
		Settings.ESP.SnaplineVisibleColor[2] = 210;
		Settings.ESP.SnaplineVisibleColor[3] = 255;


		Settings.ESP.SnaplineNotVisibleColor[0] = 210;
		Settings.ESP.SnaplineNotVisibleColor[1] = 0;
		Settings.ESP.SnaplineNotVisibleColor[2] = 0;
		Settings.ESP.SnaplineNotVisibleColor[3] = 255;

		Settings.ESP.SkeletonVisibleColor[0] = 210;
		Settings.ESP.SkeletonVisibleColor[1] = 210;
		Settings.ESP.SkeletonVisibleColor[2] = 210;
		Settings.ESP.SkeletonVisibleColor[3] = 255;

		Settings.ESP.SkeletonNotVisibleColor[0] = 210;
		Settings.ESP.SkeletonNotVisibleColor[1] = 0;
		Settings.ESP.SkeletonNotVisibleColor[2] = 0;
		Settings.ESP.SkeletonNotVisibleColor[3] = 255;

		Settings.ESP.TargetLineColor[0] = 255;
		Settings.ESP.TargetLineColor[1] = 0;
		Settings.ESP.TargetLineColor[2] = 0;
		Settings.ESP.TargetLineColor[3] = 255;

		SaveConfig();
	}
	VOID LoadSavedConfig()
	{
		CHAR path[0xFF] = { 0 };
		GetTempPathA(sizeof(path) / sizeof(path[0]), path);
		strcat(path, xorstr("fn.cfg"));

		auto file = fopen(path, xorstr("rb"));
		if (file) {
			fseek(file, 0, SEEK_END);
			auto size = ftell(file);

			if (size == sizeof(Settings)) {
				fseek(file, 0, SEEK_SET);
				fread(&Settings, sizeof(Settings), 1, file);
				fclose(file);
			}
			else {
				fclose(file);
				LoadDefaultConfig();
			}
		}
		else {
			LoadDefaultConfig();
		}
	}
	VOID LoadTheme()
	{
		//ImGuiStyle* style = &ImGui::GetStyle();

		//style->WindowPadding = ImVec2(15, 15);
		//style->WindowRounding = 5.0f;
		//style->FramePadding = ImVec2(5, 5);
		//style->FrameRounding = 4.0f;
		//style->ItemSpacing = ImVec2(12, 8);
		//style->ItemInnerSpacing = ImVec2(8, 6);
		//style->IndentSpacing = 25.0f;
		//style->ScrollbarSize = 15.0f;
		//style->ScrollbarRounding = 9.0f;
		//style->GrabMinSize = 5.0f;
		//style->GrabRounding = 3.0f;

		//style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		//style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		//style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		//style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		//style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		//style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
		//style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		//style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		//style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		//style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		//style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		//style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		//style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		//style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		//style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		//style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		//style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		//style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		//style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		//style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		//style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		//style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		//style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		//style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		//style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		//style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		//style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		//style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		//style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		//style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		//style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		//style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		//style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		//style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
		//style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
		//style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
		//style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		//style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		//style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		//style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);


		//auto& style = ImGui::GetStyle();
		//style.WindowRounding = 0.2f;

		//ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5, 0.5);

		//ImVec4* colors = ImGui::GetStyle().Colors;
		//colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		//colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		//colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f); //94
		//colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
		//colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		//colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		//colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		//colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
		//colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
		//colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
		//colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		//colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		//colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		//colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		//colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		//colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		//colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		//colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		//colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
		//colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		//colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		//colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
		//colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
		//colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
		//colors[ImGuiCol_Header] = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
		//colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
		//colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
		//colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		//colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
		//colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		//colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
		//colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
		//colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
		//colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		//colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		//colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
		//colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		//colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
		//colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		//colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	}
}