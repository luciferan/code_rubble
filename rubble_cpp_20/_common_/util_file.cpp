#include "./util_file.h"

#include <iostream>
#include <algorithm>
#include <filesystem>

namespace rubble
{
int GetFileList(string path, list<string> &fileList)
{
	try {
		for (const auto& entry : filesystem::directory_iterator(path)) {
			fileList.emplace_back(entry.path().string());
		}
	}
	catch (const filesystem::filesystem_error& e) {
		fileList.emplace_back(e.what());
		return -1;
	}

	return (int)fileList.size();
}

void unit_test_file()
{
	list<string> fileList;
	if (0 > GetFileList("./abc", fileList)) {
		auto err = fileList.rbegin();
		cout << "error: " << *err << endl;
	} else {
		for (string fileName : fileList) {
			cout << fileName << endl;
		}
	}
}
} //namespace rubble