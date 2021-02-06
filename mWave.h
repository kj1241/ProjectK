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

    //i번째 격자 점의 해를 반환
    const DirectX::XMFLOAT3& Position(int i)const;

    //i번째 격자 점에서 법선 해를 반환
    const DirectX::XMFLOAT3& Normal(int i)const;

    // 로컬 x축의 방향의 i번째 그리드 점에서 단위 탄젠트 반환
    const DirectX::XMFLOAT3& TangentX(int i)const;

    void Update(float dt);
    void Disturb(int i, int j, float magnitude);

private:
    int mNumRows = 0;
    int mNumCols = 0;

    int mVertexCount = 0;
    int mTriangleCount = 0;

    // 미리 계산할수 있는 시뮬레이션 상수
    float mK1 = 0.0f;
    float mK2 = 0.0f;
    float mK3 = 0.0f;

    float mTimeStep = 0.0f;
    float mSpatialStep = 0.0f;


    //클래스에는 XMVECTOR 대신 XMFLOAT 를 써야함
    //연산자 오버로딩이 이뤄질수 있음으로
    std::vector<DirectX::XMFLOAT3> mPrevSolution;
    std::vector<DirectX::XMFLOAT3> mCurrSolution;
    std::vector<DirectX::XMFLOAT3> mNormals;
    std::vector<DirectX::XMFLOAT3> mTangentX;
};
