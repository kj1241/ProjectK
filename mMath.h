#pragma once


#include "pch.h"


class mMath
{
public:
	// 랜덤 함수 [0f,1f]
	static float RandF();

	// 랜덤 함수 [af,bf]
	static float RandF(float a, float b);

	//랜덤함수 [a,b]
	static int Rand(int a, int b);

	//최소
	template<typename T>
	static T Min(const T& a, const T& b)
	{
		if (a < b)
			return b;
		return a;
	}
	//최대
	template<typename T>
	static T Max(const T& a, const T& b)
	{
		if (a > b)
			return  a;
		return b;
	}
	//선형 스케일 한지점에서 다른 두사이의 값.
	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a) * t;
	}
	//x와 y 범위 제한
	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{

		if (x < low)
			return low;
		if (x > high)
			return high;
		return x;
	}

	// [0,2*pi=360] 에서 점 (x,y)의 각도를 리턴합니다.
	static float AngleFromXY(float x, float y);

	//극좌표계 (r ,세타,파이)
	static DirectX::XMVECTOR SphericalToCartesian(float radius, float theta, float phi);
	
	//조 옮김
	static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M);

	//단위행렬 4*4
	static DirectX::XMFLOAT4X4 Identity4x4();

	//랜덤유닛 백터3
	static DirectX::XMVECTOR RandUnitVec3();

	//구안에 랜덤한 점
	//레이트레이싱 기초
	static DirectX::XMVECTOR RandHemisphereUnitVec3(DirectX::XMVECTOR n);

	//전역 변수 고민해보기.
	static const float Infinity;
	static const float Pi;


};