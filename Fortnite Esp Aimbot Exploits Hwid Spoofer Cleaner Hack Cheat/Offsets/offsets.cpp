#include "stdafx.h"

namespace addresses
{
	PVOID GetPlayerName = 0;
	PVOID SetPawnVisibility = 0;
	PVOID ClientSetRotation = 0;
	PVOID K2_TeleportTo = 0;
	PVOID ClientSetLocation = 0;
	PVOID IsInVehicle = 0;
	PVOID SetActorRelativeScale3D = 0;
	PVOID SetMouseLocation = 0;
	PVOID AddYawInput = 0;
	PVOID AddPitchInput = 0;
	PVOID GetVehicleActor = 0;
	PVOID GetVehicle = 0;
	PVOID K2_AttachToActor = 0;
	PVOID W2SObject = 0;
	PVOID SetForcedLodModel = 0;
	PVOID LaunchCharacter = 0;
}

namespace offsets
{
	BOOLEAN Initialize()
	{
		uint64_t BaseAddress = (uint64_t)GetModuleHandleA(NULL);
		addresses::GetPlayerName = Util::FindObject(xorstr(L"GetPlayerName"));
		addresses::SetPawnVisibility = Util::FindObject(xorstr(L"SetPawnVisibility"));
		addresses::ClientSetRotation = Util::FindObject(xorstr(L"ClientSetRotation"));
		addresses::K2_TeleportTo = Util::FindObject(xorstr(L"K2_TeleportTo"));
		addresses::ClientSetLocation = Util::FindObject(xorstr(L"ClientSetLocation"));
		addresses::SetActorRelativeScale3D = Util::FindObject(xorstr(L"SetActorRelativeScale3D"));
		addresses::IsInVehicle = Util::FindObject(xorstr(L"IsInVehicle"));
		addresses::AddYawInput = Util::FindObject(xorstr(L"AddYawInput"));
		addresses::AddPitchInput = Util::FindObject(xorstr(L"AddPitchInput"));
		addresses::K2_AttachToActor = Util::FindObject(xorstr(L"K2_AttachToActor"));
		addresses::W2SObject = Util::FindObject(xorstr(L"ProjectWorldLocationToScreen"));
		addresses::SetForcedLodModel = Util::FindObject(xorstr(L"SetForcedLodModel"));
		addresses::LaunchCharacter = Util::FindObject(xorstr(L"LaunchCharacter"));

		return TRUE;
	}
}