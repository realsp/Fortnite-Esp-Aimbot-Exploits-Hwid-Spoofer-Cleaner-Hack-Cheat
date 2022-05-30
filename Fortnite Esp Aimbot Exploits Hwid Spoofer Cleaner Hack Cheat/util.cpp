#include "stdafx.h"
#include <Windows.h>
#include <stdint.h>

uintptr_t AdolfHitler;

namespace Util {

#define URotationToRadians( URotation )		( ( URotation ) * ( PI / 32768.0f ) ) 
#define URotationToDegree( URotation )		( ( URotation ) * ( 360.0f / 65536.0f ) ) 

#define DegreeToURotation( Degree )			( ( Degree ) * ( 65536.0f / 360.0f ) )
#define DegreeToRadian( Degree )			( ( Degree ) * ( PI / 180.0f ) )

#define RadianToURotation( URotation )		( ( URotation ) * ( 32768.0f / PI ) ) 
#define RadianToDegree( Radian )			( ( Radian ) * ( 180.0f / PI ) )

	uint64_t DiscordBaseAddress = 0;
	GObjects* objects = nullptr;
	uintptr_t UWorld_Address = 0;
	uintptr_t GetNameByIndex = 0;
	uintptr_t BoneMatrix = 0;
	VOID(*FreeInternal)(PVOID) = nullptr;

	FVector CameraLocation;
	FVector CameraRotation;


	VOID CreateConsole() {
		AllocConsole();
		static_cast<VOID>(freopen(xorstr("CONIN$"), xorstr("r"), stdin));
		static_cast<VOID>(freopen(xorstr("CONOUT$"), xorstr("w"), stdout));
		static_cast<VOID>(freopen(xorstr("CONOUT$"), xorstr("w"), stderr));
	}

	bool GetPlayerViewPoint(uintptr_t CameraManager, FVector* vCameraPos, FVector* vCameraRot)
	{
		static uintptr_t pGetPlayerViewPoint = 0;
		if (!pGetPlayerViewPoint)
		{
			uintptr_t VTable = *(uintptr_t*)CameraManager;
			if (!VTable)  return false;

			pGetPlayerViewPoint = *(uintptr_t*)(VTable + 0x788);
			if (!pGetPlayerViewPoint)  return false;
		}

		auto GetPlayerViewPoint = reinterpret_cast<void(__fastcall*)(uintptr_t, FVector*, FVector*)>(pGetPlayerViewPoint);

		SpoofCall(GetPlayerViewPoint, (uintptr_t)CameraManager, vCameraPos, vCameraRot);

		return true;
	}

	BOOLEAN MaskCompare(PVOID buffer, LPCSTR pattern, LPCSTR mask) {
		for (auto b = reinterpret_cast<PBYTE>(buffer); *mask; ++pattern, ++mask, ++b) {
			if (*mask == 'x' && *reinterpret_cast<LPCBYTE>(pattern) != *b) {
				return FALSE;
			}
		}

		return TRUE;
	}

	PBYTE FindPattern(PVOID base, DWORD size, LPCSTR pattern, LPCSTR mask) {
		size -= static_cast<DWORD>(strlen(mask));

		for (auto i = 0UL; i < size; ++i) {
			auto addr = reinterpret_cast<PBYTE>(base) + i;
			if (MaskCompare(addr, pattern, mask)) {
				return addr;
			}
		}

		return NULL;
	}

	PBYTE FindPattern(LPCSTR pattern, LPCSTR mask) {
		MODULEINFO info = { 0 };
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));

		return FindPattern(info.lpBaseOfDll, info.SizeOfImage, pattern, mask);
	}

	bool DataCompare(PBYTE pData, PBYTE bSig, char* szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bSig)
		{
			if (*szMask == 'x' && *pData != *bSig)
				return false;
		}
		return (*szMask) == 0;
	}

	PBYTE FindSignature(PBYTE dwAddress, DWORD dwSize, PBYTE pbSig, char* szMask, long offset)
	{
		size_t length = strlen(szMask);
		for (size_t i = NULL; i < dwSize - length; i++)
		{
			if (DataCompare(dwAddress + i, pbSig, szMask))
				return dwAddress + i + offset;
		}
		return nullptr;
	}

	VOID Free(PVOID buffer) {
		FreeInternal(buffer);
	}

	std::wstring GetObjectFirstName(uintptr_t Object)
	{
		if (!Object) return L"";
		//MessageBoxA(0, "Before fGetObjectNameDefine", 0, 0);
		auto fGetObjName = reinterpret_cast<FString * (__fastcall*)(int* index, FString * res)>(GetNameByIndex);
		//std::string kekfe = std::to_string((uintptr_t)Object) + "    <--  Object";
		//MessageBoxA(0, "Before Getting The Index", 0, 0);
		//MessageBoxA(0, kekfe.c_str(), 0, 0);
		if (IsBadWritePtr((PVOID)(Object + 0x18), (UINT_PTR)8)) return L"";
		int index = *(int*)(Object + 0x18);
		//MessageBoxA(0, "Before DA CALLLLLLLLLL", 0, 0);
		FString internalName;
		SpoofCall(fGetObjName, &index, &internalName);
		//MessageBoxA(0, "Before Checking if valid", 0, 0);
		if (!internalName.c_str()) return L"";
		//MessageBoxA(0, "Before Defining the wstring", 0, 0);
		std::wstring name(internalName.c_str());
		//MessageBoxA(0, "Before the thing with free and check", 0, 0);
		if (internalName.c_str() != NULL)
			Free(internalName.c_str());

		//MessageBoxA(0, "Before RETURN", 0, 0);
		return name;
	}

	std::wstring GetObjectName(UObject* Object) {
		std::wstring name(L"");
		for (auto i = 0; Object; Object = Object->Outer, ++i) {

			auto fGetObjName = reinterpret_cast<FString * (__fastcall*)(int* index, FString * res)>(GetNameByIndex);

			int index = *(int*)(reinterpret_cast<uint64_t>(Object) + 0x18);

			FString internalName; SpoofCall(fGetObjName, &index, &internalName);

			if (!internalName.c_str()) {
				break;
			}


			name = internalName.c_str() + std::wstring(i > 0 ? L"." : L"") + name;
			Free(internalName.c_str());
		}

		return name;
	}

	PVOID FindObject(LPCWSTR name) {
		for (auto array : objects->ObjectArray->Objects) {
			auto fuObject = array;
			std::cout << "";
			for (auto i = 0; i < 0x10000 && fuObject->Object; ++i, ++fuObject)
			{
				std::cout << "";
				auto object = fuObject->Object;
				if (object->ObjectFlags != 0x41) {}
				else {
					std::cout << "";
					if (wcsstr(GetObjectName(object).c_str(), name)) return object;
				}
				std::cout << "";
			}
		}
		return 0;
	}


	VOID GetBoneLocation(PVOID pawn, int bone_index, FVector& out) {
		auto Mesh = ReadPointer(pawn, offsets::Character::Mesh);
		if (!Mesh) return;
		FVector result;
		auto fGetBoneMatrix = ((FMatrix * (__fastcall*)(uintptr_t, FMatrix*, int))(BoneMatrix));
		fGetBoneMatrix((uintptr_t)Mesh, myMatrix, bone_index);

		out.X = myMatrix->M[3][0];
		out.Y = myMatrix->M[3][1];
		out.Z = myMatrix->M[3][2];
	}

	struct APlayerController_ProjectWorldLocationToScreen_Params
	{
		FVector								WorldLocation;
		FVector2D							ScreenLocation;
		bool								bPlayerViewportRelative;
		bool								ReturnValue;
	};

	FVector2D ProjectWorldLocationToScreen(PVOID PlayerController, FVector WorldLocation)
	{
		APlayerController_ProjectWorldLocationToScreen_Params params;
		params.WorldLocation = WorldLocation;
		params.bPlayerViewportRelative = false;

		hooks::ProcessEvent(PlayerController, addresses::W2SObject, &params, 0);

		return params.ScreenLocation;
	}

	FVector WorldToScreen(float width, float height, FVector inPosition) {


		FVector2D ret = {};
		ret = ProjectWorldLocationToScreen(hooks::LocalPlayerController, inPosition);

		return { ret.X, ret.Y, 0 };
	}

	BOOLEAN LineOfSightTo(PVOID PlayerController, PVOID Actor, FVector* ViewPoint) {
		auto LOSTo = reinterpret_cast<bool(__fastcall*)(PVOID PlayerController, PVOID Actor, FVector * ViewPoint)>(AdolfHitler);
		return LOSTo(PlayerController, Actor, ViewPoint);
	}

	FVector* GetPawnRootLocation(PVOID pawn) {
		auto root = ReadPointer(pawn, offsets::Actor::RootComponent);
		if (!root) {
			return nullptr;
		}

		return reinterpret_cast<FVector*>(reinterpret_cast<PBYTE>(root) + offsets::SceneComponent::RelativeLocation);
	}

	FVector2D WorldToRadar(FVector Location, INT RadarPositionX, INT RadarPositionY, int Size, int RadarScale)
	{
		FVector2D Return;

		FLOAT CosYaw = cos(URotationToRadians(CameraRotation.Y));
		FLOAT SinYaw = sin(URotationToRadians(CameraRotation.Y));

		FLOAT DeltaX = Location.X - CameraLocation.X;
		FLOAT DeltaY = Location.Y - CameraLocation.Y;

		FLOAT LocationX = (DeltaY * CosYaw - DeltaX * SinYaw) / (RadarScale + 25);
		FLOAT LocationY = (DeltaX * CosYaw + DeltaY * SinYaw) / (RadarScale + 25);

		if (LocationX > ((Size / 2) - 5.0f) - 2.5f)
			LocationX = ((Size / 2) - 5.0f) - 2.5f;
		else if (LocationX < -(((Size / 2) - 5.0f) - 2.5f))
			LocationX = -(((Size / 2) - 5.0f) - 2.5f);

		if (LocationY > ((Size / 2) - 5.0f) - 2.5f)
			LocationY = ((Size / 2) - 5.0f) - 2.5f;
		else if (LocationY < -(((Size / 2) - 5.0f) - 2.5f))
			LocationY = -(((Size / 2) - 5.0f) - 2.5f);

		Return.X = LocationX + RadarPositionX;
		Return.Y = -LocationY + RadarPositionY;

		return Return;
	}


	float Normalize(float angle) {
		float a = (float)fmod(fmod(angle, 360.0) + 360.0, 360.0);
		if (a > 180.0f) {
			a -= 360.0f;
		}
		return a;
	}

	VOID CalcAngle(float* src, float* dst, float* angles) {
		float rel[3] = {
			dst[0] - src[0],
			dst[1] - src[1],
			dst[2] - src[2],
		};

		auto dist = sqrtf(rel[0] * rel[0] + rel[1] * rel[1] + rel[2] * rel[2]);
		auto yaw = atan2f(rel[1], rel[0]) * (180.0f / PI);
		auto pitch = (-((acosf((rel[2] / dist)) * 180.0f / PI) - 90.0f));

		angles[0] = Normalize(pitch);
		angles[1] = Normalize(yaw);
	}

	BOOLEAN Initialize()
	{
		auto uworld_addr = FindPattern(xorstr("\x48\x89\x05\x00\x00\x00\x00\x48\x8B\x4B\x78"), xorstr("xxx????xxxx"));
		UWorld_Address = reinterpret_cast<decltype(UWorld_Address)>(RELATIVE_ADDR(uworld_addr, 7));

		auto gobjsaddr = FindPattern(xorstr("\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\x0C\xC8\x48\x8B\x04\xD1"), xorstr("xxx????xxxxxxxx"));
		objects = reinterpret_cast<decltype(objects)>(RELATIVE_ADDR(gobjsaddr, 7));

		GetNameByIndex = (uintptr_t)FindPattern(xorstr("\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x48\x8B\xF2\x4C\x8B\xF1\xE8\x00\x00\x00\x00\x45\x8B\x06\x33\xED\x41\x0F\xB7\x16\x41\xC1\xE8\x10\x89\x54\x24\x24\x44\x89\x44\x24\x00\x48\x8B\x4C\x24\x00\x48\xC1\xE9\x20\x8D\x3C\x09\x4A\x03\x7C\xC0\x00\x0F\xB7\x17\xC1\xEA\x06\x41\x39\x6E\x04"), xorstr("xxxx?xxxx?xxxxxxx????xxx????xxxxxxx????xxxxxxx????xxxxxxxxxxxxxxxxxxxxx?xxxx?xxxxxxxxxxx?xxxxxxxxxx"));

		auto bonematrix_addr = FindPattern(xorstr("\xE8\x00\x00\x00\x00\x48\x8B\x47\x30\xF3\x0F\x10\x45"), xorstr("x????xxxxxxxx"));
		BoneMatrix = reinterpret_cast<decltype(BoneMatrix)>(RELATIVE_ADDR(bonematrix_addr, 5));

		auto free_addr = FindPattern(xorstr("\x48\x85\xC9\x0F\x84\x00\x00\x00\x00\x53\x48\x83\xEC\x20\x48\x89\x7C\x24\x30\x48\x8B\xD9\x48\x8B\x3D\x00\x00\x00\x00\x48\x85\xFF\x0F\x84\x00\x00\x00\x00\x48\x8B\x07\x4C\x8B\x40\x30\x48\x8D\x05\x00\x00\x00\x00\x4C\x3B\xC0"), xorstr("xxxxx????xxxxxxxxxxxxxxxx????xxxxx????xxxxxxxxxx????xxx"));
		FreeInternal = reinterpret_cast<decltype(FreeInternal)>(free_addr);

        AdolfHitler = (uintptr_t)FindPattern(xorstr("\x48\x8B\xC4\x48\x89\x58\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\xB8\x0F\x29\x78\xA8\x44\x0F\x29\x40\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x20\x45\x8A\xE9"), xorstr("xxxxxxxxxxxxxxxxxxxxxx?xxx????xxxxxxxxxxxx?xxx????xxxxxxxxxx"));
		return TRUE;
	}
}