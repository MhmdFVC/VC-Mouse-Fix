#define version (*(unsigned char *)0x608578)
#include "Patch.h"

void Thread()
{
    unsigned int sensResetAddr, ySensFixTarget, ySensFixAddr1, ySensFixAddr2, ySensFixAddr3, ySensFixAddr4, ySensFixAddr5, nastyGameAddr;
	switch (version)
	{
        case 0x44:  // JP
            sensResetAddr = 0x46F821;
            ySensFixTarget = 0x94ABD8;
            ySensFixAddr1 = 0x479AC9;   // Sniper first-person aim
            ySensFixAddr2 = 0x47A864;   // Rocket launcher first-person aim
            ySensFixAddr3 = 0x47B3CC;  // M4/ruger first-person aim
            ySensFixAddr4 = 0x47C496;  // Normal free aim    
            ySensFixAddr5 = 0x48238A;  // "Runabout" (classic controls?)
            nastyGameAddr = 0x68B110;         // nastyGame
            break;
        case 0x5D:  // Retail 1.0
            ySensFixTarget = 0x94DBD0;    // Retail 1.0 only
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
            break;
	}

    for (unsigned char i = 0; i < 10; i++)
        SetChar(sensResetAddr++, 0x90);
    SetInt(ySensFixAddr1, ySensFixTarget);  // Sniper first-person aim
    SetInt(ySensFixAddr2, ySensFixTarget);  // Rocket launcher first-person aim
    SetInt(ySensFixAddr3, ySensFixTarget);  // M4/ruger first-person aim
    SetInt(ySensFixAddr4, ySensFixTarget);  // Normal free aim    
    SetInt(ySensFixAddr5, ySensFixTarget);  // "Runabout" (classic controls?)
    SetChar(nastyGameAddr, 1);         // nastyGame

	return;
}

int APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		Thread();
	}
	return 1;
}