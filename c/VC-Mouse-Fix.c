#define VERSION (*(uint8_t*)0x608578)
#define JP 0x44
#define RETAIL_1_0 0x5D
#define RETAIL_1_1 0x81
#define STEAM 0x5B
#define GREEN_PEPPER 0xA1
#define FRENCH 1
#define GERMAN 2
#include <stdint.h>
#include <Windows.h>

void patch(void* address, void* data, int size);

void Thread()
{
    uint32_t sensResetAddr, ySensFixTarget, ySensFixAddr1, ySensFixAddr2, ySensFixAddr3, ySensFixAddr4, ySensFixAddr5, nastyGameAddr, langAddr;
    uint8_t nop = 0x90, nastyGameEnabled = 1;
    uint16_t nastyGameEnabled2 = 0x1B0;

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
            langAddr = 0x869680;
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

    for (uint32_t addrToWrite = sensResetAddr; addrToWrite < sensResetAddr+10; patch((void*)addrToWrite++, &nop, 1));
    patch((void*)ySensFixAddr1, &ySensFixTarget, 4);  // Sniper first-person aim
    patch((void*)ySensFixAddr2, &ySensFixTarget, 4);  // Rocket launcher first-person aim
    patch((void*)ySensFixAddr3, &ySensFixTarget, 4);  // M4/ruger first-person aim
    patch((void*)ySensFixAddr4, &ySensFixTarget, 4);  // Normal free aim    
    patch((void*)ySensFixAddr5, &ySensFixTarget, 4);  // "Runabout" (classic controls?)
    if (*(uint8_t*)langAddr != FRENCH && *(uint8_t*)langAddr != GERMAN)
        patch((void*)nastyGameAddr, &nastyGameEnabled, 1);// nastyGame -- only at game start, doesn't work for changing languages.

	return;
}

void patch(void* address, void* data, int size)
{
	unsigned long protect[2];
	VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &protect[0]);
	memcpy(address, data, size);
	VirtualProtect(address, size, protect[0], &protect[1]);
}

int APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
		Thread();
	return 1;
}