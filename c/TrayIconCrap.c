NOTIFYICONDATAW trayIcon;

char makeTrayIcon(HWND windowHandle);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void createWindow()
{
    WNDCLASSW windowClass =
    {
        0,  // style
        WndProc,    // lpfnWndProc (window procedure)
        0,  // cbClsExtra (extra bytes to allocate following window class structure)
        0,  // cbWndExtra (extra bytes to allocate following window instance)
        GetModuleHandleW(NULL), //hInstance
        LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(1)), // hIcon
        0,  // hCursor
        0,  //hbrBackground, for some painting thing
        0,  // lpszMenuName, resource name of class menu
        L"VC Mouse Fix"  // lpszClassName
    };
    RegisterClassW(&windowClass);

    CreateWindowW
    (
        L"VC Mouse Fix",
        L"VC Mouse Fix",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,  // window x
        0,              // window y, N/A if x is set to default
        CW_USEDEFAULT,  // same as above but for height/width
        0,
        0,  // hWnd
        0,  // hMenu
        GetModuleHandleW(NULL),
        0   // lpParam
    );
}

char makeTrayIcon(HWND windowHandle)
{
    NOTIFYICONDATAW temp =
    {
        sizeof(NOTIFYICONDATAW),    // MicrosoftStaff
        windowHandle,
        (UINT)1,
        NIF_ICON | NIF_TIP,
        WM_APP + 2,
        LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(1)),
        L"VC Mouse Fix",
        0,
        0,
        0,      // szInfo
        (UINT)500,    // timeout
        (UINT)0,      // version
        0,      // szInfoTitle
        0,       // dwInfoFlags
        0   // hBalloonIcon
    };
    trayIcon = temp;

    Shell_NotifyIconW(NIM_ADD, &trayIcon);
    Shell_NotifyIconW(NIM_SETVERSION, &trayIcon);
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //puts("Kippa");
    switch (uMsg)
    {
    case WM_CREATE:
        makeTrayIcon(hWnd);
        break;
    case WM_APP + 2:
        puts("Kippa");
        switch (lParam)
        {
        case WM_LBUTTONDOWN:
            puts("Kippa");
            ShowWindow(hWnd, SW_HIDE);
            break;
        }
        break;
    default:
        break;
    }
}