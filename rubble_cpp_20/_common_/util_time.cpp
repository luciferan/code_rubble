#include "./util_time.h"

namespace rubble 
{
INT32 GetTimeSec()
{
	struct __timeb64 tbTime;
	_ftime64_s(&tbTime);
	return (INT32)(tbTime.time);
}

INT64 GetTimeMilliSec()
{
	struct __timeb64 tbTime;
	_ftime64_s(&tbTime);
	return (INT64)(tbTime.time * 1000 + tbTime.millitm);
}

//
CTime::CTime()
{
	SetTimeZone();
	SetTime();
}

CTime::CTime(__time64_t tTime, bool bLocal /*= default_local*/)
{
	SetTimeZone();
	SetTime(tTime, bLocal);
}

CTime::CTime(initializer_list<int> yyMMddhhmmss, bool bLocal /*= default_local*/)
{
	SetTimeZone();
	SetTime(yyMMddhhmmss, bLocal);
}

CTime::CTime(int year, int month, int day, int hour, int min, int sec, bool bLocal /*= default_local*/)
{
	SetTimeZone();
	SetTime(year, month, day, hour, min, sec, bLocal);
}

CTime::CTime(TIMESTAMP_STRUCT dbTime, bool bLocal /*= default_local*/)
{ 
	SetTimeZone();
	SetTime(dbTime, bLocal);
}

void CTime::SetTimeZone()
{
	_tzset();
	_get_timezone(&_tzSec);
}

void CTime::SetTime()
{
	_time64(&_tGMT);
	_gmtime64_s(&_tmGMT, &_tGMT);
	_localtime64_s(&_tmLocalT, &_tGMT);
	_tLocalT = _mkgmtime64(&_tmLocalT);
}

void CTime::SetTime(__time64_t tTime, bool bLocal /*= default_local*/)
{
	if (bLocal) {
		_tGMT = tTime + _tzSec;
		_tLocalT = tTime;
		_gmtime64_s(&_tmGMT, &_tGMT);
		_gmtime64_s(&_tmLocalT, &_tLocalT);
	} else {
		_tGMT = tTime;
		_tLocalT = tTime - _tzSec;
		_gmtime64_s(&_tmGMT, &_tGMT);
		_gmtime64_s(&_tmLocalT, &_tLocalT);
	}
}

void CTime::SetTime(initializer_list<int> yyMMddhhmmss, bool bLocal /*= default_local*/)
{
	vector<int> t = { 1900, 1, 0, 0, 0, 0 };

	struct tm tmTime = {};
	auto it = yyMMddhhmmss.begin();
	for( size_t idx = 0; idx < yyMMddhhmmss.size() || idx < 6; ++idx ) {
		t[idx] = *it++;
	}

	tmTime.tm_year = t[0] - 1900;
	tmTime.tm_mon = t[1] - 1;
	tmTime.tm_mday = t[2];
	tmTime.tm_hour = t[3];
	tmTime.tm_min = t[4];
	tmTime.tm_sec = t[5];
	tmTime.tm_isdst = -1;

	struct tm* ptmTime = nullptr;
	if (bLocal) {
		__time64_t tTime = _mkgmtime64(&tmTime);
		_tGMT = tTime + _tzSec;
	} else {
		_tGMT = _mktime64(&tmTime);
	}

	_gmtime64_s(&_tmGMT, &_tGMT);
	localtime_s(&_tmLocalT, &_tGMT);
	_tLocalT = _mkgmtime64(&_tmLocalT);
}

void CTime::SetTime(int year, int month, int day, int hour, int min, int sec, bool bLocal /*= default_local*/)
{
	struct tm tmTime = {};

	tmTime.tm_year = year - 1900;
	tmTime.tm_mon = month - 1;
	tmTime.tm_mday = day;
	tmTime.tm_hour = hour;
	tmTime.tm_min = min;
	tmTime.tm_sec = sec;
	tmTime.tm_isdst = -1;

	struct tm* ptmTime = nullptr;
	if (bLocal) {
		__time64_t tTime = _mkgmtime64(&tmTime);
		_tGMT = tTime + _tzSec;
	} else {
		_tGMT = _mktime64(&tmTime);
	}

	_gmtime64_s(&_tmGMT, &_tGMT);
	localtime_s(&_tmLocalT, &_tGMT);
	_tLocalT = _mkgmtime64(&_tmLocalT);
}

void CTime::SetTime(TIMESTAMP_STRUCT dbTime, bool bLocal /*= default_local*/)
{
	SetTime({dbTime.year, dbTime.month, dbTime.day, dbTime.hour, dbTime.minute, dbTime.second}, bLocal);
}

void CTime::print()
{
	cout << format("gmt: {}", _tGMT) << endl;
	cout << format("gmt: {}-{}-{} {}:{}:{}", _tmGMT.tm_year+1900, _tmGMT.tm_mon+1, _tmGMT.tm_mday, _tmGMT.tm_hour, _tmGMT.tm_min, _tmGMT.tm_sec) << endl;
	cout << format("local: {}", _tLocalT) << endl;
	cout << format("local: {}-{}-{} {}:{}:{}", _tmLocalT.tm_year+1900, _tmLocalT.tm_mon+1, _tmLocalT.tm_mday, _tmLocalT.tm_hour, _tmLocalT.tm_min, _tmLocalT.tm_sec) << endl;
}

//
CPerformanceChecker::CPerformanceChecker(source_location loc /*= source_location::current()*/)
	: _loc(loc)
{
}

CPerformanceChecker::CPerformanceChecker(string *pstrBuffer, source_location loc /*= source_location::current()*/)
	: _pstrBuffer(pstrBuffer), _loc(loc)
{
}

CPerformanceChecker::~CPerformanceChecker()
{
	chrono::steady_clock::time_point end = chrono::steady_clock::now();
	chrono::nanoseconds processTime = end - _start;

	if (_pstrBuffer) {
		*_pstrBuffer = format("func:{} [line:{}]: process time: {}", _loc.function_name(), _loc.line(), processTime.count());
	} else {
		cout << format("func:{} [line:{}]: process time: {}", _loc.function_name(), _loc.line(), processTime.count()) << endl;
	}
}

//
void unit_test_time()
{
	cout << "unit_test_time" << endl;
	CPerformanceChecker p_check;

	CTime time1;
	time1.print();

	time1.SetTime({2025, 2, 14, 9, 0, 0}, true);
	time1.print();

	CTime time4(2025, 2, 14, 9, 0, 0, true);
	time4.print();

	auto check_gmt = time1.GetTime(false);
	auto check_local = time1.GetTime();

	CTime time2(check_gmt, false);
	time2.print();

	CTime time3(check_local);
	time3.print();
}
} //namespace rubble