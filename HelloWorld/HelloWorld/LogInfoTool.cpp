#include "LogInfoTool.h"

LogInfoTool::LogInfoTool(void)
{
	m_pfLogFile = NULL;
	memset(m_cInfo, NULL, sizeof(m_cInfo));
}

LogInfoTool::~LogInfoTool(void)
{
	if (NULL != m_pfLogFile)
	{
		fclose(m_pfLogFile);
		m_pfLogFile = NULL;
	}
}

int LogInfoTool::SetLogFile(FILE *pfLogFile)
{
	m_pfLogFile = pfLogFile;
	return 0;
}

int LogInfoTool::WriteLogInfo(const char *pInfo)
{
	if (NULL != m_pfLogFile)
	{
		CTime curTime = CTime::GetCurrentTime();
		char timeChar[30];
		FormatCTime(timeChar, curTime);
		fprintf(m_pfLogFile, "%s --> %s\n", timeChar, pInfo);
		fflush(m_pfLogFile);
		return 0;
	}
	return 1;
}

int LogInfoTool::WriteLogInfo(const string& str)
{
	if (NULL != m_pfLogFile)
	{
		CTime curTime = CTime::GetCurrentTime();
		char timeChar[30];
		FormatCTime(timeChar, curTime);
		fprintf(m_pfLogFile, "%s --> %s\n", timeChar, str.c_str());
		fflush(m_pfLogFile);
		return 0;
	}
	return 1;
}

void LogInfoTool::FormatCTime(char *charTime, CTime time)
{
	int year = time.GetYear();
	int mon = time.GetMonth();
	int day = time.GetDay();

	int hour = time.GetHour();
	int min = time.GetMinute();
	int sec = time.GetSecond();

	sprintf_s(charTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
}