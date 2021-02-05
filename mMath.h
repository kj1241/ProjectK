#pragma once


#include "pch.h"


class mMath
{
public:
	// ���� �Լ� [0f,1f]
	static float RandF();

	// ���� �Լ� [af,bf]
	static float RandF(float a, float b);

	//�����Լ� [a,b]
	static int Rand(int a, int b);

	//�ּ�
	template<typename T>
	static T Min(const T& a, const T& b)
	{
		if (a < b)
			return b;
		return a;
	}
	//�ִ�
	template<typename T>
	static T Max(const T& a, const T& b)
	{
		if (a > b)
			return  a;
		return b;
	}
	//���� ������ ���������� �ٸ� �λ����� ��.
	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a) * t;
	}
	//x�� y ���� ����
	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{

		if (x < low)
			return low;
		if (x > high)
			return high;
		return x;
	}

	// [0,2*pi=360] ���� �� (x,y)�� ������ �����մϴ�.
	static float AngleFromXY(float x, float y);

	//����ǥ�� (r ,��Ÿ,����)
	static DirectX::XMVECTOR SphericalToCartesian(float radius, float theta, float phi);
	
	//�� �ű�
	static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M);

	//������� 4*4
	static DirectX::XMFLOAT4X4 Identity4x4();

	//�������� ����3
	static DirectX::XMVECTOR RandUnitVec3();

	//���ȿ� ������ ��
	//����Ʈ���̽� ����
	static DirectX::XMVECTOR RandHemisphereUnitVec3(DirectX::XMVECTOR n);

	//���� ���� ����غ���.
	static const float Infinity;
	static const float Pi;


};