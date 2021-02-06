#include "mWave.h"

mWaves::mWaves(int m, int n, float dx, float dt, float speed, float damping) : mNumRows(m), mNumCols(n), mVertexCount(m+n), mTriangleCount((m - 1)* (n - 1) * 2), mTimeStep(dt), mSpatialStep(dx)
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

    //�ý��� �޸𸮿� �׸��� ���� ����

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

mWaves::~mWaves()
{
}

int mWaves::RowCount() const
{
    return mNumRows;
}

int mWaves::ColumnCount() const
{
    return mNumCols;
}

int mWaves::VertexCount() const
{
    return mVertexCount;
}

int mWaves::TriangleCount() const
{
    return mTriangleCount;
}

float mWaves::Width() const
{
    return mNumCols * mSpatialStep;
}

float mWaves::Depth() const
{
    return mNumRows * mSpatialStep;
}

const DirectX::XMFLOAT3& mWaves::Position(int i) const
{
    return mCurrSolution[i];
}

const DirectX::XMFLOAT3& mWaves::Normal(int i) const
{
    return mNormals[i];
}

const DirectX::XMFLOAT3& mWaves::TangentX(int i) const
{
    return mTangentX[i];
}

void mWaves::Update(float dt)
{
	static float t = 0;

	// �ð� ��ô
	t += dt;

	// ������ �ð� �ܰ迡���� �ùķ��̼��� ������Ʈ
	if (t >= mTimeStep)
	{
		// �������� ������Ʈ ���� �������
		concurrency::parallel_for(1, mNumRows - 1, [this](unsigned long long int i) //���� ó���� ���� �˰���
			//for(int i = 1; i < mNumRows-1; ++i)
			{
				for (unsigned long long int j = 1; j < (unsigned long long int)mNumCols - 1; ++j)
				{
					//������Ʈ ���ķ� �������۴� �ش���۷� ������Ʈ
					mPrevSolution[i * mNumCols + j].y = mK1 * mPrevSolution[i * mNumCols + j].y + mK2 * mCurrSolution[i * mNumCols + j].y + mK3 * (mCurrSolution[(i + 1) * mNumCols + j].y + mCurrSolution[(i - 1) * mNumCols + j].y + mCurrSolution[i * mNumCols + j + 1].y + mCurrSolution[i * mNumCols + j - 1].y);
				}
			});

		// ������Ʈ�������� �� �ַ����� ������ �Ǿ�ߵ�.
		std::swap(mPrevSolution, mCurrSolution);

		t = 0.0f; // �ð� �ʱ�ȭ

		//�������
		concurrency::parallel_for(1, mNumRows - 1, [this](unsigned long long int i) //����ó���� ����
			//for(int i = 1; i < mNumRows - 1; ++i)
			{
				for (unsigned long long int j = 1; j < (unsigned long long int)mNumCols - 1; ++j)
				{
					float l = mCurrSolution[i * mNumCols + j - 1].y;
					float r = mCurrSolution[i * mNumCols + j + 1].y;
					float t = mCurrSolution[(i - 1) * mNumCols + j].y;
					float b = mCurrSolution[(i + 1) * mNumCols + j].y;
					mNormals[i * mNumCols + j].x = -r + l;
					mNormals[i * mNumCols + j].y = 2.0f * mSpatialStep;
					mNormals[i * mNumCols + j].z = b - t;

					XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&mNormals[i * mNumCols + j]));
					XMStoreFloat3(&mNormals[i * mNumCols + j], n);

					mTangentX[i * mNumCols + j] = XMFLOAT3(2.0f * mSpatialStep, r - l, 0.0f);
					XMVECTOR T = XMVector3Normalize(XMLoadFloat3(&mTangentX[i * mNumCols + j]));
					XMStoreFloat3(&mTangentX[i * mNumCols + j], T);
				}
			});
	}
}

void mWaves::Disturb(int i, int j, float magnitude)
{
	// ��踦 ����
	assert(i > 1 && i < mNumRows - 2);
	assert(j > 1 && j < mNumCols - 2);

	float halfMag = 0.5f * magnitude;

	// i,j��° ���� ���̿� ��ó ����
	mCurrSolution[(unsigned long long int)i * mNumCols + j].y += magnitude;
	mCurrSolution[(unsigned long long int)i * mNumCols + j + 1].y += halfMag;
	mCurrSolution[(unsigned long long int)i * mNumCols + j - 1].y += halfMag;
	mCurrSolution[(unsigned long long int)((unsigned long long int)i + 1) * mNumCols + j].y += halfMag;
	mCurrSolution[(unsigned long long int)((unsigned long long int)i - 1) * mNumCols + j].y += halfMag;
}
