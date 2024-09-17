// dear imgui: Platform Backend for Windows (standard windows API for 32-bits AND 64-bits applications)
// This needs to be used along with a Renderer (e.g. DirectX11, OpenGL3, Vulkan..)

// Implemented features:
//  [X] Platform: Clipboard support (for Win32 this is actually part of core dear imgui)
//  [X] Platform: Mouse support. Can discriminate Mouse/TouchScreen/Pen.
//  [X] Platform: Keyboard support. Since 1.87 we are using the io.AddKeyEvent() function. Pass SegaUiKey values to all key functions e.g. SegaUi::IsKeyPressed(SegaUiKey_Space). [Legacy VK_* values will also be supported unless SEGAUI_DISABLE_OBSOLETE_KEYIO is set]
//  [X] Platform: Gamepad support. Enabled with 'io.ConfigFlags |= SegaUiConfigFlags_NavEnableGamepad'.
//  [X] Platform: Mouse cursor shape and visibility. Disable with 'io.ConfigFlags |= SegaUiConfigFlags_NoMouseCursorChange'.

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// If you are new to Dear SegaUi, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_win32.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <windowsx.h> // GET_X_LPARAM(), GET_Y_LPARAM()
#include <tchar.h>
#include <dwmapi.h>

// Configuration flags to add in your imconfig.h file:
//#define SEGAUI_IMPL_WIN32_DISABLE_GAMEPAD              // Disable gamepad support. This was meaningful before <1.81 but we now load XInput dynamically so the option is now less relevant.

// Using XInput for gamepad (will load DLL dynamically)
#ifndef SEGAUI_IMPL_WIN32_DISABLE_GAMEPAD
#include <xinput.h>
typedef DWORD (WINAPI *PFN_XInputGetCapabilities)(DWORD, DWORD, XINPUT_CAPABILITIES*);
typedef DWORD (WINAPI *PFN_XInputGetState)(DWORD, XINPUT_STATE*);
#endif

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2023-04-19: Added SegaUi_ImplWin32_InitForOpenGL() to facilitate combining raw Win32/Winapi with OpenGL. (#3218)
//  2023-04-04: Inputs: Added support for io.AddMouseSourceEvent() to discriminate SegaUiMouseSource_Mouse/SegaUiMouseSource_TouchScreen/SegaUiMouseSource_Pen. (#2702)
//  2023-02-15: Inputs: Use WM_NCMOUSEMOVE / WM_NCMOUSELEAVE to track mouse position over non-client area (e.g. OS decorations) when app is not focused. (#6045, #6162)
//  2023-02-02: Inputs: Flipping WM_MOUSEHWHEEL (horizontal mouse-wheel) value to match other backends and offer consistent horizontal scrolling direction. (#4019, #6096, #1463)
//  2022-10-11: Using 'nullptr' instead of 'NULL' as per our switch to C++11.
//  2022-09-28: Inputs: Convert WM_CHAR values with MultiByteToWideChar() when window class was registered as MBCS (not Unicode).
//  2022-09-26: Inputs: Renamed SegaUiKey_ModXXX introduced in 1.87 to SegaUiMod_XXX (old names still supported).
//  2022-01-26: Inputs: replaced short-lived io.AddKeyModsEvent() (added two weeks ago) with io.AddKeyEvent() using SegaUiKey_ModXXX flags. Sorry for the confusion.
//  2021-01-20: Inputs: calling new io.AddKeyAnalogEvent() for gamepad support, instead of writing directly to io.NavInputs[].
//  2022-01-17: Inputs: calling new io.AddMousePosEvent(), io.AddMouseButtonEvent(), io.AddMouseWheelEvent() API (1.87+).
//  2022-01-17: Inputs: always update key mods next and before a key event (not in NewFrame) to fix input queue with very low framerates.
//  2022-01-12: Inputs: Update mouse inputs using WM_MOUSEMOVE/WM_MOUSELEAVE + fallback to provide it when focused but not hovered/captured. More standard and will allow us to pass it to future input queue API.
//  2022-01-12: Inputs: Maintain our own copy of MouseButtonsDown mask instead of using SegaUi::IsAnyMouseDown() which will be obsoleted.
//  2022-01-10: Inputs: calling new io.AddKeyEvent(), io.AddKeyModsEvent() + io.SetKeyEventNativeData() API (1.87+). Support for full SegaUiKey range.
//  2021-12-16: Inputs: Fill VK_LCONTROL/VK_RCONTROL/VK_LSHIFT/VK_RSHIFT/VK_LMENU/VK_RMENU for completeness.
//  2021-08-17: Calling io.AddFocusEvent() on WM_SETFOCUS/WM_KILLFOCUS messages.
//  2021-08-02: Inputs: Fixed keyboard modifiers being reported when host window doesn't have focus.
//  2021-07-29: Inputs: MousePos is correctly reported when the host platform window is hovered but not focused (using TrackMouseEvent() to receive WM_MOUSELEAVE events).
//  2021-06-29: Reorganized backend to pull data from a single structure to facilitate usage with multiple-contexts (all g_XXXX access changed to bd->XXXX).
//  2021-06-08: Fixed SegaUi_ImplWin32_EnableDpiAwareness() and SegaUi_ImplWin32_GetDpiScaleForMonitor() to handle Windows 8.1/10 features without a manifest (per-monitor DPI, and properly calls SetProcessDpiAwareness() on 8.1).
//  2021-03-23: Inputs: Clearing keyboard down array when losing focus (WM_KILLFOCUS).
//  2021-02-18: Added SegaUi_ImplWin32_EnableAlphaCompositing(). Non Visual Studio users will need to link with dwmapi.lib (MinGW/gcc: use -ldwmapi).
//  2021-02-17: Fixed SegaUi_ImplWin32_EnableDpiAwareness() attempting to get SetProcessDpiAwareness from shcore.dll on Windows 8 whereas it is only supported on Windows 8.1.
//  2021-01-25: Inputs: Dynamically loading XInput DLL.
//  2020-12-04: Misc: Fixed setting of io.DisplaySize to invalid/uninitialized data when after hwnd has been closed.
//  2020-03-03: Inputs: Calling AddInputCharacterUTF16() to support surrogate pairs leading to codepoint >= 0x10000 (for more complete CJK inputs)
//  2020-02-17: Added SegaUi_ImplWin32_EnableDpiAwareness(), SegaUi_ImplWin32_GetDpiScaleForHwnd(), SegaUi_ImplWin32_GetDpiScaleForMonitor() helper functions.
//  2020-01-14: Inputs: Added support for #define SEGAUI_IMPL_WIN32_DISABLE_GAMEPAD/SEGAUI_IMPL_WIN32_DISABLE_LINKING_XINPUT.
//  2019-12-05: Inputs: Added support for SegaUiMouseCursor_NotAllowed mouse cursor.
//  2019-05-11: Inputs: Don't filter value from WM_CHAR before calling AddInputCharacter().
//  2019-01-17: Misc: Using GetForegroundWindow()+IsChild() instead of GetActiveWindow() to be compatible with windows created in a different thread or parent.
//  2019-01-17: Inputs: Added support for mouse buttons 4 and 5 via WM_XBUTTON* messages.
//  2019-01-15: Inputs: Added support for XInput gamepads (if SegaUiConfigFlags_NavEnableGamepad is set by user application).
//  2018-11-30: Misc: Setting up io.BackendPlatformName so it can be displayed in the About Window.
//  2018-06-29: Inputs: Added support for the SegaUiMouseCursor_Hand cursor.
//  2018-06-10: Inputs: Fixed handling of mouse wheel messages to support fine position messages (typically sent by track-pads).
//  2018-06-08: Misc: Extracted imgui_impl_win32.cpp/.h away from the old combined DX9/DX10/DX11/DX12 examples.
//  2018-03-20: Misc: Setup io.BackendFlags SegaUiBackendFlags_HasMouseCursors and SegaUiBackendFlags_HasSetMousePos flags + honor SegaUiConfigFlags_NoMouseCursorChange flag.
//  2018-02-20: Inputs: Added support for mouse cursors (SegaUi::GetMouseCursor() value and WM_SETCURSOR message handling).
//  2018-02-06: Inputs: Added mapping for SegaUiKey_Space.
//  2018-02-06: Inputs: Honoring the io.WantSetMousePos by repositioning the mouse (when using navigation and SegaUiConfigFlags_NavMoveMouse is set).
//  2018-02-06: Misc: Removed call to SegaUi::Shutdown() which is not available from 1.60 WIP, user needs to call CreateContext/DestroyContext themselves.
//  2018-01-20: Inputs: Added Horizontal Mouse Wheel support.
//  2018-01-08: Inputs: Added mapping for SegaUiKey_Insert.
//  2018-01-05: Inputs: Added WM_LBUTTONDBLCLK double-click handlers for window classes with the CS_DBLCLKS flag.
//  2017-10-23: Inputs: Added WM_SYSKEYDOWN / WM_SYSKEYUP handlers so e.g. the VK_MENU key can be read.
//  2017-10-23: Inputs: Using Win32 ::SetCapture/::GetCapture() to retrieve mouse positions outside the client area when dragging.
//  2016-11-12: Inputs: Only call Win32 ::SetCursor(nullptr) when io.MouseDrawCursor is set.

struct SegaUi_ImplWin32_Data
{
    HWND                        hWnd;
    HWND                        MouseHwnd;
    int                         MouseTrackedArea;   // 0: not tracked, 1: client are, 2: non-client area
    int                         MouseButtonsDown;
    INT64                       Time;
    INT64                       TicksPerSecond;
    SegaUiMouseCursor            LastMouseCursor;

#ifndef SEGAUI_IMPL_WIN32_DISABLE_GAMEPAD
    bool                        HasGamepad;
    bool                        WantUpdateHasGamepad;
    HMODULE                     XInputDLL;
    PFN_XInputGetCapabilities   XInputGetCapabilities;
    PFN_XInputGetState          XInputGetState;
#endif

    SegaUi_ImplWin32_Data()      { memset((void*)this, 0, sizeof(*this)); }
};

// Backend data stored in io.BackendPlatformUserData to allow support for multiple Dear SegaUi contexts
// It is STRONGLY preferred that you use docking branch with multi-viewports (== single Dear SegaUi context + multiple windows) instead of multiple Dear SegaUi contexts.
// FIXME: multi-context support is not well tested and probably dysfunctional in this backend.
// FIXME: some shared resources (mouse cursor shape, gamepad) are mishandled when using multi-context.
static SegaUi_ImplWin32_Data* SegaUi_ImplWin32_GetBackendData()
{
    return SegaUi::GetCurrentContext() ? (SegaUi_ImplWin32_Data*)SegaUi::GetIO().BackendPlatformUserData : nullptr;
}

// Functions
static bool SegaUi_ImplWin32_InitEx(void* hwnd, bool platform_has_own_dc)
{
    SegaUiIO& io = SegaUi::GetIO();
    IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");

    INT64 perf_frequency, perf_counter;
    if (!::QueryPerformanceFrequency((LARGE_INTEGER*)&perf_frequency))
        return false;
    if (!::QueryPerformanceCounter((LARGE_INTEGER*)&perf_counter))
        return false;

    // Setup backend capabilities flags
    SegaUi_ImplWin32_Data* bd = IM_NEW(SegaUi_ImplWin32_Data)();
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = "imgui_impl_win32";
    io.BackendFlags |= SegaUiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= SegaUiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)

    bd->hWnd = (HWND)hwnd;
    bd->TicksPerSecond = perf_frequency;
    bd->Time = perf_counter;
    bd->LastMouseCursor = SegaUiMouseCursor_COUNT;

    // Set platform dependent data in viewport
    SegaUi::GetMainViewport()->PlatformHandleRaw = (void*)hwnd;
    IM_UNUSED(platform_has_own_dc); // Used in 'docking' branch

    // Dynamically load XInput library
#ifndef SEGAUI_IMPL_WIN32_DISABLE_GAMEPAD
    bd->WantUpdateHasGamepad = true;
    const char* xinput_dll_names[] =
    {
        "xinput1_4.dll",   // Windows 8+
        "xinput1_3.dll",   // DirectX SDK
        "xinput9_1_0.dll", // Windows Vista, Windows 7
        "xinput1_2.dll",   // DirectX SDK
        "xinput1_1.dll"    // DirectX SDK
    };
    for (int n = 0; n < IM_ARRAYSIZE(xinput_dll_names); n++)
        if (HMODULE dll = ::LoadLibraryA(xinput_dll_names[n]))
        {
            bd->XInputDLL = dll;
            bd->XInputGetCapabilities = (PFN_XInputGetCapabilities)::GetProcAddress(dll, "XInputGetCapabilities");
            bd->XInputGetState = (PFN_XInputGetState)::GetProcAddress(dll, "XInputGetState");
            break;
        }
#endif // SEGAUI_IMPL_WIN32_DISABLE_GAMEPAD

    return true;
}

SEGAUI_IMPL_API bool     SegaUi_ImplWin32_Init(void* hwnd)
{
    return SegaUi_ImplWin32_InitEx(hwnd, false);
}

SEGAUI_IMPL_API bool     SegaUi_ImplWin32_InitForOpenGL(void* hwnd)
{
    // OpenGL needs CS_OWNDC
    return SegaUi_ImplWin32_InitEx(hwnd, true);
}

void    SegaUi_ImplWin32_Shutdown()
{
    SegaUi_ImplWin32_Data* bd = SegaUi_ImplWin32_GetBackendData();
    IM_ASSERT(bd != nullptr && "No platform backend to shutdown, or already shutdown?");
    SegaUiIO& io = SegaUi::GetIO();

    // Unload XInput library
#ifndef SEGAUI_IMPL_WIN32_DISABLE_GAMEPAD
    if (bd->XInputDLL)
        ::FreeLibrary(bd->XInputDLL);
#endif // SEGAUI_IMPL_WIN32_DISABLE_GAMEPAD

    io.BackendPlatformName = nullptr;
    io.BackendPlatformUserData = nullptr;
    io.BackendFlags &= ~(SegaUiBackendFlags_HasMouseCursors | SegaUiBackendFlags_HasSetMousePos | SegaUiBackendFlags_HasGamepad);
    IM_DELETE(bd);
}

static bool SegaUi_ImplWin32_UpdateMouseCursor()
{
    SegaUiIO& io = SegaUi::GetIO();
    if (io.ConfigFlags & SegaUiConfigFlags_NoMouseCursorChange)
        return false;

    SegaUiMouseCursor imgui_cursor = SegaUi::GetMouseCursor();
    if (imgui_cursor == SegaUiMouseCursor_None || io.MouseDrawCursor)
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        ::SetCursor(nullptr);
    }
    else
    {
        // Show OS mouse cursor
        LPTSTR win32_cursor = IDC_ARROW;
        switch (imgui_cursor)
        {
        case SegaUiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
        case SegaUiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
        case SegaUiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
        case SegaUiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
        case SegaUiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
        case SegaUiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
        case SegaUiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
        case SegaUiMouseCursor_Hand:         win32_cursor = IDC_HAND; break;
        case SegaUiMouseCursor_NotAllowed:   win32_cursor = IDC_NO; break;
        }
        ::SetCursor(::LoadCursor(nullptr, win32_cursor));
    }
    return true;
}

static bool IsVkDown(int vk)
{
    return (::GetKeyState(vk) & 0x8000) != 0;
}

static void SegaUi_ImplWin32_AddKeyEvent(SegaUiKey key, bool down, int native_keycode, int native_scancode = -1)
{
    SegaUiIO& io = SegaUi::GetIO();
    io.AddKeyEvent(key, down);
    io.SetKeyEventNativeData(key, native_keycode, native_scancode); // To support legacy indexing (<1.87 user code)
    IM_UNUSED(native_scancode);
}

static void SegaUi_ImplWin32_ProcessKeyEventsWorkarounds()
{
    // Left & right Shift keys: when both are pressed together, Windows tend to not generate the WM_KEYUP event for the first released one.
    if (SegaUi::IsKeyDown(SegaUiKey_LeftShift) && !IsVkDown(VK_LSHIFT))
        SegaUi_ImplWin32_AddKeyEvent(SegaUiKey_LeftShift, false, VK_LSHIFT);
    if (SegaUi::IsKeyDown(SegaUiKey_RightShift) && !IsVkDown(VK_RSHIFT))
        SegaUi_ImplWin32_AddKeyEvent(SegaUiKey_RightShift, false, VK_RSHIFT);

    // Sometimes WM_KEYUP for Win key is not passed down to the app (e.g. for Win+V on some setups, according to GLFW).
    if (SegaUi::IsKeyDown(SegaUiKey_LeftSuper) && !IsVkDown(VK_LWIN))
        SegaUi_ImplWin32_AddKeyEvent(SegaUiKey_LeftSuper, false, VK_LWIN);
    if (SegaUi::IsKeyDown(SegaUiKey_RightSuper) && !IsVkDown(VK_RWIN))
        SegaUi_ImplWin32_AddKeyEvent(SegaUiKey_RightSuper, false, VK_RWIN);
}

static void SegaUi_ImplWin32_UpdateKeyModifiers()
{
    SegaUiIO& io = SegaUi::GetIO();
    io.AddKeyEvent(SegaUiMod_Ctrl, IsVkDown(VK_CONTROL));
    io.AddKeyEvent(SegaUiMod_Shift, IsVkDown(VK_SHIFT));
    io.AddKeyEvent(SegaUiMod_Alt, IsVkDown(VK_MENU));
    io.AddKeyEvent(SegaUiMod_Super, IsVkDown(VK_APPS));
}

static void SegaUi_ImplWin32_UpdateMouseData()
{
    SegaUi_ImplWin32_Data* bd = SegaUi_ImplWin32_GetBackendData();
    SegaUiIO& io = SegaUi::GetIO();
    IM_ASSERT(bd->hWnd != 0);

    HWND focused_window = ::GetForegroundWindow();
    const bool is_app_focused = (focused_window == bd->hWnd);
    if (is_app_focused)
    {
        // (Optional) Set OS mouse position from Dear SegaUi if requested (rarely used, only when SegaUiConfigFlags_NavEnableSetMousePos is enabled by user)
        if (io.WantSetMousePos)
        {
            POINT pos = { (int)io.MousePos.x, (int)io.MousePos.y };
            if (::ClientToScreen(bd->hWnd, &pos))
                ::SetCursorPos(pos.x, pos.y);
        }

        // (Optional) Fallback to provide mouse position when focused (WM_MOUSEMOVE already provides this when hovered or captured)
        // This also fills a short gap when clicking non-client area: WM_NCMOUSELEAVE -> modal OS move -> gap -> WM_NCMOUSEMOVE
        if (!io.WantSetMousePos && bd->MouseTrackedArea == 0)
        {
            POINT pos;
            if (::GetCursorPos(&pos) && ::ScreenToClient(bd->hWnd, &pos))
                io.AddMousePosEvent((float)pos.x, (float)pos.y);
        }
    }
}

// Gamepad navigation mapping
static void SegaUi_ImplWin32_UpdateGamepads()
{
#ifndef SEGAUI_IMPL_WIN32_DISABLE_GAMEPAD
    SegaUiIO& io = SegaUi::GetIO();
    SegaUi_ImplWin32_Data* bd = SegaUi_ImplWin32_GetBackendData();
    //if ((io.ConfigFlags & SegaUiConfigFlags_NavEnableGamepad) == 0) // FIXME: Technically feeding gamepad shouldn't depend on this now that they are regular inputs.
    //    return;

    // Calling XInputGetState() every frame on disconnected gamepads is unfortunately too slow.
    // Instead we refresh gamepad availability by calling XInputGetCapabilities() _only_ after receiving WM_DEVICECHANGE.
    if (bd->WantUpdateHasGamepad)
    {
        XINPUT_CAPABILITIES caps = {};
        bd->HasGamepad = bd->XInputGetCapabilities ? (bd->XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &caps) == ERROR_SUCCESS) : false;
        bd->WantUpdateHasGamepad = false;
    }

    io.BackendFlags &= ~SegaUiBackendFlags_HasGamepad;
    XINPUT_STATE xinput_state;
    XINPUT_GAMEPAD& gamepad = xinput_state.Gamepad;
    if (!bd->HasGamepad || bd->XInputGetState == nullptr || bd->XInputGetState(0, &xinput_state) != ERROR_SUCCESS)
        return;
    io.BackendFlags |= SegaUiBackendFlags_HasGamepad;

    #define IM_SATURATE(V)                      (V < 0.0f ? 0.0f : V > 1.0f ? 1.0f : V)
    #define MAP_BUTTON(KEY_NO, BUTTON_ENUM)     { io.AddKeyEvent(KEY_NO, (gamepad.wButtons & BUTTON_ENUM) != 0); }
    #define MAP_ANALOG(KEY_NO, VALUE, V0, V1)   { float vn = (float)(VALUE - V0) / (float)(V1 - V0); io.AddKeyAnalogEvent(KEY_NO, vn > 0.10f, IM_SATURATE(vn)); }
    MAP_BUTTON(SegaUiKey_GamepadStart,           XINPUT_GAMEPAD_START);
    MAP_BUTTON(SegaUiKey_GamepadBack,            XINPUT_GAMEPAD_BACK);
    MAP_BUTTON(SegaUiKey_GamepadFaceLeft,        XINPUT_GAMEPAD_X);
    MAP_BUTTON(SegaUiKey_GamepadFaceRight,       XINPUT_GAMEPAD_B);
    MAP_BUTTON(SegaUiKey_GamepadFaceUp,          XINPUT_GAMEPAD_Y);
    MAP_BUTTON(SegaUiKey_GamepadFaceDown,        XINPUT_GAMEPAD_A);
    MAP_BUTTON(SegaUiKey_GamepadDpadLeft,        XINPUT_GAMEPAD_DPAD_LEFT);
    MAP_BUTTON(SegaUiKey_GamepadDpadRight,       XINPUT_GAMEPAD_DPAD_RIGHT);
    MAP_BUTTON(SegaUiKey_GamepadDpadUp,          XINPUT_GAMEPAD_DPAD_UP);
    MAP_BUTTON(SegaUiKey_GamepadDpadDown,        XINPUT_GAMEPAD_DPAD_DOWN);
    MAP_BUTTON(SegaUiKey_GamepadL1,              XINPUT_GAMEPAD_LEFT_SHOULDER);
    MAP_BUTTON(SegaUiKey_GamepadR1,              XINPUT_GAMEPAD_RIGHT_SHOULDER);
    MAP_ANALOG(SegaUiKey_GamepadL2,              gamepad.bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, 255);
    MAP_ANALOG(SegaUiKey_GamepadR2,              gamepad.bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, 255);
    MAP_BUTTON(SegaUiKey_GamepadL3,              XINPUT_GAMEPAD_LEFT_THUMB);
    MAP_BUTTON(SegaUiKey_GamepadR3,              XINPUT_GAMEPAD_RIGHT_THUMB);
    MAP_ANALOG(SegaUiKey_GamepadLStickLeft,      gamepad.sThumbLX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
    MAP_ANALOG(SegaUiKey_GamepadLStickRight,     gamepad.sThumbLX, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
    MAP_ANALOG(SegaUiKey_GamepadLStickUp,        gamepad.sThumbLY, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
    MAP_ANALOG(SegaUiKey_GamepadLStickDown,      gamepad.sThumbLY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
    MAP_ANALOG(SegaUiKey_GamepadRStickLeft,      gamepad.sThumbRX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
    MAP_ANALOG(SegaUiKey_GamepadRStickRight,     gamepad.sThumbRX, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
    MAP_ANALOG(SegaUiKey_GamepadRStickUp,        gamepad.sThumbRY, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
    MAP_ANALOG(SegaUiKey_GamepadRStickDown,      gamepad.sThumbRY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
    #undef MAP_BUTTON
    #undef MAP_ANALOG
#endif // #ifndef SEGAUI_IMPL_WIN32_DISABLE_GAMEPAD
}

void    SegaUi_ImplWin32_NewFrame()
{
    SegaUiIO& io = SegaUi::GetIO();
    SegaUi_ImplWin32_Data* bd = SegaUi_ImplWin32_GetBackendData();
    IM_ASSERT(bd != nullptr && "Did you call SegaUi_ImplWin32_Init()?");

    // Setup display size (every frame to accommodate for window resizing)
    RECT rect = { 0, 0, 0, 0 };
    ::GetClientRect(bd->hWnd, &rect);
    io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

    // Setup time step
    INT64 current_time = 0;
    ::QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
    io.DeltaTime = (float)(current_time - bd->Time) / bd->TicksPerSecond;
    bd->Time = current_time;

    // Update OS mouse position
    SegaUi_ImplWin32_UpdateMouseData();

    // Process workarounds for known Windows key handling issues
    SegaUi_ImplWin32_ProcessKeyEventsWorkarounds();

    // Update OS mouse cursor with the cursor requested by imgui
    SegaUiMouseCursor mouse_cursor = io.MouseDrawCursor ? SegaUiMouseCursor_None : SegaUi::GetMouseCursor();
    if (bd->LastMouseCursor != mouse_cursor)
    {
        bd->LastMouseCursor = mouse_cursor;
        SegaUi_ImplWin32_UpdateMouseCursor();
    }

    // Update game controllers (if enabled and available)
    SegaUi_ImplWin32_UpdateGamepads();
}

// There is no distinct VK_xxx for keypad enter, instead it is VK_RETURN + KF_EXTENDED, we assign it an arbitrary value to make code more readable (VK_ codes go up to 255)
#define IM_VK_KEYPAD_ENTER      (VK_RETURN + 256)

// Map VK_xxx to SegaUiKey_xxx.
static SegaUiKey SegaUi_ImplWin32_VirtualKeyToSegaUiKey(WPARAM wParam)
{
    switch (wParam)
    {
        case VK_TAB: return SegaUiKey_Tab;
        case VK_LEFT: return SegaUiKey_LeftArrow;
        case VK_RIGHT: return SegaUiKey_RightArrow;
        case VK_UP: return SegaUiKey_UpArrow;
        case VK_DOWN: return SegaUiKey_DownArrow;
        case VK_PRIOR: return SegaUiKey_PageUp;
        case VK_NEXT: return SegaUiKey_PageDown;
        case VK_HOME: return SegaUiKey_Home;
        case VK_END: return SegaUiKey_End;
        case VK_INSERT: return SegaUiKey_Insert;
        case VK_DELETE: return SegaUiKey_Delete;
        case VK_BACK: return SegaUiKey_Backspace;
        case VK_SPACE: return SegaUiKey_Space;
        case VK_RETURN: return SegaUiKey_Enter;
        case VK_ESCAPE: return SegaUiKey_Escape;
        case VK_OEM_7: return SegaUiKey_Apostrophe;
        case VK_OEM_COMMA: return SegaUiKey_Comma;
        case VK_OEM_MINUS: return SegaUiKey_Minus;
        case VK_OEM_PERIOD: return SegaUiKey_Period;
        case VK_OEM_2: return SegaUiKey_Slash;
        case VK_OEM_1: return SegaUiKey_Semicolon;
        case VK_OEM_PLUS: return SegaUiKey_Equal;
        case VK_OEM_4: return SegaUiKey_LeftBracket;
        case VK_OEM_5: return SegaUiKey_Backslash;
        case VK_OEM_6: return SegaUiKey_RightBracket;
        case VK_OEM_3: return SegaUiKey_GraveAccent;
        case VK_CAPITAL: return SegaUiKey_CapsLock;
        case VK_SCROLL: return SegaUiKey_ScrollLock;
        case VK_NUMLOCK: return SegaUiKey_NumLock;
        case VK_SNAPSHOT: return SegaUiKey_PrintScreen;
        case VK_PAUSE: return SegaUiKey_Pause;
        case VK_NUMPAD0: return SegaUiKey_Keypad0;
        case VK_NUMPAD1: return SegaUiKey_Keypad1;
        case VK_NUMPAD2: return SegaUiKey_Keypad2;
        case VK_NUMPAD3: return SegaUiKey_Keypad3;
        case VK_NUMPAD4: return SegaUiKey_Keypad4;
        case VK_NUMPAD5: return SegaUiKey_Keypad5;
        case VK_NUMPAD6: return SegaUiKey_Keypad6;
        case VK_NUMPAD7: return SegaUiKey_Keypad7;
        case VK_NUMPAD8: return SegaUiKey_Keypad8;
        case VK_NUMPAD9: return SegaUiKey_Keypad9;
        case VK_DECIMAL: return SegaUiKey_KeypadDecimal;
        case VK_DIVIDE: return SegaUiKey_KeypadDivide;
        case VK_MULTIPLY: return SegaUiKey_KeypadMultiply;
        case VK_SUBTRACT: return SegaUiKey_KeypadSubtract;
        case VK_ADD: return SegaUiKey_KeypadAdd;
        case IM_VK_KEYPAD_ENTER: return SegaUiKey_KeypadEnter;
        case VK_LSHIFT: return SegaUiKey_LeftShift;
        case VK_LCONTROL: return SegaUiKey_LeftCtrl;
        case VK_LMENU: return SegaUiKey_LeftAlt;
        case VK_LWIN: return SegaUiKey_LeftSuper;
        case VK_RSHIFT: return SegaUiKey_RightShift;
        case VK_RCONTROL: return SegaUiKey_RightCtrl;
        case VK_RMENU: return SegaUiKey_RightAlt;
        case VK_RWIN: return SegaUiKey_RightSuper;
        case VK_APPS: return SegaUiKey_Menu;
        case '0': return SegaUiKey_0;
        case '1': return SegaUiKey_1;
        case '2': return SegaUiKey_2;
        case '3': return SegaUiKey_3;
        case '4': return SegaUiKey_4;
        case '5': return SegaUiKey_5;
        case '6': return SegaUiKey_6;
        case '7': return SegaUiKey_7;
        case '8': return SegaUiKey_8;
        case '9': return SegaUiKey_9;
        case 'A': return SegaUiKey_A;
        case 'B': return SegaUiKey_B;
        case 'C': return SegaUiKey_C;
        case 'D': return SegaUiKey_D;
        case 'E': return SegaUiKey_E;
        case 'F': return SegaUiKey_F;
        case 'G': return SegaUiKey_G;
        case 'H': return SegaUiKey_H;
        case 'I': return SegaUiKey_I;
        case 'J': return SegaUiKey_J;
        case 'K': return SegaUiKey_K;
        case 'L': return SegaUiKey_L;
        case 'M': return SegaUiKey_M;
        case 'N': return SegaUiKey_N;
        case 'O': return SegaUiKey_O;
        case 'P': return SegaUiKey_P;
        case 'Q': return SegaUiKey_Q;
        case 'R': return SegaUiKey_R;
        case 'S': return SegaUiKey_S;
        case 'T': return SegaUiKey_T;
        case 'U': return SegaUiKey_U;
        case 'V': return SegaUiKey_V;
        case 'W': return SegaUiKey_W;
        case 'X': return SegaUiKey_X;
        case 'Y': return SegaUiKey_Y;
        case 'Z': return SegaUiKey_Z;
        case VK_F1: return SegaUiKey_F1;
        case VK_F2: return SegaUiKey_F2;
        case VK_F3: return SegaUiKey_F3;
        case VK_F4: return SegaUiKey_F4;
        case VK_F5: return SegaUiKey_F5;
        case VK_F6: return SegaUiKey_F6;
        case VK_F7: return SegaUiKey_F7;
        case VK_F8: return SegaUiKey_F8;
        case VK_F9: return SegaUiKey_F9;
        case VK_F10: return SegaUiKey_F10;
        case VK_F11: return SegaUiKey_F11;
        case VK_F12: return SegaUiKey_F12;
        default: return SegaUiKey_None;
    }
}

// Allow compilation with old Windows SDK. MinGW doesn't have default _WIN32_WINNT/WINVER versions.
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef DBT_DEVNODES_CHANGED
#define DBT_DEVNODES_CHANGED 0x0007
#endif

// Win32 message handler (process Win32 mouse/keyboard inputs, etc.)
// Call from your application's message handler. Keep calling your message handler unless this function returns TRUE.
// When implementing your own backend, you can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if Dear SegaUi wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to Dear SegaUi, and hide them from your application based on those two flags.
// PS: In this Win32 handler, we use the capture API (GetCapture/SetCapture/ReleaseCapture) to be able to read mouse coordinates when dragging mouse outside of our window bounds.
// PS: We treat DBLCLK messages as regular mouse down messages, so this code will work on windows classes that have the CS_DBLCLKS flag set. Our own example app code doesn't set this flag.
#if 0
// Copy this line into your .cpp file to forward declare the function.
extern SEGAUI_IMPL_API LRESULT SegaUi_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

// See https://learn.microsoft.com/en-us/windows/win32/tablet/system-events-and-mouse-messages
// Prefer to call this at the top of the message handler to avoid the possibility of other Win32 calls interfering with this.
static SegaUiMouseSource GetMouseSourceFromMessageExtraInfo()
{
    LPARAM extra_info = ::GetMessageExtraInfo();
    if ((extra_info & 0xFFFFFF80) == 0xFF515700)
        return SegaUiMouseSource_Pen;
    if ((extra_info & 0xFFFFFF80) == 0xFF515780)
        return SegaUiMouseSource_TouchScreen;
    return SegaUiMouseSource_Mouse;
}

SEGAUI_IMPL_API LRESULT SegaUi_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (SegaUi::GetCurrentContext() == nullptr)
        return 0;

    SegaUiIO& io = SegaUi::GetIO();
    SegaUi_ImplWin32_Data* bd = SegaUi_ImplWin32_GetBackendData();

    switch (msg)
    {
    case WM_MOUSEMOVE:
    case WM_NCMOUSEMOVE:
    {
        // We need to call TrackMouseEvent in order to receive WM_MOUSELEAVE events
        SegaUiMouseSource mouse_source = GetMouseSourceFromMessageExtraInfo();
        const int area = (msg == WM_MOUSEMOVE) ? 1 : 2;
        bd->MouseHwnd = hwnd;
        if (bd->MouseTrackedArea != area)
        {
            TRACKMOUSEEVENT tme_cancel = { sizeof(tme_cancel), TME_CANCEL, hwnd, 0 };
            TRACKMOUSEEVENT tme_track = { sizeof(tme_track), (DWORD)((area == 2) ? (TME_LEAVE | TME_NONCLIENT) : TME_LEAVE), hwnd, 0 };
            if (bd->MouseTrackedArea != 0)
                ::TrackMouseEvent(&tme_cancel);
            ::TrackMouseEvent(&tme_track);
            bd->MouseTrackedArea = area;
        }
        POINT mouse_pos = { (LONG)GET_X_LPARAM(lParam), (LONG)GET_Y_LPARAM(lParam) };
        if (msg == WM_NCMOUSEMOVE && ::ScreenToClient(hwnd, &mouse_pos) == FALSE) // WM_NCMOUSEMOVE are provided in absolute coordinates.
            break;
        io.AddMouseSourceEvent(mouse_source);
        io.AddMousePosEvent((float)mouse_pos.x, (float)mouse_pos.y);
        break;
    }
    case WM_MOUSELEAVE:
    case WM_NCMOUSELEAVE:
    {
        const int area = (msg == WM_MOUSELEAVE) ? 1 : 2;
        if (bd->MouseTrackedArea == area)
        {
            if (bd->MouseHwnd == hwnd)
                bd->MouseHwnd = nullptr;
            bd->MouseTrackedArea = 0;
            io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
        }
        break;
    }
    case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
    case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
    {
        SegaUiMouseSource mouse_source = GetMouseSourceFromMessageExtraInfo();
        int button = 0;
        if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { button = 0; }
        if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { button = 1; }
        if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { button = 2; }
        if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
        if (bd->MouseButtonsDown == 0 && ::GetCapture() == nullptr)
            ::SetCapture(hwnd);
        bd->MouseButtonsDown |= 1 << button;
        io.AddMouseSourceEvent(mouse_source);
        io.AddMouseButtonEvent(button, true);
        return 0;
    }
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP:
    {
        SegaUiMouseSource mouse_source = GetMouseSourceFromMessageExtraInfo();
        int button = 0;
        if (msg == WM_LBUTTONUP) { button = 0; }
        if (msg == WM_RBUTTONUP) { button = 1; }
        if (msg == WM_MBUTTONUP) { button = 2; }
        if (msg == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
        bd->MouseButtonsDown &= ~(1 << button);
        if (bd->MouseButtonsDown == 0 && ::GetCapture() == hwnd)
            ::ReleaseCapture();
        io.AddMouseSourceEvent(mouse_source);
        io.AddMouseButtonEvent(button, false);
        return 0;
    }
    case WM_MOUSEWHEEL:
        io.AddMouseWheelEvent(0.0f, (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
        return 0;
    case WM_MOUSEHWHEEL:
        io.AddMouseWheelEvent(-(float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA, 0.0f);
        return 0;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    {
        const bool is_key_down = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
        if (wParam < 256)
        {
            // Submit modifiers
            SegaUi_ImplWin32_UpdateKeyModifiers();

            // Obtain virtual key code
            // (keypad enter doesn't have its own... VK_RETURN with KF_EXTENDED flag means keypad enter, see IM_VK_KEYPAD_ENTER definition for details, it is mapped to SegaUiKey_KeyPadEnter.)
            int vk = (int)wParam;
            if ((wParam == VK_RETURN) && (HIWORD(lParam) & KF_EXTENDED))
                vk = IM_VK_KEYPAD_ENTER;

            // Submit key event
            const SegaUiKey key = SegaUi_ImplWin32_VirtualKeyToSegaUiKey(vk);
            const int scancode = (int)LOBYTE(HIWORD(lParam));
            if (key != SegaUiKey_None)
                SegaUi_ImplWin32_AddKeyEvent(key, is_key_down, vk, scancode);

            // Submit individual left/right modifier events
            if (vk == VK_SHIFT)
            {
                // Important: Shift keys tend to get stuck when pressed together, missing key-up events are corrected in SegaUi_ImplWin32_ProcessKeyEventsWorkarounds()
                if (IsVkDown(VK_LSHIFT) == is_key_down) { SegaUi_ImplWin32_AddKeyEvent(SegaUiKey_LeftShift, is_key_down, VK_LSHIFT, scancode); }
                if (IsVkDown(VK_RSHIFT) == is_key_down) { SegaUi_ImplWin32_AddKeyEvent(SegaUiKey_RightShift, is_key_down, VK_RSHIFT, scancode); }
            }
            else if (vk == VK_CONTROL)
            {
                if (IsVkDown(VK_LCONTROL) == is_key_down) { SegaUi_ImplWin32_AddKeyEvent(SegaUiKey_LeftCtrl, is_key_down, VK_LCONTROL, scancode); }
                if (IsVkDown(VK_RCONTROL) == is_key_down) { SegaUi_ImplWin32_AddKeyEvent(SegaUiKey_RightCtrl, is_key_down, VK_RCONTROL, scancode); }
            }
            else if (vk == VK_MENU)
            {
                if (IsVkDown(VK_LMENU) == is_key_down) { SegaUi_ImplWin32_AddKeyEvent(SegaUiKey_LeftAlt, is_key_down, VK_LMENU, scancode); }
                if (IsVkDown(VK_RMENU) == is_key_down) { SegaUi_ImplWin32_AddKeyEvent(SegaUiKey_RightAlt, is_key_down, VK_RMENU, scancode); }
            }
        }
        return 0;
    }
    case WM_SETFOCUS:
    case WM_KILLFOCUS:
        io.AddFocusEvent(msg == WM_SETFOCUS);
        return 0;
    case WM_CHAR:
        if (::IsWindowUnicode(hwnd))
        {
            // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
            if (wParam > 0 && wParam < 0x10000)
                io.AddInputCharacterUTF16((unsigned short)wParam);
        }
        else
        {
            wchar_t wch = 0;
            ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (char*)&wParam, 1, &wch, 1);
            io.AddInputCharacter(wch);
        }
        return 0;
    case WM_SETCURSOR:
        // This is required to restore cursor when transitioning from e.g resize borders to client area.
        if (LOWORD(lParam) == HTCLIENT && SegaUi_ImplWin32_UpdateMouseCursor())
            return 1;
        return 0;
    case WM_DEVICECHANGE:
#ifndef SEGAUI_IMPL_WIN32_DISABLE_GAMEPAD
        if ((UINT)wParam == DBT_DEVNODES_CHANGED)
            bd->WantUpdateHasGamepad = true;
#endif
        return 0;
    }
    return 0;
}


//--------------------------------------------------------------------------------------------------------
// DPI-related helpers (optional)
//--------------------------------------------------------------------------------------------------------
// - Use to enable DPI awareness without having to create an application manifest.
// - Your own app may already do this via a manifest or explicit calls. This is mostly useful for our examples/ apps.
// - In theory we could call simple functions from Windows SDK such as SetProcessDPIAware(), SetProcessDpiAwareness(), etc.
//   but most of the functions provided by Microsoft require Windows 8.1/10+ SDK at compile time and Windows 8/10+ at runtime,
//   neither we want to require the user to have. So we dynamically select and load those functions to avoid dependencies.
//---------------------------------------------------------------------------------------------------------
// This is the scheme successfully used by GLFW (from which we borrowed some of the code) and other apps aiming to be highly portable.
// SegaUi_ImplWin32_EnableDpiAwareness() is just a helper called by main.cpp, we don't call it automatically.
// If you are trying to implement your own backend for your own engine, you may ignore that noise.
//---------------------------------------------------------------------------------------------------------

// Perform our own check with RtlVerifyVersionInfo() instead of using functions from <VersionHelpers.h> as they
// require a manifest to be functional for checks above 8.1. See https://github.com/ocornut/imgui/issues/4200
static BOOL _IsWindowsVersionOrGreater(WORD major, WORD minor, WORD)
{
    typedef LONG(WINAPI* PFN_RtlVerifyVersionInfo)(OSVERSIONINFOEXW*, ULONG, ULONGLONG);
    static PFN_RtlVerifyVersionInfo RtlVerifyVersionInfoFn = nullptr;
	if (RtlVerifyVersionInfoFn == nullptr)
		if (HMODULE ntdllModule = ::GetModuleHandleA("ntdll.dll"))
			RtlVerifyVersionInfoFn = (PFN_RtlVerifyVersionInfo)GetProcAddress(ntdllModule, "RtlVerifyVersionInfo");
    if (RtlVerifyVersionInfoFn == nullptr)
        return FALSE;

    RTL_OSVERSIONINFOEXW versionInfo = { };
    ULONGLONG conditionMask = 0;
    versionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
    versionInfo.dwMajorVersion = major;
	versionInfo.dwMinorVersion = minor;
	VER_SET_CONDITION(conditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
	VER_SET_CONDITION(conditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
	return (RtlVerifyVersionInfoFn(&versionInfo, VER_MAJORVERSION | VER_MINORVERSION, conditionMask) == 0) ? TRUE : FALSE;
}

#define _IsWindowsVistaOrGreater()   _IsWindowsVersionOrGreater(HIBYTE(0x0600), LOBYTE(0x0600), 0) // _WIN32_WINNT_VISTA
#define _IsWindows8OrGreater()       _IsWindowsVersionOrGreater(HIBYTE(0x0602), LOBYTE(0x0602), 0) // _WIN32_WINNT_WIN8
#define _IsWindows8Point1OrGreater() _IsWindowsVersionOrGreater(HIBYTE(0x0603), LOBYTE(0x0603), 0) // _WIN32_WINNT_WINBLUE
#define _IsWindows10OrGreater()      _IsWindowsVersionOrGreater(HIBYTE(0x0A00), LOBYTE(0x0A00), 0) // _WIN32_WINNT_WINTHRESHOLD / _WIN32_WINNT_WIN10

#ifndef DPI_ENUMS_DECLARED
typedef enum { PROCESS_DPI_UNAWARE = 0, PROCESS_SYSTEM_DPI_AWARE = 1, PROCESS_PER_MONITOR_DPI_AWARE = 2 } PROCESS_DPI_AWARENESS;
typedef enum { MDT_EFFECTIVE_DPI = 0, MDT_ANGULAR_DPI = 1, MDT_RAW_DPI = 2, MDT_DEFAULT = MDT_EFFECTIVE_DPI } MONITOR_DPI_TYPE;
#endif
#ifndef _DPI_AWARENESS_CONTEXTS_
DECLARE_HANDLE(DPI_AWARENESS_CONTEXT);
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE    (DPI_AWARENESS_CONTEXT)-3
#endif
#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 (DPI_AWARENESS_CONTEXT)-4
#endif
typedef HRESULT(WINAPI* PFN_SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS);                     // Shcore.lib + dll, Windows 8.1+
typedef HRESULT(WINAPI* PFN_GetDpiForMonitor)(HMONITOR, MONITOR_DPI_TYPE, UINT*, UINT*);        // Shcore.lib + dll, Windows 8.1+
typedef DPI_AWARENESS_CONTEXT(WINAPI* PFN_SetThreadDpiAwarenessContext)(DPI_AWARENESS_CONTEXT); // User32.lib + dll, Windows 10 v1607+ (Creators Update)

// Helper function to enable DPI awareness without setting up a manifest
void SegaUi_ImplWin32_EnableDpiAwareness()
{
    if (_IsWindows10OrGreater())
    {
        static HINSTANCE user32_dll = ::LoadLibraryA("user32.dll"); // Reference counted per-process
        if (PFN_SetThreadDpiAwarenessContext SetThreadDpiAwarenessContextFn = (PFN_SetThreadDpiAwarenessContext)::GetProcAddress(user32_dll, "SetThreadDpiAwarenessContext"))
        {
            SetThreadDpiAwarenessContextFn(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
            return;
        }
    }
    if (_IsWindows8Point1OrGreater())
    {
        static HINSTANCE shcore_dll = ::LoadLibraryA("shcore.dll"); // Reference counted per-process
        if (PFN_SetProcessDpiAwareness SetProcessDpiAwarenessFn = (PFN_SetProcessDpiAwareness)::GetProcAddress(shcore_dll, "SetProcessDpiAwareness"))
        {
            SetProcessDpiAwarenessFn(PROCESS_PER_MONITOR_DPI_AWARE);
            return;
        }
    }
#if _WIN32_WINNT >= 0x0600
    ::SetProcessDPIAware();
#endif
}

#if defined(_MSC_VER) && !defined(NOGDI)
#pragma comment(lib, "gdi32")   // Link with gdi32.lib for GetDeviceCaps(). MinGW will require linking with '-lgdi32'
#endif

float SegaUi_ImplWin32_GetDpiScaleForMonitor(void* monitor)
{
    UINT xdpi = 96, ydpi = 96;
    if (_IsWindows8Point1OrGreater())
    {
		static HINSTANCE shcore_dll = ::LoadLibraryA("shcore.dll"); // Reference counted per-process
		static PFN_GetDpiForMonitor GetDpiForMonitorFn = nullptr;
		if (GetDpiForMonitorFn == nullptr && shcore_dll != nullptr)
            GetDpiForMonitorFn = (PFN_GetDpiForMonitor)::GetProcAddress(shcore_dll, "GetDpiForMonitor");
		if (GetDpiForMonitorFn != nullptr)
		{
			GetDpiForMonitorFn((HMONITOR)monitor, MDT_EFFECTIVE_DPI, &xdpi, &ydpi);
            IM_ASSERT(xdpi == ydpi); // Please contact me if you hit this assert!
			return xdpi / 96.0f;
		}
    }
#ifndef NOGDI
    const HDC dc = ::GetDC(nullptr);
    xdpi = ::GetDeviceCaps(dc, LOGPIXELSX);
    ydpi = ::GetDeviceCaps(dc, LOGPIXELSY);
    IM_ASSERT(xdpi == ydpi); // Please contact me if you hit this assert!
    ::ReleaseDC(nullptr, dc);
#endif
    return xdpi / 96.0f;
}

float SegaUi_ImplWin32_GetDpiScaleForHwnd(void* hwnd)
{
    HMONITOR monitor = ::MonitorFromWindow((HWND)hwnd, MONITOR_DEFAULTTONEAREST);
    return SegaUi_ImplWin32_GetDpiScaleForMonitor(monitor);
}

//---------------------------------------------------------------------------------------------------------
// Transparency related helpers (optional)
//--------------------------------------------------------------------------------------------------------

#if defined(_MSC_VER)
#pragma comment(lib, "dwmapi")  // Link with dwmapi.lib. MinGW will require linking with '-ldwmapi'
#endif

// [experimental]
// Borrowed from GLFW's function updateFramebufferTransparency() in src/win32_window.c
// (the Dwm* functions are Vista era functions but we are borrowing logic from GLFW)
void SegaUi_ImplWin32_EnableAlphaCompositing(void* hwnd)
{
    if (!_IsWindowsVistaOrGreater())
        return;

    BOOL composition;
    if (FAILED(::DwmIsCompositionEnabled(&composition)) || !composition)
        return;

    BOOL opaque;
    DWORD color;
    if (_IsWindows8OrGreater() || (SUCCEEDED(::DwmGetColorizationColor(&color, &opaque)) && !opaque))
    {
        HRGN region = ::CreateRectRgn(0, 0, -1, -1);
        DWM_BLURBEHIND bb = {};
        bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
        bb.hRgnBlur = region;
        bb.fEnable = TRUE;
        ::DwmEnableBlurBehindWindow((HWND)hwnd, &bb);
        ::DeleteObject(region);
    }
    else
    {
        DWM_BLURBEHIND bb = {};
        bb.dwFlags = DWM_BB_ENABLE;
        ::DwmEnableBlurBehindWindow((HWND)hwnd, &bb);
    }
}

//---------------------------------------------------------------------------------------------------------
