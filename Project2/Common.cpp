
#include <Windows.h>
#include <string>
#include <iostream>

using namespace std;

#include "Commons.h"

void Log(const char* message) {
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
	string result = FormatString("%d-%02d-%02d %02d:%02d:%02d", 28,
		sysTime.wYear,
		sysTime.wMonth,
		sysTime.wDay,
		sysTime.wHour,
		sysTime.wMinute,
		sysTime.wSecond
	);

	cout << result << "\t" << message << endl;
}

string FormatString(const char* format, int maxSize, ...) {
	va_list vl;
	std::string result;
	char* str = (char*)malloc(maxSize * sizeof(char));

	ZeroMemory(str, maxSize * sizeof(char));

	va_start(vl, maxSize);
	vsprintf_s(str, maxSize, format, vl);
	va_end(vl);

	result = std::string(str);
	free(str);
	return result;
}