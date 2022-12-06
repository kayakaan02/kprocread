#include "kprocread.h"

SYSTEM_INFO sysInfo;

std::wstring KPROC::widen(const std::string& str)
{
    std::wostringstream wstm;
    const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());
    for (size_t i = 0; i < str.size(); ++i)
        wstm << ctfacet.widen(str[i]);
    return wstm.str();
}

bool KPROC::checkProcess(std::string cstr)
{
    const std::wstring widestring = KPROC::widen(cstr);
    const wchar_t* wcstr = widestring.c_str();

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (_tcsicmp(entry.szExeFile, wcstr) == 0)
            {
                return 1;
            }
        }
    }
    return -0;
}

float KPROC::getCpuUsage(std::string cstr)
{
    const std::wstring widestring = KPROC::widen(cstr);
    const wchar_t* wcstr = widestring.c_str();

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (_tcsicmp(entry.szExeFile, wcstr) == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

                static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
                static int numProcessors;
                static HANDLE self;
                
                
                double percent;

                numProcessors = sysInfo.dwNumberOfProcessors;

                //Getting system times information
                FILETIME SysidleTime;
                FILETIME SyskernelTime;
                FILETIME SysuserTime;
                ULARGE_INTEGER SyskernelTimeInt, SysuserTimeInt;
                GetSystemTimes(&SysidleTime, &SyskernelTime, &SysuserTime);
                memcpy(&SyskernelTimeInt, &SyskernelTime, sizeof(FILETIME));
                memcpy(&SysuserTimeInt, &SysuserTime, sizeof(FILETIME));
                __int64 denomenator = SysuserTimeInt.QuadPart + SyskernelTimeInt.QuadPart;

                //Getting process times information
                FILETIME ProccreationTime, ProcexitTime, ProcKernelTime, ProcUserTime;
                ULARGE_INTEGER ProccreationTimeInt, ProcexitTimeInt, ProcKernelTimeInt, ProcUserTimeInt;
                GetProcessTimes(hProcess, &ProccreationTime, &ProcexitTime, &ProcKernelTime, &ProcUserTime);
                memcpy(&ProcKernelTimeInt, &ProcKernelTime, sizeof(FILETIME));
                memcpy(&ProcUserTimeInt, &ProcUserTime, sizeof(FILETIME));
                __int64 numerator = ProcUserTimeInt.QuadPart + ProcKernelTimeInt.QuadPart;
                //QuadPart represents a 64-bit signed integer (ULARGE_INTEGER)
                percent = 100 * long float(((long float(numerator)) / (long float(denomenator))));

                return percent;

                CloseHandle(hProcess);
            }
        }
    }
    CloseHandle(snapshot);
    return -1;
    


    
}

float KPROC::getPhyMem(std::string cstr)
{
    const std::wstring widestring = KPROC::widen(cstr);
    const wchar_t* wcstr = widestring.c_str();

    PROCESS_MEMORY_COUNTERS_EX pmc;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (_tcsicmp(entry.szExeFile, wcstr) == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
                return float(pmc.WorkingSetSize / 1024)/1000;
                CloseHandle(hProcess);
            }
        }
    }
    CloseHandle(snapshot);
    return -1;
}

float KPROC::getVirMem(std::string cstr)
{
    const std::wstring widestring = KPROC::widen(cstr);
    const wchar_t* wcstr = widestring.c_str();

    PROCESS_MEMORY_COUNTERS_EX pmc;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (_wcsicmp(entry.szExeFile, wcstr) == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
                return float(pmc.PrivateUsage / 1024)/1000;
                CloseHandle(hProcess);
            }
        }
    }
    CloseHandle(snapshot);
    return -1;
}
