# About
Based off of [Lighnat0r's mouse sensitivity reset script](https://github.com/Lighnat0r-pers/Files/blob/master/GTA%20VC%20No%20Mouse%20Sensitivity%20Reset.zip), but modified to be specifically for VC and includes y-axis sensitivity fix functionality.  
**This is the only mouse fix permitted for Vice City speedruns that are to be submitted to the speedrun.com leaderboards as of 2023-04-29**  

To use, run the program and keep it open for the duration of your gaming session. You will not need to manually set the sensitivity in your settings more than once, even if you forget to open the program and attempt to start a new game; running the program and resetting at that point will adjust your sensitivity to the correct settings.
### Downloads
#### - [Version 1.02](https://github.com/MhmdFVC/VC-Mouse-Fix/releases/download/1.02/VC-Mouse-Fix-1.02.exe) - Normal AHK version. No window, just sits as a low-key icon in your task area.
#### - [Version 2.01](https://github.com/MhmdFVC/VC-Mouse-Fix/releases/download/2.01/VC-Mouse-Fix-2.01.exe) - New experimental version of the VC Mouse Fix written in C. Comes with console output which logs program status and session attempt count. Recommended for gamers with a Pentium IV or below.

# Technical
## Sensitivity reset fix
This works by overwriting the commands which set mouse sensitivity upon new game with 10 NOPs (value 0x90), which makes nothing happen instead.
* For Retail 1.0/1.1, 0x90 is written to addresses 0x0046F4B1 - 0x0046F4BA
* For Steam, 0x90 is written to addresses 0x0046F391 - 0x0046F39A
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
* For Steam, the value 0x94CBD8 is written to addresses 0x004795D2, 0x0047A36D, 0x0047AED5, 0x0047BF9F, and 0x00481E93.
* For JP, the value 0x94ABD8 is written to addresses 0x00479AC9, 0x0047A864, 0x0047B3CC, 0x0047C496, and 0x0048238A.

# Credits
All memory addresses taken from Lighnat0r's scripts (the mouse sens reset script and the y-axis fix ASI, which this is a fork of; the mouse sens reset fix didn't have its own repo). Technical explanation of the addresses' functions graciously provided by Nick007J. Mouse icon within the program icon taken from [Freepik on Flaticon](https://www.flaticon.com/free-icons/mouse)
