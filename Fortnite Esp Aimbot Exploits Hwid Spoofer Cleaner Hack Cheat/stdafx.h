#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <psapi.h>
#include <intrin.h>

#include <string>
#include <vector>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>

#include <MinHook.h>
#pragma comment(lib, "minhook.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "xorstr.h"

#include "structs.h"
#include "offsets.h"
#include "util.h"
#include "settings.h"
#include "hooks.h"
#include "renderer.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <map>

namespace Colours
{
	inline bool IsWhiteMenu = false;
	inline bool IsBlackMenu = false;

	inline ImVec4 BoxColor = { 255, 174, 0, 1.f };
	inline ImVec4 FOVCircleColour = { 255.f / 255.f, 255.f / 255.f, 5.f / 255.f, 1 };				// Yellow
	inline ImVec4 BoundingBoxESPColour = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1 };		// White
	inline ImVec4 BoundingBoxESPColourVisible = { 255.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1 };	// Red
	inline ImVec4 SkeletonESPColour = { 255.f / 255.f, 0.f / 255.f, 255.f / 255.f, 1 };			// Purple
	inline ImVec4 GlobalTextColour = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1 };			// White

	inline ImVec4 White = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1 };			// White
	inline ImVec4 Red = { 255.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1 };			// White


	inline ImVec4 FOVColorOutline = { 0, 0, 0, 1.f };
	//inline ImVec4 FOVColor = { 179.f / 179.f, 179.f / 179.f, 179.f / 179.f, 1 };

	inline ImVec4 FOVColor = { 179.f / 255.f, 179.f / 255.f, 179.f / 255.f, 1 };
	inline ImVec4 PlayerColors = { 255.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1 };
}

#define M_PI 3.14159265358979323846264338327950288419716939937510

class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	float x;
	float y;
	float z;

	inline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(Vector3 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	inline double Length() {
		return sqrt(x * x + y * y + z * z);
	}

	inline FRotator ToFRotator() {
		FRotator rot = FRotator();
		float RADPI = (float)(180 / M_PI);
		rot.Yaw = (float)atan2(y, x) * RADPI;
		rot.Pitch = (float)atan2(z, sqrt((x * x) + (y * y))) * RADPI;
		rot.Roll = 0;
		return rot;
	}
	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(float flNum) { return Vector3(x * flNum, y * flNum, z * flNum); }
};
