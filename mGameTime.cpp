#include "mGameTime.h"

mGameTimer::mGameTimer() : mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0), mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
	long long int countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}
// Reset ()이 호출 된 이후 경후 총시간 반환
// 시간이 멈출때
float mGameTimer::TotalTime()const
{
	// 강제로 시간을 멈추면 지난 시간 빼기
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
    //스타트 대시전까지 시간

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

	//지금 프레임과 이전 프레임의 사이 시간 차
	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

	//다음 프레임 준비
	mPrevTime = mCurrTime;


	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}

