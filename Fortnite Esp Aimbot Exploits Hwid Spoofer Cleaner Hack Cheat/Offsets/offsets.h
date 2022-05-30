#pragma once




namespace addresses
{
	extern PVOID GetPlayerName;
	extern PVOID SetPawnVisibility;
	extern PVOID ClientSetRotation;
	extern PVOID ClientSetLocation;
	extern PVOID IsInVehicle;
	extern PVOID SetActorRelativeScale3D;
	extern PVOID AddYawInput;
	extern PVOID AddPitchInput;
	extern PVOID GetVehicleActor;
	extern PVOID GetVehicle;

	extern PVOID W2SObject;


	extern PVOID SetForcedLodModel;
	extern PVOID LaunchCharacter;
}



namespace offsets
{

	enum World : uint64_t
	{
		OwningGameInstance = 0x190,
		Levels = 0x148,
	};

	enum Level : uint64_t
	{
		AActors = 0x98,
	};

	enum GameInstance : uint64_t
	{
		LocalPlayers = 0x38,
	};

	enum Player : uint64_t
	{
		PlayerController = 0x30,
	};

	enum PlayerController : uint64_t
	{
		AcknowledgedPawn = 0x2A8,
	};

	enum Pawn : uint64_t
	{
		PlayerState = 0x238,
	};

	enum Actor : uint64_t
	{
		RootComponent = 0x130,
		CustomTimeDilation = 0x98,
	};

	enum Character : uint64_t
	{
		Mesh = 0x280,
	};

	enum SceneComponent : uint64_t
	{
		RelativeLocation = 0x11C,
		ComponentVelocity = 0x140,
	};




	enum FortPawn : uint64_t
	{
		CurrentWeapon = 0x5F8,
		LastFireTime = 0x9E4,
		LastFireTimeVerified = 0x9E8,
	};


	enum AFortWeapon : uint64_t
	{
		bIsReloadingWeapon = 0x2B1
	};

	enum FortPickup : uint64_t
	{
		PrimaryPickupItemEntry = 0x2A0,
	};

	enum FortItemEntry : uint64_t
	{
		ItemDefinition = 0x18,
	};

	enum FortItemDefinition : uint64_t
	{
		DisplayName = 0x88,
		Tier = 0x6C,
	};

	enum FortPlayerStateAthena : uint64_t
	{
		TeamIndex = 0xF28,
	};

	enum FortWeapon : uint64_t
	{
		AmmoCount = 0xA70,
		WeaponData = 0x378,
	};

	enum BuildingContainer : uint64_t
	{
		bAlreadySearched = 0xDF1,
	};

	BOOLEAN Initialize();
}