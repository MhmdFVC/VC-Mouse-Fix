#define VERSION (*(uint8_t*)0x608578)
#define JP 0x44
#define RETAIL_1_0 0x5D
#define RETAIL_1_1 0x81
#define STEAM 0x5B
#define GREEN_PEPPER 0xA1
#define NOP 0x90
#include <stdint.h>
#include "Patch.h"

void Thread()
{
    uint32_t sensResetAddr, ySensFixTarget, ySensFixAddr1, ySensFixAddr2, ySensFixAddr3, ySensFixAddr4, ySensFixAddr5, nastyGameAddr;
	switch (VERSION)
	{
        case JP:
            sensResetAddr = 0x46F821;
            ySensFixTarget = 0x94ABD8;
            ySensFixAddr1 = 0x479AC9;
            ySensFixAddr2 = 0x47A864;
            ySensFixAddr3 = 0x47B3CC;
            ySensFixAddr4 = 0x47C496;   
            ySensFixAddr5 = 0x48238A;
            nastyGameAddr = 0x68B110;
            break;
        case RETAIL_1_0:
            ySensFixTarget = 0x94DBD0;    // Retail 1.0 only
        case RETAIL_1_1:
            sensResetAddr = 0x46F4B1;
            ySensFixAddr1 = 0x4796F2;
            ySensFixAddr2 = 0x47A48D;
            ySensFixAddr3 = 0x47AFF5;
            ySensFixAddr4 = 0x47C0BF;
            ySensFixAddr5 = 0x481FB3;
            nastyGameAddr = 0x68DD68;
            if (VERSION == RETAIL_1_0) break;   // The above values also apply to 1.0
            ySensFixTarget = 0x94DBD8;    // Retail 1.1 only
            break;
        case STEAM:
        case GREEN_PEPPER:
            sensResetAddr = 0x46F391;
            ySensFixAddr1 = 0x4795D2;
            ySensFixAddr2 = 0x47A36D;
            ySensFixAddr3 = 0x47AED5;
            ySensFixAddr4 = 0x47BF9F;
            ySensFixAddr5 = 0x481E93;
            ySensFixTarget = 0x94CBD8;
            nastyGameAddr = 0x68CD68;
            break;
	}

    for (uint32_t addrToWrite = sensResetAddr-1; addrToWrite < sensResetAddr+10; SetChar(++addrToWrite, NOP));
    SetInt(ySensFixAddr1, ySensFixTarget);  // Sniper first-person aim
    SetInt(ySensFixAddr2, ySensFixTarget);  // Rocket launcher first-person aim
    SetInt(ySensFixAddr3, ySensFixTarget);  // M4/ruger first-person aim
    SetInt(ySensFixAddr4, ySensFixTarget);  // Normal free aim    
    SetInt(ySensFixAddr5, ySensFixTarget);  // "Runabout" (classic controls?)
    SetChar(nastyGameAddr, 1);              // nastyGame

	return;
}

int APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
		Thread();
	return 1;
}