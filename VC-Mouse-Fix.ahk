#SingleInstance force
OnExit, ExitSequence
SplitPath, A_ScriptName,,,,ProgramNameNoExt
menu, tray, tip, %ProgramNameNoExt%
menu, tray, NoStandard
menu, tray, Add, Restart Program, RestartSequence
menu, tray, Add, Exit, ExitSequence
RefreshRate = 20
TrayTip, VC Mouse Fix, The script will fix y-axis sens and prevent the game from resetting the mouse sensitivity when you reset. The effect will last until the game is closed.,20

;If (IsLabel("DebugFunctions") AND A_IsCompiled != 1)
;  gosub DebugFunctions
goto MainScript

MainScript:
  WindowClass := "Grand theft auto 3"
  WindowName := "GTA: Vice City"
  WinWait ahk_class %WindowClass%
  WinGetTitle, CurrentWindowName
  If (CurrentWindowName != WindowName)
    goto MainScript
  WinGet, PID, PID
  Memory(1, PID)
  Process, Exist, %PID%
  if ErrorLevel != 0
    VersionOffset := GameVersionCheck()
  else
    goto MainScript
  
  GameRunningAddress := 0x00400000
  
  if VersionOffset = -4088            ; Steam
  {
    SensResetAddress := 0x0046F391
    YSensFixAddress1 := 0x004795D2
    YSensFixAddress2 := 0x0047A36D
    YSensFixAddress3 := 0x0047AED5
    YSensFixAddress4 := 0x0047BF9F
    YSensFixAddress5 := 0x00481E93
    YSensFixTarget := 0x94CBD8        ; 9751512
  }
  Else if VersionOffset = -12280    ; JP
  {
    SensResetAddress := 0x0046F821    
    YSensFixAddress1 := 0x00479AC9    ; Sniper first-person aim
    YSensFixAddress2 := 0x0047A864    ; Rocket launcher first-person aim
    YSensFixAddress3 := 0x0047B3CC    ; M4/ruger first-person aim
    YSensFixAddress4 := 0x0047C496    ; Normal free aim
    YSensFixAddress5 := 0x0048238A    ; "Runabout" (classic controls?)
    YSensFixTarget := 0x94ABD8         ; 9743320
  }
  Else
  {
    SensResetAddress := 0x0046F4B1    ; Retail 1.0 and 1.1 probably
    YSensFixAddress1 := 0x004796F2    ; Y Sens Fix addresses are for Retail 1.x
    YSensFixAddress2 := 0x0047A48D
    YSensFixAddress3 := 0x0047AFF5
    YSensFixAddress4 := 0x0047C0BF
    YSensFixAddress5 := 0x00481FB3
    YSensFixTarget := 0x94DBD0        ; Retail 1.0; 9755600
;   YSensFixTarget := 0x94DBD8        ; Retail 1.1, commenting out till proper support added; 9755608
  }
  
  While Memory(3, GameRunningAddress, 1) != "Fail"
  {
    sleep %RefreshRate%
    if Memory(3, SensResetAddress, 1) != 0x90                                        
      loop 10
      {
        Address := SensResetAddress + A_Index - 1
        Memory(4, Address, 0x90, 1)
      }
    if Memory(3, YSensFixAddress1, 1) != YSensFixTarget                                           
      Memory(4, YSensFixAddress1, YSensFixTarget, 4)
    if Memory(3, YSensFixAddress2, 1) != YSensFixTarget                                           
      Memory(4, YSensFixAddress2, YSensFixTarget, 4)
    if Memory(3, YSensFixAddress3, 1) != YSensFixTarget                                           
      Memory(4, YSensFixAddress3, YSensFixTarget, 4)
    if Memory(3, YSensFixAddress4, 1) != YSensFixTarget                                           
      Memory(4, YSensFixAddress4, YSensFixTarget, 4)
    if Memory(3, YSensFixAddress5, 1) != YSensFixTarget                                           
      Memory(4, YSensFixAddress5, YSensFixTarget, 4)
  }
  sleep 5000
  goto MainScript

ExitSequence:
  exitapp

RestartSequence:
  reload
  sleep 1000
  return
  
; DebugFunctions:
;   Hotkey, F7, DebugListvars, On
;   return
  
; DebugListvars:
;   Listvars
;   return

Memory(Type=3,Param1=0,Param2=0,Param3=0,Param4=0)
{
  Static ProcessHandle
  If Type = 1
  {
    ProcessHandle := DllCall("OpenProcess","Int",2035711,"Int", 0,"UInt",Param1)
    If ProcessHandle = 0
    {
      Error := GetLastErrorMessage()
      outputdebug Open process error`: %Error%
      Return % DllCall("GetLastError")
    }
    Else
      Return 0
  }
  Else If Type = 2
    DllCall("CloseHandle","UInt",ProcessHandle)
  Else If Type = 3 ; Read memory value
  {
    Param2 := ((!Param2) ? 4 : Param2)
    VarSetCapacity(MVALUE,Param2,0)
    If (Param3 = "Unicode" OR Param3 = "Ascii")
    {
      CharLength := ((Param3 = "Unicode") ? 2 : 1)
      LoopAmount := Param2/CharLength
      Loop %LoopAmount%
      {
        ReadAddress := Param1+((A_Index-1)*CharLength)
        OutputChar%A_Index% := Memory(3, ReadAddress, CharLength)
        OutputChar%A_Index% := Chr(OutputChar%A_Index%)
        OutputString := OutputString . OutputChar%A_Index%
      }
      Return OutputString
    }
    If (ProcessHandle) && DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",Param1,"Str",MVALUE,"UInt",Param2,"UInt",0)
    {
      Result = 0
      Loop %Param2%
      Result += *(&MVALUE + A_Index-1) << 8*(A_Index-1)
      If (Param3 = "Float")
        Result := HexToFloat(Result)
      Return Result
    }
    Return !ProcessHandle ? "Handle Closed: " Closed : "Fail"
  }
  Else If Type = 4    ; Write memory value
  {
    If (Param4 = "Unicode" OR Param4 = "Ascii")
    {
      CharLength := ((Param4 = "Unicode") ? 2 : 1)
      StringSplit, InputStringArray, Param2
      Param3 := ((!Param3) ? InputStringArray0 : Param3)
      Loop %Param3%
      {
        TextCharacter%A_Index%Address := Param1  + (A_Index-1) * CharLength
        Memory(4, TextCharacter%A_Index%Address, 0, 1)
      }
      If (InputStringArray0 > Param3)
        InputStringArray0 := Param3
      Loop %InputStringArray0%
      {
        CurrentLetter := InputStringArray%A_Index%
        If CurrentLetter = `%
          CurrentLetter = 37
        Else
          CurrentLetter := Asc(CurrentLetter)
        TextCharacter%A_Index%Address := Param1 + (A_Index-1) * CharLength
        Memory(4, TextCharacter%A_Index%Address, CurrentLetter, 1)
      }
    }
    Else
    {
      If (Param4 = "Float")
        Param2 := FloatToHex(Param2)
      Param3 := ((!Param3) ? 4 : Param3)
      ;  WriteProcessMemory DLL call accepts 5 parameters (in order):
      ;  Process handle, base address (Param1), buffer of data to be written (Param2), # of bytes to be written, and 0 (unused in this program).
      If (ProcessHandle) && DllCall("WriteProcessMemory","UInt",ProcessHandle,"UInt",Param1,"Uint*",Param2,"Uint",Param3,"Uint",0)
        Return "Success"
      Return !ProcessHandle ? "Handle Closed: " closed : "Fail"
    }
  }
  Else If Type = 5
  {
    Param1 := Memory(3, Param1)
    If Param1 is not xdigit
      Return Param1
    Return Param1 + Param2
  }
}

GameVersionCheck()
{
  Value := Memory(3, 0x00608578, 1)
  if Value = 0x5D    ; Retail 1.0
    Return 0
  if Value = 0x81    ; Retail 1.1
    Return 8
  if Value = 0x5B    ; Steam
    Return -0xFF8
  if Value = 0x44    ; JP
    Return -0x2FF8
  Msgbox Error`: The script could not determine the version of GTA Vice City %Value%
}

GetLastErrorMessage()
{
  ErrorCode := DllCall("GetLastError")
  VarSetCapacity(ReturnedMessage, 2000)
  ReturnValue := DllCall("FormatMessage", "UInt", 0x1000, "UInt", 0, "UInt", ErrorCode, "UInt", 0x800, "Str", ReturnedMessage, "UInt", 2000, "UInt", 0)
  If ReturnValue = 0
  {
    ErrorCode := DllCall("GetLastError")
    outputdebug Error getting error message`: %ErrorCode%
  }
  Else
  {
    Error = %ErrorCode%`: %ReturnedMessage%
    return Error
  }
}

HexToFloat(x)
{
  Return (1-2*(x>>31)) * (2**((x>>23 & 255)-150)) * (0x800000 | x & 0x7FFFFF)
}

FloatToHex(f)
{
  form := A_FormatInteger
  SetFormat Integer, HEX
  v := DllCall("MulDiv", Float,f, Int,1, Int,1, UInt)
  SetFormat Integer, %form%
  Return v
}