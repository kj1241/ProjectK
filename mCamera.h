#pragma once
#include "pch.h"
#include "mGameTime.h"
#include "mMath.h"


class mCamera
{
	DirectX::XMFLOAT3 mEyePos = { 0.0f, 0.0f, 0.0f };
	XMFLOAT4X4 mView = mMath::Identity4x4();
	float mPhi = XM_PIDIV2 - 0.1f;
	float mTheta = 1.5f * XM_PI;
	float mRadius = 50.0f;


public:
	mCamera();
	~mCamera();
	void UpdateCamera(const mGameTimer& gt);



};