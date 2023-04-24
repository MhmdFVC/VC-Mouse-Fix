Based off of [Lighnat0r's mouse sensitivity reset script](https://github.com/Lighnat0r-pers/Files/blob/master/GTA%20VC%20No%20Mouse%20Sensitivity%20Reset.zip), but modified to be specifically for VC and includes y-axis sensitivity fix functionality.  
**This is the only mouse fix permitted for Vice City speedruns that are to be submitted to the speedrun.com leaderboards as of 2023/04/24.**  

To use, run the program and keep it open for the duration of your gaming session. You will not need to manually set the sensitivity in your settings more than once, even if you forget to open the program and attempt to start a new game; running the program and resetting at that point will adjust your sensitivity to the correct settings.

I also removed any writing to memory addresses that did not actually seem to do anything. So for each game version, only two values will be written to two addresses; one for preventing sens reset, and one to fix y-axis sens.  
All memory addresses taken from Lighnat0r's scripts (the mouse sens reset script and the y-axis fix ASI, which this is a fork of; the mouse sens reset fix didn't have its own repo).  
Mouse icon within the program icon taken from [Freepik on Flaticon](https://www.flaticon.com/free-icons/mouse)