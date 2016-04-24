
/*
*   写日志类
*
*/
#pragma once

#include <windows.h>
#include <atltime.h>
#include <sys/timeb.h>
#include <iostream>
using namespace std;

class LogInfoTool
{
public:
	LogInfoTool(void);
	~LogInfoTool(void);

public:
	//日志文件
	FILE* m_pfLogFile;
	char m_cInfo[255];

	int  SetLogFile(FILE *pfLogFile);
	int  WriteLogInfo(const char *pInfo);
	int  WriteLogInfo(const string &str);
private:
	void FormatCTime(char *charTime, CTime time);
};