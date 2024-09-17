#pragma once
//#include "../Driver/DRIVER.H"
#include <Windows.h>
#include <iostream>
#include <chrono>
#include <algorithm>
#include "GlobalDefine.h"

//#define DMA

#ifdef Driver
#include "../Driver/mem.h"
#endif // Driver

#ifdef DMA
#include "Memory/Memory.h"
#endif
using namespace std::chrono;

HMODULE hModule;
GameInfoStruct GameInfo;
//inline GameInfoStruct GameInfo;
std::vector<WeaponDataDefinition> WeaponDataBase;
class PseudoInterupt
{
public:
	PseudoInterupt();
	template<typename T>
	bool is_interuption(uint32_t TimeDelta);

private:
	steady_clock::time_point Base;
};

PseudoInterupt::PseudoInterupt() {
	Base = high_resolution_clock::now();
}

template<typename T>
bool PseudoInterupt::is_interuption(uint32_t TimeDelta)
{
	steady_clock::time_point Now = high_resolution_clock::now();
	T duration = duration_cast<T>(Now - this->Base);
	uint32_t cnt = duration.count();
	if (cnt >= TimeDelta) {
		this->Base = high_resolution_clock::now();
		return true;
	}

	return false;
}

namespace offsets
{
	uintptr_t uworld = 0x0;
	uintptr_t mem = 0x0;
	uintptr_t pigg = 0x0;
}

//namespace strings
//{
//	std::string d1 = "";
//	std::string d2 = "";
//	std::string Loader = "https://cdn.discordapp.com/attachments/1134122482060759100/1154122643201597530/d3dGearLoad64.dll";
//}

FVector GetBonePos(uintptr_t USkeletalMeshComponent, uintptr_t TBoneArray, int BoneFname)
{
	FTransform Bone = driver->read<FTransform>(TBoneArray + (BoneFname * 0x60));
	FTransform CompToWorld = driver->read<FTransform>(USkeletalMeshComponent + 0x240);
	D3DMATRIX matrix = matrix_multiplication(Bone.to_matrix(), CompToWorld.to_matrix());
	return FVector(matrix._41, matrix._42, matrix._43);
}

FVector ChildToWorld(FTransform Child, FTransform CompToWorld)
{
	D3DMATRIX matrix = matrix_multiplication(Child.to_matrix(), CompToWorld.to_matrix());
	return FVector(matrix._41, matrix._42, matrix._43);
}


bool ProjectWorldToScreen(const FVector& WorldPosition, const FIntRect& ViewRect, const FMatrix& ViewProjectionMatrix, FVector2D& out_ScreenPos)
{
	FVector4 Result = ViewProjectionMatrix.TransformFVector4(FVector4(WorldPosition.X, WorldPosition.Y, WorldPosition.Z, 1.f));
	float RHW = 1.0f / Result.W;
	if (Result.W < 0.0f) {
		RHW = 1.0f * Result.W;
	}

	FPlane PosInScreenSpace = FPlane(Result.X * RHW, Result.Y * RHW, Result.Z * RHW, Result.W);
	const float NormalizedX = (PosInScreenSpace.X / 2.f) + 0.5f;
	const float NormalizedY = 1.f - (PosInScreenSpace.Y / 2.f) - 0.5f;

	FVector2D RayStartViewRectSpace(
		NormalizedX * static_cast<float>(ViewRect.Width()),
		NormalizedY * static_cast<float>(ViewRect.Height())
	);

	out_ScreenPos = RayStartViewRectSpace + FVector2D(static_cast<float>(ViewRect.Left), static_cast<float>(ViewRect.Top));

	if (Result.W < 0.0f)
	{
		out_ScreenPos *= -1.0f;
	}

	return true;
}

FVector2D FTToW2S(FTransform FT, FTransform CompToWorld)
{
	D3DMATRIX matrix = matrix_multiplication(FT.to_matrix(), CompToWorld.to_matrix());
	FVector Location = FVector(matrix._41, matrix._42, matrix._43);
	FVector2D W2S;
	ProjectWorldToScreen(Location, GameInfo.ViewRect, GameInfo.ViewInfo.ViewProjectionMatrix, W2S);
	return W2S;
}


//bool bIsDbno(uintptr_t AActorPawn)
//{
//	BYTE dbnostatus = driver->read<BYTE>(AActorPawn + 0x93a);
//	return dbnostatus == 93;
//}
// 

float ConvertRadiansToDegrees(float radians)
{
	return radians * (180.0f / M_PI);
}

// Функция нормализации угла в диапазоне [-180, 180]
float NormalizeAngle(float angleDegrees)
{
	while (angleDegrees > 180.0f)
	{
		angleDegrees -= 360.0f;
	}
	while (angleDegrees < -180.0f)
	{
		angleDegrees += 360.0f;
	}
	return angleDegrees;
}

 void GetViewInfo(const FWorldCachedViewInfo& ViewInfo, FVector& OutLocation, FRotator& OutRotation, int Flags)
{
	OutLocation = FVector(ViewInfo.ViewToWorld.M[3][0], ViewInfo.ViewToWorld.M[3][1], ViewInfo.ViewToWorld.M[3][2]);

	double Pitch = atan2(-ViewInfo.ViewMatrix.M[2][1], ViewInfo.ViewMatrix.M[2][2]);
	double Yaw = atan2(ViewInfo.ViewMatrix.M[1][0], ViewInfo.ViewMatrix.M[0][0]);
	double Roll = atan2(ViewInfo.ViewMatrix.M[2][0], sqrt(ViewInfo.ViewMatrix.M[2][1] * ViewInfo.ViewMatrix.M[2][1] + ViewInfo.ViewMatrix.M[2][2] * ViewInfo.ViewMatrix.M[2][2]));

	if (Flags == 0)
	{
		OutRotation.Pitch = static_cast<float>(Pitch);
		OutRotation.Yaw = static_cast<float>(Yaw);
		OutRotation.Roll = static_cast<float>(Roll);
	}
	else if (Flags == 1)
	{
		OutRotation.Pitch = ConvertRadiansToDegrees(static_cast<float>(Pitch));
		OutRotation.Yaw = ConvertRadiansToDegrees(static_cast<float>(Yaw));
		OutRotation.Roll = ConvertRadiansToDegrees(static_cast<float>(Roll));
	}
	else if (Flags == 2)
	{
		OutRotation.Pitch = ConvertRadiansToDegrees(static_cast<float>(Pitch));
		OutRotation.Yaw = ConvertRadiansToDegrees(static_cast<float>(Yaw));
		OutRotation.Roll = ConvertRadiansToDegrees(static_cast<float>(Roll));

		// Нормализация углов
		OutRotation.Pitch = NormalizeAngle(OutRotation.Pitch + 90.0f);
		OutRotation.Yaw = NormalizeAngle(OutRotation.Yaw - 90.0f);
		OutRotation.Roll = NormalizeAngle(OutRotation.Roll);
	}
	else if (Flags == 3)
	{
		OutRotation.Pitch = ConvertRadiansToDegrees(static_cast<float>(Pitch));
		OutRotation.Yaw = ConvertRadiansToDegrees(static_cast<float>(Yaw));
		OutRotation.Roll = ConvertRadiansToDegrees(static_cast<float>(Roll));

		// Нормализация углов
		OutRotation.Pitch = NormalizeAngle(OutRotation.Pitch);
		OutRotation.Yaw = NormalizeAngle(OutRotation.Yaw);
		OutRotation.Roll = NormalizeAngle(OutRotation.Roll);
	}
}

// Функция преобразования радиан в градусы



//double NormalizeAngle(double angle)
//{
//	angle = fmod(angle, 360.0); // Остаток от деления на 360
//	if (angle > 180.0)
//		angle -= 360.0;
//	else if (angle < -180.0)
//		angle += 360.0;
//	return angle;
//}
//
//
//double ConvertRadiansToDegrees(double Radians)
//{
//	double Degrees = Radians * (180.0 / M_PI);
//	if (Degrees < 0.0)
//		Degrees += 360.0;
//	return Degrees;
//}


//void GetViewInfo(const FWorldCachedViewInfo& ViewInfo, FVector& OutLocation, FRotator& OutRotation, int Flags)
//{
//	OutLocation = FVector(ViewInfo.ViewToWorld.M[3][0], ViewInfo.ViewToWorld.M[3][1], ViewInfo.ViewToWorld.M[3][2]);
//	double Pitch = atan2(-ViewInfo.ViewMatrix.M[2][1], ViewInfo.ViewMatrix.M[2][2]);
//	double Yaw = atan2(ViewInfo.ViewMatrix.M[1][0], ViewInfo.ViewMatrix.M[0][0]);
//	double Roll = atan2(ViewInfo.ViewMatrix.M[2][0], sqrt(ViewInfo.ViewMatrix.M[2][1] * ViewInfo.ViewMatrix.M[2][1] + ViewInfo.ViewMatrix.M[2][2] * ViewInfo.ViewMatrix.M[2][2]));
//
//	if (Flags == 0)
//	{
//		OutRotation.Pitch = (Pitch);
//		OutRotation.Yaw = (Yaw);
//		OutRotation.Roll = (Roll);
//	}
//	else if (Flags == 1)
//	{
//		OutRotation.Pitch = ConvertRadiansToDegrees(Pitch);
//		OutRotation.Yaw = ConvertRadiansToDegrees(Yaw);
//		OutRotation.Roll = ConvertRadiansToDegrees(Roll);
//	}
//	else if (Flags == 2)
//	{
//		OutRotation.Pitch = ConvertRadiansToDegrees(Pitch);
//		OutRotation.Yaw = ConvertRadiansToDegrees(Yaw);
//		OutRotation.Roll = ConvertRadiansToDegrees(Roll);
//
//		OutRotation.Pitch = NormalizeAngle(OutRotation.Pitch + 90);
//		OutRotation.Yaw = NormalizeAngle(OutRotation.Yaw - 90);
//		OutRotation.Roll = NormalizeAngle(OutRotation.Roll);
//	}
//}
//

float GetW2SDistance(FVector2D ObjectW2S, FIntRect ViewRect)
{
	auto dx = ObjectW2S.X - (ViewRect.Width() / 2);
	auto dy = ObjectW2S.Y - (ViewRect.Height() / 2);
	auto dist = sqrtf(dx * dx + dy * dy);
	return dist;
}

void ControllWeapon(FVector2D Position, float smooth, uintptr_t AFortWeaponRanged, FIntRect ViewRect, BYTE IsRanged)
{
	double target_x, target_y = 0.0;
	int centerx = ViewRect.Right / 2;
	int centery = ViewRect.Bottom / 2;
	if (Position.X > centerx)
		target_x = -(centerx - Position.X) / 250.0 * 100 / smooth;
	else if (Position.X < centerx)
		target_x = (Position.X - centerx) / 250.0 * 100 / smooth;
	if (Position.Y > centery)
		target_y = -(centery - Position.Y) / 250.0 * 100 / smooth;
	else if (Position.Y < centery)
		target_y = (Position.Y - centery) / 250.0 * 100 / smooth;


	if (IsRanged == 1)
	{
		driver->write<FVector2D>(AFortWeaponRanged + Offsets::Aim + 0x18, FVector2D(target_y * 2, -target_x * 2));
	}
}

std::string DecryptShit(DWORD64TArray FString, bool inGame) {
	int pNameLength;
	WORD* pNameBufferPointer;
	wchar_t* pNameBuffer;
	int i;
	char v25;
	int v26;
	int v29;

	if (!FString.Array) { return "Bot"; }
	if (!FString.ArrayCount) { return "Bot"; }

	pNameLength = FString.ArrayCount;
	pNameLength = std::clamp(pNameLength, 0, 50);
	pNameBuffer = new wchar_t[pNameLength];

	DWORD bytesRead = 0;
	driver->read_mem(FString.Array, PBYTE(pNameBuffer), pNameLength * sizeof(wchar_t));

	if (inGame)
	{

		v25 = pNameLength - 1;
		v26 = 0;
		pNameBufferPointer = (WORD*)pNameBuffer;
		for (i = (v25) & 3; ; *pNameBufferPointer++ += i & 7) {
			v29 = pNameLength - 1;
			if (!(DWORD)pNameLength) {
				v29 = 0;
			}
			if (v26 >= v29) {
				break;
			}
			i += 3;
			++v26;
		}
	}

	std::wstring temp_wstring(pNameBuffer);
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string sItemName = converter.to_bytes(temp_wstring);
	return sItemName;
	//return ("u8%s", std::string(temp_wstring.begin(), temp_wstring.end()));
}

std::string PlayerName(uintptr_t pState, bool inGame) {
	int pNameLength;
	WORD* pNameBufferPointer;
	int i;
	char v25;
	int v26;
	int v29;
	wchar_t* pNameBuffer;
	DWORD64TArray FString;
	if (inGame) {
		uintptr_t pNameStructure = driver->read<uintptr_t>(pState + Offsets::PlayerNameEncrypted);
		//uintptr_t pNameStructure = driver->read<uintptr_t>(pState + 0xAF0);
		if (!pNameStructure) { return "Bot"; }
		FString = driver->read<DWORD64TArray>(pNameStructure + 0x8);
	}
	else {
		FString = driver->read<DWORD64TArray>(pState + 0x328);
	}
	if (!FString.Array) { return "Bot"; }
	if (!FString.ArrayCount) { return "Bot"; }

	pNameLength = FString.ArrayCount;
	pNameLength = std::clamp(pNameLength, 0, 50);
	pNameBuffer = new wchar_t[pNameLength];

	DWORD bytesRead = 0;
	driver->read_mem(FString.Array, PBYTE(pNameBuffer), pNameLength * sizeof(wchar_t));


	v25 = pNameLength - 1;
	v26 = 0;
	pNameBufferPointer = (WORD*)pNameBuffer;
	for (i = (v25) & 3; ; *pNameBufferPointer++ += i & 7) {
		v29 = pNameLength - 1;
		if (!(DWORD)pNameLength) {
			v29 = 0;
		}
		if (v26 >= v29) {
			break;
		}
		i += 3;
		++v26;
	}

	std::wstring temp_wstring(pNameBuffer);
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string sItemName = converter.to_bytes(temp_wstring);
	return sItemName;
	//return ("u8%s", std::string(temp_wstring.begin(), temp_wstring.end()));
}

FVector CalculatePredictedPosition(const FVector& initialPosition, const FVector& initialVelocity, float gravity, float speed, float distance) {
	// Рассчитываем время полета пули
	//float theta = M_PI / 4.0f; // 45 градусов в радианах
	//float t = (2 * speed * sin(theta)) / gravity;

	float SpeedTime = distance / fabs(speed);
	float GravityTime = distance / fabs(gravity);


	// Рассчитываем предсказанную позицию
	FVector predictedPosition;
	predictedPosition.X = initialPosition.X + initialVelocity.X * SpeedTime;
	predictedPosition.Y = initialPosition.Y + initialVelocity.Y * SpeedTime;
	predictedPosition.Z = initialPosition.Z * GravityTime * SpeedTime * GravityTime * SpeedTime;
	predictedPosition.Z = initialPosition.Z + initialVelocity.Z * SpeedTime * GravityTime;

	return predictedPosition;
}


FVector AimbotPrediction(float bulletVelocity, float bulletGravity, float targetDistance, FVector targetPosition, FVector targetVelocity) {
	FVector recalculated = targetPosition;
	float gravity = fabs(bulletGravity);
	float time = targetDistance / fabs(bulletVelocity);
	float bulletDrop = (gravity / 250) * time * time;
	recalculated.Z += bulletDrop * 120;
	recalculated.X += time * (targetVelocity.X);
	recalculated.Y += time * (targetVelocity.Y);
	recalculated.Z += time * (targetVelocity.Z);
	return recalculated;
}

