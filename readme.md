# About
Based off of [Lighnat0r's mouse sensitivity reset script](https://github.com/Lighnat0r-pers/Files/blob/master/GTA%20VC%20No%20Mouse%20Sensitivity%20Reset.zip), but modified to be specifically for VC and includes y-axis sensitivity fix functionality. Now also includes a nasty game fix which means that you can get blood etc. even on French/German locales. 
**This is the only mouse fix permitted for Vice City speedruns that are to be submitted to the speedrun.com leaderboards as of 2023-04-29**  

## Downloads
Comes as an ASI file. Just put this into your VC directory and play.
### [Download Latest Version (ASI, recommended)](https://github.com/MhmdFVC/VC-Mouse-Fix/releases/download/3.2/VC-Mouse-Fix-3.2.asi)

You should probably just use the ASI so you don't need to open another program, but below are previous versions which do exactly the same thing.
 - [Version 1.03 (AHK)](https://github.com/MhmdFVC/VC-Mouse-Fix/releases/download/1.03/VC-Mouse-Fix-1.03.exe) - Normal AHK version. No window, just sits as a low-key icon in your task area.
 - [Version 2.02 (EXE)](https://github.com/MhmdFVC/VC-Mouse-Fix/releases/download/2.02/VC-Mouse-Fix-2.02.exe) - Program with console output which logs status and session attempt count.

# Technical
## Sensitivity reset fix
This works by overwriting the command(s) which set mouse sensitivity upon new game with NOPs, which makes nothing happen instead.
* For Retail 1.0/1.1, 0x90 is written to addresses 0x0046F4B1 - 0x0046F4BA
* For Steam/Green Pepper, 0x90 is written to addresses 0x0046F391 - 0x0046F39A
* For JP, 0x90 is written to addresses 0x0046F821 - 0x0046F82A

## Y-axis sensitivity fix
This works by patching 5 different memory addresses that each have to do with the y-axis sensitivity in some fashion: 
* Sniper rifle first-person aim sensitivity
* RPG first-person aim sensitivity
* M4/Ruger/M60 first-person aim sensitivity
* Normal free aim
* Something called "runabout" which may have to do with classic controls.

The addresses noted below will be provided in the above order.
* For Retail 1.0 [1.1], the value 0x94DBD0 [0x94DBD8] is written to addresses 0x004796F2, 0x0047A48D, 0x0047AFF5, 0x0047C0BF, and 0x00481FB3.
* For Steam/Green Pepper, the value 0x94CBD8 is written to addresses 0x004795D2, 0x0047A36D, 0x0047AED5, 0x0047BF9F, and 0x00481E93.
* For JP, the value 0x94ABD8 is written to addresses 0x00479AC9, 0x0047A864, 0x0047B3CC, 0x0047C496, and 0x0048238A.

## Nasty game fix
This simply enables "nastyGame" so long as the initial game language is not French nor German.
* For Retail 1.0/1.1, 1 is written to address 0x68DD68
* For Steam/Green Pepper, 1 is written to address 0x68CD68
* For JP, 1 is written to address 0x68B110.

# Credits
All memory addresses, values to write, and the memory read/write code are taken from Lighnat0r's scripts (the mouse sens reset script, and the y-axis fix ASI which this is a fork of; the mouse sens reset fix didn't have its own repo). Lighnat0r's original y-sens fix ASI and the CPatch.h header was used as a base for my ASI version. Technical explanation of the addresses' functions graciously provided by Nick007J. Mouse icon within the program icon taken from [Freepik on Flaticon](https://www.flaticon.com/free-icons/mouse)
