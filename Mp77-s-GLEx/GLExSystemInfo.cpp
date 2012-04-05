#include "GLExSystemInfo.h"

GLExSystemInfo::GLExSystemInfo(void):cpuusage(0)
{	
	pid = ::GetCurrentProcessId();
	handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);	

	time = QDateTime::currentMSecsSinceEpoch();
	
	GetSystemTimes(&lastidle, &lastker, &lastuser);
	lasttime = FileTimetoInt64(lastidle)+FileTimetoInt64(lastuser)+FileTimetoInt64(lastker);

	GetProcessTimes(handle, &create, &exit, &pker, &puser);
	lastusertime = FileTimetoInt64(pker)+FileTimetoInt64(puser);

	//vedio card
	//memset( &device_display, 0, sizeof(device_display) );
	device_display.cb = sizeof(device_display);
	int num = 0;
	while(EnumDisplayDevices(0,num,&device_display,0))
	{		
		if( device_display.StateFlags & DISPLAY_DEVICE_ACTIVE)
		{
			break;
		}
		num ++;
	}
	return;
}


GLExSystemInfo::~GLExSystemInfo(void)
{
}

void GLExSystemInfo::Update()
{

	//memory usage
	DWORD cb;
	::GetProcessMemoryInfo(handle,&psmemCounters,sizeof(psmemCounters));

	//cpu usage
	GetProcessTimes(handle, &create, &exit, &pker, &puser);
	
	GetSystemTimes(&lastidle, &lastker, &lastuser);

	__int64 delta = FileTimetoInt64(lastidle)+FileTimetoInt64(lastuser)+FileTimetoInt64(lastker) - lasttime;
	if(delta > 0)
	{
		cpuusage = 1.0*(FileTimetoInt64(pker)+FileTimetoInt64(puser) - lastusertime) / delta;
		lastusertime = FileTimetoInt64(pker)+FileTimetoInt64(puser);
		lasttime = FileTimetoInt64(lastidle)+FileTimetoInt64(lastuser)+FileTimetoInt64(lastker);
	}

	time = QDateTime::currentMSecsSinceEpoch() - time;

	if( time > 0 )
		framerate = 1000.0 / time;
	else
		framerate = 1000;

	time = QDateTime::currentMSecsSinceEpoch();
	
	return;

}

double GLExSystemInfo::GetFrameRate()
{
	return framerate;
}

__int64 GLExSystemInfo::FileTimetoInt64(FILETIME time)
{
	ULARGE_INTEGER tt;
	tt.LowPart = time.dwLowDateTime;
	tt.HighPart = time.dwHighDateTime;
	return(tt.QuadPart);
}

double GLExSystemInfo::GetCpuUsage()
{
	return cpuusage*100;
}

double GLExSystemInfo::GetPageFaultCount()
{
	return psmemCounters.PageFaultCount;
}

double GLExSystemInfo::GetPagefileUsage()
{
	return psmemCounters.PagefileUsage/1000;
}

double GLExSystemInfo::GetPeakPagefileUsage()
{
	return psmemCounters.PeakPagefileUsage/1000;
}

double GLExSystemInfo::GetPeakWorkingSetSize()
{
	return psmemCounters.PeakWorkingSetSize/1000;
}

double GLExSystemInfo::GetWorkingSetSize()
{
	return psmemCounters.WorkingSetSize/1000;
}

double GLExSystemInfo::GetQuotaNonPagedPoolUsage()
{
	return psmemCounters.QuotaNonPagedPoolUsage;
}

double GLExSystemInfo::GetQuotaPagedPoolUsage()
{
	return psmemCounters.QuotaPagedPoolUsage;
}

double GLExSystemInfo::GetQuotaPeakNonPagedPoolUsage()
{
	return psmemCounters.QuotaPeakNonPagedPoolUsage;
}
	
double GLExSystemInfo::GetQuotaPeakPagedPoolUsage()
{
	return psmemCounters.QuotaPeakPagedPoolUsage;
}

void GLExSystemInfo::GetDisplayDeviceString(char *m)
{
	wcstombs(m,device_display.DeviceString,wcslen(device_display.DeviceString));
	
}

void GLExSystemInfo::GetDisplayDeviceID(char *m)
{
	wcstombs(m,device_display.DeviceID,wcslen(device_display.DeviceID));
}