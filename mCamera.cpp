#include "mCamera.h"

mCamera::mCamera()
{
}

mCamera::~mCamera()
{
}

void mCamera::UpdateCamera(const mGameTimer& gt)
{
	//����ǥ�� ��ī��Ʈ ��ǥ�� �ٲ�
	mEyePos.x = mRadius * sinf(mPhi) * cosf(mTheta);
	mEyePos.z = mRadius * sinf(mPhi) * sinf(mTheta);
	mEyePos.y = mRadius * cosf(mPhi);
	//���Ʈ���� ����
	DirectX::XMVECTOR pos = XMVectorSet(mEyePos.x, mEyePos.y, mEyePos.z, 1.0f);
	DirectX::XMVECTOR target = XMVectorZero();
	DirectX::XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);

}
