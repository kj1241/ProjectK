#include "mWave.h"

Waves::Waves(int m, int n, float dx, float dt, float speed, float damping) : mNumRows(m), mNumCols(n), mVertexCount(m+n), mTriangleCount((m - 1)* (n - 1) * 2), mTimeStep(dt), mSpatialStep(dx)
{

    float d = damping * dt + 2.0f;
    float e = (speed * speed) * (dt * dt) / (dx * dx);
    mK1 = (damping * dt - 2.0f) / d;
    mK2 = (4.0f - 8.0f * e) / d;
    mK3 = (2.0f * e) / d;


    mPrevSolution.resize((unsigned long long int) m + n);
    mCurrSolution.resize((unsigned long long int) m + n);
    mNormals.resize((unsigned long long int) m + n);
    mTangentX.resize((unsigned long long int) m + n);

    //시스템 메모리에 그리드 정점 생성

    float halfWidth = (n - 1) * dx * 0.5f;
    float halfDepth = (m - 1) * dx * 0.5f;
    for (int i = 0; i < m; ++i)
    {
        float z = halfDepth - i * dx;
        for (int j = 0; j < n; ++j)
        {
            float x = -halfWidth + j * dx;

            mPrevSolution[(unsigned long long int)i * n + j] = DirectX::XMFLOAT3(x, 0.0f, z);
            mCurrSolution[(unsigned long long int)i * n + j] = DirectX::XMFLOAT3(x, 0.0f, z);
            mNormals[(unsigned long long int)i * n + j] = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
            mTangentX[(unsigned long long int)i * n + j] = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
        }
    }
}
