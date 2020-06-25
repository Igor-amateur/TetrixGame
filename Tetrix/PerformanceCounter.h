﻿// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#include <windows.h>

class CStopwatch
{
public:
	CStopwatch();
	void Start();
	int Now();
	int Time();
private:
	LARGE_INTEGER m_liPerfFreq;
	LARGE_INTEGER m_liPerfStart;
};

#ifdef MY_PERFORMENCE_COUNTER

CStopwatch::CStopwatch()
{
	QueryPerformanceFrequency(&m_liPerfFreq);
	Start();
}

void CStopwatch::Start()
{
	QueryPerformanceCounter(&m_liPerfStart);
}

int CStopwatch::Now()
{
	// возвращает число миллисекунд после вызова Start
	LARGE_INTEGER liPerfNow;
	QueryPerformanceCounter(&liPerfNow);
	return  static_cast<int>(((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) / m_liPerfFreq.QuadPart);
}

int CStopwatch::Time()
{
	LARGE_INTEGER liPerfNow;
	QueryPerformanceCounter(&liPerfNow);
	return static_cast<int>((liPerfNow.QuadPart * 1000) / m_liPerfFreq.QuadPart);
}

#endif