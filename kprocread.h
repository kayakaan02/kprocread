#pragma once
#ifndef KPROCREAD_H
#define KPROCREAD_H
#include "windows.h"
#include "TCHAR.h"
#include "psapi.h"
#include <tlhelp32.h>
#include <Pdh.h>
#include <locale>
#include <string>
#include <sstream>
#pragma comment(lib,"Pdh")

class KPROC
{
public:
	//check if a process is running or not, 1 if running 0 if not
	//usage: KPROC::checkProcess("sandbox.exe);
	static bool checkProcess(std::string cstr);

	//get cpu usage percent of process
	//returns -1 if process is not running
	//I don't know what the output value is but various processes has direct proportion with their CPU usage
	//usage: KPROC::getCpuUsage("sandbox.exe);
	static float getCpuUsage(std::string cstr);

	//get physical memory usage of process as KB
	//returns -1 if process is not running
	//usage: KPROC::getPhyMem("sandbox.exe);
	static float getPhyMem(std::string cstr);

	//get virtual memory usage of process as KB
	//returns -1 if process is not running
	//usage: KPROC::getVirMem("sandbox.exe);
	static float getVirMem(std::string cstr);

private:
	//must use this function to get string as an input and cast it to call the process(string into widestring)
	static std::wstring widen(const std::string& str);
};

#endif
