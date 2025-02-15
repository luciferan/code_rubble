#include "./util_string.h"

namespace rubble {

string FormatA(const char* pFormat, ...)
{
    char szBuffer[MAX_BUFFER_SIZE + 1] = {};

    va_list args;
    va_start(args, pFormat);
    _vsnprintf_s(szBuffer, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, pFormat, args);
    va_end(args);

    return string(szBuffer);
}

wstring FormatW(const WCHAR* pFormat, ...)
{
    WCHAR wcsBuffer[MAX_BUFFER_SIZE + 1] = {};

    va_list args;
    va_start(args, pFormat);
    _vsnwprintf_s(wcsBuffer, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, pFormat, args);
    va_end(args);

    return wstring(wcsBuffer);
}

void TokenizeA(string str, vector<string>& tokens, string delimiter)
{
    auto lastPos = str.find_first_not_of(delimiter, 0);
    auto pos = str.find_first_of(delimiter, lastPos);

    while (string::npos != pos || string::npos != lastPos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos));

        lastPos = str.find_first_not_of(delimiter, pos);
        pos = str.find_first_of(delimiter, lastPos);
    }
}

void TokenizeW(wstring str, vector<wstring>& tokens, wstring delimiter)
{
    auto lastPos = str.find_first_not_of(delimiter, 0);
    auto pos = str.find_first_of(delimiter, lastPos);

    while (wstring::npos != pos || wstring::npos != lastPos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos));

        lastPos = str.find_first_not_of(delimiter, pos);
        pos = str.find_first_of(delimiter, lastPos);
    }
}

void unit_test_string()
{
    string str = FormatA("%s-%d-%f", "str", 123, 0.123f);
    cout << str << endl;

    wstring wstr = FormatW(L"%s-%d-%f", L"str", 123, 0.123f);
    wcout << wstr << endl;

    vector<string> strVector;
    TokenizeA("a|b|c|d|10|20|30|30", strVector, "|");
    for (auto &s : strVector) {
        cout << s << " ";
    }
    cout << endl;

    vector<wstring> wstrVector;
    TokenizeW(L"a|b|c|d|10|20|30|30", wstrVector, L"|");
    for (auto &s : wstrVector) {
        wcout << s << " ";
    }
    wcout << endl;
}

} //namespace rubble