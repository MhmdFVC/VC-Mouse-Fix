#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <sys/types.h>
#include <windows.h>
#include <tlhelp32.h>
//#include "TrayIconCrap.c"

// Prototypes
void exitProg(int signum);

// Var declarations
const unsigned char nopVal=0x90, oneVal=1;
unsigned char *addrToWrite=NULL, *sensResetAddr=NULL, *nastyGameAddr=NULL, version=0, nastyGame=0;
unsigned int vcPid=0, ySensFixTarget=0, *ySensFixAddr1=NULL, *ySensFixAddr2=NULL, *ySensFixAddr3=NULL, *ySensFixAddr4=NULL, *ySensFixAddr5=NULL, gameRunning=0, attemptCount=0;
HANDLE hViceCity;

//NOTIFYICONDATAW trayIcon;
//MSG msg;

// Main
int main()
{
    signal(SIGINT, exitProg);
    signal(SIGTERM, exitProg);
    signal(SIGABRT, exitProg);
    signal(SIGBREAK, exitProg);

    SetConsoleTitleW(L"VC Mouse Fix");
    Sleep(40);
    
    //createWindow(); // Windows API crap for making a tray icon (see TrayIconCrap.c)
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////MAIN LOOP////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    puts("VC Mouse Fix running.");
    while (1)
    {
        vcPid = 0;
        GetWindowThreadProcessId(FindWindowW(L"Grand theft auto 3", L"GTA: Vice City"), &vcPid);  // Get window handle of VC and store its PID

        if (!vcPid)
        {
            Sleep(500); // Slow refresh rate since it doesn't matter much at game launch
            continue;
        }
        attemptCount++; // Adding an attempt counter for fun
        printf("Game process found (attempt #%d).\n", attemptCount);
        hViceCity = OpenProcess(PROCESS_ALL_ACCESS, FALSE, vcPid);  // Needed PID to open process handle
        gameRunning = 1;
    
        ReadProcessMemory(hViceCity, (void*)0x608578, &version, 1, 0); // Address used to detect version across all games. Not sure if it really was meant for that but it works.

        switch (version)   // Set relevant mem addresses based on version
        {
            case 0x44:  // JP
                sensResetAddr = 0x46F821;    
                ySensFixAddr1 = 0x479AC9;    // Sniper first-person aim
                ySensFixAddr2 = 0x47A864;    // Rocket launcher first-person aim
                ySensFixAddr3 = 0x47B3CC;    // M4/ruger first-person aim
                ySensFixAddr4 = 0x47C496;    // Normal free aim
                ySensFixAddr5 = 0x48238A;    // "Runabout" (classic controls?)
                ySensFixTarget = 0x94ABD8;   // The patch value for the above 5 addresses
                nastyGameAddr = 0x68B110; 
                puts("JP version detected.");
                break;
            case 0x5D:  // Retail 1.0
                ySensFixTarget = 0x94DBD0;    // Retail 1.0 only
                puts("Retail 1.0 version detected.");
            case 0x81:   // Retail 1.1
                sensResetAddr = 0x46F4B1;    
                ySensFixAddr1 = 0x4796F2;
                ySensFixAddr2 = 0x47A48D;
                ySensFixAddr3 = 0x47AFF5;
                ySensFixAddr4 = 0x47C0BF;
                ySensFixAddr5 = 0x481FB3;
                nastyGameAddr = 0x68DD68;
                if (version == 0x5D) break;    // The above values also apply to 1.0 so I'm cheating a bit here
                ySensFixTarget = 0x94DBD8;    // Retail 1.1 only
                puts("Retail 1.1 version detected.");
                break;
            case 0x5B:  // Steam
            case 0xA1:  // Green Pepper
                sensResetAddr = 0x46F391;
                ySensFixAddr1 = 0x4795D2;
                ySensFixAddr2 = 0x47A36D;
                ySensFixAddr3 = 0x47AED5;
                ySensFixAddr4 = 0x47BF9F;
                ySensFixAddr5 = 0x481E93;
                ySensFixTarget = 0x94CBD8;
                nastyGameAddr = 0x68CD68;
                puts("Steam/Green Pepper/RGL version detected.");
                break;
            default:
                puts("Could not identify game version. Supported versions are INTL Retail 1.0/1.1, Steam, JP, RGL, and Green Pepper.");
                CloseHandle(hViceCity);
                system("pause");
                continue;
        };

        addrToWrite = sensResetAddr;
        for (char byte = 0; byte < 10; byte++)  // Writes 10 NOP instructions into this block to override sens reset
            WriteProcessMemory(hViceCity, addrToWrite++, &nopVal, 1, 0);
        WriteProcessMemory(hViceCity, ySensFixAddr1, &ySensFixTarget, 4, 0);
        WriteProcessMemory(hViceCity, ySensFixAddr2, &ySensFixTarget, 4, 0);
        WriteProcessMemory(hViceCity, ySensFixAddr3, &ySensFixTarget, 4, 0);
        WriteProcessMemory(hViceCity, ySensFixAddr4, &ySensFixTarget, 4, 0);
        WriteProcessMemory(hViceCity, ySensFixAddr5, &ySensFixTarget, 4, 0);
        puts("Sens reset and y-axis sens fixes applied.");

        ReadProcessMemory(hViceCity, nastyGameAddr, &nastyGame, 1, 0);
        if (!nastyGame)
        {
            WriteProcessMemory(hViceCity, nastyGameAddr, &oneVal, 1, 0);
            puts("Nasty game fix applied.");
        }

        while (gameRunning)
        {
            Sleep(200);    // Shorter refresh rate to catch quick game restarts
            gameRunning = ReadProcessMemory(hViceCity, (void*)0x608578, &version, 1, 0);
        }
        puts("Game closed. Awaiting new instance of VC process.");
        puts("");
        CloseHandle(hViceCity);
    };  // End of infinite loop

    return 0;
}   // End of main

void exitProg(int signum)
{
    //Shell_NotifyIconW(NIM_DELETE, &trayIcon);
    if (gameRunning)
        CloseHandle(hViceCity);
    puts("VC Mouse Fix closed.");
    exit(0);
}
