#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <windows.h>
#include <tlhelp32.h>

DWORD getPidFromExe(char exe[]);
void msleep (int ms);

const char nop = 0x90;
DWORD vcPid, addrToWrite, sensResetAddr, ySensFixAddr1, ySensFixAddr2, ySensFixAddr3, ySensFixAddr4, ySensFixAddr5, ySensFixTarget;
short versionValue = 0x0;
int gameRunning = 0;

int main()
{
    
    while (1)
    {
        vcPid = getPidFromExe("gta-vc.exe");
        //printf("Waiting for game to open.\n");
        while (!vcPid)
        {
            msleep(500); // Slow refresh rate since it doesn't matter much at game launch
            vcPid = getPidFromExe("gta-vc.exe");
        }
        HANDLE hViceCity = OpenProcess(PROCESS_ALL_ACCESS, FALSE, vcPid);
        gameRunning = 1;
    
        ReadProcessMemory(hViceCity, (void*)0x608578, &versionValue, 1, 0);

        switch (versionValue)   // Set relevant mem addresses based on version
        {
            case 0x44:  // JP
                sensResetAddr = 0x46F821;    
                ySensFixAddr1 = 0x479AC9;    // Sniper first-person aim
                ySensFixAddr2 = 0x47A864;    // Rocket launcher first-person aim
                ySensFixAddr3 = 0x47B3CC;    // M4/ruger first-person aim
                ySensFixAddr4 = 0x47C496;    // Normal free aim
                ySensFixAddr5 = 0x48238A;    // "Runabout" (classic controls?)
                ySensFixTarget = 0x94ABD8;   // 9743320
                break;
            case 0x5D:  // Retail 1.0
                ySensFixTarget = 0x94DBD0;    // Retail 1.0; 9755600
            case 0x81:   // Retail 1.1
                sensResetAddr = 0x46F4B1;    
                ySensFixAddr1 = 0x4796F2;
                ySensFixAddr2 = 0x47A48D;
                ySensFixAddr3 = 0x47AFF5;
                ySensFixAddr4 = 0x47C0BF;
                ySensFixAddr5 = 0x481FB3;
                if (versionValue == 0x5D) break;    // The above values also apply to 1.0 so I'm cheating a bit here
                ySensFixTarget = 0x94DBD8;    // Retail 1.1; 9755608
                break;
            case 0x5B:   // Steam
                sensResetAddr = 0x46F391;
                ySensFixAddr1 = 0x4795D2;
                ySensFixAddr2 = 0x47A36D;
                ySensFixAddr3 = 0x47AED5;
                ySensFixAddr4 = 0x47BF9F;
                ySensFixAddr5 = 0x481E93;
                ySensFixTarget = 0x94CBD8;        // 9751512
                break;
        };

        for (char byte = 0; byte < 10; byte++)
        {
            addrToWrite = sensResetAddr + byte;
            WriteProcessMemory(hViceCity, (LPVOID)addrToWrite, &nop, 1, 0);
        }
        WriteProcessMemory(hViceCity, (LPVOID)ySensFixAddr1, &ySensFixTarget, 4, 0);
        WriteProcessMemory(hViceCity, (LPVOID)ySensFixAddr2, &ySensFixTarget, 4, 0);
        WriteProcessMemory(hViceCity, (LPVOID)ySensFixAddr3, &ySensFixTarget, 4, 0);
        WriteProcessMemory(hViceCity, (LPVOID)ySensFixAddr4, &ySensFixTarget, 4, 0);
        WriteProcessMemory(hViceCity, (LPVOID)ySensFixAddr5, &ySensFixTarget, 4, 0);

        while (gameRunning)
        {
            msleep(200);    // Shorter refresh rate to catch quick game restarts
            gameRunning = ReadProcessMemory(hViceCity, (void*)0x608578, &versionValue, 1, 0) ? 1 : 0;
        }
        CloseHandle(hViceCity);

    };
    
    return 0;
}

DWORD getPidFromExe(char exe[])
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE processes = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(processes, &entry))
        while (Process32Next(processes, &entry))
            if (!stricmp(entry.szExeFile, exe)) // i.e. a running process's EXE file name matches the parameter 
            {
                CloseHandle(processes);
                return entry.th32ProcessID;
            }
    CloseHandle(processes);
    return 0;
}

void msleep(int ms)
{
    usleep(1000 * ms);
}