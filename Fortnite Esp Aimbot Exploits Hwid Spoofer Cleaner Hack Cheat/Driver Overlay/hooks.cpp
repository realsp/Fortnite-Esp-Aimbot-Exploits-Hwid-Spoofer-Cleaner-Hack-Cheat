#include "stdafx.h"
/*
possible methods:

FortniteGame.FortWeaponRanged:
bUseAthenaRecoil
FirstShotAccuracyMinWaitTime
FireDownsightsAnimation
bEnableRecoilDelay
OverheatState
bCooldownWhileOverheated

*/

namespace hooks {

	bool NoSpread = false;
	bool IsLocalPlayerInVehicle = false;
	PVOID LocalPlayerCurrentWeapon = nullptr;
	PVOID LocalPlayerPawn = nullptr;
	PVOID LocalPlayerController = nullptr;
	PVOID LocalPosition = nullptr;
	PVOID TargetPawn = nullptr;
	PVOID VehicleTargetPawn = nullptr;
	PVOID ClosestVehicle = nullptr;
	PVOID LocalRootComp = nullptr;
	PVOID PlayerCameraManager = nullptr;
	FVector LocalplayerPosition = { 0, 0, 0 };
	FVector ClosestTargetCoord = { 0, 0, 0 };

	float Distance = 0;
	bool IsSniper = 0;
	PVOID(*ProcessEvent)(PVOID, PVOID, PVOID, PVOID) = nullptr;
	float* (*CalculateShot)(PVOID, PVOID, PVOID) = nullptr;
	VOID(*ReloadOriginal)(PVOID, PVOID) = nullptr;
	INT(*GetViewPoint)(PVOID, FMinimalViewInfo*, BYTE) = nullptr;

	float originalReloadTime = 0.0f;

	PVOID(*CalculateSpread)(PVOID, float*, float*) = nullptr;

	PVOID calculateSpreadCaller = nullptr;


	//SPEEDHACK SHIT -_-
#include "detours.h"

#pragma comment(lib, "detours.lib")

//#include "SpeedHack.h"
// TODO: put in another file, and rename to something better
	template<class DataType>
	class SpeedHack {
		DataType time_offset;
		DataType time_last_update;

		double speed_;

	public:
		SpeedHack(DataType currentRealTime, double initialSpeed) {
			time_offset = currentRealTime;
			time_last_update = currentRealTime;

			speed_ = initialSpeed;
		}

		// TODO: put lock around for thread safety
		void setSpeed(DataType currentRealTime, double speed) {
			time_offset = getCurrentTime(currentRealTime);
			time_last_update = currentRealTime;

			speed_ = speed;
		}

		// TODO: put lock around for thread safety
		DataType getCurrentTime(DataType currentRealTime) {
			DataType difference = currentRealTime - time_last_update;

			return (DataType)(speed_ * difference) + time_offset;
		}
	};


	// function signature typedefs
	typedef DWORD(WINAPI* GetTickCountType)(void);
	typedef ULONGLONG(WINAPI* GetTickCount64Type)(void);

	typedef BOOL(WINAPI* QueryPerformanceCounterType)(LARGE_INTEGER* lpPerformanceCount);

	// globals
	GetTickCountType   g_GetTickCountOriginal;
	GetTickCount64Type g_GetTickCount64Original;
	GetTickCountType   g_TimeGetTimeOriginal;    // Same function signature as GetTickCount

	QueryPerformanceCounterType g_QueryPerformanceCounterOriginal;


	const double kInitialSpeed = 1.0; // initial speed hack speed

	//                                  (initialTime,      initialSpeed)
	SpeedHack<DWORD>     g_speedHack(GetTickCount(), kInitialSpeed);
	SpeedHack<ULONGLONG> g_speedHackULL(GetTickCount64(), kInitialSpeed);
	SpeedHack<LONGLONG>  g_speedHackLL(0, kInitialSpeed); // Gets set properly in DllMain

	// function prototypes

	DWORD     WINAPI GetTickCountHacked(void);
	ULONGLONG WINAPI GetTickCount64Hacked(void);

	BOOL      WINAPI QueryPerformanceCounterHacked(LARGE_INTEGER* lpPerformanceCount);

	DWORD     WINAPI KeysThread(LPVOID lpThreadParameter);

	// functions

	void MainGay()
	{
		// TODO: split up this function for readability.

		HMODULE kernel32 = GetModuleHandleA(xorstr("Kernel32.dll"));
		HMODULE winmm = GetModuleHandleA(xorstr("Winmm.dll"));

		// TODO: check if the modules are even loaded.

		// Get all the original addresses of target functions
		g_GetTickCountOriginal = (GetTickCountType)GetProcAddress(kernel32, xorstr("GetTickCount"));
		g_GetTickCount64Original = (GetTickCount64Type)GetProcAddress(kernel32, xorstr("GetTickCount64"));

		g_TimeGetTimeOriginal = (GetTickCountType)GetProcAddress(winmm, xorstr("timeGetTime"));

		g_QueryPerformanceCounterOriginal = (QueryPerformanceCounterType)GetProcAddress(kernel32, xorstr("QueryPerformanceCounter"));

		// Setup the speed hack object for the Performance Counter
		LARGE_INTEGER performanceCounter;
		g_QueryPerformanceCounterOriginal(&performanceCounter);

		g_speedHackLL = SpeedHack<LONGLONG>(performanceCounter.QuadPart, kInitialSpeed);

		// Detour functions
		DetourTransactionBegin();

		DetourAttach((PVOID*)&g_GetTickCountOriginal, (PVOID)GetTickCountHacked);
		DetourAttach((PVOID*)&g_GetTickCount64Original, (PVOID)GetTickCount64Hacked);

		// Detour timeGetTime to the hacked GetTickCount (same signature)
		DetourAttach((PVOID*)&g_TimeGetTimeOriginal, (PVOID)GetTickCountHacked);

		DetourAttach((PVOID*)&g_QueryPerformanceCounterOriginal, (PVOID)QueryPerformanceCounterHacked);

		DetourTransactionCommit();
	}

	void setAllToSpeed(double speed) {
		g_speedHack.setSpeed(g_GetTickCountOriginal(), speed);

		g_speedHackULL.setSpeed(g_GetTickCount64Original(), speed);

		LARGE_INTEGER performanceCounter;
		g_QueryPerformanceCounterOriginal(&performanceCounter);

		g_speedHackLL.setSpeed(performanceCounter.QuadPart, speed);
	}

	DWORD WINAPI GetTickCountHacked(void) {
		return g_speedHack.getCurrentTime(g_GetTickCountOriginal());
	}

	ULONGLONG WINAPI GetTickCount64Hacked(void) {
		return g_speedHackULL.getCurrentTime(g_GetTickCount64Original());
	}

	BOOL WINAPI QueryPerformanceCounterHacked(LARGE_INTEGER* lpPerformanceCount) {
		LARGE_INTEGER performanceCounter;

		BOOL result = g_QueryPerformanceCounterOriginal(&performanceCounter);

		lpPerformanceCount->QuadPart = g_speedHackLL.getCurrentTime(performanceCounter.QuadPart);

		return result;
	}

	BOOLEAN GetTargetHead(FVector& out) {
		if (!hooks::TargetPawn) {
			return FALSE;
		}

		Util::GetBoneLocation(hooks::TargetPawn, BONE_HEAD_ID, out); //BONE_HEAD_ID
		return TRUE;
	}
	template<typename T>
	T WriteMemory(DWORD_PTR address, T value)
	{
		return *(T*)address = value;
	}
	template<typename T>
	T ReadMemory(DWORD_PTR address, const T& def = T())
	{
		return *(T*)address;
	}

	void WriteAngles(float TargetX, float TargetY)
	{
		if (hooks::TargetPawn && hooks::LocalPlayerPawn && hooks::LocalPlayerController)
		{
			float X = TargetX / PI; //6.666666666666667f
			float Y = TargetY / PI;
			Y = -(Y);

			*reinterpret_cast<float*>(reinterpret_cast<PBYTE>(LocalPlayerController) + 0x418) = Y;
			*reinterpret_cast<float*>(reinterpret_cast<PBYTE>(LocalPlayerController) + 0x418 + 0x4) = X;
		}
	}


	FVector Subtract(FVector point1, FVector point2)
	{
		FVector vector{ 0, 0, 0 };
		vector.X = point1.X - point2.X;
		vector.Y = point1.Y - point2.Y;
		vector.Z = point1.Z - point2.Z;
		return vector;
	}

	float GetDistance(FVector x, FVector y)
	{
		auto z = Subtract(x, y);
		return sqrt(z.X * z.X + z.Y * z.Y + z.Z * z.Z);
	}

	void SetPlayerVisibility(int VisibilityValue)
	{
		if (hooks::LocalPlayerPawn && hooks::LocalPlayerController)
		{
			hooks::ProcessEvent(hooks::LocalPlayerPawn, addresses::SetPawnVisibility, &VisibilityValue, 0);
		}
	}
	void Teleport(PVOID Pawn, FVector Coords) {

	}


	void Teleport2(FVector Coords)
	{
		if (hooks::LocalPlayerPawn && hooks::LocalPlayerController)
		{
			ProcessEvent(hooks::LocalPlayerController, addresses::ClientSetLocation, &Coords, 0);
		}
	}




	uint64_t BaseAddr = (uint64_t)GetModuleHandleA(NULL);
	int hitbox;

	PVOID ProcessEventHook(UObject* object, UObject* func, PVOID params, PVOID result) {
		if (object && func) {
			auto objectName = Util::GetObjectFirstName((uintptr_t)(uintptr_t)object);
			auto funcName = Util::GetObjectFirstName((uintptr_t)(uintptr_t)func);

			do {

				if (Settings.FastActions)
				{
					if (hooks::LocalPlayerPawn && hooks::LocalPlayerController)
					{
						if (GetAsyncKeyState(0x45))
						{
							setAllToSpeed(220);
						}
						else
						{
							setAllToSpeed(1.0);
						}
					}
				}

				if (Settings.FastReload)
				{
					if (hooks::LocalPlayerPawn && hooks::LocalPlayerController)
					{
						if (GetAsyncKeyState(0x52))
						{
							setAllToSpeed(200);
						}
						else
						{
							setAllToSpeed(1.0);
						}
					}
				}

				if (Settings.AirStuck)
				{
					if (hooks::LocalPlayerPawn && hooks::LocalPlayerController)
					{
						if (GetAsyncKeyState(Settings.AirstuckKey))
						{
							*reinterpret_cast<float*>(reinterpret_cast<PBYTE>(hooks::LocalPlayerPawn) + offsets::Actor::CustomTimeDilation) = 0;
						}
						else
						{
							*reinterpret_cast<float*>(reinterpret_cast<PBYTE>(hooks::LocalPlayerPawn) + offsets::Actor::CustomTimeDilation) = 1;
						}
					}
				}

				if (Settings.CustomSpeedHack)
				{
					if (GetAsyncKeyState(Settings.CustomSpeedKeybind))
					{
						setAllToSpeed(Settings.CustomSpeedValue);
					}
					else
					{
						setAllToSpeed(1.0);
					}
				}

				if (Settings.BulletTP)
				{
					if (Settings.IsBulletTeleporting)
					{
						if (hooks::TargetPawn && hooks::LocalPlayerController)
						{
							if (wcsstr(objectName.c_str(), xorstr(L"B_Prj_Bullet_Sniper")) && funcName == xorstr(L"OnRep_FireStart")) {
								FVector head = { 0 };
								if (!GetTargetHead(head)) {
									break;
								}

								auto root = reinterpret_cast<PBYTE>(ReadPointer(object, offsets::Actor::RootComponent));
								*reinterpret_cast<FVector*>(root + offsets::SceneComponent::RelativeLocation) = head;
								memset(root + offsets::SceneComponent::ComponentVelocity, 0, sizeof(FVector));
							}
						}
					}
				}

				if (Settings.VehicleBoost)
				{
					if (hooks::LocalPlayerPawn)
					{
						if (hooks::IsLocalPlayerInVehicle)
						{
							//helicopter
							*reinterpret_cast<float*>(reinterpret_cast<PBYTE>(hooks::ClosestVehicle) + 0x49f) = 1; //FortHoagieVehicleAnimInstance -> bIsBoostReady
							*reinterpret_cast<float*>(reinterpret_cast<PBYTE>(hooks::ClosestVehicle) + 0x10fc) = 0.001; //FortHoagieVehicle -> BoostCooldown
							*reinterpret_cast<float*>(reinterpret_cast<PBYTE>(hooks::ClosestVehicle) + 0x10f4) = 5; //FortHoagieVehicle -> BoostTimeLeft

							//boat
							*reinterpret_cast<float*>(reinterpret_cast<PBYTE>(hooks::ClosestVehicle) + 0x3f8) = 1; //FortMeatballVehicleAnimInstance -> bIsBoostReady
						}
					}
				}

				//FortniteGame.FortPlayerController.SetRotatePawnToCamera
				//FortniteGame.FortPlayerController.SetFirstPersonCamera
				//bPlayerOutlinesEnabled
				//FortniteGame.FortClientSettingsRecord.SetPlayerOutlinesEnabled

				if (hooks::LocalPlayerPawn && hooks::TargetPawn && hooks::LocalPlayerController)
				{
					if (Settings.AimbotModePos == 0 && wcsstr(funcName.c_str(), xorstr(L"Tick")))
					{
						if (Settings.HitBoxPos == 0) //head
						{
							hitbox = 66;
						}
						else if (Settings.HitBoxPos == 1)
						{
							hitbox = 65;
						}
						else if (Settings.HitBoxPos == 2)
						{
							hitbox = 5;
						}
						else if (Settings.HitBoxPos == 3)
						{
							hitbox = 0;
						}
						else if (Settings.HitBoxPos == 4)
						{
							hitbox = 2;
						}

						int width;
						int height;

						HWND hWnd = FindWindow(0, xorstr(L"Fortnite  "));

						RECT rect;
						if (GetWindowRect(hWnd, &rect))
						{
							width = rect.right - rect.left;
							height = rect.bottom - rect.top;
						}
						int ScreenCenterX = width / 2.0f;
						int ScreenCenterY = height / 2.0f;
						FVector viewPoint = { 0 };
						FVector pos = { 0 };

						Util::GetBoneLocation(hooks::TargetPawn, hitbox, pos);

						auto aimpos = pos;

						if (Settings.Prediction)
						{
							auto entityRootComponent = ReadPointer(TargetPawn, offsets::Actor::RootComponent);
							if (!entityRootComponent) break;

							FVector entityVelocity = ReadFVector(entityRootComponent, offsets::SceneComponent::ComponentVelocity);

							float bulletSpeed = ReadFloat(LocalPlayerCurrentWeapon, 0xd18);

							float time = Distance / (bulletSpeed / 100);

							aimpos.X += entityVelocity.X * time;
							aimpos.Y += entityVelocity.Y * time;
							aimpos.Z += entityVelocity.Z * time;
						}


						aimpos = Util::WorldToScreen(width, height, aimpos);
						float TargetX = aimpos.X - ScreenCenterX;
						float TargetY = aimpos.Y - ScreenCenterY;
						aimpos.X - ScreenCenterX;
						aimpos.Y - ScreenCenterY;

						if (Settings.Prediction)
						{
							TargetX /= Settings.SniperAimbotSlow;
							TargetY /= Settings.SniperAimbotSlow;
						}
						else
						{
							TargetX /= Settings.AimbotSlow;
							TargetY /= Settings.AimbotSlow;
						}
						ProcessEvent(hooks::LocalPlayerController, addresses::AddYawInput, &TargetX, 0);
						ProcessEvent(hooks::LocalPlayerController, addresses::AddPitchInput, &TargetY, 0);
					}
				}
			} while (FALSE);
		}

		return ProcessEvent(object, func, params, result);
	}
	
	float* CalculateShotHook(PVOID arg0, PVOID arg1, PVOID arg2) {
		auto ret = CalculateShot(arg0, arg1, arg2);
		if (ret && Settings.AimbotModePos == 1 && hooks::TargetPawn && hooks::LocalPlayerPawn)
		{
			if (Settings.HitBoxPos == 0) //head
			{
				hitbox = 66;
			}
			else if (Settings.HitBoxPos == 1)
			{
				hitbox = 65;
			}
			else if (Settings.HitBoxPos == 2)
			{
				hitbox = 5;
			}
			else if (Settings.HitBoxPos == 3)
			{
				hitbox = 0;
			}
			else if (Settings.HitBoxPos == 4)
			{
				hitbox = 2;
			}

			FVector head = { 0 };
			Util::GetBoneLocation(hooks::TargetPawn, hitbox, head);

			auto rootPtr = Util::GetPawnRootLocation(hooks::LocalPlayerPawn);
			if (!rootPtr) return ret;
			auto root = *rootPtr;

			auto dx = head.X - root.X;
			auto dy = head.Y - root.Y;
			auto dz = head.Z - root.Z;
			if (dx * dx + dy * dy + dz * dz < 125000.0f) {
				ret[4] = head.X;
				ret[5] = head.Y;
				ret[6] = head.Z;
			}
			else {
				head.Z -= 16.0f;
				root.Z += 45.0f;

				auto y = atan2f(head.Y - root.Y, head.X - root.X);

				root.X += cosf(y + 1.5708f) * 32.0f;
				root.Y += sinf(y + 1.5708f) * 32.0f;

				auto length = Util::SpoofCall(sqrtf, powf(head.X - root.X, 2) + powf(head.Y - root.Y, 2));
				auto x = -atan2f(head.Z - root.Z, length);
				y = atan2f(head.Y - root.Y, head.X - root.X);

				x /= 2.0f;
				y /= 2.0f;

				ret[0] = -(sinf(x) * sinf(y));
				ret[1] = sinf(x) * cosf(y);
				ret[2] = cosf(x) * sinf(y);
				ret[3] = cosf(x) * cosf(y);
			}
		}

		return ret;
	}
	
	INT GetViewPointHook(PVOID player, FMinimalViewInfo* viewInfo, BYTE stereoPass)
	{
		int fovval;
		if (Settings.FovChanger)
		{
			fovval = Settings.FovValue;
		}
		else
		{
			fovval = 80;
		}
		const float upperFOV = 50.534008f;
		const float lowerFOV = 40.0f;

		auto ret = GetViewPoint(player, viewInfo, stereoPass);

		auto fov = viewInfo->FOV;
		auto desired = (((180.0f - upperFOV) / (180.0f - 80.0f)) * (fovval - 80.0f)) + upperFOV;

		if (fov > upperFOV) {
			fov = desired;
		}
		else if (fov > lowerFOV) {
			fov = (((fov - lowerFOV) / (upperFOV - lowerFOV)) * (desired - lowerFOV)) + lowerFOV;
		}
		viewInfo->FOV = fov;

		return ret;
	}
	
	BOOLEAN Initialize()
	{
		// ProcessEvent
		auto addr = Util::FindPattern(xorstr("\xE8\x00\x00\x00\x00\x44\x88\xA3\x00\x00\x00\x00\xEB\x05"), xorstr("x????xxx????xx"));
		addr = reinterpret_cast<decltype(addr)>(RELATIVE_ADDR(addr, 5));

		MH_CreateHook(addr, ProcessEventHook, (PVOID*)&ProcessEvent);
		MH_EnableHook(addr);

		MainGay();

		return TRUE;
	}
}