#include "mMath.h"

const float mMath::Infinity = FLT_MAX;
const float mMath::Pi = 3.1415926535f;

float mMath::RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

float mMath::RandF(float a, float b)
{
	return a + RandF() * (b - a);
}

int mMath::Rand(int a, int b)
{
	return a + rand() % ((b - a) + 1);
}

float mMath::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	//1사분면,4사분면 
	if (x >= 0.0f)
	{
		//  x = 0 이면 atanf(y/x) = +pi/2 if y > 0
		//             atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); //  [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f * Pi; //  [0, 2*pi]
	}

	// 2사분면, 3사분면
	else
		theta = atanf(y / x) + Pi; //  [0, 2*pi]

	return theta;
}

DirectX::XMVECTOR mMath::SphericalToCartesian(float radius, float theta, float phi)
{
	return DirectX::XMVectorSet(radius * sinf(phi) * cosf(theta),radius * cosf(phi),radius * sinf(phi) * sinf(theta),1.0f);
}

DirectX::XMMATRIX mMath::InverseTranspose(DirectX::CXMMATRIX M)
{
	//원,구 같은것 스케일 할때 역행렬로 법선 만들수 있음.

	DirectX::XMMATRIX matrix = M;
	matrix.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(matrix);
	return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, matrix));
}

DirectX::XMFLOAT4X4 mMath::Identity4x4()
{
	static DirectX::XMFLOAT4X4 I(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return I;
}

DirectX::XMVECTOR mMath::RandUnitVec3()
{
	DirectX::XMVECTOR One = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR Zero = DirectX::XMVectorZero();

	// 반구안의 좌표를 얻을 때 까지
	while (true)
	{
		// [-1,1] 임의의 점 생성
		DirectX::XMVECTOR Vec = DirectX::XMVectorSet(mMath::RandF(-1.0f, 1.0f), mMath::RandF(-1.0f, 1.0f), mMath::RandF(-1.0f, 1.0f), 0.0f);

		//균일 한 분포를 얻기 위해 단위 구체 외부의 점을 무시합니다
		//단위 구체 그렇지 않으면 근처의 구에
		//큐브의 모서리
		if (DirectX::XMVector3Greater(DirectX::XMVector3LengthSq(Vec), One))
			continue;

		return DirectX::XMVector3Normalize(Vec);
	}
}

DirectX::XMVECTOR mMath::RandHemisphereUnitVec3(DirectX::XMVECTOR n)
{
	DirectX::XMVECTOR One  = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR Zero = DirectX::XMVectorZero();

	// 반구안의 좌표를 얻을 때 까지
	while(true)
	{
		// [-1,1]^3의 임의의 점
		DirectX::XMVECTOR Vec = DirectX::XMVectorSet(mMath::RandF(-1.0f, 1.0f), mMath::RandF(-1.0f, 1.0f), mMath::RandF(-1.0f, 1.0f), 0.0f);

		
		if(DirectX::XMVector3Greater(DirectX::XMVector3LengthSq(Vec), One) )
			continue;

		// 하단의 반구 점 무시
		if(DirectX::XMVector3Less(DirectX::XMVector3Dot(n, Vec), Zero ) )
			continue;

		return DirectX::XMVector3Normalize(Vec);
	}
}

