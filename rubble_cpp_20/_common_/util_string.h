#pragma once
#ifndef __UTIL_STRING_H__
#define __UTIL_STRING_H__

#include <Windows.h>

#include <iostream>
#include <string>
#include <format>
#include <algorithm>

#include <vector>
#include <list>

using namespace std;

namespace rubble {

const int MAX_BUFFER_SIZE = 1024;

string FormatA(const char* pFormat, ...);
wstring FormatW(const WCHAR* pFormat, ...);

void TokenizeA(string str, vector<string>& tokens, string delimiter);
void TokenizeW(wstring str, vector<wstring>& tokens, wstring delimiter);

void unit_test_string();

} //namespace rubble

#endif //__UTIL_STRING_H__
