#pragma once
#ifndef __UTIL_FILE_H__
#define __UTIL_FILE_H__

#include <Windows.h>

#include <string>
#include <vector>
#include <list>

using namespace std;

namespace rubble
{
int GetFileList(string path, list<string>& fileList);

void unit_test_file();
} //namespace rubble

#endif //__UTIL_FILE_H__