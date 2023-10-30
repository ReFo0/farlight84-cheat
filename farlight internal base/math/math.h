#pragma once
#include <unordered_set>
#include <cerrno>
#include <cfenv>
#include <cstring>
#include <iostream>
#include <cmath>
#include <d3d9.h>
#include "../../../../../../Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3dx9math.h"
#define powFFFFFFFFFFFFFFFFFFFFFF(n) (n)*(n)
#define M_PI 3.14159265358979323846264338327950288419716939937510

struct vec2
{
	float x, y;
};

struct vec3
{

	inline static float sqrtf(float number)
	{
		long i;
		float x2, y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		y = number;
		i = *(long*)&y;
		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;
		y = y * (threehalfs - (x2 * y * y));
		y = y * (threehalfs - (x2 * y * y));

		return 1 / y;
	}

	float distance(vec3 vec)
	{
		return sqrt(
			powFFFFFFFFFFFFFFFFFFFFFF(vec.x - x, 2) +
			powFFFFFFFFFFFFFFFFFFFFFF(vec.y - y, 2)
		);
	}

	inline float Distance(vec3 v)
	{
		float x = this->x - v.x;
		float y = this->y - v.y;
		float z = this->z - v.z;

		return sqrtf((x * x) + (y * y) + (z * z)) * 0.03048f;
	}


	vec3()
	{
		x = y = z = 0.f;
	}

	vec3(float fx, float fy, float fz)
	{
		x = fx;
		y = fy;
		z = fz;
	}

	inline float operator[](int i) const {
		return ((float*)this)[i];
	}

	inline float Length() const
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	float x, y, z;

	vec3 operator+(const vec3& input) const
	{
		return vec3{ x + input.x, y + input.y, z + input.z };
	}

	vec3 operator-(const vec3& input) const
	{
		return vec3{ x - input.x, y - input.y, z - input.z };
	}

	vec3 operator/(float input) const
	{
		return vec3{ x / input, y / input, z / input };
	}

	vec3 operator*(float input) const
	{
		return vec3{ x * input, y * input, z * input };
	}

	vec3& operator+=(const vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	vec3& operator-=(const vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	vec3& operator/=(float input)
	{
		x /= input;
		y /= input;
		z /= input;
		return *this;
	}

	vec3& operator*=(float input)
	{
		x *= input;
		y *= input;
		z *= input;
		return *this;
	}

	vec3 midPoint(vec3 v2)
	{
		return vec3((x + v2.x) / 2, (y + v2.y) / 2, (z + v2.z) / 2);
	}

	bool operator==(const vec3& input) const
	{
		return x == input.x && y == input.y && z == input.z;
	}



	float clamp0to1(float value)
	{
		float result;
		if (value < 0)
		{
			result = 0;
		}
		else if (value > 1.f)
		{
			result = 1.f;
		}
		else
		{
			result = value;
		}
		return result;
	}

	float Lerp()
	{
		return x + (y - x) * clamp0to1(z);
	}

	float length_sqr() const
	{
		return (x * x) + (y * y) + (z * z);
	}

	float length() const
	{
		return (float)sqrt(length_sqr());
	}

	float length_2d() const
	{
		return (float)sqrt((x * x) + (y * y));
	}

	auto length() -> float { return sqrtf((x * x) + (y * y) + (z * z)); }

	vec3 normalized()
	{
		float len = length();
		return vec3(x / len, y / len, z / len);
	}

	vec3 cross(vec3 rhs)
	{
		return vec3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
	}



	float dot(vec3 input) const
	{
		return (x * input.x) + (y * input.y) + (z * input.z);
	}


	auto empty() -> bool { return x == 0.000000 && y == 0.000000 && z == 0.000000; }

	bool is_valid() const
	{
		return !(x == 0.f && y == 0.f && z == 0.f) || (x == -1.f && y == -1.f && z == -1.f);
	}

	inline float Dot(vec3 v) {
		return x * v.x + y * v.y + z * v.z;
	}







	float distance_to(const vec3& other) {
		vec3 delta;
		delta.x = x - other.x;
		delta.y = y - other.y;
		delta.z = z - other.z;

		return delta.length();
	}
};

float distancee(vec3 vec)
{
	return sqrt(pow(vec.x - vec.x, 2) + pow(vec.y - vec.y, 2));
}

struct FQuat
{
	float x;
	float y;
	float z;
	float w;
};


struct FTransform
{
	FQuat rot;
	vec3 translation;
	char pad[4];
	vec3 scale;
	char pad1[4];

	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

struct FVector
{
	float                                              X;                                                         // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                              Y;                                                         // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                              Z;                                                         // 0x0008(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)


	inline FVector() : X(0), Y(0), Z(0) {}

	inline FVector(float x, float y, float z) : X(x), Y(y), Z(z) {}

	inline FVector operator + (const FVector& other) const { return FVector(X + other.X, Y + other.Y, Z + other.Z); }

	inline FVector operator - (const FVector& other) const { return FVector(X - other.X, Y - other.Y, Z - other.Z); }

	inline FVector operator * (float scalar) const { return FVector(X * scalar, Y * scalar, Z * scalar); }

	inline FVector operator * (const FVector& other) const { return FVector(X * other.X, Y * other.Y, Z * other.Z); }

	inline FVector operator / (float scalar) const { return FVector(X / scalar, Y / scalar, Z / scalar); }

	inline FVector operator / (const FVector& other) const { return FVector(X / other.X, Y / other.Y, Z / other.Z); }

	inline FVector& operator=  (const FVector& other) { X = other.X; Y = other.Y; Z = other.Z; return *this; }

	inline FVector& operator+= (const FVector& other) { X += other.X; Y += other.Y; Z += other.Z; return *this; }

	inline FVector& operator-= (const FVector& other) { X -= other.X; Y -= other.Y; Z -= other.Z; return *this; }

	inline FVector& operator*= (const float other) { X *= other; Y *= other; Z *= other; return *this; }

	inline float Dot(const FVector& b) const { return (X * b.X) + (Y * b.Y) + (Z * b.Z); }

	inline float MagnitudeSqr() const { return Dot(*this); }

	inline float Magnitude() const { return std::sqrtf(MagnitudeSqr()); }

	inline FVector Unit() const
	{
		const float fMagnitude = Magnitude();
		return FVector(X / fMagnitude, Y / fMagnitude, Z / fMagnitude);
	}


	friend bool operator==(const FVector& first, const FVector& second) { return first.X == second.X && first.Y == second.Y && first.Z == second.Z; }

	friend bool operator!=(const FVector& first, const FVector& second) { return !(first == second); }

};
struct FRotator
{
	float                                              Pitch;                                                     // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                              Yaw;                                                       // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                              Roll;                                                      // 0x0008(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)


	inline FRotator() : Pitch(0), Yaw(0), Roll(0) {}

	inline FRotator(float pitch, float yaw, float roll) : Pitch(pitch), Yaw(yaw), Roll(roll) {}

	inline float Size() { return sqrt(Pitch * Pitch + Yaw * Yaw + Roll * Roll); }

	inline FRotator Clamp()
	{
		FRotator r = { Pitch, Yaw, Roll };
		if (r.Yaw > 180.f)
			r.Yaw -= 360.f;
		else if (r.Yaw < -180.f)
			r.Yaw += 360.f;

		if (r.Pitch > 180.f)
			r.Pitch -= 360.f;
		else if (r.Pitch < -180.f)
			r.Pitch += 360.f;

		if (r.Pitch < -89.f)
			r.Pitch = -89.f;
		else if (r.Pitch > 89.f)
			r.Pitch = 89.f;

		r.Roll = 0.f;
		return r;
	}

	inline FRotator operator + (const FRotator& other) const { return FRotator(Pitch + other.Pitch, Yaw + other.Yaw, Roll + other.Roll); }

	inline FRotator operator - (const FRotator& other) const { return FRotator(Pitch - other.Pitch, Yaw - other.Yaw, Roll - other.Roll); }

	inline FRotator operator * (float scalar) const { return FRotator(Pitch * scalar, Yaw * scalar, Roll * scalar); }

	inline FRotator operator * (const FRotator& other) const { return FRotator(Pitch * other.Pitch, Yaw * other.Yaw, Roll * other.Roll); }

	inline FRotator operator / (float scalar) const { return FRotator(Pitch / scalar, Yaw / scalar, Roll / scalar); }

	inline FRotator operator / (const FRotator& other) const { return FRotator(Pitch / other.Pitch, Yaw / other.Yaw, Roll / other.Roll); }

	inline FRotator& operator=  (const FRotator& other) { Pitch = other.Pitch; Yaw = other.Yaw; Roll = other.Roll; return *this; }

	inline FRotator& operator+= (const FRotator& other) { Pitch += other.Pitch; Yaw += other.Yaw; Roll += other.Roll; return *this; }

	inline FRotator& operator-= (const FRotator& other) { Pitch -= other.Pitch; Yaw -= other.Yaw; Roll -= other.Roll; return *this; }

	inline FRotator& operator*= (const float other) { Pitch *= other; Yaw *= other; Roll *= other; return *this; }


};
struct FVector2D
{
	float                                              X;                                                         // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                              Y;                                                         // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)


	inline FVector2D() : X(0), Y(0) {}

	inline FVector2D(float x, float y) : X(x), Y(y) {}

	inline FVector2D operator + (const FVector2D& other) const { return FVector2D(X + other.X, Y + other.Y); }

	inline FVector2D operator - (const FVector2D& other) const { return FVector2D(X - other.X, Y - other.Y); }

	inline FVector2D operator * (float scalar) const { return FVector2D(X * scalar, Y * scalar); }

	inline FVector2D operator * (const FVector2D& other) const { return FVector2D(X * other.X, Y * other.Y); }

	inline FVector2D operator / (float scalar) const { return FVector2D(X / scalar, Y / scalar); }

	inline FVector2D operator / (const FVector2D& other) const { return FVector2D(X / other.X, Y / other.Y); }

	inline FVector2D& operator=  (const FVector2D& other) { X = other.X; Y = other.Y; return *this; }

	inline FVector2D& operator+= (const FVector2D& other) { X += other.X; Y += other.Y; return *this; }

	inline FVector2D& operator-= (const FVector2D& other) { X -= other.X; Y -= other.Y; return *this; }

	inline FVector2D& operator*= (const float other) { X *= other; Y *= other; return *this; }

};

class FMinimalViewInfo
{
public:
	FVector Location; // 0x00(0x0c)
	FRotator Rotation; // 0x0c(0x0c)
	float FOV; // 0x18(0x04)
	float DesiredFOV; // 0x1c(0x04)
	float OrthoWidth; // 0x20(0x04)
	float OrthoNearClipPlane; // 0x24(0x04)
	float OrthoFarClipPlane; // 0x28(0x04)
	float AspectRatio; // 0x2c(0x04)
};



struct FMinimalViewInfoo2
{
	vec3 Location;
	vec3 Rotation;
	float FOV;
};
enum bones
{
	Root = 0,
	Bip001 = 1,
	pelvis = 2,
	spine_01 = 3,
	spine_02 = 4,
	spine_03 = 5,
	clavicle_l = 6,
	upperarm_l = 7,
	lowerarm_l = 8,
	hand_l = 9,
	thumb_01_l = 10,
	thumb_02_l = 11,
	thumb_03_l = 12,
	index_01_l = 13,
	index_02_l = 14,
	index_03_l = 15,
	middle_01_l = 16,
	middle_02_l = 17,
	middle_03_l = 18,
	ring_01_l = 19,
	ring_02_l = 20,
	ring_03_l = 21,
	pinky_01_l = 22,
	pinky_02_l = 23,
	pinky_03_l = 24,
	clavicle_r = 25,
	upperarm_r = 26,
	lowerarm_r = 27,
	hand_r = 28,
	thumb_01_r = 29,
	thumb_02_r = 30,
	thumb_03_r = 31,
	index_01_r = 32,
	index_02_r = 33,
	index_03_r = 34,
	middle_01_r = 35,
	middle_02_r = 36,
	middle_03_r = 37,
	ring_01_r = 38,
	ring_02_r = 39,
	ring_03_r = 40,
	pinky_01_r = 41,
	pinky_02_r = 42,
	pinky_03_r = 43,
	RightHandWeaponAttach = 44,
	neck_01 = 45,
	head = 46,
	BackpackAttach = 47,
	thigh_l = 48,
	calf_l = 49,
	foot_l = 50,
	ball_l = 51,
	thigh_r = 52,
	calf_r = 53,
	foot_r = 54,
	ball_r = 55,
	VB_spine_03_RightHandWeaponAttach = 56,
	VB_VB_spine_03_RightHandWeaponAttach_hand_r = 57,
	VB_VB_VB_spine_03_RightHandWeaponAttach_hand_r_lowerarm_r = 58,
};


D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}
