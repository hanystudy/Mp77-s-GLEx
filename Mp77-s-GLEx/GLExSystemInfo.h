#pragma once

#include <windows.h>
#include <Psapi.h>


#include <QDateTime>


class GLExSystemInfo
{
public:
	GLExSystemInfo(void);
	~GLExSystemInfo(void);

	void Update();

	PPROCESS_MEMORY_COUNTERS GetMemoryInfo();
	double GetFrameRate();

	double GetCpuUsage();
	double GetPageFaultCount();
	double GetPagefileUsage();
	double GetPeakPagefileUsage();
	double GetPeakWorkingSetSize();
	double GetWorkingSetSize();
	double GetQuotaNonPagedPoolUsage();
	double GetQuotaPagedPoolUsage();
	double GetQuotaPeakNonPagedPoolUsage();
	
	double GetQuotaPeakPagedPoolUsage();
	
	void GetDisplayDeviceString(char*);
	void GetDisplayDeviceID(char*);
private:

	__int64 FileTimetoInt64(FILETIME);

	//

	PROCESS_MEMORY_COUNTERS psmemCounters;
	DISPLAY_DEVICE device_display;
	qint64 time;
	HANDLE handle;
	DWORD pid;

	FILETIME create;
	FILETIME exit;
	FILETIME pker;  //�ں�ռ��ʱ��
	FILETIME puser; //�û�ռ��ʱ��
	
	FILETIME lastidle;
	FILETIME lastker;  //�ں�ռ��ʱ��
	FILETIME lastuser; //�û�ռ��ʱ��
	__int64 lasttime;
	__int64 lastusertime;
	double cpuusage;
	double framerate;

};

