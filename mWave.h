#pragma once

#include "pch.h"

class mWaves
{
public:
    mWaves(int m, int n, float dx, float dt, float speed, float damping);
    mWaves(const mWaves& rhs) = delete;
    mWaves& operator=(const mWaves& rhs) = delete;
    ~mWaves();

    int RowCount()const;
    int ColumnCount()const;
    int VertexCount()const;
    int TriangleCount()const;
    float Width()const;
    float Depth()const;

    //i��° ���� ���� �ظ� ��ȯ
    const DirectX::XMFLOAT3& Position(int i)const;

    //i��° ���� ������ ���� �ظ� ��ȯ
    const DirectX::XMFLOAT3& Normal(int i)const;

    // ���� x���� ������ i��° �׸��� ������ ���� ź��Ʈ ��ȯ
    const DirectX::XMFLOAT3& TangentX(int i)const;

    void Update(float dt);
    void Disturb(int i, int j, float magnitude);

private:
    int mNumRows = 0;
    int mNumCols = 0;

    int mVertexCount = 0;
    int mTriangleCount = 0;

    // �̸� ����Ҽ� �ִ� �ùķ��̼� ���
    float mK1 = 0.0f;
    float mK2 = 0.0f;
    float mK3 = 0.0f;

    float mTimeStep = 0.0f;
    float mSpatialStep = 0.0f;


    //Ŭ�������� XMVECTOR ��� XMFLOAT �� �����
    //������ �����ε��� �̷����� ��������
    std::vector<DirectX::XMFLOAT3> mPrevSolution;
    std::vector<DirectX::XMFLOAT3> mCurrSolution;
    std::vector<DirectX::XMFLOAT3> mNormals;
    std::vector<DirectX::XMFLOAT3> mTangentX;
};
