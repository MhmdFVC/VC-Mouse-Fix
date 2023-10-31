#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <windows.h>
#include <tlhelp32.h>

// Prototypes
DWORD getPidFromExe(char exe[]);
void msleep (int ms);
void exitProg(int signum);

// Var declarations
const unsigned char nopVal = 0x90;
const char* nop = &nopVal;
unsigned char versionValue = 0x0;
int gameRunning = 0, attemptCount = 0;
DWORD vcPid, addrToWrite, sensResetAddr, ySensFixAddr1, ySensFixAddr2, ySensFixAddr3, ySensFixAddr4, ySensFixAddr5, ySensFixTarget;
HANDLE processes, hViceCity;

// Main
int main()
{
    SetConsoleTitle("VC Mouse Fix");
    printf("VC Mouse Fix running.\n");

    signal(SIGINT, exitProg);
    signal(SIGTERM, exitProg);

    while (1)
    {
    startOfLoop:
        vcPid = getPidFromExe("gta-vc.exe");
        //printf("Waiting for game to open.\n");
        while (!vcPid)
        {
            msleep(500); // Slow refresh rate since it doesn't matter much at game launch
            vcPid = getPidFromExe("gta-vc.exe");
        }

        attemptCount++; // Adding an attempt counter for fun
        printf("Game process found (attempt #%d).\n", attemptCount);
        hViceCity = OpenProcess(PROCESS_ALL_ACCESS, FALSE, vcPid);
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
                ySensFixTarget = 0x94ABD8;   // The patch value for the above 5 addresses
                printf("JP version detected.\n");
                break;
            case 0x5D:  // Retail 1.0
                ySensFixTarget = 0x94DBD0;    // Retail 1.0
                printf("Retail 1.0 version detected.\n");
            case 0x81:   // Retail 1.1
                sensResetAddr = 0x46F4B1;    
                ySensFixAddr1 = 0x4796F2;
                ySensFixAddr2 = 0x47A48D;
                ySensFixAddr3 = 0x47AFF5;
                ySensFixAddr4 = 0x47C0BF;
                ySensFixAddr5 = 0x481FB3;
                if (versionValue == 0x5D) break;    // The above values also apply to 1.0 so I'm cheating a bit here
                ySensFixTarget = 0x94DBD8;    // Retail 1.1
                printf("Retail 1.1 version detected.\n");
                break;
            case 0x5B:   // Steam
                sensResetAddr = 0x46F391;
                ySensFixAddr1 = 0x4795D2;
                ySensFixAddr2 = 0x47A36D;
                ySensFixAddr3 = 0x47AED5;
                ySensFixAddr4 = 0x47BF9F;
                ySensFixAddr5 = 0x481E93;
                ySensFixTarget = 0x94CBD8;
                printf("Steam version detected.\n");
                break;
            case 0xA1:  // Green Pepper
                printf("Green Pepper version detected. Sorry, this version is not supported at this time. Exiting.\n");
                return(1);
            default:
                printf("Could not identify game version. Assuming this is disc verification. Retrying in 10 seconds.\n");
                CloseHandle(hViceCity);
                sleep(10);
                goto startOfLoop;
        };

        addrToWrite = sensResetAddr-1;  // Priming address to write so that I can simply use ++ in the following loop
        for (char byte = 0; byte < 10; byte++)  // Writes 10 NOP instructions into this block to override sens reset
        {
            addrToWrite++;
            WriteProcessMemory(hViceCity, (LPVOID)addrToWrite, nop, 1, 0);
        }
        WriteProcessMemory(hViceCity, (LPVOID)ySensFixAddr1, &ySensFixTarget, 4, 0);
        WriteProcessMemory(hViceCity, (LPVOID)ySensFixAddr2, &ySensFixTarget, 4, 0);
        WriteProcessMemory(hViceCity, (LPVOID)ySensFixAddr3, &ySensFixTarget, 4, 0);
        WriteProcessMemory(hViceCity, (LPVOID)ySensFixAddr4, &ySensFixTarget, 4, 0);
        WriteProcessMemory(hViceCity, (LPVOID)ySensFixAddr5, &ySensFixTarget, 4, 0);
        printf("Sens reset and y-axis sens fixes applied.\n");

        while (gameRunning)
        {
            msleep(200);    // Shorter refresh rate to catch quick game restarts
            gameRunning = ReadProcessMemory(hViceCity, (void*)0x608578, &versionValue, 1, 0) ? 1 : 0;
        }
        printf("Game closed. Awaiting new instance of VC process.\n\n");
        CloseHandle(hViceCity);
    };  // End of infinite loop

    return 0;
}   // End of main

DWORD getPidFromExe(char exe[])
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    processes = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

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

void exitProg(int signum)
{
    CloseHandle(processes);
    CloseHandle(hViceCity);
    printf("VC Mouse Fix closed.\n");
    exit(0);
}