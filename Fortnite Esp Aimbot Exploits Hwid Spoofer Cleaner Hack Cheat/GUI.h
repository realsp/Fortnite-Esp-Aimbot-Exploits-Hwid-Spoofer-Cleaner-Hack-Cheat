#include "stdafx.h"
#define reseller_name xorstr("Fortnite")
int boxmodepos = 0;
static int tab = 0;
ImFont* DefaultFont;
ImVec2 p; // i think neutron is aids do you have a  ok dont worries ill try to fix it myself i think ik how ty for help
float gayx = 930;
float gayy = 360;
static int tabs = 4;

static const char* aimmodes[]
{
	"Memory",
	"Mouse",
	"Silent"
};

static const char* hitboxes[]
{
	"head",
	"neck",
	"body",
	"pelvis"
};

static const char* boxmodes[]
{
	"2D",
	"2D-Filled"
};

static const char* linemodes[]
{
	"bottom",
	"top",
	"center"
};

static const char* spinpitchmodes[]
{
	"static",
	"down",
	"up",
	"jitter",
	"jitter v2",
	"jitter v3",
	"random"
};

static const char* spinyawmodes[]
{
	"static",
	"spin slow",
	"spin fast",
	"jitter",
	"random"
};

namespace Render
{
	struct vec_2
	{
		int x, y;
	};

	struct Colors
	{
		ImColor red = { 255, 0, 0, 255 };
		ImColor green = { 0, 255, 0, 255 };
		ImColor blue = { 0, 136, 255, 255 };
		ImColor aqua_blue = { 0, 255, 255, 255 };
		ImColor cyan = { 0, 210, 210, 255 };
		ImColor royal_purple = { 102, 0, 255, 255 };
		ImColor dark_pink = { 255, 0, 174, 255 };
		ImColor black = { 0, 0, 0, 255 };
		ImColor white = { 255, 255, 255, 255 };
		ImColor purple = { 255, 0, 255, 255 };
		ImColor yellow = { 255, 255, 0, 255 };
		ImColor orange = { 255, 140, 0, 255 };
		ImColor gold = { 234, 255, 0, 255 };
		ImColor royal_blue = { 0, 30, 255, 255 };
		ImColor dark_red = { 150, 5, 5, 255 };
		ImColor dark_green = { 5, 150, 5, 255 };
		ImColor dark_blue = { 100, 100, 255, 255 };
		ImColor navy_blue = { 0, 73, 168, 255 };
		ImColor light_gray = { 200, 200, 200, 255 };
		ImColor dark_gray = { 150, 150, 150, 255 };
	};
	Colors color;

	void Text(int posx, int posy, ImColor clr, const char* text)
	{
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx, posy), ImColor(clr), text);
	}

	void OutlinedText(int posx, int posy, ImColor clr, const char* text)
	{
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx + 1, posy + 1), ImColor(color.black), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx - 1, posy - 1), ImColor(color.black), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx + 1, posy + 1), ImColor(color.black), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx - 1, posy - 1), ImColor(color.black), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx, posy), ImColor(clr), text);
	}

	void ShadowText(int posx, int posy, ImColor clr, const char* text)
	{
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx + 1, posy + 2), ImColor(0, 0, 0, 200), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx + 1, posy + 2), ImColor(0, 0, 0, 200), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx, posy), ImColor(clr), text);
	}

	void Rect(int x, int y, int w, int h, ImColor color, int thickness)
	{
		ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0, thickness);
	}

	void RectFilledGradient(int x, int y, int w, int h, ImColor color)
	{
		ImGui::GetOverlayDrawList()->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0);
	}

	void RectFilled(int x, int y, int w, int h, ImColor color)
	{
		ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0);
	}

	void Checkbox(const char* v, bool* option, float x, float y, const char* title)
	{
		ImGui::SetCursorPos({ x, y });
		ImGui::Checkbox(v, option);

		p = ImGui::GetWindowPos();

		ImGui::SetCursorPos({ x + 2, y - 20 });
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(0, 245, 245)));
		ImGui::Text(title);
		ImGui::PopStyleColor();

		Render::RectFilled(p.x + x + 3, p.y + y + 3, 16, 16, ImColor(200, 200, 200));

		if (*option)
		{
			Render::RectFilled(p.x + x + 7, p.y + y + 7, 8, 8, ImColor(0, 0, 0, 255));
		}
	}

	void Slider(const char* v, float* option, float x, float y, float min, float max, const char* title)
	{
		ImGui::SetCursorPos({ x, y });
		ImGui::SliderFloat(v, option, min, max);

		ImGui::SetCursorPos({ x + 2, y - 20 });
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(0, 245, 245)));
		ImGui::Text(title);
		ImGui::PopStyleColor();
	}

	void Tab(const char* v, float size_x, float size_y, static int tab_name, int tab_next)
	{
		if (ImGui::Button(v, ImVec2{ size_x, size_y })) tab_name = tab_next;
	}
}






bool Keys(bool LocalplayerIsInGui, bool Appol) {
	if (LocalplayerIsInGui == true and Appol == true) {
		if (GetAsyncKeyState(VK_UP)) {
			gayy -= 8;
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			gayy += 8;
		}

		if (GetAsyncKeyState(VK_LEFT)) {
			gayx -= 8;
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			gayx += 8;
		}
	}

	return true;
}

namespace AppleCheatsFramework
{
	void Text(float x, float y, const char* title) {
		ImGui::SetCursorPos({ x, y });
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(0, 0, 0, 255)));
		ImGui::Text(title);
		ImGui::PopStyleColor();
	}

	void Checkbox2(const char* v, bool* option, float x, float y)
	{
		ImGui::SetCursorPos({ x, y });
		ImGui::Checkbox(v, option);

		p = ImGui::GetWindowPos();

		Render::RectFilled(p.x + x + 3, p.y + y + 3, 16, 16, ImColor(200, 200, 200));
		Render::Text(p.x + x + 28, p.y + y + 2, ImColor(0, 0, 0, 255), v);

		if (option)
		{
			Render::RectFilled(p.x + x + 7, p.y + y + 7, 8, 8, ImColor(0, 0, 0, 255));
		}
	}

	void Checkbox2(const char* v, bool* option, float x, float y, const char* title)
	{
		ImGui::SetCursorPos({ x, y });
		ImGui::Checkbox(v, option);

		p = ImGui::GetWindowPos();

		ImGui::SetCursorPos({ x + 2, y - 20 });
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(0, 245, 245)));
		ImGui::Text(title);
		ImGui::PopStyleColor();

		Render::RectFilled(p.x + x + 3, p.y + y + 3, 16, 16, ImColor(200, 200, 200));

		if (*option)
		{
			Render::RectFilled(p.x + x + 7, p.y + y + 7, 8, 8, ImColor(0, 0, 0, 255));
		}
	}

	void URGAYLMAO(const char* v, bool* option, float x, float y)
	{
		ImGui::SetCursorPos({ x + 5, y });
		ImGui::Checkbox(v, option);

		p = ImGui::GetWindowPos();

		Render::RectFilled(p.x + x + 8, p.y + y + 3, 16, 16, ImColor(200, 200, 200));
		Render::Text(p.x + x + 33, p.y + y + 2, ImColor(0, 0, 0, 255), v);

		if (option)
		{
			Render::RectFilled(p.x + x + 12, p.y + y + 7, 8, 8, ImColor(0, 0, 0, 255));
		}
	}

	void AC_Checkbox(const char* v, bool* option, float x, float y, const char* title)
	{
		ImGui::SetCursorPos({ x, y });
		ImGui::Checkbox(v, option);

		p = ImGui::GetWindowPos();

		ImGui::SetCursorPos({ x + 2, y - 20 });
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(0, 0, 0, 255)));
		ImGui::Text(title);
		ImGui::PopStyleColor();

		Render::RectFilled(p.x + x + 3, p.y + y + 3, 16, 16, ImColor(200, 200, 200));
		Render::Text(p.x + x + 28, p.y + y + 2, ImColor(0, 0, 0, 255), v);

		if (*option)
		{
			Render::RectFilled(p.x + x + 7, p.y + y + 7, 8, 8, ImColor(0, 0, 0, 255));
		}
	}


	void Tab(const char* v, int tab_next, float x, float y)
	{
		ImGui::SetCursorPos({ x, y });
		if (ImGui::Button(v, ImVec2(140, 30))) tabs = tab_next;

		ImVec2 pos = ImGui::GetWindowPos();

		if (tabs == tab_next) {
			Render::RectFilled(pos.x + x, pos.y + y, 140, 30, ImColor(217, 217, 217));
			Render::Rect(pos.x + x, pos.y + y, 140, 30, ImColor(175, 175, 175, 255), 1);
			Render::Text(pos.x + x + 48, pos.y + y + 6, ImColor(0, 0, 0, 255), v);
		}
		else {
			Render::RectFilled(pos.x + x, pos.y + y, 140, 30, ImColor(234, 234, 234, 255));
			Render::Rect(pos.x + x, pos.y + y, 140, 30, ImColor(175, 175, 175, 255), 1);
			Render::Text(pos.x + x + 48, pos.y + y + 6, ImColor(0, 0, 0, 255), v);
		}
	}
}


namespace ImGui
{
	IMGUI_API bool Tab(unsigned int index, const char* label, int* selected, float width = 0, float height = 0)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4 color = style.Colors[ImGuiCol_Button];
		ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
		ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];

		if (index > 0)
			ImGui::SameLine();

		if (index == *selected)
		{
			style.Colors[ImGuiCol_Button] = colorActive;
			style.Colors[ImGuiCol_ButtonActive] = colorActive;
			style.Colors[ImGuiCol_ButtonHovered] = colorActive;
		}
		else
		{
			style.Colors[ImGuiCol_Button] = color;
			style.Colors[ImGuiCol_ButtonActive] = colorActive;
			style.Colors[ImGuiCol_ButtonHovered] = colorHover;
		}

		if (ImGui::Button(label, ImVec2(width, height)))
			*selected = index;

		style.Colors[ImGuiCol_Button] = color;
		style.Colors[ImGuiCol_ButtonActive] = colorActive;
		style.Colors[ImGuiCol_ButtonHovered] = colorHover;

		return *selected == index;
	}
}

ImVec4 backgroundColorRGB = ImVec4(255, 255, 255, 255);
ImVec4 innerColorRGB = ImVec4(219, 9, 72, 255);

ImVec4 backgroundColor = ImVec4(backgroundColorRGB.x / 255.f, backgroundColorRGB.y / 255.f, backgroundColorRGB.z / 255.f, backgroundColorRGB.w / 255.f);
ImVec4 innerColor = ImVec4(innerColorRGB.x / 255.f, innerColorRGB.y / 255.f, innerColorRGB.z / 255.f, innerColorRGB.w / 255.f);
ImVec4 textColor = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);


namespace utils {


	void ToggleButton(const char* str_id, bool* v)
	{
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetOverlayDrawList();

		float height = ImGui::GetFrameHeight() - 10;
		float width = height * 1.45f;
		float radius = height * 0.50f;

		ImGui::InvisibleButton(str_id, ImVec2(width, height));
		if (ImGui::IsItemClicked())
			*v = !*v;

		float t = *v ? 1.0f : 0.0f;

		ImGuiContext& g = *GImGui;
		float ANIM_SPEED = 0.05f;
		if (g.ActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
		{
			float t_anim = ImSaturate(g.ActiveIdTimer / ANIM_SPEED);
			t = *v ? (t_anim) : (1.0f - t_anim);
		}

		ImU32 col_bg;
		if (ImGui::IsItemHovered())
			col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), innerColor, t));
		else
			col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), innerColor, t));

		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
		draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f + 1.0f, IM_COL32(0, 0, 0, 255));
		draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
	}

	void ToggleButtonA(const char* str_id, bool* v, int A)
	{
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetOverlayDrawList();

		float height = ImGui::GetFrameHeight() - 10;
		float width = height * 1.45f;
		float radius = height * 0.50f;

		ImGui::InvisibleButton(str_id, ImVec2(width, height));
		if (ImGui::IsItemClicked()) {
			if (A == 1) {
				Settings.SilentAimbot = false;
			}
			else if (A == 2) {
				Settings.Aimbot = false;
			}
			else if (A == 3) {
				Settings.Aimbot = false;
				Settings.SilentAimbot = false;
			}
			*v = !*v;
		}

		float t = *v ? 1.0f : 0.0f;

		ImGuiContext& g = *GImGui;
		float ANIM_SPEED = 0.05f;
		if (g.ActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
		{
			float t_anim = ImSaturate(g.ActiveIdTimer / ANIM_SPEED);
			t = *v ? (t_anim) : (1.0f - t_anim);
		}

		ImU32 col_bg;
		if (ImGui::IsItemHovered())
			col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), innerColor, t));
		else
			col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), innerColor, t));

		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
		draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f + 1.0f, IM_COL32(0, 0, 0, 255));
		draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
	}

	void PushDisabled(bool disabled = true) {
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, disabled);
	}
	void PopDisabled(int num = 1) {
		while (num--) {
			ImGui::PopItemFlag();
		}
	}
}

namespace Menu
{
	void DrawMenu()
	{


		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
		colors[ImGuiCol_Button] = ImColor();
		colors[ImGuiCol_ButtonActive] = ImColor();
		colors[ImGuiCol_ButtonHovered] = ImColor();
		colors[ImGuiCol_FrameBg] = ImColor(200, 200, 200);
		colors[ImGuiCol_FrameBgHovered] = ImColor(200, 200, 200);
		colors[ImGuiCol_FrameBgActive] = ImColor(200, 200, 200);
		colors[ImGuiCol_CheckMark] = ImColor(0, 0, 0, 255);


		/*ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.89f, 0.89f, 0.89f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.89f, 0.89f, 0.89f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.89f, 0.89f, 0.89f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.39f, 0.39f, 0.39f, 1.00f));*/

		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 0.0f;
		style->FramePadding = ImVec2(2, 2);
		style->FrameRounding = 0.0f;
		style->ItemSpacing = ImVec2(8, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 0.0f;

		style->GrabMinSize = 4.5;
		style->GrabRounding = 0;
		style->ChildRounding = 0.f;
		style->WindowTitleAlign.x = 0.50f;
		style->FrameRounding = 2.0f;
		style->WindowBorderSize = 0.0f;

		ImGui::SetNextWindowSize({ 620, 450 });
		ImGui::SetNextWindowPos({ gayx,gayy });

		ImGui::GetOverlayDrawList()->AddRectFilled(ImGui::GetIO().MousePos, ImVec2(ImGui::GetIO().MousePos.x + 6.f, ImGui::GetIO().MousePos.y + 6.f), ImColor(0, 0, 0, 255));
		ImGui::Begin(xorstr("#FloydCoinCheats"), NULL, ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse);
		p = ImGui::GetWindowPos();

		Render::RectFilled(p.x, p.y, 620, 25, ImColor(0, 0, 0, 255));
		Render::Text(p.x + 3, p.y + 3, ImColor(255, 255, 255, 255), xorstr("SENTINEL FORTNITE @ " __DATE__ " " __TIME__ " (INSERT) (menu is only usable in game)"));

		AppleCheatsFramework::Tab(xorstr("Aimbot"), 1, 15, 40);
		AppleCheatsFramework::Tab(xorstr("Visuals"), 2, 165, 40);
		AppleCheatsFramework::Tab(xorstr("Mods"), 3, 315, 40);
		AppleCheatsFramework::Tab(xorstr("Misc"), 4, 465, 40);

		if (tabs == 1)
		{
			ImGui::SetCursorPos({ 15, 84 });
			ImGui::Checkbox(xorstr("Enable "), &Settings.enablenegro); ;
			/*if (!Settings.Aimbot) {
				Settings.Aimbot = true;
			}*/
			AppleCheatsFramework::Text(18, 104, xorstr("Aimbot mode (ENTER)"));

			ImGui::SetCursorPos({ 23, 124 });
			ImGui::Checkbox(xorstr("Memory Aim "), &Settings.Aimbot); ImGui::SetCursorPos({ 44, 124 });
			if (Settings.Aimbot) {
				Settings.SilentAimbot = false;
			}

			ImGui::SetCursorPos({ 23, 144 });
			ImGui::Checkbox(xorstr("Silent (V1) (might be unsafe (!!!)) "), &Settings.SilentAimbot);
			if (Settings.SilentAimbot) {
				Settings.Aimbot = false;
			}

			ImGui::SetCursorPos({ 23, 164 });
			ImGui::Checkbox(xorstr("Silent (V2, sticky) (might be unsafe (!!!)) "), &Settings.BulletTP);
			if (Settings.BulletTP) {
				Settings.VisibleCheck = false;
			}

			ImGui::SetCursorPos({ 15, 184 });
			ImGui::Checkbox(xorstr("Spinbot "), &Settings.Spinbot);

			ImGui::SetCursorPos({ 15, 204 });
			ImGui::Checkbox(xorstr("Reset rotation after spin "), &Settings.RestoreRotation);

			ImGui::SetCursorPos({ 15, 224 });
			ImGui::Checkbox(xorstr("Visible targets only (F4) "), &Settings.VisibleCheck);

			ImGui::SetCursorPos({ 15, 244 });
			ImGui::Text(xorstr("Aim bone (BACKSPACE) ")); ImGui::SetCursorPos({ 15, 244 });

			ImGui::SetCursorPos({ 21, 264 });
			ImGui::Checkbox(xorstr("Head "), &Settings.headxd); ImGui::SetCursorPos({ 44, 264 });
			if (Settings.headxd) {
				Settings.pelvisxd = false;
				Settings.chestxd = false;
			}
			ImGui::SetCursorPos({ 21, 284 });
			ImGui::Checkbox(xorstr("Chest "), &Settings.chestxd);
			if (Settings.chestxd) {
				Settings.headxd = false;
				Settings.pelvisxd = false;
			}

			ImGui::SetCursorPos({ 21, 304 });
			ImGui::Checkbox(xorstr("Pelvis "), &Settings.pelvisxd);
			if (Settings.pelvisxd) {
				Settings.headxd = false;
				Settings.chestxd = false;
			}

			ImGui::SetCursorPos({ 15, 344 });
			ImGui::PushItemWidth(-410);
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32(ImColor(0, 0, 0, 255)));
			ImGui::AppolSliderFloat(xorstr("FOV: "), &Settings.AimbotFOV, 0, 360);
			ImGui::AppolSliderFloat(xorstr("Smooth: "), &Settings.AimbotSlow, 500, 1000);

			ImGui::PopStyleColor();
		}

		if (tabs == 2)
		{
#pragma region VisualsTabLeftSide
			AppleCheatsFramework::Text(18, 84, xorstr("Environment: "));

			ImGui::SetCursorPos({ 15, 104 });
			ImGui::Checkbox(xorstr("Loot (F1) "), &Settings.ESP.Weapons);

			ImGui::SetCursorPos({ 15, 124 });
			ImGui::Checkbox(xorstr("Chests / ammo boxes (F2) "), &Settings.ESP.Containers);

			ImGui::SetCursorPos({ 15, 144 });
			ImGui::Checkbox(xorstr("Traps "), &Settings.Traps);

			ImGui::SetCursorPos({ 15, 164 });
			ImGui::Checkbox(xorstr("Rifts "), &Settings.Rifts);

			ImGui::SetCursorPos({ 15, 184 });
			ImGui::Checkbox(xorstr("Supply drops "), &Settings.ESP.SupplyDrops);

			ImGui::SetCursorPos({ 15, 204 });
			ImGui::Checkbox(xorstr("Draw Crosshair"), &Settings.Crosshair);

			ImGui::SetCursorPos({ 15, 224 });
			ImGui::Checkbox(xorstr("Taxi ESP"), &Settings.ESP.Vehicles);


			AppleCheatsFramework::Text(15, 244, xorstr("Loot tier (PageUp/PageDown) "));

			ImGui::SetCursorPos({ 21, 264 });
			ImGui::Checkbox(xorstr("Ultra Common "), &Settings.UltraCommon); /*, 16, 264*/
			if (Settings.UltraCommon) {
				Settings.Common = false;
				Settings.Uncommon = false;
				Settings.Rare = false;
				Settings.Legendary = false;
				Settings.Tier = 1;
			}

			ImGui::SetCursorPos({ 21, 284 });
			ImGui::Checkbox(xorstr("Common "), &Settings.Common); /*, 16, 264*/
			if (Settings.Common) {
				Settings.UltraCommon = false;
				Settings.Uncommon = false;
				Settings.Rare = false;
				Settings.Legendary = false;
				Settings.Tier = 2;
			}

			ImGui::SetCursorPos({ 21, 304 });
			ImGui::Checkbox(xorstr("Uncommon "), &Settings.Uncommon); /*, 16, 264*/
			if (Settings.Uncommon) {
				Settings.UltraCommon = false;
				Settings.Common = false;
				Settings.Rare = false;
				Settings.Legendary = false;
				Settings.Tier = 3;
			}

			ImGui::SetCursorPos({ 21, 324 });
			ImGui::Checkbox(xorstr("Rare "), &Settings.Rare); /*, 16, 264*/
			if (Settings.Rare) {
				Settings.UltraCommon = false;
				Settings.Common = false;
				Settings.Uncommon = false;
				Settings.Legendary = false;
				Settings.Tier = 4;
			}

			ImGui::SetCursorPos({ 21, 344 });
			ImGui::Checkbox(xorstr("Legendary "), &Settings.Legendary); /*, 16, 264*/
			if (Settings.Legendary) {
				Settings.UltraCommon = false;
				Settings.Common = false;
				Settings.Uncommon = false;
				Settings.Rare = false;
				Settings.Tier = 5;
			}

			ImGui::SetCursorPos({ 25, 380 });
			ImGui::PushItemWidth(-400);

			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32(ImColor(0, 0, 0, 255)));

			ImGui::AppolSliderFloat(xorstr("Chests distance: "), &Settings.ChestDist, 0, 600);

			ImGui::SetCursorPos({ 25, 416 });
			ImGui::AppolSliderFloat(xorstr("Loot distance: "), &Settings.LootDist, 0, 600);

			ImGui::PopStyleColor();
#pragma endregion

#pragma region VisualsTabMiddleside
			AppleCheatsFramework::Text(238, 84, (xorstr("Player: ")));

			ImGui::SetCursorPos({ 258, 104 });
			ImGui::Checkbox(xorstr("Box "), &Settings.ESP.Boxes);
			if (Settings.ESP.Boxes) {
				Settings.ESP.cornerbox = false;
				Settings.ESP.three = false;
			}

			ImGui::SetCursorPos({ 258, 124 });
			ImGui::Checkbox(xorstr("Cornered Box "), &Settings.ESP.cornerbox);
			if (Settings.ESP.cornerbox) {
				Settings.ESP.Boxes = false;
				Settings.ESP.three = false;
			}

			ImGui::SetCursorPos({ 258, 144 });
			ImGui::Checkbox(xorstr("3D Box "), &Settings.ESP.three);
			if (Settings.ESP.three) {
				Settings.ESP.Boxes = false;
				Settings.ESP.cornerbox = false;
			}

			ImGui::SetCursorPos({ 258, 164 });
			ImGui::Checkbox(xorstr("Name "), &Settings.ESP.PlayerNames);

			ImGui::SetCursorPos({ 258, 184 });
			ImGui::Checkbox(xorstr("Skeleton "), &Settings.ESP.realskel);

			ImGui::SetCursorPos({ 258, 204 });
			ImGui::Checkbox(xorstr("Distance "), &Settings.ESP.Distance);

			ImGui::SetCursorPos({ 258, 224 });
			ImGui::Checkbox(xorstr("Weapon/Active item "), &Settings.ESP.PlayerWeapons);

			ImGui::SetCursorPos({ 258, 244 });
			ImGui::Checkbox(xorstr("Snaplines"), &Settings.ESP.PlayerLines);
			{
				ImGui::SetCursorPos({ 235, 284 });

				ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32(ImColor(0, 0, 0, 255)));
				ImGui::PushItemWidth(-190);
				ImGui::AppolSliderFloat((xorstr("Snapline distance ")), &Settings.ESP.snaplinesdistance, 0, 300);
				ImGui::PopStyleColor();
			}

#pragma endregion

#pragma region VisualsTabRightSide
			AppleCheatsFramework::Text(403, 84, xorstr("Misc: "));

			ImGui::SetCursorPos({ 423, 104 });
			ImGui::Checkbox(xorstr("Aimbot FOV "), &Settings.ESP.AimbotFOV);

			ImGui::SetCursorPos({ 423, 124 });
			ImGui::Checkbox(xorstr("Aimbot indictators "), &Settings.ESP.streamproof);
			{
				ImGui::SetCursorPos({ 405, 164 });

				ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32(ImColor(0, 0, 0, 255)));
				ImGui::PushItemWidth(-18);
				ImGui::AppolSliderFloat(xorstr("Overall ESP Distance: "), &Settings.espdist, 0, 600);
				ImGui::PopStyleColor();
			}
#pragma endregion
		}



		if (tabs == 3) {

			ImGui::SetCursorPos({ 15, 84 });
			ImGui::Checkbox(xorstr("No weapon Spread (might be unsafe unless its nobloom only) "), &Settings.NoSpread);


			ImGui::SetCursorPos({ 15, 104 });
			ImGui::Checkbox(xorstr("No Minigun Cooldown (might be unsafe) "), &Settings.NoRecoil);

			ImGui::SetCursorPos({ 15, 124 });
			ImGui::Checkbox(xorstr("Aim in Air (Beta, might be unsafe (!!!))"), &Settings.AimWhileJumping);

			ImGui::SetCursorPos({ 15, 144 });
			ImGui::Checkbox(xorstr("Draw Targetline"), &Settings.TargetLine);

			ImGui::SetCursorPos({ 15, 164 });
			ImGui::Checkbox(xorstr("BULLET TP XD"), &Settings.BulletTP);

			ImGui::SetCursorPos({ 15, 184 });
			ImGui::Checkbox(xorstr("Camera FOV changer (might be unsafe (!!!)) "), &Settings.FovChanger);

			ImGui::SetCursorPos({ 15, 204 });
			ImGui::Checkbox(xorstr("Vehicle Waypoint Teleport (might be unsafe (ENTER) (!!!))"), &Settings.cartp);

			ImGui::SetCursorPos({ 15, 224 });
			ImGui::Checkbox(xorstr("Car Fly (Updated, might be unsafe)"), &Settings.VehicleTeleport);

			ImGui::SetCursorPos({ 15, 244 });
			ImGui::Checkbox(xorstr("Rapid fire - works fine ONLY on SNIPERS/BURTS ARs "), &Settings.RapidFire);

			ImGui::SetCursorPos({ 15, 304 });
			ImGui::PushItemWidth(-410);

			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32(ImColor(0, 0, 0, 255)));
			ImGui::AppolSliderFloat(xorstr("Camera FOV: "), &Settings.FOV, 0, 180.f);
			ImGui::AppolSliderFloat(xorstr("Firing rate modifier: "), &Settings.RapidFireValueL, 0, 7.50);

			ImGui::PopStyleColor();


			//AppleCheatsFramework::Checkbox(xorstr("No bloom only (legit nospread, only active when you're targeting your weapon)"), &Settings::NoBloom, 15, 84);
		   //AppleCheatsFramework::Checkbox(xorstr("Sniper TP xd"), &Settings::BulletTP, 15, 84);
		  //AppleCheatsFramework::Checkbox(xorstr("Teleport loot (CAPS, might be unsafe (!!!))"), &Settings::TP, 15, 104);
		 //AppleCheatsFramework::Checkbox(xorstr("Teleport to City (F7, Change City: pgup/pgdown)"), &Settings::TeleportToCity, 15, 104);
		//AppleCheatsFramework::Checkbox(xorstr("Self Revive (F5) (might be unsafe (!!!)) "), &Settings::SelfRevive, 15, 164);
	   //	AppleCheatsFramework::Checkbox(xorstr("Draw Targetline v2"), &Settings::DrawAimLinev2, 15, 164);
	  //AppleCheatsFramework::Text(18, 141, xorstr("Discord: https://discord.gg/resolutionfn"));

		}


		if (tabs == 4) {
			AppleCheatsFramework::Text(18, 86, xorstr("Add voidless#1337 (RESELLING QUESTIONS ONLY)"));
			AppleCheatsFramework::Text(18, 106, xorstr("Cheat Status: BE, Unknown, EAC Undetected"));
			AppleCheatsFramework::Text(18, 121, xorstr("Spoofer Status, BE Unknown, EAC Undetected"));
		}







		//ImGui::StyleColorsLight();

		//ImGuiStyle* style = &ImGui::GetStyle();
		//style->WindowPadding = ImVec2(15, 15);
		//style->WindowRounding = 1.0f;
		//style->FramePadding = ImVec2(5, 5);
		//style->ItemSpacing = ImVec2(12, 8);
		//style->ItemInnerSpacing = ImVec2(8, 6);
		//style->IndentSpacing = 25.0f;
		//style->ScrollbarSize = 0.0f;
		//style->ScrollbarRounding = 1.0f;
		//style->GrabMinSize = 5.0f;
		//style->GrabRounding = 3.0f;
		//style->WindowTitleAlign = ImVec2(0.06, 1.1);
		//style->Alpha = 1.f;
		//// title 
		//ImGui::PushStyleColor(ImGuiCol_TitleBgActive, backgroundColor);
		//ImGui::PushStyleColor(ImGuiCol_TitleBg, backgroundColor);
		//ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, backgroundColor);
		//ImGui::PushStyleColor(ImGuiCol_Border, innerColor); // 
		//ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.00f, 0.00f, 0.00f, 0.00f)); //
		//// window
		//ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 255, 255));
		//ImGui::PushStyleColor(ImGuiCol_TextDisabled, textColor);
		//ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, textColor);
		//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1.f));
		//ImGui::PushStyleColor(ImGuiCol_Border, innerColor);
		//// button
		//ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32({ 0, 255, 255, 1.f }));
		//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetColorU32({ 0, 255, 255, 1.f }));
		//ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetColorU32({ 0, 255, 255, 1.f }));
		//ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.40f, 0.39f, 0.38f, 1.00f));
		//ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
		//ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.40f, 0.39f, 0.38f, 1.00f));
		//ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
		//ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
		//ImGui::PushStyleColor(ImGuiCol_ModalWindowDarkening, ImVec4(94 / 255.f, 94 / 255.f, 94 / 255.f, 1.00f));
		//// Slider
		//ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImGui::GetColorU32({ 0, 255, 255, 255 }));
		//ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImGui::GetColorU32({ 0, 255, 255, 1.f }));
		//ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.89f, 0.89f, 0.89f, 1.00f));
		//ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.89f, 0.89f, 0.89f, 1.00f));
		//ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.89f, 0.89f, 0.89f, 1.00f));
		//ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.39f, 0.39f, 0.39f, 1.00f));

		//std::string title = xorstr("");
		////title += __DATE__;
		////title += xorstr(" ");
		////title += __TIME__;
		////title += xorstr(")");
		//ImGuiWindowFlags TargetFlags;
		//TargetFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
		//bool trueSend = false;
		//bool falseSend = false;

		//if (ImGui::Begin(title.c_str(), 0, ImVec2(500, 490), -1.f, TargetFlags)) 
		//{
		//	ImGui::GetOverlayDrawList()->AddRectFilled(ImGui::GetIO().MousePos, ImVec2(ImGui::GetIO().MousePos.x + 6.f, ImGui::GetIO().MousePos.y + 6.f), ImColor(255, 0, 0, 255));
		//	ImGui::Spacing();

		//	if (tab == 0) { // AIMBOT TAB
		//		ImGui::BeginGroup();
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::Spacing();
		//		//ImGui::Spacing();

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, -NULL/*9*/);
		//		ImGui::Checkbox(xorstr(" loot    [F1]"), &Settings.ESP.Weapons);
		//		ImGui::Checkbox(xorstr(" chests    [F2]"), &Settings.ESP.Containers);
		//		ImGui::Checkbox(xorstr(" boat fly    [F3]"), &Settings.TeleportToEnemies);
		//		//ImGui::Checkbox(xorstr(" pickaxe noclip    [F4]"), &Settings.SilentPickaxe);
		//		ImGui::Checkbox(xorstr(" rapidfire    [F4]"), &Settings.RapidFire);
		//		if (ImGui::Checkbox(xorstr(" SPINBOT    [F5]"), &Settings.Spinbot))
		//		{
		//			Settings.SilentAimbot = false;
		//		}
		//		ImGui::Checkbox(xorstr(" restore rotation after spin"), &Settings.RestoreRotation);
		//		ImGui::Checkbox(xorstr(" [detected] fov changer"), &Settings.FovChanger);
		//		if (ImGui::Checkbox(xorstr(" noclip    [F6]"), &Settings.NoClip))
		//		{
		//			//Settings.SilentAimbot = false;
		//		}
		//		if (ImGui::Checkbox(xorstr(" aimbot"), &Settings.Memory))
		//		{
		//			Settings.SilentAimbot = false;
		//			Settings.MemoryV2 = false;
		//		}
		//		ImGui::Checkbox(xorstr(" nospread"), &Settings.NoSpread);
		//		if (ImGui::Checkbox(xorstr(" silent"), &Settings.SilentAimbot))
		//		{
		//			Settings.Memory = false;
		//			Settings.Spinbot = false;
		//			Settings.MemoryV2 = false;
		//		}
		//		if (ImGui::Checkbox(xorstr(" silent V2"), &Settings.MemoryV2))
		//		{
		//			Settings.Memory = false;
		//			Settings.Silent = false;
		//		}
		//		ImGui::Checkbox(xorstr(" check visible    [F7]"), &Settings.VisibleCheck);
		//		ImGui::Checkbox(xorstr(" supply drops"), &Settings.ESP.SupplyDrops);
		//		ImGui::Checkbox(xorstr(" llamas"), &Settings.ESP.LLamas);
		//		//ImGui::Checkbox(xorstr(" player weapons"), &Settings.ESP.PlayerWeapons);
		//		//ImGui::Checkbox(xorstr(" target line"), &Settings.TargetLine);
		//		ImGui::Checkbox(xorstr(" vehicles"), &Settings.ESP.Vehicles);
		//		ImGui::Checkbox(xorstr(" radar"), &Settings.ESP.Radar);
		//		ImGui::Checkbox(xorstr(" names"), &Settings.ESP.PlayerNames);
		//		ImGui::Checkbox(xorstr(" distance"), &Settings.ESP.Distance);
		//		ImGui::Checkbox(xorstr(" weapon"), &Settings.ESP.PlayerWeapons);
		//		ImGui::Checkbox(xorstr(" skeletons"), &Settings.ESP.Skeletons);
		//		ImGui::Checkbox(xorstr(" boxes"), &Settings.ESP.Boxes);
		//		ImGui::Checkbox(xorstr(" fov"), &Settings.ESP.AimbotFOV);
		//		ImGui::Checkbox(xorstr(" crosshair"), &Settings.Crosshair);
		//		ImGui::Checkbox(xorstr(" boat boost"), &Settings.VehicleBoost);
		//		//ImGui::Checkbox(xorstr(" player fly"), &Settings.PlayerFly);

		//		ImGui::EndGroup();

		//		////////////////////////////////////////////
		//		// ON THE SIDE:
		//		ImGui::SameLine(NULL, 82/*47*/);
		//		ImGui::BeginGroup();
		//		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		//		//ImGui::Spacing();

		//		ImGui::Dummy(ImVec2(6, 6));
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::PushItemWidth(200);
		//		ImGui::SliderFloat(xorstr("loot distance"), &Settings.LootDist, 1.0f, 250.f);

		//		ImGui::Dummy(ImVec2(6, 6));
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::PushItemWidth(200);
		//		ImGui::SliderFloat(xorstr("chest distance"), &Settings.ChestDist, 1.0f, 250.f);

		//		/*ImGui::Dummy(ImVec2(6, 6));
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::PushItemWidth(200);
		//		ImGui::SliderFloat(xorstr("vehicle distance"), &Settings.BoatFlySpeed, 1.0f, 200.f);*/

		//		ImGui::Dummy(ImVec2(6, 6));
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::PushItemWidth(200);
		//		ImGui::SliderFloat(xorstr("fov"), &Settings.AimbotFOV, 1.0f, 360.f);

		//		ImGui::Dummy(ImVec2(6, 6));
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::PushItemWidth(200);
		//		ImGui::SliderFloat(xorstr("smooth"), &Settings.AimbotSlow, 0.0f, 1000.f, xorstr("%.2f"));

		//		ImGui::Dummy(ImVec2(6, 6));
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::PushItemWidth(200);
		//		ImGui::SliderFloat(xorstr("fly speed"), &Settings.BoatFlySpeed, 1.0f, 200.f);

		//		ImGui::Dummy(ImVec2(6, 6));
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::PushItemWidth(200);
		//		ImGui::SliderFloat(xorstr("camera fov"), &Settings.FovValue, 80.0f, 160.f);

		//		/*ImGui::Dummy(ImVec2(6, 6));
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::PushItemWidth(200);
		//		ImGui::SliderFloat(xorstr("rapidfire value"), &Settings.RFMod, 0.00f, 1.f, xorstr("%.3f"));*/

		//		//ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		//ImGui::SameLine(NULL, 1);

		//		ImGui::EndGroup();
		//	}

		//	if (tab == 1) { // VISUALS TAB
		//		ImGui::BeginGroup();
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::Spacing();
		//		//ImGui::Spacing();

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);


		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);


		//		ImGui::Dummy(ImVec2(0.0f, 0.0f)); // oke let me test something
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);


		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);


		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::EndGroup();

		//		////////////////////////////////////////////
		//		// ON THE SIDE:
		//		ImGui::SameLine(NULL, 0/*47*/);
		//		ImGui::BeginGroup();
		//		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		//		//ImGui::Spacing();

		//		ImGui::Dummy(ImVec2(6, 6));
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::PushItemWidth(200);

		//		ImGui::Dummy(ImVec2(6, 6));
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::PushItemWidth(200);

		//		ImGui::Dummy(ImVec2(6, 6));
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::PushItemWidth(200);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 6);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 6);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 6);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 6);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 6);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 6);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 6);

		//		ImGui::EndGroup();

		//	}
		//	if (tab == 2) { // EXPLOITS TAB
		//		ImGui::BeginGroup();
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::Spacing();
		//		//ImGui::Spacing();

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		
		//		/*
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		utils::ToggleButton(E("Show warning about enemies in the area"), &Settings.WarningEnemiesArea);
		//		ImGui::SameLine(NULL, 8);
		//		ImGui::Text(E("Show warning about enemies in the area"));*/

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);


		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		/*
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		utils::ToggleButton(E("Sniper Bullet TP"), &Settings.SniperHits);
		//		ImGui::SameLine(NULL, 8);
		//		ImGui::Text(E("Sniper Bullet TP"));

		//		if (Settings.SniperHits) {
		//			ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//			ImGui::SameLine(NULL, 9);
		//			utils::ToggleButton(E("[OPTION] Shoot through walls (Rage)"), &Settings.SniperThroughWalls);
		//			ImGui::SameLine(NULL, 8);
		//			ImGui::Text(E("[OPTION] Shoot through walls (Rage)"));

		//			ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//			ImGui::SameLine(NULL, 9);
		//			utils::ToggleButton(E("[OPTION] Sniper No Aimbot"), &Settings.SniperNoAimbot);
		//			ImGui::SameLine(NULL, 8);
		//			ImGui::Text(E("[OPTION] Sniper No Aimbot"));
		//		}
		//		else {
		//			Settings.SniperThroughWalls = false;
		//			Settings.SniperNoAimbot = false;
		//		}*/
		//		/*
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		utils::ToggleButton(E("Boat Super Boost"), &Settings.BoatBoost);
		//		ImGui::SameLine(NULL, 8);
		//		ImGui::Text(E("Boat Super Boost"));

		//		if (Settings.BoatBoost) {
		//			ImGui::Dummy(ImVec2(6, 6));
		//			ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//			ImGui::SameLine(NULL, 9);
		//			ImGui::PushItemWidth(200);
		//			ImGui::SliderFloat(E("[OPTION] Boost Speed Multiplier"), &Settings.BoatBoostMultiplier, 1.f, 10.f);
		//		}*/
		//		/*
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		utils::ToggleButton(E("Helicopter Infinite Boost"), &Settings.HelicopterInfiniteBoost);
		//		ImGui::SameLine(NULL, 8);
		//		ImGui::Text(E("Helicopter Infinite Boost"));

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		utils::ToggleButton(E("Helicopter Super Speed"), &Settings.HelicopterSpeed);
		//		ImGui::SameLine(NULL, 8);
		//		ImGui::Text(E("Helicopter Super Speed"));

		//		if (Settings.HelicopterSpeed) {
		//			ImGui::Dummy(ImVec2(6, 6));
		//			ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//			ImGui::SameLine(NULL, 9);
		//			ImGui::PushItemWidth(200);
		//			ImGui::SliderFloat(E("[OPTION] Super Speed Multiplier"), &Settings.HelicopterSpeedMultiplier, 1.f, 10.f);
		//		}*/

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::SameLine(NULL, 8);

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::SameLine(NULL, 8);

		//		ImGui::Dummy(ImVec2(6, 6));
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		ImGui::PushItemWidth(200);
		//		/*
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		utils::ToggleButton(E("Invisible Player"), &Settings.InvisiblePlayer);
		//		ImGui::SameLine(NULL, 8);
		//		ImGui::Text(E("Invisible Player"));
		//		*/
		//		/*
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		utils::ToggleButton(E("Freecam [BETA]"), &Settings.FreeCam);
		//		ImGui::SameLine(NULL, 8);
		//		ImGui::Text(E("Freecam [BETA]"));
		//		*/
		//		/*
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		utils::ToggleButton(E("Boat Fly"), &Settings.BoatFly);
		//		ImGui::SameLine(NULL, 8);
		//		ImGui::Text(E("Boat Fly"));*/

		//		//if (rank == 1) {
		//		ImGui::Dummy(ImVec2(0.0f, 0.5f));
		//		ImGui::SameLine(NULL, 9);
		//		/*
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		utils::ToggleButton(E("Annas Nonsense"), &Settings.AnnasMisc);
		//		ImGui::SameLine(NULL, 8);
		//		ImGui::Text(E("Annas Nonsense"));
		//		*/
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);

		//		/*
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 9);
		//		utils::ToggleButton(E("[BETA] Player TP"), &Settings.PlrTP);
		//		ImGui::SameLine(NULL, 8);
		//		ImGui::Text(E("[BETA] Player TP"));*/


		//		//}
		//		ImGui::EndGroup();

		//		////////////////////////////////////////////
		//		// ON THE SIDE:
		//		ImGui::SameLine(ImGui::GetWindowWidth() - 111/*111: size of the button*/ - 10);
		//		ImGui::BeginGroup();
		//		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		//		//ImGui::Spacing();

		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 1);
		//		/*
		//		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//		ImGui::SameLine(NULL, 1);
		//		ImGui::Text(E("TP Key"));
		//		ImGui::Hotkey(E("TP Key"), &Settings.BoatTPkey, ImVec2{ 111, 36 }, 0);
		//		*/
		//		ImGui::EndGroup();
		//	}


			ImGui::End();
		
	}
}