#pragma once
#include "pch.h"


class mGameTimer
{
public:
	mGameTimer();

	float TotalTime()const; // �� ����
	float DeltaTime()const; // �� ����

	void Reset(); // �޽������� ��
	void Start(); // �Ͻ����� �ȉ�����
	void Stop();  // �Ͻ������Ǹ�
	void Tick();  // ��������� ȣ��

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