#include "stdafx.h"
#include "GUI.h"
#include "lazyimporter.h"
#include "Discord.h"
int aimkey;
uint64_t DiscordBase;
ImFont* m_pFont;
FVector ClosestPlayerCoords;
FVector Coords;
bool InVehicle;
bool firstS = false;
PVOID VehiclePawn;
float X;
float Y;
float GlobalFOV = 80.f;
static float ScreenCenterX = 0.0f;
static float ScreenCenterY = 0.0f;
Vector3 CamRot;
Vector3 CamLoc;
std::vector<uint64_t> radarPawns;
std::vector<PVOID> boatPawns;

static const void* game_rbx_jmp;


SHORT SafeKeyState(int kode)
{
	return GetAsyncKeyState(kode);
}

void RadarRange(float* x, float* y, float range)
{
	if (fabs((*x)) > range || fabs((*y)) > range)
	{
		if ((*y) > (*x))
		{
			if ((*y) > -(*x))
			{
				(*x) = range * (*x) / (*y);
				(*y) = range;
			}
			else
			{
				(*y) = -range * (*y) / (*x);
				(*x) = -range;
			}
		}
		else
		{
			if ((*y) > -(*x))
			{
				(*y) = range * (*y) / (*x);
				(*x) = range;
			}
			else
			{
				(*x) = -range * (*x) / (*y);
				(*y) = -range;
			}
		}
	}
}

void CalcRadarPoint(FVector vOrigin, int& screenx, int& screeny)
{
	FRotator vAngle = FRotator{ CamRot.x, CamRot.y, CamRot.z };
	auto fYaw = vAngle.Yaw * PI / 180.0f;
	float dx = vOrigin.X - CamLoc.x;
	float dy = vOrigin.Y - CamLoc.y;

	float fsin_yaw = sinf(fYaw);
	float fminus_cos_yaw = -cosf(fYaw);

	float x = dy * fminus_cos_yaw + dx * fsin_yaw;
	x = -x;
	float y = dx * fminus_cos_yaw - dy * fsin_yaw;

	float range = (float)Settings.ESP.RadarDistance;

	RadarRange(&x, &y, range);

	ImVec2 DrawPos = ImGui::GetCursorScreenPos();
	ImVec2 DrawSize = ImGui::GetContentRegionAvail();

	int rad_x = (int)DrawPos.x;
	int rad_y = (int)DrawPos.y;

	float r_siz_x = DrawSize.x;
	float r_siz_y = DrawSize.y;

	int x_max = (int)r_siz_x + rad_x - 5;
	int y_max = (int)r_siz_y + rad_y - 5;

	screenx = rad_x + ((int)r_siz_x / 2 + int(x / range * r_siz_x));
	screeny = rad_y + ((int)r_siz_y / 2 + int(y / range * r_siz_y));

	if (screenx > x_max)
		screenx = x_max;

	if (screenx < rad_x)
		screenx = rad_x;

	if (screeny > y_max)
		screeny = y_max;

	if (screeny < rad_y)
		screeny = rad_y;
}

//FBox GetFBox(uintptr_t Actor)
//{
//	if (!Actor) return {};
//
//	Vector3 Origin, BoxExtend;
//
//	auto fGetActorBounds = reinterpret_cast<void(__fastcall*)(__int64, char, Vector3*, Vector3*)>(offsets::fnGetBounds);
//
//	Util::Spoof(game_rbx_jmp, fGetActorBounds, (__int64)Actor, (char)true, &Origin, &BoxExtend);
//
//	FBox NewBox;
//	NewBox.IsValid = 1;
//	NewBox.Min = Origin - BoxExtend;
//	NewBox.Max = Origin + BoxExtend;
//
//	return NewBox;
//}

//void Draw3DBoundingBox(uintptr_t mesh, uint64_t CurrentActor, ImU32 color)
//{
//	if (!CurrentActor) return;
//
//	Vector3 min, max, vec1, vec2, vec3, vec4, vec5, vec6, vec7, vec8;
//
//	FBox box = GetFBox(CurrentActor);
//
//	if (!box.IsValid) return;
//
//	min = box.Min;
//	max = box.Max;
//
//	vec3 = min;
//	vec3.x = max.x;
//	vec4 = min;
//	vec4.y = max.y;
//	vec5 = min;
//	vec5.z = max.z;
//	vec6 = max;
//	vec6.x = min.x;
//	vec7 = max;
//	vec7.y = min.y;
//	vec8 = max;
//	vec8.z = min.z;
//	if (Util::WorldToScreen(width, height, &min.x)) {
//		vec1 = min;
//	}
//	if (Util::WorldToScreen(width, height, &max.x)) {
//		vec2 = max;
//	}
//	if (Util::WorldToScreen(width, height, &vec3.x)) {
//		vec3 = vec3;
//	}
//	if (Util::WorldToScreen(width, height, &vec4.x)) {
//		vec4 = vec4;
//	}
//	if (Util::WorldToScreen(width, height, &vec5.x)) {
//		vec5 = vec5;
//	}
//	if (Util::WorldToScreen(width, height, &vec6.x)) {
//		vec6 = vec6;
//	}
//	if (Util::WorldToScreen(width, height, &vec7.x)) {
//		vec7 = vec7;
//	}
//	if (Util::WorldToScreen(width, height, &vec8.x)) {
//		vec8 = vec8;
//	}
//
//	if (vec1.x == 0 && vec1.y == 0) return;
//	if (vec2.x == 0 && vec2.y == 0) return;
//	if (vec3.x == 0 && vec3.y == 0) return;
//	if (vec4.x == 0 && vec4.y == 0) return;
//	if (vec5.x == 0 && vec5.y == 0) return;
//	if (vec6.x == 0 && vec6.y == 0) return;
//	if (vec7.x == 0 && vec7.y == 0) return;
//	if (vec8.x == 0 && vec8.y == 0) return;
//
//	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vec1.x, vec1.y), ImVec2(vec5.x, vec5.y), color, Settings.ESP.SnaplinePower);
//	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vec2.x, vec2.y), ImVec2(vec8.x, vec8.y), color, Settings.ESP.SnaplinePower);
//	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vec3.x, vec3.y), ImVec2(vec7.x, vec7.y), color, Settings.ESP.SnaplinePower);
//	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vec4.x, vec4.y), ImVec2(vec6.x, vec6.y), color, Settings.ESP.SnaplinePower);
//	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vec1.x, vec1.y), ImVec2(vec3.x, vec3.y), color, Settings.ESP.SnaplinePower);
//	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vec1.x, vec1.y), ImVec2(vec4.x, vec4.y), color, Settings.ESP.SnaplinePower);
//	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vec8.x, vec8.y), ImVec2(vec3.x, vec3.y), color, Settings.ESP.SnaplinePower);
//	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vec8.x, vec8.y), ImVec2(vec4.x, vec4.y), color, Settings.ESP.SnaplinePower);
//	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vec2.x, vec2.y), ImVec2(vec6.x, vec6.y), color, Settings.ESP.SnaplinePower);
//	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vec2.x, vec2.y), ImVec2(vec7.x, vec7.y), color, Settings.ESP.SnaplinePower);
//	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vec5.x, vec5.y), ImVec2(vec6.x, vec6.y), color, Settings.ESP.SnaplinePower);
//	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vec5.x, vec5.y), ImVec2(vec7.x, vec7.y), color, Settings.ESP.SnaplinePower);
//}


void renderRadar() {
	//for (auto pawn : radarPawns) {
	//	auto player = pawn;

	//	int screenx = 0;
	//	int screeny = 0;

	//	float Color_R = 255 / 255.f;
	//	float Color_G = 128 / 255.f;
	//	float Color_B = 0 / 255.f;

	//	FVector pos = *Util::GetPawnRootLocation((PVOID)pawn);

	//	CalcRadarPoint(pos, screenx, screeny);

	//	ImDrawList* Draw = ImGui::GetOverlayDrawList();

	//	FVector viewPoint = { 0 };
	//	//if (IsTargetVisible(pawn)) {
	//	//	Color_R = 128 / 255.f;
	//	//	Color_G = 224 / 255.f;
	//	//	Color_B = 0 / 255.f;
	//	//}
	//	Draw->AddRectFilled(ImVec2((float)screenx, (float)screeny),
	//		ImVec2((float)screenx + 5, (float)screeny + 5),
	//		ImColor(Color_R, Color_G, Color_B));
	//}

}

template<typename T>
T WriteMem(DWORD_PTR address, T value)
{
	return *(T*)address = value;
}



static void Rapid(float time)
{
	uint64_t CurrentWeapon = 0;
	float i = 0;
	float c = 0;

	CurrentWeapon = (uint64_t)ReadPointer(hooks::LocalPlayerPawn, offsets::FortPawn::CurrentWeapon); //current weapon


	if (CurrentWeapon) {
		i = *(float*)(CurrentWeapon + offsets::FortPawn::LastFireTime);
		c = *(float*)(CurrentWeapon + offsets::FortPawn::LastFireTimeVerified);
		*(float*)(CurrentWeapon + offsets::FortPawn::LastFireTime) = i + c - time;

		return;
	}
}

namespace Classes
{
	namespace StaticOffsets {

		uintptr_t NIGGGEEEER = 0x2B1;

	}
}


#define ITEM_COLOR_TIER_WHITE ImVec4{ 0.8f, 0.8f, 0.8f, 0.95f }
#define ITEM_COLOR_TIER_GREEN ImVec4{ 0.0f, 0.95f, 0.0f, 0.95f }
#define ITEM_COLOR_TIER_BLUE ImVec4{ 0.2f, 0.4f, 1.0f, 0.95f }
#define ITEM_COLOR_TIER_PURPLE ImVec4{ 0.7f, 0.25f, 0.85f, 0.95f }
#define ITEM_COLOR_TIER_ORANGE ImVec4{ 0.85f, 0.65f, 0.0f, 0.95f }
#define ITEM_COLOR_TIER_GOLD ImVec4{ 0.95f, 0.85f, 0.45f, 0.95f }
#define ITEM_COLOR_TIER_UNKNOWN ImVec4{ 1.0f, 0.0f, 1.0f, 0.95f }
#define ITEM_COLOR_MEDS ImVec4{ 0.9f, 0.55f, 0.55f, 0.95f }
#define ITEM_COLOR_SHIELDPOTION ImVec4{ 0.35f, 0.55f, 0.85f, 0.95f }
#define ITEM_COLOR_CHEST ImVec4{0.95f, 0.95f, 0.0f, 0.95f}
#define ITEM_COLOR_SUPPLYDROP ImVec4{ 0.9f, 0.1f, 0.1f, 0.9f }

inline auto DrawLine2(const ImVec2& from, const ImVec2& to, float color[4], float thickness) -> void
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->AddLine(from, to, ImGui::GetColorU32(ImVec4(color[0], color[1], color[2], color[3])), thickness);
}

namespace rend {
	BOOLEAN showMenu = FALSE;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* immediateContext = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;

	WNDPROC WndProcOriginal = nullptr;
	HRESULT(*PresentOriginal)(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) = nullptr;
	HRESULT(*ResizeOriginal)(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) = nullptr;

	ImGuiWindow& BeginScene() {
		ImGui_ImplDX11_NewFrame();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
		ImGui::Begin(xorstr("##scene"), nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);

		auto& io = ImGui::GetIO();
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

		return *ImGui::GetCurrentWindow();
	}

	float color_red = 1.;
	float color_green = 0;
	float color_blue = 0;
	float color_random = 0.0;
	float color_speed = -10.0;

	void ColorChange()
	{
			static float Color[3];
			static DWORD Tickcount = 0;
			static DWORD Tickcheck = 0;
			ImGui::ColorConvertRGBtoHSV(color_red, color_green, color_blue, Color[0], Color[1], Color[2]);
			if (GetTickCount() - Tickcount >= 1)
			{
				if (Tickcheck != Tickcount)
				{
					Color[0] += 0.001f * color_speed;
					Tickcheck = Tickcount;
				}
				Tickcount = GetTickCount();
			}
			if (Color[0] < 0.0f) Color[0] += 1.0f;
			ImGui::ColorConvertHSVtoRGB(Color[0], Color[1], Color[2], color_red, color_green, color_blue);	
	}

	ImVec4 GetItemColor(BYTE tier)
	{
		switch (tier)
		{
		case 1:
			return ITEM_COLOR_TIER_WHITE;
		case 2:
			return ITEM_COLOR_TIER_GREEN;
		case 3:
			return ITEM_COLOR_TIER_BLUE;
		case 4:
			return ITEM_COLOR_TIER_PURPLE;
		case 5:
			return ITEM_COLOR_TIER_ORANGE;
		case 6:
		case 7:
			return ITEM_COLOR_TIER_GOLD;
		case 8:
		case 9:
			return ImVec4{ 200 / 255.f, 0 / 255.f, 0 / 255.f, 0.95f };
		case 10:
			return ITEM_COLOR_TIER_UNKNOWN;
		default:
			return ITEM_COLOR_TIER_WHITE;
		}
	}

	VOID EndScene(ImGuiWindow& window)
	{
		//ColorChange();
		window.DrawList->PushClipRectFullScreen();
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		if (Settings.TeleportToEnemies)
		{
			if (hooks::LocalPlayerPawn && hooks::LocalPlayerController)
			{
				if (Util::SpoofCall(GetAsyncKeyState, Settings.EnemyTeleportKey))
				{
					if (InVehicle)
					{
						if (hooks::VehicleTargetPawn != nullptr)
						{
							hooks::Teleport(hooks::ClosestVehicle, ClosestPlayerCoords);
						}
					}
				}
			}
		}

		FVector nigger = { 500 };

		FVector loc = { hooks::LocalplayerPosition };

		if (Settings.TeleportToEnemies) //boat fly
		{
			if (hooks::LocalPlayerController)
			{
				if (InVehicle)
				{
					auto LocPtr = Util::GetPawnRootLocation(hooks::ClosestVehicle);
					auto TpPos = *LocPtr;

					if (Util::SpoofCall(GetAsyncKeyState, 0x57)) { // W
						TpPos.X = TpPos.X + Settings.BoatFlySpeed;
						// move to front
					}
					if (Util::SpoofCall(GetAsyncKeyState, 0x53)) { // S
						TpPos.X = TpPos.X - Settings.BoatFlySpeed;
						// move to back
					}
					if (Util::SpoofCall(GetAsyncKeyState, 0x41)) { // A
						TpPos.Y = TpPos.Y + Settings.BoatFlySpeed;
						// move to left
					}
					if (Util::SpoofCall(GetAsyncKeyState, 0x44)) { // D
						TpPos.Y = TpPos.Y - Settings.BoatFlySpeed;
						// move to right
					}
					//if ((MOUSEEVENTF_LEFTUP)) { // Space
					if (Util::SpoofCall(GetAsyncKeyState, VK_SPACE)) { // Space
						TpPos.Z = TpPos.Z + Settings.BoatFlySpeed;
						// Move up
					}
					hooks::ProcessEvent(hooks::ClosestVehicle, addresses::LaunchCharacter, &TpPos, 0);
				}
			}
		}

		/*if (Settings.PlayerFly)
		{
			if (hooks::LocalPlayerController)
			{
				auto LocPtr = Util::GetPawnRootLocation(hooks::LocalPlayerPawn);
				auto TpPos = *LocPtr;

				if (Util::SpoofCall(GetAsyncKeyState, 0x57)) { // W
					TpPos.X = TpPos.X + Settings.BoatFlySpeed;
					// move to front
				}
				if (Util::SpoofCall(GetAsyncKeyState, 0x53)) { // S
					TpPos.X = TpPos.X - Settings.BoatFlySpeed;
					// move to back
				}
				if (Util::SpoofCall(GetAsyncKeyState, 0x41)) { // A
					TpPos.Y = TpPos.Y + Settings.BoatFlySpeed;
					// move to left
				}
				if (Util::SpoofCall(GetAsyncKeyState, 0x44)) { // D
					TpPos.Y = TpPos.Y - Settings.BoatFlySpeed;
						// move to right
				}
				if (Util::SpoofCall(GetAsyncKeyState, VK_SPACE)) { // Space
					TpPos.Z = TpPos.Z + Settings.BoatFlySpeed;
					// Move up
				}
				hooks::ProcessEvent(hooks::LocalPlayerPawn, addresses::K2_SetRelativeLocation, &TpPos, 0);
			}
		}*/

		if (showMenu)
		{
			Menu::DrawMenu();
		}
		ImGui::Render();
	}

	VOID AddLine(ImGuiWindow& window, float width, float height, FVector a, FVector b, ImU32 color, float& minX, float& maxX, float& minY, float& maxY) {
		FVector a_tmp = Util::WorldToScreen(width, height, a);
		FVector b_tmp = Util::WorldToScreen(width, height, b);
		window.DrawList->AddLine(ImVec2(a_tmp.X, a_tmp.Y), ImVec2(b_tmp.X, b_tmp.Y), color, 1.0f);

		minX = min(a_tmp.X, minX);
		minX = min(b_tmp.X, minX);

		maxX = max(a_tmp.X, maxX);
		maxX = max(b_tmp.X, maxX);

		minY = min(a_tmp.Y, minY);
		minY = min(b_tmp.Y, minY);

		maxY = max(a_tmp.Y, maxY);
		maxY = max(b_tmp.Y, maxY);
	}
	float player_esp_color[4] = { 255, 255, 255, 255 };
	VOID AddMarker(ImGuiWindow& window, float width, float height, float* start, PVOID pawn, LPCSTR text, ImU32 color) {
		auto root = Util::GetPawnRootLocation(pawn);
		if (root) {
			auto pos = *root;
			float dx = start[0] - pos.X;
			float dy = start[1] - pos.Y;
			float dz = start[2] - pos.Z;

			pos = Util::WorldToScreen(width, height, pos);
			float dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

			if (dist < 250)
			{
				CHAR modified[0xFF] = { 0 };
				snprintf(modified, sizeof(modified), xorstr("%s [%dm]"), text, static_cast<INT>(dist));

				auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, modified);
				window.DrawList->AddText(ImVec2(pos.X - size.x / 2.0f, pos.Y - size.y / 2.0f), ImGui::GetColorU32(color), modified);
			}
		}
	}


	float DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		std::stringstream stream(text);
		std::string line;

		float y = 0.0f;
		int i = 0;

		while (std::getline(stream, line))
		{
			ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

			if (center)
			{
				window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				//window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				//window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

				window->DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
			}
			else
			{
				window->DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				//window->DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				//window->DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

				window->DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
			}

			y = pos.y + textSize.y * (i + 1);
			i++;
		}
		return y;
	}
	float DrawNormalText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		std::stringstream stream(text);
		std::string line;

		float y = 0.0f;
		int i = 0;

		while (std::getline(stream, line))
		{
			ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

			if (center)
			{
				window->DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
			}
			else
			{
				window->DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
			}

			y = pos.y + textSize.y * (i + 1);
			i++;
		}
		return y;
	}


	std::string TextFormat(const char* format, ...)
	{
		va_list argptr;
		va_start(argptr, format);

		char buffer[2048];
		vsprintf(buffer, format, argptr);

		va_end(argptr);

		return buffer;
	}
	std::string string_To_UTF8(const std::string& str)
	{
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		ZeroMemory(pwBuf, nwLen * 2 + 2);

		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

		char* pBuf = new char[nLen + 1];
		ZeroMemory(pBuf, nLen + 1);

		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		std::string retStr(pBuf);

		delete[]pwBuf;
		delete[]pBuf;

		pwBuf = NULL;
		pBuf = NULL;

		return retStr;

	}

	void DrawNewText(int x, int y, int R, int G, int B, int A, const char* str)
	{
		ImFont a;
		std::string utf_8_1 = std::string(str);
		std::string utf_8_2 = string_To_UTF8(utf_8_1);
		//ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(R / 255.0, G / 255.0, B / 255.0, A / 255.0)), utf_8_2.c_str());
	}

	static void Text(const ImVec2& pos, ImColor col, bool outline, const char* fmt, ...)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window)
		{
			va_list args1;
			va_start(args1, fmt);
			va_list args2;
			va_copy(args2, args1);
			std::vector<char> buf(1 + std::vsnprintf(nullptr, 0, fmt, args1));
			va_end(args1);
			std::vsnprintf(buf.data(), buf.size(), fmt, args2);
			va_end(args2);
			if (outline)
			{
				window->DrawList->AddText(ImVec2(pos.x + 1, pos.y + 1), ImColor(0, 0, 0, 255), buf.data());
				window->DrawList->AddText(ImVec2(pos.x - 1, pos.y - 1), ImColor(0, 0, 0, 255), buf.data());
			}
			window->DrawList->AddText(pos, col, buf.data());
		}
	}

	void DrawStrokeText(int x, int y, const ImVec4& color, const char* str)
	{
		ImFont a;
		std::string utf_8_1 = std::string(str);
		std::string utf_8_2 = string_To_UTF8(utf_8_1);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x - 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::GetColorU32(color), utf_8_2.c_str());
	}
	void DrawRoundedRect(int x, int y, int w, int h, ImU32& color, int thickness)
	{
		//ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(w, h), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0, 0, 3);
		ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(w, h), ImGui::GetColorU32(color), 0, 0, thickness);
	}
	void DrawLine(int x1, int y1, int x2, int y2, const ImVec4& color, int thickness)
	{
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::GetColorU32(color), thickness);
	}
	void DrawCorneredBox(int X, int Y, int W, int H, const ImU32& color, int thickness) {

		float lineW = (W / 4.5);
		float lineH = (H / 3);

		//black outlines
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);

		//corners
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	}

	__declspec(dllexport) LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
	{
		if (msg == WM_KEYUP && (wParam == VK_PRIOR))
		{
			if (Settings.ESP.MinWeaponTier != 5)
				Settings.ESP.MinWeaponTier += 1;
		}

		if (msg == WM_KEYUP && (wParam == VK_NEXT))
		{
			if (Settings.ESP.MinWeaponTier != 1)
				Settings.ESP.MinWeaponTier -= 1;
		}

		if (msg == WM_KEYUP && (wParam == VK_INSERT)) 
		{
			showMenu = !showMenu;
		}

		if (msg == WM_KEYUP && (wParam == VK_F1))
		{
			Settings.ESP.Weapons = !Settings.ESP.Weapons;
		}

		if (msg == WM_KEYUP && (wParam == VK_F2))
		{
			Settings.ESP.Containers = !Settings.ESP.Containers;
		}

		if (msg == WM_KEYUP && (wParam == VK_F3))
		{
			Settings.TeleportToEnemies = !Settings.TeleportToEnemies;
		}

		if (msg == WM_KEYUP && (wParam == VK_F4))
		{
			Settings.RapidFire = !Settings.RapidFire;
		}

		if (msg == WM_KEYUP && (wParam == VK_F5))
		{
			Settings.Spinbot = !Settings.Spinbot;
		}

		if (msg == WM_KEYUP && (wParam == VK_F6))
		{
			Settings.NoClip = !Settings.NoClip;
		}

		if (msg == WM_KEYUP && (wParam == VK_F7))
		{
			Settings.VisibleCheck = !Settings.VisibleCheck;
		}

		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam) && showMenu == true)
		{
			return true;
		}

		return CallWindowProc(WndProcOriginal, hWnd, msg, wParam, lParam);
	}

	using f_present = HRESULT(__stdcall*)(IDXGISwapChain * pthis, UINT sync_interval, UINT flags);
	f_present o_present = nullptr;
	HRESULT __stdcall hk_present(IDXGISwapChain* pSwapChain, UINT sync_interval, UINT flags)
	{
		static HWND hWnd = 0;

		if (!device) {
			pSwapChain->GetDevice(__uuidof(device), reinterpret_cast<PVOID*>(&device));
			device->GetImmediateContext(&immediateContext);

			ID3D11Texture2D* renderTarget = nullptr;
			pSwapChain->GetBuffer(0, __uuidof(renderTarget), reinterpret_cast<PVOID*>(&renderTarget));
			device->CreateRenderTargetView(renderTarget, nullptr, &renderTargetView);
			renderTarget->Release();

			ID3D11Texture2D* backBuffer = 0;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (PVOID*)&backBuffer);
			D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
			backBuffer->GetDesc(&backBufferDesc);

			hWnd = FindWindow(xorstr(L"UnrealWindow"), xorstr(L"Fortnite  "));
			if (!width) {
				WndProcOriginal = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));
			}

			width = (float)backBufferDesc.Width;
			height = (float)backBufferDesc.Height;
			backBuffer->Release();

			m_pFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(xorstr("C:\\Windows\\Fonts\\Arial.ttf"), 16.0f);
			DefaultFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(xorstr("C:\\Windows\\Fonts\\Arial.ttf"), 16.0f);

			SettingsHelper::LoadTheme();

			ImGui_ImplDX11_Init(hWnd, device, immediateContext);
			ImGui_ImplDX11_CreateDeviceObjects();
		}
		immediateContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
		auto& window = BeginScene();

		auto success = FALSE;
		//auto FovColor = ImGui::GetColorU32({ 0, 0, 0, 1.f });

		auto FovColor = ImGui::GetColorU32({ 255, 255, 255, 255 });

		auto FovColor1 = ImGui::GetColorU32({ 0, 0, 0, 1.f });

		X = (float)Util::SpoofCall(GetSystemMetrics, SM_CXSCREEN);
		Y = (float)Util::SpoofCall(GetSystemMetrics, SM_CYSCREEN);


		if (!Settings.AutoAimbot && Settings.ESP.AimbotFOV) {
			auto raim = ImGui::GetColorU32({ color_red, color_green, color_blue, 255 });

			window.DrawList->AddCircle(ImVec2(width / 2, height / 2), Settings.AimbotFOV, raim, 128); //128
		}
		static float ScreenCenterX = 960.0f;
		static float ScreenCenterY = 540.0f;

		static float col_crosshair[4] = { 0, 0, 0, 1.0f }; // 1.000f, 0.123f, 0.000f, 1.000f 


		if (Settings.Crosshair)
		{
			window.DrawList->AddLine(ImVec2(ScreenCenterX - 12, ScreenCenterY), ImVec2((ScreenCenterX - 12) + (12 * 2), ScreenCenterY), ImGui::GetColorU32({ 255, 255, 255, 0.65f }), 1.9);
			window.DrawList->AddLine(ImVec2(ScreenCenterX, ScreenCenterY - 12), ImVec2(ScreenCenterX, (ScreenCenterY - 12) + (12 * 2)), ImGui::GetColorU32({ 255, 255, 255, 0.65f }), 1.9);
		}


		//if (Settings.ESP.Weapons) 
		//{
			ImU32 LootTextScreenColor = ImGui::GetColorU32({ 101.f / 255.f, 103.f / 255.f, 105.f / 255.f, 1 });
			std::string TextLoot = xorstr("Minimum loot tier (Page Up/Down): ");
			switch (Settings.ESP.MinWeaponTier)
			{
			case 1: // Common - Grey
				LootTextScreenColor = ImGui::GetColorU32({ 168.f / 255.f, 168.f / 255.f, 168.f / 255.f, 1 });
				TextLoot += xorstr("Common");
				break;
			case 2: // Uncommon - Green
				LootTextScreenColor = ImGui::GetColorU32({ 7.f / 255.f, 143.f / 255.f, 4.f / 255.f, 1 });
				TextLoot += xorstr("Uncommon");
				break;
			case 3: // Rare - Blue
				LootTextScreenColor = ImGui::GetColorU32({ 8.f / 255.f, 1.f / 255.f, 255.f / 255.f, 1 });
				TextLoot += xorstr("Rare");
				break;
			case 4: // Epic - Purple
				LootTextScreenColor = ImGui::GetColorU32({ 112.f / 255.f, 7.f / 255.f, 135.f / 255.f, 1 });
				TextLoot += xorstr("Epic");
				break;
			case 5: // Legendary - Gold
				LootTextScreenColor = ImGui::GetColorU32({ 255.f / 255.f, 200.f / 255.f, 0.f / 255.f, 1 });
				TextLoot += xorstr("Legendary");
				break;
			case 6: // Mythic - Dark Gold
				LootTextScreenColor = ImGui::GetColorU32({ 255.f / 255.f, 150.f / 255.f, 0.f / 255.f, 1 });
				TextLoot += xorstr("Mythic");
				break;
			}
			float PositionXLootText = 40;
			float PositionYLootText = 350;
			Text(ImVec2(PositionXLootText, PositionYLootText), ImGui::GetColorU32(LootTextScreenColor), Settings.OutlineText, TextLoot.c_str());
			//DrawOutlinedText(m_pFont, TextLoot.c_str(), ImVec2(PositionXLootText, PositionYLootText), 14.f, ImGui::GetColorU32(LootTextScreenColor), true);
		//}
			 // I FUCKING HAVE 3 / 4 LOOPS /SKULL

		uint64_t BaseAddress = (uint64_t)GetModuleHandleA(NULL);

			do {
				float closestDistance = FLT_MAX;
				PVOID closestPawn = NULL;

				float closestDistanceForVehicle = FLT_MAX;
				PVOID closestPawnForVehicle = NULL;

				float closestVehicle = FLT_MAX;
				PVOID closestPawnVehicle = NULL;

				//auto world = ReadPointer(BaseAddress, offsets::Main::UWorld);
				//if (!world) break;
				auto world = ReadPointer(Util::UWorld_Address, 0);
				if (!world) break;

				auto gameInstance = ReadPointer(world, offsets::World::OwningGameInstance);
				if (!gameInstance) break;

				auto localPlayers = ReadPointer(gameInstance, offsets::GameInstance::LocalPlayers);
				if (!localPlayers) break;

				auto localPlayer = ReadPointer(localPlayers, 0);
				if (!localPlayer) break;

				auto localPlayerController = ReadPointer(localPlayer, offsets::Player::PlayerController);
				if (!localPlayerController) break;

				auto localPlayerPawn = reinterpret_cast<UObject*>(ReadPointer(localPlayerController, offsets::PlayerController::AcknowledgedPawn));
				//if (!localPlayerPawn) break;
				// cuz loot esp doesnt works anymore

				PVOID localPlayerWeapon = 0;
				PVOID localPlayerRoot = 0;
				PVOID localPlayerState = 0;
				PVOID localteamindex = 0;
				DWORD localPlayerTeamIndex = 0;
				float* localPlayerLocation;
				float* localplayerRotation;

				const wchar_t* isProjectileWeapon;

				if (localPlayerPawn)
				{
					localPlayerWeapon = ReadPointer(localPlayerPawn, offsets::FortPawn::CurrentWeapon);
					if (!localPlayerWeapon) break;

					localPlayerRoot = ReadPointer(localPlayerPawn, offsets::Actor::RootComponent);
					if (!localPlayerRoot) break;

					localPlayerState = ReadPointer(localPlayerPawn, offsets::Pawn::PlayerState);
					if (!localPlayerState) break;

					localteamindex = ReadPointer(localPlayerState, offsets::FortPlayerStateAthena::TeamIndex);

					localPlayerLocation = reinterpret_cast<float*>(reinterpret_cast<PBYTE>(localPlayerRoot) + offsets::SceneComponent::RelativeLocation);

					localplayerRotation = reinterpret_cast<float*>(reinterpret_cast<PBYTE>(localPlayerRoot) + 0x128); //RelativeRotation

					localPlayerTeamIndex = ReadDWORD(localPlayerState, offsets::FortPlayerStateAthena::TeamIndex);
				
					hooks::ProcessEvent(localPlayerPawn, addresses::IsInVehicle, &InVehicle, 0);

					auto weaponName = Util::GetObjectFirstName((uintptr_t)(uintptr_t)localPlayerWeapon);
					isProjectileWeapon = wcsstr(weaponName.c_str(), xorstr(L"Rifle_Sniper"));
					auto isRocketLauncher = wcsstr(weaponName.c_str(), xorstr(L"RocketLauncher"));
					auto isGrenadeLauncher = wcsstr(weaponName.c_str(), xorstr(L"GrenadeLauncher"));
					auto isCrossbow = wcsstr(weaponName.c_str(), xorstr(L"Crossbow"));
					auto isBoomBow = wcsstr(weaponName.c_str(), xorstr(L"ExplosiveBow"));

					if (isProjectileWeapon || isRocketLauncher || isGrenadeLauncher || isCrossbow || isBoomBow)
					{

						if (Settings.BulletTP)
						{
							Settings.IsBulletTeleporting = true;
						}
						else
						{
							Settings.Prediction = true;
						}
					}
					else
					{
						if (Settings.BulletTP)
						{
							Settings.IsBulletTeleporting = false;
						}
						else
						{
							Settings.Prediction = false;
						}
					}

				}


			
				hooks::IsLocalPlayerInVehicle = hooks::IsLocalPlayerInVehicle;
				hooks::LocalPlayerPawn = localPlayerPawn;
				hooks::LocalPlayerController = localPlayerController;
				hooks::LocalPlayerCurrentWeapon = localPlayerWeapon;
				hooks::LocalRootComp = localPlayerRoot;
				hooks::LocalplayerPosition.X = localPlayerLocation[0];
				hooks::LocalplayerPosition.Y = localPlayerLocation[1];
				hooks::LocalplayerPosition.Z = localPlayerLocation[2];

				
				hooks::ClosestTargetCoord.X = ClosestPlayerCoords.X;
				hooks::ClosestTargetCoord.Y = ClosestPlayerCoords.Y;
				hooks::ClosestTargetCoord.Z = ClosestPlayerCoords.Z;


				hooks::IsSniper = isProjectileWeapon;
				std::vector<PVOID> playerPawns;

				 
				// ok


				auto Ulevel = ReadPointer(world, 0x30);    if (!Ulevel) break;
				auto AActors = ReadPointer(Ulevel, 0x98); if (!AActors) break;
				auto ActorCount = ReadInt(Ulevel, 0xA0); if (!ActorCount) break; // actorcount offset is correct?? idk  5 months ago nigger


				for (int i = 0; i < ActorCount; i++) {

					auto pawn = reinterpret_cast<UObject*>(ReadPointer(AActors, i * sizeof(PVOID)));
					if (!pawn) continue;

					auto name = Util::GetObjectFirstName((uintptr_t)pawn);

					if (localPlayerPawn and pawn == localPlayerPawn) continue; // 

					if (wcsstr(name.c_str(), xorstr(L"PlayerPawn_Athena_C")) || wcsstr(name.c_str(), xorstr(L"BP_GasketPlayerPawn_Tomato_C")) || wcsstr(name.c_str(), xorstr(L"PlayerPawn_Athena_Phoebe_C")) || wcsstr(name.c_str(), xorstr(L"BP_MangPlayerPawn")))
					{
						playerPawns.push_back(pawn);
					}
					if (localPlayerPawn)
					{
						if (Settings.ESP.debug)
						{
							//CHAR Obj[0xFF] = { 0 };
							//wcstombs(Obj, name.c_str(), sizeof(Obj));

							//auto ObjectRoot = Util::GetPawnRootLocation(pawn);
							//if (ObjectRoot)
							//{
							//	auto ObjectPosition = *ObjectRoot;

							//	ObjectPosition = Util::WorldToScreen(width, height, ObjectPosition);

							//	auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, Obj);
							//	Text(ImVec2((ObjectPosition.X + ObjectPosition.X) / 2 - size.x / 2, ObjectPosition.Y + 4), ImGui::GetColorU32((Colours::Red)), true, Obj);
							//	//DrawOutlinedText(m_pFont, TextFormat(xorstr("%s"), Obj), ImVec2(ObjectPosition.X, ObjectPosition.Y), 14.5f, ImGui::GetColorU32({ 255, 255, 255,255 }), true);

							//}
							//	AddMarker(window, width, height, localPlayerLocation, pawn, Obj, ImGui::GetColorU32({ 255, 255, 255,255 }));
						}

						if (InVehicle)
						{
							if (wcsstr(name.c_str(), xorstr(L"MeatballVehicle_L")) || wcsstr(name.c_str(), xorstr(L"JackalVehicle_Athena_C")) || wcsstr(name.c_str(), xorstr(L"Valet_BasicTruck_Vehicle_C")) || wcsstr(name.c_str(), xorstr(L"Valet_SportsCar_Vehicle_C")) || wcsstr(name.c_str(), xorstr(L"Valet_BigRig_Vehicle_C")) || wcsstr(name.c_str(), xorstr(L"Valet_TaxiCab_Vehicle_C")) || wcsstr(name.c_str(), xorstr(L"Valet_BasicCar_Vehicle_C")) || wcsstr(name.c_str(), xorstr(L"FerretVehicle_C")) || wcsstr(name.c_str(), xorstr(L"AntelopeVehicle_C")) || /*wcsstr(name.c_str(), (L"GolfCartVehicleSK_C")) ||*/ wcsstr(name.c_str(), xorstr(L"TestMechVehicle_C")) || wcsstr(name.c_str(), xorstr(L"OctopusVehicle_C")) || wcsstr(name.c_str(), xorstr(L"ShoppingCartVehicleSK_C")) || wcsstr(name.c_str(), xorstr(L"HoagieVehicle_C")))
							{

								auto VehRoot = Util::GetPawnRootLocation(pawn);
								if (VehRoot)
								{
									auto VehiclePos = *VehRoot;

									VehiclePos = Util::WorldToScreen(width, height, VehiclePos);

									auto ttdx = VehiclePos.X - (width / 2);
									auto ttdy = VehiclePos.Y - (height / 2);
									auto ttdist = Util::SpoofCall(sqrtf, ttdx * ttdx + ttdy * ttdy);
									if (ttdist < INT_MAX && ttdist < closestVehicle) {
										closestVehicle = ttdist;
										closestPawnVehicle = pawn;
									}

								}
							}
						}

						else if (wcsstr(name.c_str(), xorstr(L"FortPickupAthena")))
						{
							auto item = ReadPointer(pawn, offsets::FortPickup::PrimaryPickupItemEntry + offsets::FortItemEntry::ItemDefinition);
							if (!item) continue;

							auto itemName = reinterpret_cast<FText*>(ReadPointer(item, offsets::FortItemDefinition::DisplayName));
							if (!itemName || !itemName->c_str()) continue;

							ImU32 ItemColor;
							auto itemTier = ReadBYTE(item, offsets::FortItemDefinition::Tier);
							auto isAmmo = wcsstr(itemName->c_str(), xorstr(L"Ammo: "));
							if ((!Settings.ESP.Ammo && isAmmo) || ((!Settings.ESP.Weapons || itemTier < Settings.ESP.MinWeaponTier) && !isAmmo)) continue;

							CHAR text[0xFF] = { 0 };
							wcstombs(text, itemName->c_str() + (isAmmo ? 6 : 0), sizeof(text));
							if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 0)
							{
								ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
							}
							else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 1)
							{
								ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
							}
							else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 2)
							{
								ItemColor = ImGui::GetColorU32({ 0.0f, 0.95f, 0.0f, 0.95f });
							}
							else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 3)
							{
								ItemColor = ImGui::GetColorU32({ 0.4f, 0.65f, 1.0f, 0.95f });
							}
							else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 4)
							{
								ItemColor = ImGui::GetColorU32({ 0.7f, 0.25f, 0.85f, 0.95f });
							}
							else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 5)
							{
								ItemColor = ImGui::GetColorU32({ 0.85f, 0.65f, 0.0f, 0.95f });
							}
							else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 6)
							{
								ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
							}
							else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 7)
							{
								ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
							}
							else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 8)
							{
								ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
							}
							else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 9)
							{
								ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
							}
							else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 10)
							{
								ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
							}
							auto ItemRoot = Util::GetPawnRootLocation(pawn);
							if (ItemRoot) {
								auto ItemPos = *ItemRoot;
								float dx = localPlayerLocation[0] - ItemPos.X;
								float dy = localPlayerLocation[1] - ItemPos.Y;
								float dz = localPlayerLocation[2] - ItemPos.Z;

								ItemPos = Util::WorldToScreen(width, height, ItemPos);
								float dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;


								std::string DistanceTo = std::to_string((int)dist); DistanceTo += std::string(xorstr(" m"));
								std::string FullLootESP;

								FullLootESP = std::string(DistanceTo + xorstr(" [ ") + text + xorstr(" ]"));

								if (dist < Settings.LootDist)
								{
									//auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, FullLootESP.c_str());
									//Text(ImVec2(ItemPos.X - size.x / 2, ItemPos.Y - size.y / 2), ImGui::GetColorU32(GetItemColor(itemTier)), Settings.OutlineText, FullLootESP.c_str());
									//DrawOutlinedText(m_pFont, text, ImVec2(ItemPos.X, ItemPos.Y), 15.f, ImGui::GetColorU32(GetItemColor(itemTier)), true);
									DrawOutlinedText(m_pFont, FullLootESP.c_str(), ImVec2(ItemPos.X, ItemPos.Y), 14.f, ImGui::GetColorU32(GetItemColor(itemTier)), true);
								}
							}
						}

						/*	else if (wcsstr(name.c_str(), xorstr(L"FortPickupAthena")))
							{
								auto item = ReadPointer(pawn, offsets::FortPickup::PrimaryPickupItemEntry + offsets::FortItemEntry::ItemDefinition);
								if (!item) continue;

								auto itemName = reinterpret_cast<FText*>(ReadPointer(item, offsets::FortItemDefinition::DisplayName));
								if (!itemName || !itemName->c_str()) continue;

								ImU32 ItemColor;
								auto isAmmo = wcsstr(itemName->c_str(), xorstr(L"Ammo: "));
								if ((!Settings.ESP.Ammo && isAmmo) || ((!Settings.ESP.Weapons) && !isAmmo)) continue;

								CHAR text[0xFF] = { 0 };
								wcstombs(text, itemName->c_str() + (isAmmo ? 6 : 0), sizeof(text));
								if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 0)
								{
									ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
								}
								else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 1)
								{
									ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
								}
								else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 2)
								{
									ItemColor = ImGui::GetColorU32({ 0.0f, 0.95f, 0.0f, 0.95f });
								}
								else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 3)
								{
									ItemColor = ImGui::GetColorU32({ 0.4f, 0.65f, 1.0f, 0.95f });
								}
								else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 4)
								{
									ItemColor = ImGui::GetColorU32({ 0.7f, 0.25f, 0.85f, 0.95f });
								}
								else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 5)
								{
									ItemColor = ImGui::GetColorU32({ 0.85f, 0.65f, 0.0f, 0.95f });
								}
								else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 6)
								{
									ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
								}
								else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 7)
								{
									ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
								}
								else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 8)
								{
									ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
								}
								else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 9)
								{
									ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
								}
								else if (ReadBYTE(item, offsets::FortItemDefinition::Tier) == 10)
								{
									ItemColor = ImGui::GetColorU32({ 255, 255, 255, 255 });
								}
								auto ItemRoot = Util::GetPawnRootLocation(pawn);
								if (ItemRoot) {
									auto ItemPos = *ItemRoot;
									float dx = localPlayerLocation[0] - ItemPos.X;
									float dy = localPlayerLocation[1] - ItemPos.Y;
									float dz = localPlayerLocation[2] - ItemPos.Z;

									if (Util::WorldToScreen(width, height, &ItemPos.X)) {
										float dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

										if (dist < 120)
										{
											DrawOutlinedText(m_pFont, TextFormat(xorstr("%s [%.0f m]"), text, dist), ImVec2(ItemPos.X, ItemPos.Y), 14.5f, ItemColor, true);
										}
									}
								}
							}*/

						else if (Settings.ESP.Containers && wcsstr(name.c_str(), xorstr(L"Tiered_Chest")) && !((ReadBYTE(pawn, offsets::BuildingContainer::bAlreadySearched) >> 7) & 1)) {
							auto ChestRoot = Util::GetPawnRootLocation(pawn);
							if (ChestRoot) {
								auto ChestPos = *ChestRoot;
								float dx = localPlayerLocation[0] - ChestPos.X;
								float dy = localPlayerLocation[1] - ChestPos.Y;
								float dz = localPlayerLocation[2] - ChestPos.Z;

								ChestPos = Util::WorldToScreen(width, height, ChestPos);

								float dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

								std::string DisplayName = xorstr("Chest");
								std::string DistanceTo = std::to_string((int)dist); DistanceTo += std::string(xorstr(" m"));
								std::string FullChestESP;

								//FullChestESP = std::string(DisplayName.begin(), DisplayName.end()) + xorstr("[") + DistanceTo + xorstr("]");
								FullChestESP = std::string(DistanceTo + xorstr(" [ ") + DisplayName + xorstr(" ]"));

								if (dist < Settings.ChestDist)
								{
									//auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, FullChestESP.c_str());
									//Text(ImVec2(ChestPos.X - size.x / 2, ChestPos.Y - size.y / 2), ImGui::GetColorU32({ 255.f / 255.f, 255.f / 255.f, 0.f / 255.f, 1 }), Settings.OutlineText, FullChestESP.c_str());
									DrawOutlinedText(m_pFont, FullChestESP.c_str(), ImVec2(ChestPos.X, ChestPos.Y), 14.f, ImGui::GetColorU32({ 255.f / 255.f, 255.f / 255.f, 0.f / 255.f, 1 }), true);
								}

							}
						}
						else if (Settings.ESP.LLamas && wcsstr(name.c_str(), xorstr(L"AthenaSupplyDrop_Llama"))) {

							auto LLamaRoot = Util::GetPawnRootLocation(pawn);
							if (LLamaRoot) {
								auto LLamaPos = *LLamaRoot;
								float dx = localPlayerLocation[0] - LLamaPos.X;
								float dy = localPlayerLocation[1] - LLamaPos.Y;
								float dz = localPlayerLocation[2] - LLamaPos.Z;

								LLamaPos = Util::WorldToScreen(width, height, LLamaPos);
								float dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

								std::string DisplayName = xorstr("Llama");
								std::string DistanceTo = std::to_string((int)dist); DistanceTo += std::string(xorstr(" m"));
								std::string FullLLamaESP;

								//FullLLamaESP = std::string(DisplayName.begin(), DisplayName.end()) + xorstr(" [") + DistanceTo + xorstr("]");
								FullLLamaESP = std::string(DistanceTo + xorstr(" [ ") + DisplayName + xorstr(" ]"));

								if (dist < 1500)
								{
									//auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, FullLLamaESP.c_str());
									//Text(ImVec2(LLamaPos.X - size.x / 2, LLamaPos.Y - size.y / 2), ImGui::GetColorU32({ 179.f / 255.f, 0.f / 255.f, 255.f / 255.f, 1 }), Settings.OutlineText, FullLLamaESP.c_str());
									//DrawOutlinedText(m_pFont, TextFormat(xorstr("Llama! [%.0f m]"), dist), ImVec2(LLamaPos.X, LLamaPos.Y), 14.5f, ImGui::GetColorU32({ 179.f / 255.f, 0.f / 255.f, 255.f / 255.f, 1 }), true);
									DrawOutlinedText(m_pFont, FullLLamaESP.c_str(), ImVec2(LLamaPos.X, LLamaPos.Y), 14.f, ImGui::GetColorU32({ 179.f / 255.f, 0.f / 255.f, 255.f / 255.f, 1 }), true);
								}

							}
						}
						else if (Settings.ESP.AmmoBox && wcsstr(name.c_str(), xorstr(L"Tiered_Ammo")) && !((ReadBYTE(pawn, offsets::BuildingContainer::bAlreadySearched) >> 7) & 1)) {
							auto AmmoBoxRoot = Util::GetPawnRootLocation(pawn);
							if (AmmoBoxRoot) {
								auto AmmoBoxPos = *AmmoBoxRoot;
								float dx = localPlayerLocation[0] - AmmoBoxPos.X;
								float dy = localPlayerLocation[1] - AmmoBoxPos.Y;
								float dz = localPlayerLocation[2] - AmmoBoxPos.Z;

								AmmoBoxPos = Util::WorldToScreen(width, height, AmmoBoxPos);

								float dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

								std::string DisplayName = xorstr("Ammo Box");
								std::string DistanceTo = std::to_string((int)dist); DistanceTo += std::string(xorstr(" m"));
								std::string FullAmmoBoxESP;

								//FullAmmoBoxESP = std::string(DisplayName.begin(), DisplayName.end()) + xorstr(" [") + DistanceTo + xorstr("]");
								FullAmmoBoxESP = std::string(DistanceTo + xorstr(" [ ") + DisplayName + xorstr(" ]"));

								if (dist < 100)
								{
									//auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, FullAmmoBoxESP.c_str());
									//Text(ImVec2(AmmoBoxPos.X - size.x / 2, AmmoBoxPos.Y - size.y / 2), ImGui::GetColorU32({ 0.75f, 0.75f, 0.75f, 1.0f }), Settings.OutlineText, FullAmmoBoxESP.c_str());
									DrawOutlinedText(m_pFont, FullAmmoBoxESP.c_str(), ImVec2(AmmoBoxPos.X, AmmoBoxPos.Y), 14.f, ImGui::GetColorU32({ 0.75f, 0.75f, 0.75f, 1.0f }), true);
								}
							}
						}

						else if (Settings.ESP.SupplyDrops && wcsstr(name.c_str(), xorstr(L"AthenaSupplyDrop_C")) && !((ReadBYTE(pawn, offsets::BuildingContainer::bAlreadySearched) >> 7) & 1)) {
							auto SupplyDropRoot = Util::GetPawnRootLocation(pawn);
							if (SupplyDropRoot) {
								auto SupplyDropPos = *SupplyDropRoot;
								float dx = localPlayerLocation[0] - SupplyDropPos.X;
								float dy = localPlayerLocation[1] - SupplyDropPos.Y;
								float dz = localPlayerLocation[2] - SupplyDropPos.Z;

								SupplyDropPos = Util::WorldToScreen(width, height, SupplyDropPos);
								float dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

								std::string DisplayName = xorstr("Supply Drop");
								std::string DistanceTo = std::to_string((int)dist); DistanceTo += std::string(xorstr(" m"));
								std::string FullDropESP;

								//FullDropESP = std::string(DisplayName.begin(), DisplayName.end()) + xorstr(" [") + DistanceTo + xorstr("]");
								FullDropESP = std::string(DistanceTo + xorstr(" [ ") + DisplayName + xorstr(" ]"));

								if (dist < 1200)
								{
									//auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, FullDropESP.c_str());
									//Text(ImVec2(SupplyDropPos.X - size.x / 2, SupplyDropPos.Y - size.y / 2), ImGui::GetColorU32({ 0.f / 255.f, 26.f / 255.f, 255.f / 255.f, 1 }), Settings.OutlineText, FullDropESP.c_str());
									DrawOutlinedText(m_pFont, FullDropESP.c_str(), ImVec2(SupplyDropPos.X, SupplyDropPos.Y), 14.f, ImGui::GetColorU32({ 0.f / 255.f, 26.f / 255.f, 255.f / 255.f, 1 }), true);
								}
							}
						}
						//else if (Settings.ESP.Vehicles && wcsstr(name.c_str(), (L"JackalVehicle_Athena_C")) || wcsstr(name.c_str(), (L"MeatballVehicle_L")) || wcsstr(name.c_str(), (L"Valet_SportsCar_Vehicle_C")) || wcsstr(name.c_str(), (L"Valet_BasicTruck_Vehicle_C")) || wcsstr(name.c_str(), (L"Valet_BigRig_Vehicle_C")) || wcsstr(name.c_str(), (L"Valet_TaxiCab_Vehicle_C")) || wcsstr(name.c_str(), (L"Valet_BasicCar_Vehicle_C")) || wcsstr(name.c_str(), (L"TestMechVehicle_C")) || wcsstr(name.c_str(), (L"OctopusVehicle_C")) || wcsstr(name.c_str(), (L"ShoppingCartVehicleSK_C")) || wcsstr(name.c_str(), (L"HoagieVehicle_C")))
						//{
						//	auto VehicleRoot = Util::GetPawnRootLocation(pawn);
						//	if (VehicleRoot) {
						//		auto VehiclePos = *VehicleRoot;
						//		float dx = localPlayerLocation[0] - VehiclePos.X;
						//		float dy = localPlayerLocation[1] - VehiclePos.Y;
						//		float dz = localPlayerLocation[2] - VehiclePos.Z;


						//		VehiclePos = Util::WorldToScreen(width, height, VehiclePos);

						//		int dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

						//		std::string DisplayName = xorstr("VEHICLE");
						//		std::string DistanceTo = std::to_string((int)dist); DistanceTo += std::string(xorstr(" m"));
						//		std::string FullBoatESP;

						//		FullBoatESP = std::string(DistanceTo + xorstr(" [ ") + DisplayName + xorstr(" ]"));

						//		if (dist < 250)
						//		{

						//			//auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, FullBoatESP.c_str());
						//			//Text(ImVec2(VehiclePos.X - size.x / 2, VehiclePos.Y - size.y / 2), ImGui::GetColorU32({ Colours::Red }), Settings.OutlineText, FullBoatESP.c_str());
						//			DrawOutlinedText(m_pFont, FullBoatESP.c_str(), ImVec2(VehiclePos.X, VehiclePos.Y), 14.f, ImGui::GetColorU32({ Colours::Red }), true);
						//		}
						//	}



						else if (Settings.ESP.Vehicles && wcsstr(name.c_str(), (L"ShoppingCartVehicleSK_C")))
							{
							auto VehicleRoot = Util::GetPawnRootLocation(pawn);
							if (VehicleRoot) {
								auto VehiclePos = *VehicleRoot;
								float dx = localPlayerLocation[0] - VehiclePos.X;
								float dy = localPlayerLocation[1] - VehiclePos.Y;
								float dz = localPlayerLocation[2] - VehiclePos.Z;


								VehiclePos = Util::WorldToScreen(width, height, VehiclePos);

								int dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

								std::string DisplayName = xorstr("ShoppingCart");
								std::string DistanceTo = std::to_string((int)dist); DistanceTo += std::string(xorstr(" m"));
								std::string FullBoatESP;

								FullBoatESP = std::string(DistanceTo + xorstr(" [ ") + DisplayName + xorstr(" ]"));

								if (dist < 250)
								{

									//auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, FullBoatESP.c_str());
									//Text(ImVec2(VehiclePos.X - size.x / 2, VehiclePos.Y - size.y / 2), ImGui::GetColorU32({ Colours::Red }), Settings.OutlineText, FullBoatESP.c_str());
									DrawOutlinedText(m_pFont, FullBoatESP.c_str(), ImVec2(VehiclePos.X, VehiclePos.Y), 14.f, ImGui::GetColorU32({ Colours::Red }), true);
								}
							}
							}




							else if (Settings.ESP.Vehicles && wcsstr(name.c_str(), (L"Valet_BasicCar_Vehicle_C")))
							{
								auto VehicleRoot = Util::GetPawnRootLocation(pawn);
								if (VehicleRoot) {
									auto VehiclePos = *VehicleRoot;
									float dx = localPlayerLocation[0] - VehiclePos.X;
									float dy = localPlayerLocation[1] - VehiclePos.Y;
									float dz = localPlayerLocation[2] - VehiclePos.Z;


									VehiclePos = Util::WorldToScreen(width, height, VehiclePos);

									int dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

									std::string DisplayName = xorstr("Car");
									std::string DistanceTo = std::to_string((int)dist); DistanceTo += std::string(xorstr(" m"));
									std::string FullBoatESP;

									FullBoatESP = std::string(DistanceTo + xorstr(" [ ") + DisplayName + xorstr(" ]"));

									if (dist < 250)
									{

										//auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, FullBoatESP.c_str());
										//Text(ImVec2(VehiclePos.X - size.x / 2, VehiclePos.Y - size.y / 2), ImGui::GetColorU32({ Colours::Red }), Settings.OutlineText, FullBoatESP.c_str());
										DrawOutlinedText(m_pFont, FullBoatESP.c_str(), ImVec2(VehiclePos.X, VehiclePos.Y), 14.f, ImGui::GetColorU32({ Colours::Red }), true);
									}
								}
							}











							else if (Settings.ESP.Vehicles && wcsstr(name.c_str(), (L"Valet_TaxiCab_Vehicle_C")))
							{
								auto VehicleRoot = Util::GetPawnRootLocation(pawn);
								if (VehicleRoot) {
									auto VehiclePos = *VehicleRoot;
									float dx = localPlayerLocation[0] - VehiclePos.X;
									float dy = localPlayerLocation[1] - VehiclePos.Y;
									float dz = localPlayerLocation[2] - VehiclePos.Z;


									VehiclePos = Util::WorldToScreen(width, height, VehiclePos);

									int dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

									std::string DisplayName = xorstr("Taxi");
									std::string DistanceTo = std::to_string((int)dist); DistanceTo += std::string(xorstr(" m"));
									std::string FullBoatESP;

									FullBoatESP = std::string(DistanceTo + xorstr(" [ ") + DisplayName + xorstr(" ]"));

									if (dist < 250)
									{

										//auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, FullBoatESP.c_str());
										//Text(ImVec2(VehiclePos.X - size.x / 2, VehiclePos.Y - size.y / 2), ImGui::GetColorU32({ Colours::Red }), Settings.OutlineText, FullBoatESP.c_str());
										DrawOutlinedText(m_pFont, FullBoatESP.c_str(), ImVec2(VehiclePos.X, VehiclePos.Y), 14.f, ImGui::GetColorU32({ Colours::Red }), true);
									}
								}
							}



							else if (Settings.ESP.Vehicles && wcsstr(name.c_str(), (L"Valet_BigRig_Vehicle_C")))
							{
								auto VehicleRoot = Util::GetPawnRootLocation(pawn);
								if (VehicleRoot) {
									auto VehiclePos = *VehicleRoot;
									float dx = localPlayerLocation[0] - VehiclePos.X;
									float dy = localPlayerLocation[1] - VehiclePos.Y;
									float dz = localPlayerLocation[2] - VehiclePos.Z;


									VehiclePos = Util::WorldToScreen(width, height, VehiclePos);

									int dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

									std::string DisplayName = xorstr("BigRig");
									std::string DistanceTo = std::to_string((int)dist); DistanceTo += std::string(xorstr(" m"));
									std::string FullBoatESP;

									FullBoatESP = std::string(DistanceTo + xorstr(" [ ") + DisplayName + xorstr(" ]"));

									if (dist < 250)
									{

										//auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, FullBoatESP.c_str());
										//Text(ImVec2(VehiclePos.X - size.x / 2, VehiclePos.Y - size.y / 2), ImGui::GetColorU32({ Colours::Red }), Settings.OutlineText, FullBoatESP.c_str());
										DrawOutlinedText(m_pFont, FullBoatESP.c_str(), ImVec2(VehiclePos.X, VehiclePos.Y), 14.f, ImGui::GetColorU32({ Colours::Red }), true);
									}
								}
							}

							else if (Settings.ESP.Vehicles && wcsstr(name.c_str(), (L"Valet_BasicTruck_Vehicle_C")))
							{
								auto VehicleRoot = Util::GetPawnRootLocation(pawn);
								if (VehicleRoot) {
									auto VehiclePos = *VehicleRoot;
									float dx = localPlayerLocation[0] - VehiclePos.X;
									float dy = localPlayerLocation[1] - VehiclePos.Y;
									float dz = localPlayerLocation[2] - VehiclePos.Z;


									VehiclePos = Util::WorldToScreen(width, height, VehiclePos);

									int dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

									std::string DisplayName = xorstr("Truck");
									std::string DistanceTo = std::to_string((int)dist); DistanceTo += std::string(xorstr(" m"));
									std::string FullBoatESP;

									FullBoatESP = std::string(DistanceTo + xorstr(" [ ") + DisplayName + xorstr(" ]"));

									if (dist < 250)
									{

										//auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, FullBoatESP.c_str());
										//Text(ImVec2(VehiclePos.X - size.x / 2, VehiclePos.Y - size.y / 2), ImGui::GetColorU32({ Colours::Red }), Settings.OutlineText, FullBoatESP.c_str());
										DrawOutlinedText(m_pFont, FullBoatESP.c_str(), ImVec2(VehiclePos.X, VehiclePos.Y), 14.f, ImGui::GetColorU32({ Colours::Red }), true);
									}
								}


								else if (Settings.ESP.Vehicles && wcsstr(name.c_str(), (L"Valet_SportsCar_Vehicle_C")))
								{
									auto VehicleRoot = Util::GetPawnRootLocation(pawn);
									if (VehicleRoot) {
										auto VehiclePos = *VehicleRoot;
										float dx = localPlayerLocation[0] - VehiclePos.X;
										float dy = localPlayerLocation[1] - VehiclePos.Y;
										float dz = localPlayerLocation[2] - VehiclePos.Z;


										VehiclePos = Util::WorldToScreen(width, height, VehiclePos);

										int dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

										std::string DisplayName = xorstr("SportCar");
										std::string DistanceTo = std::to_string((int)dist); DistanceTo += std::string(xorstr(" m"));
										std::string FullBoatESP;

										FullBoatESP = std::string(DistanceTo + xorstr(" [ ") + DisplayName + xorstr(" ]"));

										if (dist < 250)
										{

											//auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, FullBoatESP.c_str());
											//Text(ImVec2(VehiclePos.X - size.x / 2, VehiclePos.Y - size.y / 2), ImGui::GetColorU32({ Colours::Red }), Settings.OutlineText, FullBoatESP.c_str());
											DrawOutlinedText(m_pFont, FullBoatESP.c_str(), ImVec2(VehiclePos.X, VehiclePos.Y), 14.f, ImGui::GetColorU32({ Colours::Red }), true);
										}
									}
								}
							}
						


						std::wstring O1 = Util::GetObjectFirstName((uintptr_t)(UObject*)pawn);
						if (wcsstr(O1.c_str(), xorstr(L"WeakSpot")) && Settings.Aimbot)
						{

							FRotator rot = {  };

							auto WeakSpotRoot = Util::GetPawnRootLocation(pawn);
							if (WeakSpotRoot) {
								auto WeakSpotPos = *WeakSpotRoot;
								float dx = localPlayerLocation[0] - WeakSpotPos.X;
								float dy = localPlayerLocation[1] - WeakSpotPos.Y;
								float dz = localPlayerLocation[2] - WeakSpotPos.Z;

								WeakSpotPos = Util::WorldToScreen(width, height, WeakSpotPos);
								{
									float dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;

									//if (ReadDWORD(pawn, offsets::BuildingWeakSpot::bActive) != 0 && dist < 5)
									{
										DrawOutlinedText(m_pFont, TextFormat("WeakSpot"), ImVec2(WeakSpotPos.X, WeakSpotPos.Y), 14.f, ImGui::GetColorU32({ Colours::Red }), true);
										hooks::ProcessEvent(hooks::LocalPlayerPawn, addresses::ClientSetRotation, &O1, 0);
									}
								}
							}
						}
					}
				}// no but its a better qy
				
				for (auto pawn : playerPawns) {

					auto state = ReadPointer(pawn, offsets::Pawn::PlayerState);
					if (!state) continue;
					auto mesh = ReadPointer(pawn, offsets::Character::Mesh);
					if (!mesh) continue;

					auto actorTeamIndex = ReadDWORD(state, offsets::FortPlayerStateAthena::TeamIndex);
					auto playerRoot = Util::GetPawnRootLocation(pawn);
					if (playerRoot and localPlayerPawn) {
						auto playerPos = *playerRoot;
						float dx = localPlayerLocation[0] - playerPos.X;
						float dy = localPlayerLocation[1] - playerPos.Y;
						float dz = localPlayerLocation[2] - playerPos.Z;

						float dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;
						hooks::Distance = dist;
					}


					// bone ids: updated

					// root
					FVector root = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 0, root);
					// Top
					FVector head = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, BONE_HEAD_ID, head);

					FVector head2 = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, BONE_HEAD_ID, head2);


					FVector body = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 5, body);

					FVector neck = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 66, neck);

					FVector chest = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 37, chest);

					FVector pelvis = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 2, pelvis);

					// Arms
					FVector leftShoulder = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 93, leftShoulder);

					FVector rightShoulder = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 9, rightShoulder);

					FVector leftElbow = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 94, leftElbow);

					FVector rightElbow = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 10, rightElbow);

					FVector leftHand = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 100, leftHand);

					FVector rightHand = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 99, rightHand);

					// Legs
					FVector leftLeg = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 76, leftLeg);

					FVector rightLeg = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 69, rightLeg);

					FVector leftThigh = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 82, leftThigh);

					FVector rightThigh = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 75, rightThigh);

					FVector leftFoot = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 78, leftFoot);

					FVector rightFoot = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 71, rightFoot);

					FVector leftFeet = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 85, leftFeet);

					FVector rightFeet = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 84, rightFeet);

					FVector leftFeetFinger = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 81, leftFeetFinger);

					FVector rightFeetFinger = { 0, 0, 0 };
					Util::GetBoneLocation(pawn, 74, rightFeetFinger);

					auto AimTargetColor = ImGui::GetColorU32({ Settings.ESP.TargetLineColor[0], Settings.ESP.TargetLineColor[1], Settings.ESP.TargetLineColor[2], Settings.ESP.TargetLineColor[3] });
					auto BoxColor = ImGui::GetColorU32({ 255, 255, 255, 255 }); //box color when not visible
					auto NameColor = ImGui::GetColorU32({ 255.f / 255.f, 150.f / 255.f, 0.f / 255.f, 1 });
					auto gaycolor = ImGui::GetColorU32({ 0.95f, 0.85f, 0.45f, 0.95f });
				
					auto DistanceColor = ImGui::GetColorU32({ 112.f / 255.f, 7.f / 255.f, 135.f / 255.f, 1 });
					auto SnaplineColor = ImGui::GetColorU32({ Settings.ESP.SnaplineNotVisibleColor[0], Settings.ESP.SnaplineNotVisibleColor[1], Settings.ESP.SnaplineNotVisibleColor[2], Settings.ESP.SnaplineNotVisibleColor[3] }); //snapline color when not visible
					auto SkeletonColor = ImGui::GetColorU32({ 52, 235, 222, 1 }); //skeleton color when not visible
					auto RadarColor = ImGui::GetColorU32({ 255, 0, 0, 255 });
					auto WeaponsColor = ImGui::GetColorU32({ 128,128,128,255 });
					// if you want 20 fps
					
					FVector viewPoint = { 0 };


					// function static BOOle...... declared but undefinied AHHHH

					if (localPlayerPawn)
					{

						// VISIBLE SEXION lmao
						printf("nameless has 3 nipples\n"); // no console dingus
						if (Settings.IsBulletTeleporting || Util::LineOfSightTo(localPlayerController, pawn, &viewPoint))
						{
							AimTargetColor = ImGui::GetColorU32({ Settings.ESP.TargetLineColor[0], Settings.ESP.TargetLineColor[1], Settings.ESP.TargetLineColor[2], Settings.ESP.TargetLineColor[3] });
							auto gaycolor = ImGui::GetColorU32({ 0.95f, 0.85f, 0.45f, 0.95f });
							BoxColor = ImGui::GetColorU32({ 255, 0, 0, 255 }); //box color when visible
							NameColor = ImGui::GetColorU32({ 255.f / 255.f, 150.f / 255.f, 0.f / 255.f, 1 });
							SnaplineColor = ImGui::GetColorU32({ Settings.ESP.SnaplineVisibleColor[0], Settings.ESP.SnaplineVisibleColor[1], Settings.ESP.SnaplineVisibleColor[2], Settings.ESP.SnaplineVisibleColor[3] }); //snapline color when visible
							SkeletonColor = ImGui::GetColorU32({ 255, 0, 0, 255 }); //skeleton color when visible
						}
					}

					if (Settings.RapidFire && hooks::LocalPlayerPawn)
					{
						Rapid((float)0.02);
					}

					if (Settings.ESP.Radar)
					{

						ImGuiStyle* style = &ImGui::GetStyle();
						style->WindowRounding = 0.2f;

						ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.13529413f, 0.14705884f, 0.15490198f, 0.82f));
						ImGuiWindowFlags TargetFlags;

						TargetFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

						if (!firstS) {
							ImGui::SetNextWindowPos(ImVec2{ /*220, 400*/15, 830 }, ImGuiCond_Once);
							firstS = true;
						}
						if (ImGui::Begin(xorstr("radar"), 0, ImVec2(200, 200), -1.f, TargetFlags))
						{
							ImDrawList* Draw = ImGui::GetOverlayDrawList();
							ImVec2 DrawPos = ImGui::GetCursorScreenPos();
							ImVec2 DrawSize = ImGui::GetContentRegionAvail();

							ImVec2 midRadar = ImVec2(DrawPos.x + (DrawSize.x / 2), DrawPos.y + (DrawSize.y / 2));

							ImGui::GetWindowDrawList()->AddLine(ImVec2(midRadar.x - DrawSize.x / 2.f, midRadar.y), ImVec2(midRadar.x + DrawSize.x / 2.f, midRadar.y), IM_COL32(0, 0, 0, 50));
							ImGui::GetWindowDrawList()->AddLine(ImVec2(midRadar.x, midRadar.y - DrawSize.y / 2.f), ImVec2(midRadar.x, midRadar.y + DrawSize.y / 2.f), IM_COL32(0, 0, 0, 50));

							//if (offsets::Player::PlayerController || offsets::PlayerController::PlayerCameraManager && hooks::LocalPlayerPawn && CamRot.y) {
								renderRadar();
							//}
						}
						ImGui::PopStyleColor();
						ImGui::End();
					}

					if (localPlayerPawn)
					{
						if (Settings.ESP.PlayerNames)
						{
							head.Z += 15;
							auto headPos = head;
							headPos = Util::WorldToScreen(width, height, headPos);
							auto localroot = Util::GetPawnRootLocation(pawn);
							if (&root)
							{
								auto position = *localroot;
								float dx = localPlayerLocation[0] - position.X;
								float dy = localPlayerLocation[1] - position.Y;
								float dz = localPlayerLocation[2] - position.Z;
								position = Util::WorldToScreen(width, height, position);
								float dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;
								FString playerName;
								hooks::ProcessEvent(state, addresses::GetPlayerName, &playerName, 0);
								if (playerName.c_str()) {
									CHAR copy[0xFF] = { 0 };
									wcstombs(copy, playerName.c_str(), sizeof(copy)); // i will make the distance esp soon!
									Util::FreeInternal(playerName.c_str());
									DrawOutlinedText(m_pFont, TextFormat(xorstr("%s"), copy, dist), ImVec2(headPos.X, headPos.Y - 24), 16.0f, NameColor, true); // [%.0f m]
								}
							}
						}


						if (Settings.ESP.PlayerWeapons)
						{

							auto actorCurrentWeapon = ReadPointer(pawn, offsets::FortPawn::CurrentWeapon);
							if (!actorCurrentWeapon) continue;

							auto actorItemDef = ReadPointer(actorCurrentWeapon, offsets::FortWeapon::WeaponData);
							if (!actorItemDef) continue;

							auto actorItemDisplayName = reinterpret_cast<FText*>(ReadPointer(actorItemDef, offsets::FortItemDefinition::DisplayName));
							if (!actorItemDisplayName || !actorItemDisplayName->c_str()) continue;

						/*	root.Z -= 15;
							auto bottomPos = root;
							bottomPos = Util::WorldToScreen(width, height, bottomPos);*/
							head.Z += 15;
							auto headPos = head;
							headPos = Util::WorldToScreen(width, height, headPos);
							CHAR text[0xFF] = { 0 };
							wcstombs(text, actorItemDisplayName->c_str(), sizeof(text));

							DrawOutlinedText(m_pFont, TextFormat(xorstr("%s"), text), ImVec2(headPos.X, headPos.Y - 27), 16.0f, WeaponsColor, true);
						}





						if (localPlayerTeamIndex != actorTeamIndex)
						{

							if (Settings.VisibleCheck)
							{
								if (Settings.IsBulletTeleporting)
								{
									if (Settings.AutoAimbot) //closest by distance
									{
										auto dx = head.X - localPlayerLocation[0];
										auto dy = head.Y - localPlayerLocation[1];
										auto dz = head.Z - localPlayerLocation[2];
										auto dist = dx * dx + dy * dy + dz * dz;
										if (dist < closestDistance) {
											closestDistance = dist;
											closestPawn = pawn;
										}
									}
									else //closest from crosshair
									{
										auto w2s = head;
										w2s = Util::WorldToScreen(width, height, w2s);
										auto dx = w2s.X - (width / 2);
										auto dy = w2s.Y - (height / 2);
										auto dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy);
										if (dist < Settings.AimbotFOV && dist < closestDistance) {
											closestDistance = dist;
											closestPawn = pawn;
										}
									}
								}
								else
								{
									if (Util::LineOfSightTo(localPlayerController, pawn, &viewPoint))
									{
										if (Settings.AutoAimbot) //closest by distance
										{
											auto dx = head.X - localPlayerLocation[0];
											auto dy = head.Y - localPlayerLocation[1];
											auto dz = head.Z - localPlayerLocation[2];
											auto dist = dx * dx + dy * dy + dz * dz;
											if (dist < closestDistance) {
												closestDistance = dist;
												closestPawn = pawn;
											}
										}
										else //closest from crosshair
										{
											auto w2s = head;
											w2s = Util::WorldToScreen(width, height, w2s);
											auto dx = w2s.X - (width / 2);
											auto dy = w2s.Y - (height / 2);
											auto dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy);
											if (dist < Settings.AimbotFOV && dist < closestDistance) {
												closestDistance = dist;
												closestPawn = pawn;
											}
										}
									}
								}
							}
							else
							{
								if (Settings.AutoAimbot) //closest by distance
								{
									auto dx = head.X - localPlayerLocation[0];
									auto dy = head.Y - localPlayerLocation[1];
									auto dz = head.Z - localPlayerLocation[2];
									auto dist = dx * dx + dy * dy + dz * dz;
									if (dist < closestDistance) {
										closestDistance = dist;
										closestPawn = pawn;
									}
								}
								else //closest from crosshair
								{
									auto w2s = head;
									w2s = Util::WorldToScreen(width, height, w2s);
									auto dx = w2s.X - (width / 2);
									auto dy = w2s.Y - (height / 2);
									auto dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy);
									if (dist < Settings.AimbotFOV && dist < closestDistance) {
										closestDistance = dist;
										closestPawn = pawn;
									}
								}
							}
							auto VehicleTargetW2S = head;
							VehicleTargetW2S = Util::WorldToScreen(width, height, VehicleTargetW2S);
							auto tdx = VehicleTargetW2S.X - (width / 2);
							auto tdy = VehicleTargetW2S.Y - (height / 2);
							auto tdist = Util::SpoofCall(sqrtf, tdx * tdx + tdy * tdy);
							if (tdist < 5000 && tdist < closestDistanceForVehicle) {
								closestDistanceForVehicle = tdist;
								closestPawnForVehicle = pawn;
							}

							if (Settings.TeleportToEnemies)
							{
								if (InVehicle)
								{
									if (hooks::VehicleTargetPawn != nullptr)
									{

										FVector VehicleTargetRootLocation = { 0 };
										Util::GetBoneLocation(hooks::VehicleTargetPawn, 66, VehicleTargetRootLocation);

										ClosestPlayerCoords.X = VehicleTargetRootLocation.X;
										ClosestPlayerCoords.Y = VehicleTargetRootLocation.Y;
										ClosestPlayerCoords.Z = VehicleTargetRootLocation.Z;

										auto VehicleTargetPosition = VehicleTargetRootLocation;
										VehicleTargetPosition = Util::WorldToScreen(width, height, VehicleTargetPosition);
										window.DrawList->AddLine(ImVec2(width / 2, height / 2), ImVec2(VehicleTargetPosition.X, VehicleTargetPosition.Y), ImGui::GetColorU32({ 255, 0, 0, 255 }));
									}
								}
							}

							/*
							//if (InVehicle)
							//{
								auto VehicleTargetW2S = head;
								if (Util::WorldToScreen(width, height, &VehicleTargetW2S.X))
								{
									auto tdx = VehicleTargetW2S.X - (width / 2);
									auto tdy = VehicleTargetW2S.Y - (height / 2);
									auto tdist = Util::SpoofCall(sqrtf, tdx * tdx + tdy * tdy);
									if (tdist < 5000 && tdist < closestDistanceForVehicle) {
										closestDistanceForVehicle = tdist;
										closestPawnForVehicle = pawn;
									}
								}
							//}
								if (Settings.TeleportToEnemies)
								{
									if (hooks::VehicleTargetPawn != nullptr)
									{
										auto VehicleTargetMesh = ReadPointer(hooks::VehicleTargetPawn, offsets::Character::Mesh);
										if (!VehicleTargetMesh) continue;

										auto VehicleTargetBones = ReadPointer(VehicleTargetMesh, offsets::StaticMeshComponent::StaticMesh);
										if (!VehicleTargetBones) VehicleTargetBones = ReadPointer(VehicleTargetMesh, offsets::StaticMeshComponent::StaticMesh + 0x10);
										if (!VehicleTargetBones) continue;

										float VehicleTargetCompMatrix[4][4] = { 0 };
										Util::ToMatrixWithScale(reinterpret_cast<float*>(reinterpret_cast<PBYTE>(VehicleTargetMesh) + offsets::StaticMeshComponent::ComponentToWorld), VehicleTargetCompMatrix);

										float VehicleTargetRootLocation[3] = { 0 };
										Util::GetBoneLocation(VehicleTargetCompMatrix, VehicleTargetBones, 66, VehicleTargetRootLocation);

										ClosestPlayerCoords.X = VehicleTargetRootLocation[0];
										ClosestPlayerCoords.Y = VehicleTargetRootLocation[1];
										ClosestPlayerCoords.Z = VehicleTargetRootLocation[2];

										auto VehicleTargetPosition = *reinterpret_cast<FVector*>(VehicleTargetRootLocation);
										if (Util::WorldToScreen(width, height, &VehicleTargetPosition.X))
										{
											window.DrawList->AddLine(ImVec2(width / 2, height / 2), ImVec2(VehicleTargetPosition.X, VehicleTargetPosition.Y), ImGui::GetColorU32({ 255, 0, 0, 255 }));
										}
									}
								}

								if (Settings.TeleportToEnemies)
								{
									if (localPlayerController)
									{
										if (hooks::VehicleTargetPawn != nullptr)
										{
											if (GetAsyncKeyState(VK_XBUTTON2))
											{
												hooks::Teleport(localPlayerPawn, ClosestPlayerCoords);
											}
										}
									}
								}
								*/
								/*	if (Settings.VehicleTeleporter)
									{
										if (localPlayerController)
										{
											if (InVehicle)
											{
												if (CurrentVehicle)
												{
													if (hooks::VehicleTargetPawn != nullptr)
													{
														if (GetAsyncKeyState(VK_XBUTTON2))
														{
															hooks::ProcessEvent(CurrentVehicle2, addresses::K2_TeleportTo, &ClosestPlayerCoords, 0);
														}
													}
												}
											}
										}
									}*/
							if (Settings.BigPlayers)
							{
								if (localPlayerPawn && pawn && localPlayerController)
								{
									auto mesh = ReadPointer(pawn, offsets::Character::Mesh);
									if (!mesh) continue;

									hooks::ProcessEvent(mesh, addresses::SetForcedLodModel, &Settings.gayy, 0);
								}
							}
							if (Settings.TargetLine)
							{
								if (Settings.Aimbot)
								{
									int TargetHitbox; // WAIT THERE IS 2 SKEELETON  yes
									if (Settings.HitBoxPos == 0)
									{
										TargetHitbox = 66;
									}
									else if (Settings.HitBoxPos == 1)
									{
										TargetHitbox = 65;
									}
									else if (Settings.HitBoxPos == 2)
									{
										TargetHitbox = 5;
									}
									else if (Settings.HitBoxPos == 3)
									{
										TargetHitbox = 0;
									}
									else if (Settings.HitBoxPos == 4)
									{
										TargetHitbox = 2;
									}

									if (hooks::TargetPawn != nullptr)
									{
										FVector targetRoot = { 0 };
										Util::GetBoneLocation(hooks::TargetPawn, TargetHitbox, targetRoot);

										auto targetPos = targetRoot;
										targetPos = Util::WorldToScreen(width, height, targetPos);
										window.DrawList->AddLine(ImVec2(width / 2, height / 2), ImVec2(targetPos.X, targetPos.Y), AimTargetColor);
									}
								}
							}

							if (Settings.ESP.TestChams)
							{
								//if (localPlayerTeamIndex != actorTeamIndex)
								//{
								auto FpsColorcacaa = ImGui::GetColorU32({ color_red, color_green, color_blue, 255 });

								//*reinterpret_cast<PVOID*>(reinterpret_cast<PBYTE>(state) + offsets::FortPlayerStateAthena::TeamIndex) = Index;
								//*reinterpret_cast<PVOID*>(reinterpret_cast<PBYTE>(state) + 0x10ac) = SquadID;
								//DrawOutlinedText(m_pFont, xorstr("ENABLEDDINOOOOOOOOWIOJGWEIHGNJIWUOQGBIQEUWGBQUIEBGIQWG"), ImVec2(100, 400), 16.0f, FpsColorcacaa, false);
								//}
							}

							if (Settings.ESP.PlayerLines)
							{
								if (Settings.SnaplineStartPoint == 0) //bottom
								{
									root.Z -= 10;
									auto end = root;
									end = Util::WorldToScreen(width, height, end);
									window.DrawList->AddLine(ImVec2(width / 2, height), ImVec2(end.X, end.Y), SnaplineColor);
								}
								else if (Settings.SnaplineStartPoint == 1) //top
								{
									head.Z += 15;
									auto end = head;
									end = Util::WorldToScreen(width, height, end);
									window.DrawList->AddLine(ImVec2(width / 2, height / 20000), ImVec2(end.X, end.Y), SnaplineColor);
								}
								else if (Settings.SnaplineStartPoint == 2) //center
								{
									auto end = body;
									end = Util::WorldToScreen(width, height, end);
									window.DrawList->AddLine(ImVec2(width / 2, height / 2), ImVec2(end.X, end.Y), SnaplineColor);
								}
							}

							float minX = FLT_MAX;
							float maxX = -FLT_MAX;
							float minY = FLT_MAX;
							float maxY = -FLT_MAX;

							//if (Settings.ESP.Boxes)
							//{
							//	head.Z += 15;
							//	root.Z -= 10;
							//	auto headPos = head;
							//	auto bottomPos = root;

							//	bottomPos = Util::WorldToScreen(width, height, bottomPos);
							//	headPos = Util::WorldToScreen(width, height, headPos);

							//	float BoxHeight = (float)(headPos.Y - bottomPos.Y);
							//	float BoxWidth = BoxHeight * 0.380f;

							//	float LeftX = (float)headPos.X - (BoxWidth / 1);
							//	float LeftY = (float)bottomPos.Y;

							//	float CornerHeight = abs(headPos.Y - bottomPos.Y);
							//	float CornerWidth = CornerHeight * 0.75; //0.5

							//	if (boxmodepos == 0) //2d
							//	{
							//		DrawRoundedRect(LeftX, LeftY, headPos.X + BoxWidth, headPos.Y, BoxColor, 1.5);
							//	}
							//	else if (boxmodepos == 1) //cornered
							//	{
							//		DrawCorneredBox(headPos.X - (CornerWidth / 2), headPos.Y, CornerWidth, CornerHeight, BoxColor, 1.5);
							//	}
							//	else if (boxmodepos == 2) //2d filled
							//	{
							//		ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(LeftX, LeftY), ImVec2(headPos.X + BoxWidth, headPos.Y), IM_COL32(0, 0, 0, 70));
							//		DrawRoundedRect(LeftX, LeftY, headPos.X + BoxWidth, headPos.Y, BoxColor, 1.5);
							//	}
							//	else if (boxmodepos == 3) //cornered filled
							//	{
							//		ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(LeftX, LeftY), ImVec2(headPos.X + BoxWidth, headPos.Y), IM_COL32(0, 0, 0, 70));
							//		DrawCorneredBox(headPos.X - (CornerWidth / 2), headPos.Y, CornerWidth, CornerHeight, BoxColor, 1.5);
							//	}
							//}
						







							
							if (Settings.ESP.PlayerWeapons)
							{
								auto actorCurrentWeapon = ReadPointer(pawn, offsets::FortPawn::CurrentWeapon);
								if (!actorCurrentWeapon) continue;

								int actorAmmoCount = ReadInt(actorCurrentWeapon, offsets::FortWeapon::AmmoCount);
								if (!actorAmmoCount) continue;

								auto itemDef = ReadPointer(actorCurrentWeapon, offsets::FortWeapon::WeaponData);
								if (itemDef != nullptr)

								if (actorAmmoCount)
								{
									auto weaponName = reinterpret_cast<FText*>(ReadPointer(itemDef, offsets::FortItemDefinition::DisplayName));
									if (!weaponName || !weaponName->c_str()) continue;



									auto isReloading = reinterpret_cast<bool*>(ReadPointer(actorCurrentWeapon, offsets::AFortWeapon::bIsReloadingWeapon));




									root.Z -= 15;
									auto bottomPos = root;
									bottomPos = Util::WorldToScreen(width, height, bottomPos);
									int value;

									/*if (isReloading)
									{
										auto reloadingcolor = ImGui::GetColorU32({ 255, 0, 0, 255 });
										DrawOutlinedText(m_pFont, TextFormat(xorstr("Reloading..")), ImVec2(bottomPos.X, bottomPos.Y + 20), 16.0f, reloadingcolor, true);

									}*/
									if (Settings.ESP.PlayerWeapons)
									{
										value = 15;
									}
									else
									{
										value = 5;
									}


									DrawOutlinedText(m_pFont, TextFormat(xorstr("%i / 30"), actorAmmoCount), ImVec2(bottomPos.X, bottomPos.Y + 5), 16.0f, gaycolor, true);
									/*else
									{
										
									}*/
									
								}
							}
						}
					}

					if (Settings.TeleportToEnemies)
					{
						if (hooks::VehicleTargetPawn != nullptr)
						{
							FVector nigger;
							Util::GetBoneLocation(hooks::VehicleTargetPawn, 66, nigger);
							
							ClosestPlayerCoords.X = nigger.Z;
							ClosestPlayerCoords.Y = nigger.Y;
							ClosestPlayerCoords.Z = nigger.Z;

							auto VehicleTargetPosition = *reinterpret_cast<FVector*>(Util::GetPawnRootLocation(hooks::VehicleTargetPawn));
							VehicleTargetPosition = Util::WorldToScreen(width, height, VehicleTargetPosition);
							{
								//window.DrawList->AddLine(ImVec2(width / 2, height / 2), ImVec2(VehicleTargetPosition.X, VehicleTargetPosition.Y), ImGui::GetColorU32({ brightred })); //255, 0, 0, 255
							}
						}
					}
					// WAIT BTW
					if (InVehicle)
					{
						auto VehicleTargetW2S = head;
						VehicleTargetW2S = Util::WorldToScreen(width, height, VehicleTargetW2S);

						{
							auto tdx = VehicleTargetW2S.X - (width / 2);
							auto tdy = VehicleTargetW2S.Y - (height / 2);
							auto tdist = Util::SpoofCall(sqrtf, tdx * tdx + tdy * tdy);
							if (tdist < 5000 && tdist < closestDistanceForVehicle) {
								closestDistanceForVehicle = tdist;
								closestPawnForVehicle = pawn;
							}
						}
					}

					if (Settings.TeleportToEnemies)
					{
						if (hooks::VehicleTargetPawn != nullptr)
						{
							FVector VehicleTargetRootLocation;
							Util::GetBoneLocation(hooks::VehicleTargetPawn, 66, VehicleTargetRootLocation);

							ClosestPlayerCoords.X = VehicleTargetRootLocation.X;
							ClosestPlayerCoords.Y = VehicleTargetRootLocation.Y;
							ClosestPlayerCoords.Z = VehicleTargetRootLocation.Z;

							VehicleTargetRootLocation = Util::WorldToScreen(width, height, VehicleTargetRootLocation);
							{
								//window.DrawList->AddLine(ImVec2(width / 2, height / 2), ImVec2(VehicleTargetPosition.X, VehicleTargetPosition.Y), ImGui::GetColorU32({ brightred }));  //255, 0, 0, 255
							}
						}
					}

					if (Settings./*TeleportToEnemies*/TeleportToEnemies and localPlayerPawn)
					{
						if (localPlayerController)
						{
							if (hooks::VehicleTargetPawn != nullptr)
							{
								if (Util::SpoofCall(GetAsyncKeyState, VK_XBUTTON2))
								{
									hooks::Teleport(localPlayerPawn, ClosestPlayerCoords);
								}
							}
						}
					}

					if (Settings./*VehicleTeleporter*/TeleportToEnemies)
					{
						if (localPlayerController)
						{
							if (InVehicle)
							{
								if (hooks::VehicleTargetPawn)
								{
									if (hooks::VehicleTargetPawn != nullptr)
									{
										if (Util::SpoofCall(GetAsyncKeyState, VK_XBUTTON2))
										{
											hooks::ProcessEvent(hooks::VehicleTargetPawn, addresses::LaunchCharacter/*addresses::LaunchCharacter*/, &ClosestPlayerCoords, 0);
										}
									}
								}
							}
						}
					}

					if (Settings./*VehicleTeleporter*/TeleportToEnemies)
					{
						if (localPlayerController)
						{
							if (InVehicle)
							{
								if (hooks::VehicleTargetPawn != nullptr)
								{
									if (Util::SpoofCall(GetAsyncKeyState, VK_XBUTTON2))
									{
										hooks::Teleport2(ClosestPlayerCoords);
									}
								}
							}
						}
					}

					/*if (Settings.ESP.Chams)
					{
						if (localPlayerTeamIndex != actorTeamIndex)
						{
							*reinterpret_cast<PVOID*>(reinterpret_cast<PBYTE>(state) + offsets::FortPlayerStateAthena::TeamIndex) = localteamindex;
							*reinterpret_cast<PVOID*>(reinterpret_cast<PBYTE>(state) + 0x1004) = monkey; // /Script/FortniteGame.FortPlayerStateAthena.SquadId -> 0x1004
						}
					}*/

					if (Settings.ESP.PlayerLines)
					{
						root.Y -= 10;

						auto gay = Util::WorldToScreen(width, height, root);

						window.DrawList->AddLine(ImVec2(width / 2, height), ImVec2(gay.X, gay.Y), SnaplineColor, 0.2);
					}

					float minX = FLT_MAX;
					float maxX = -FLT_MAX;
					float minY = FLT_MAX;
					float maxY = -FLT_MAX;

					if (Settings.ESP.Skeletons)
					{
						
					}
					if (Settings.ESP.realskel)
					{
						AddLine(window, width, height, head2, neck, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, neck, pelvis, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, chest, leftShoulder, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, chest, rightShoulder, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, leftShoulder, leftElbow, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, rightShoulder, rightElbow, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, leftElbow, leftHand, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, rightElbow, rightHand, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, pelvis, leftLeg, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, pelvis, rightLeg, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, leftLeg, leftThigh, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, rightLeg, rightThigh, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, leftThigh, leftFoot, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, rightThigh, rightFoot, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, leftFoot, leftFeet, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, rightFoot, rightFeet, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, leftFeet, leftFeetFinger, SkeletonColor, minX, maxX, minY, maxY);
						AddLine(window, width, height, rightFeet, rightFeetFinger, SkeletonColor, minX, maxX, minY, maxY);
					}

					if (Settings.ESP.ThreeDBox)
					{

					}

					if (Settings.ESP.Boxes)
					{
						if (minX < width && maxX > 0 && minY < height && maxY > 0) {

							auto topLeft = ImVec2(minX - 15.0f, minY - 20.0f); // nono its fixed everything is good with that ok what should i do 1: I want name esp, distance in lobby please ): let me try yk
							auto bottomRight = ImVec2(maxX + 15.0f, maxY + 15.0f);


							auto boxcol = ImGui::GetColorU32({ 255, 255, 255, 255 });

							auto centerTop = ImVec2((topLeft.x + bottomRight.x) / 2.0f + +10 + 1.65, topLeft.y);

							//ImGui::GetOverlayDrawList()->AddRectFilled(topLeft, bottomRight, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, 0.75f }));

							auto theAdditionY = (bottomRight.y - topLeft.y) / 5;
							auto theAdditionX = (bottomRight.x - topLeft.x) / 4;

							auto topRight = ImVec2(bottomRight.x, topLeft.y);
							auto bottomLeft = ImVec2(topLeft.x, bottomRight.y);

							ImGui::GetOverlayDrawList()->AddLine(topLeft, ImVec2(topLeft.x, topLeft.y + theAdditionY), BoxColor, 2); // Top Left Y
							ImGui::GetOverlayDrawList()->AddLine(topLeft, ImVec2(topLeft.x + theAdditionX, topLeft.y), BoxColor, 2); // Top Left X

							ImGui::GetOverlayDrawList()->AddLine(topRight, ImVec2(topRight.x, topRight.y + theAdditionY), BoxColor, 2); // Top Right Y
							ImGui::GetOverlayDrawList()->AddLine(topRight, ImVec2(topRight.x - theAdditionX, topRight.y), BoxColor, 2); // Top Right X

							ImGui::GetOverlayDrawList()->AddLine(bottomLeft, ImVec2(bottomLeft.x, bottomLeft.y - theAdditionY), BoxColor, 2); // Bottom Left Y
							ImGui::GetOverlayDrawList()->AddLine(bottomLeft, ImVec2(bottomLeft.x + theAdditionX, bottomLeft.y), BoxColor, 2); // Bottom Left X

							ImGui::GetOverlayDrawList()->AddLine(bottomRight, ImVec2(bottomRight.x, bottomRight.y - theAdditionY), BoxColor, 2); // Bottom Right Y
							ImGui::GetOverlayDrawList()->AddLine(bottomRight, ImVec2(bottomRight.x - theAdditionX, bottomRight.y), BoxColor, 2); // Bottom Right X
						}
					}
					
					if (Settings.ESP.Distance)
					{
						head.Z += 15;

						auto headPos = Util::WorldToScreen(width, height, head);
						{
							auto localroot = Util::GetPawnRootLocation(pawn);
							if (localroot)
							{
								auto position = *localroot;
								float dx = localPlayerLocation[0] - position.X;
								float dy = localPlayerLocation[1] - position.Y;
								float dz = localPlayerLocation[2] - position.Z;

								position = Util::WorldToScreen(width, height, position);

								float dist = Util::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;
								std::string DistanceTo = std::to_string((int)dist); DistanceTo += std::string(xorstr(" m"));
								std::string FullDistESP;

								FullDistESP = std::string(DistanceTo);

								//auto size = ImGui::GetFont()->CalcTextSizeA(window.DrawList->_Data->FontSize, FLT_MAX, 0, DistanceTo.c_str());
								//Text(ImVec2((headPos.X + headPos.X) / 2 - size.x / 2, headPos.Y + 3), NameColor, Settings.OutlineText, FullDistESP.c_str());
								DrawOutlinedText(m_pFont, FullDistESP, ImVec2(headPos.X, headPos.Y - 28), 14.0f, DistanceColor, true);

							}
						}
					}
				}
				if (Settings.TeleportToEnemies)
				{
					if (InVehicle)
					{
						if (closestPawnForVehicle != nullptr)
						{
							hooks::VehicleTargetPawn = closestPawnForVehicle;
						}
						if (closestPawnVehicle != nullptr)
						{
							hooks::ClosestVehicle = closestPawnVehicle;
						}
					}
					else
					{
						hooks::VehicleTargetPawn = nullptr;
						hooks::ClosestVehicle = nullptr;
					}
				}

				if (hooks::LocalPlayerController && hooks::LocalPlayerPawn)
				{
				}
			//	static float OldPitch = Util::GetViewInfo().Rotation.Pitch;
				//static float OldYaw = Util::GetViewInfo().Rotation.Yaw;

				if (Settings.Spinbot)
				{
					if (localPlayerController)
					{
						if (Util::SpoofCall(GetAsyncKeyState, Settings.AimKey))
						{
							if (hooks::TargetPawn != nullptr && hooks::LocalPlayerCurrentWeapon);
							{
								FRotator rot = {};
								int jitter = rot.Pitch = rand() % 360;
								int random = rand() % 100;
								int desiredspinspeed = rand() % 11;


								int rndm = rot.Pitch = rand() % 360;

								if (rndm)
								{
									static bool ChangeSide = false;
									if (ChangeSide)
									{
										rot.Pitch -= 218;
									}
									else
									{
										rot.Pitch -= 10;
									}
									ChangeSide = !ChangeSide;
								}

								//int slow = +35;
								rot.Yaw = 0.f; // spin
								rot.Pitch = rndm; // up/down
								hooks::ProcessEvent(hooks::LocalPlayerController, addresses::ClientSetRotation, &rot, 0);
							}
						}
						else
						{
							if (Settings.RestoreRotation)
							{
								//OldPitch;
								//OldYaw;
							}
						}
					}
				}
				


			

				if (Settings.Aimbot)
				{
					if (closestPawn && GetAsyncKeyState(VK_RBUTTON) && Util::SpoofCall(GetForegroundWindow) == hWnd)
					{
						hooks::TargetPawn = closestPawn;
					}
					else
					{
						hooks::TargetPawn = nullptr;
					}
				}
				else
				{
					hooks::TargetPawn = nullptr;
				}

				success = TRUE;
			} while (FALSE);

		if (!success) {
			hooks::LocalPlayerController = hooks::LocalPlayerPawn = hooks::TargetPawn = hooks::VehicleTargetPawn = hooks::ClosestVehicle = VehiclePawn = nullptr;
		}
		EndScene(window);

		return o_present(pSwapChain, sync_interval, flags);
	}



	__declspec(dllexport) HRESULT DoResize(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
		ImGui_ImplDX11_Shutdown();
		renderTargetView->Release();
		immediateContext->Release();
		device->Release();
		device = nullptr;

		return ResizeOriginal(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
	}

	BOOLEAN Initialize()
	{
		IDXGISwapChain* swapChain = nullptr;
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
		auto                 featureLevel = D3D_FEATURE_LEVEL_11_0;

		DXGI_SWAP_CHAIN_DESC sd = { 0 };
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.OutputWindow = FindWindow(xorstr(L"UnrealWindow"), xorstr(L"Fortnite  "));
		sd.SampleDesc.Count = 1;
		sd.Windowed = TRUE;

		if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1, D3D11_SDK_VERSION, &sd, &swapChain, &device, nullptr, &context))) {
			MessageBox(0, xorstr(L"failed: dx11"), xorstr(L"neutron"), MB_ICONERROR);
			return FALSE;
		}

		auto table = *reinterpret_cast<PVOID**>(swapChain);
		auto present = table[8];
		auto resize = table[13];

		context->Release();
		device->Release();
		swapChain->Release();

		//InitializeDiscordHook();
		MH_CreateHook(present, hk_present, reinterpret_cast<PVOID*>(&o_present));
		MH_EnableHook(present);

		//Menu::Config();

		MH_CreateHook(resize, DoResize, reinterpret_cast<PVOID*>(&ResizeOriginal));
		MH_EnableHook(resize);

		return TRUE;
	}
}