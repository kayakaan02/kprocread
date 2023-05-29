# kprocread
Displays a process's memory and CPU usage


**`KPROC::checkProcess("sandbox.exe")`**

-returns bool.

-Checks if sandbox.exe is running or not. returns 1 if running, 0 if not.


**`KPROC::getCpuUsage("sandbox.exe")`**

-returns float.

-get cpu usage percent of sandbox.exe

>I don't know what the output value is but various processes has direct proportion with their CPU usage


**`KPROC::getPhyMem("sandbox.exe")`**

-returns float.

-get physical memory usage of sandbox.exe as KB


**`KPROC::getVirMem("sandbox.exe")`**

-returns float.

-get virtual memory usage of sandbox.exe as KB
