#include "mGameTime.h"

mGameTimer::mGameTimer() : mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0), mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
	long long int countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}
// Reset ()�� ȣ�� �� ���� ���� �ѽð� ��ȯ
// �ð��� ���⶧
float mGameTimer::TotalTime()const
{
	// ������ �ð��� ���߸� ���� �ð� ����
	if (mStopped)
	{
		return (float)(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
	else
	{
		return (float)(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
}

float mGameTimer::DeltaTime()const
{
	return (float)mDeltaTime;
}

void mGameTimer::Reset()
{
	long long int currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void mGameTimer::Start()
{
	long long int startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
    //��ŸƮ ��������� �ð�

	if (mStopped)
	{
		mPausedTime += (startTime - mStopTime);

		mPrevTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

void mGameTimer::Stop()
{
	if (!mStopped)
	{
		long long int currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopTime = currTime;
		mStopped = true;
	}
}

void mGameTimer::Tick()
{
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	//���� �����Ӱ� ���� �������� ���� �ð� ��
	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

	//���� ������ �غ�
	mPrevTime = mCurrTime;


	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}

