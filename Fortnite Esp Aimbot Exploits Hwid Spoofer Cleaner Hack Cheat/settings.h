#pragma once

typedef struct NIGGER{

	bool chestxd;
	bool headxd;
	bool pelvisxd;
	 bool Tier;
	 bool Legendary;
	 bool Rare;
	 bool Uncommon;
	 bool Common;
	 bool UltraCommon;
	 bool Traps;
	 bool Rifts;
	bool gay = false;
	bool Aimbot;
	bool enablenegro;
	bool AutoAimbot;
	bool SilentAimbot;
	float AimbotFOV;
	int HitBoxPos = 0;
	bool AirStuck;
	int AirstuckKey;
	int FreezetargetKey;
	bool jumptest;
	float AimbotSlow;
	float SniperAimbotSlow;
	bool InstantReload;
	float FOV;
	bool Crosshair;
	bool WaterMark;
	bool MemoryV2;
	bool FastReload;
	bool DrawSelf;
	bool FastActions;
	bool SilentPickaxe;
	float LootDist;
	float ChestDist;
	bool Test;
	bool Rainbow;
	bool Memory;
	bool Silent;
	bool NoRecoil;
	bool AimWhileJumping;
	bool Prediction;
	bool Head;
	bool RapidFire1;
	bool RestoreRotation;
	bool SilentTP;
	bool FirstPerson;
	bool BoatTeleport;
	bool FovChanger;
	float NoSpreadValue;
	bool cartp;
	bool Crosshair2;
	bool NoClip;
	float FovValue;
	bool PlayerTP;
	bool TargetLine;
	bool OutlineText;
	bool PlayerFly;
	bool Freecam;
	int AimbotModePos;
	bool ColorAdjuster;

	int CrosshairThickness;
	int CrosshairScale;
	int vehiclespeed;
	bool CustomSpeedHack;
	int CustomSpeedValue;
	int CustomSpeedKeybind;

	bool MemesTest;
	bool TestKek;
	bool WarningEnemiesArea;
	bool ShowInfoBar;
	bool Info;
	bool BulletTP;
	bool IsBulletTeleporting;

	bool VisibleCheck;
	bool FPS;
	bool Spinbot;
	int SpinKey;
	float SpinSpeed;
	int OverallDistance;
	bool VehicleTeleport;
	bool VehicleTeleporter;
	int AimKey;
	bool BigPlayers;

	float BoatFlySpeed;
	int BoatFlyKey;
	bool BoatFly;

	bool InActionTeleport;

	int SpinbotPitchMode;
	int SpinbotYawMode;
	int SnaplineStartPoint;

	bool RapidFire;
    float RapidFireValueL = 0.001f;


	float RFMod;

	bool NoSpread;

	bool TeleportToEnemies;
	int EnemyTeleportKey;

	bool VehicleBoost;
	int gayy;
	float espdist;
	struct {


		bool AimbotFOV;
		bool Boxes;
		bool Visuals;
		bool Skeletons;
		bool realskel;
		bool PlayerLines;
		float snaplinesdistance;
		bool streamproof;
		bool PlayerNames = true;
		bool PlayerWeapons;
		bool PlayerAmmo;
		bool LLamas;
		int RadarDistance;
		bool Memes;
		bool Tracked;
		bool Distance;
		bool Radar;
		bool TestChams;
		bool cornerbox;
		bool three;
		bool Chams;
		float SnaplinePower;
		bool ThreeDBox;
		float PlayerNameVisibleColor[4];
		float PlayerNameNotVisibleColor[4];
		float PlayerNameColor[4];
		float BoxVisibleColor[4];
		float BoxNotVisibleColor[4];
		float SnaplineVisibleColor[4];
		float SnaplineNotVisibleColor[4];
		float SkeletonVisibleColor[4];
		float SkeletonNotVisibleColor[4];
		float FovColor[4];
		float TargetLineColor[4];
		bool debug ;
		bool debug2;
		bool Ammo;
		bool AmmoBox;
		bool Containers;
		bool Weapons;
		bool Vehicles;
		bool WeakSpots;
		bool SupplyDrops;
		INT MinWeaponTier;
	} ESP;
} SETTINGS;

extern SETTINGS Settings;

namespace SettingsHelper
{
	VOID LoadSavedConfig();
	VOID SaveConfig();
	VOID LoadDefaultConfig();
	VOID LoadTheme();
}