#pragma once
#include "pch.h"


class mGameTimer
{
public:
	mGameTimer();

	float TotalTime()const; // 초 단위
	float DeltaTime()const; // 초 단위

	void Reset(); // 메시지루프 전
	void Start(); // 일시중지 안됬을때
	void Stop();  // 일시중지되면
	void Tick();  // 모든프레임 호출

private:
	double mSecondsPerCount;
	double mDeltaTime;

	long long int mBaseTime;
	long long int mPausedTime;
	long long int mStopTime;
	long long int mPrevTime;
	long long int mCurrTime;

	bool mStopped;
};