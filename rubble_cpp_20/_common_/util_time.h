#pragma once
#ifndef __UTIL_TIME_H__
#define __UTIL_TIME_H__

#include <Windows.h>
#include <time.h>
#include <sys/timeb.h>
#include <sqltypes.h>

#include <initializer_list>

#include <iostream>
#include <string>
#include <format>
#include <source_location>
#include <chrono>

using namespace std;

namespace rubble 
{
const INT32 SEC_A_SEC = 1;
const INT32 SEC_A_MIN = SEC_A_SEC * 60;
const INT32 SEC_A_HOUR = SEC_A_MIN * 60;
const INT32 SEC_A_DAY = SEC_A_HOUR * 24;
const INT32 SEC_A_WEEK = SEC_A_DAY * 7;
const INT32 SEC_A_YEAR = SEC_A_DAY * 365;

const INT64 MSEC_A_SEC = 1000;
const INT64 MSEC_A_MIN = MSEC_A_SEC * 60;
const INT64 MSEC_A_HOUR = MSEC_A_MIN * 60;
const INT64 MSEC_A_DAY = MSEC_A_HOUR * 24;
const INT64 MSEC_A_WEEK = MSEC_A_DAY * 7;
const INT64 MSEC_A_YEAR = MSEC_A_WEEK * 365;

INT32 GetTimeSec();
INT64 GetTimeMilliSec();

const bool default_local = true;

//
class CTime {
private:
	long _tzSec = 0;
	__time64_t _tGMT = 0;
	__time64_t _tLocalT = 0;

	struct tm _tmGMT = {};
	struct tm _tmLocalT = {};

public:
	CTime();
	CTime(__time64_t tTime, bool bLocal = default_local);
	CTime(initializer_list<int> yyMMddhhmmss, bool bLocal = default_local);
	CTime(int year, int month, int day, int hour, int min, int sec, bool bLocal = default_local);
	CTime(TIMESTAMP_STRUCT dbTime, bool bLocal = default_local);

	__time64_t GetTime(bool bLocal = true) { return (bLocal ? _tLocalT : _tGMT); }
	tm* GetTimeStruct(bool bLocal = true) { return (bLocal ? &_tmLocalT : &_tmGMT); }

	void SetTimeZone();

	void SetTime();
	void SetTime(__time64_t tTime, bool bLocal = default_local);
	void SetTime(initializer_list<int> yyMMddhhmmss, bool bLocal = default_local);
	void SetTime(int year, int month, int day, int hour, int min, int sec, bool bLocal = default_local);
	void SetTime(TIMESTAMP_STRUCT dbTime, bool bLocal = default_local);

	void print();
};

//
class CPerformanceChecker {
private:
	chrono::steady_clock::time_point _start = chrono::steady_clock::now();
	source_location _loc;

	string* _pstrBuffer = nullptr;

public:
	CPerformanceChecker(source_location loc = source_location::current());
	CPerformanceChecker(string* pstrBuffer, source_location loc = source_location::current());
	~CPerformanceChecker();
};

void unit_test_time();
} //namespace rubble

#endif //__UTIL_TIME_H__