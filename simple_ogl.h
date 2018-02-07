
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif


// [INTERNAL] Types --------------
#include <stdint.h>
//Unsigned
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t  uint8;
//Signed
typedef int32_t int32;
//Bool
typedef int32 bool32;

// [INTERNAL] Static Declarations
#define internal        static
#define global_variable static
#define local_persist   static

// [INTERNAL] Utility Macros

#define SGL_Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}

#define Bytes(n)  (n)
#define KiloBytes(n)  (Bytes(n)*1024)
#define InvalidCodePath SGL_Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;} break

struct Win32Window{
    int32 width;
    int32 height;
    HWND  handle;

    char* title;

    bool32 full_screen;
    bool32 running;

    WINDOWPLACEMENT window_position;
    
    HDC device_context;
    HGLRC rendering_context;    
};

//[INTERNAL] Function Pointers Typedefs
#define WIN32_WINDOW_CALLBACK(name) LRESULT CALLBACK name(HWND Window,UINT Message,WPARAM WParam,LPARAM LParam)
typedef WIN32_WINDOW_CALLBACK(win32_window_callback);

internal void 
sgl_win32_window_setup(Win32Window* window, int32 width, int32 height, char* title, bool32 full_screen = 0)
{    
    window->width  = width;//1920
    window->height = height;//1080
    window->handle = 0;
    window->title = title;
    window->full_screen = full_screen;
    window->running = true;
    window->window_position = {};
}

internal void
sgl_win32_window_create(Win32Window* win32_window,HINSTANCE instance,win32_window_callback* window_callback)
{

    //TODO: Make it so that we can make a fullscreen window.    
    WNDCLASSA window_class = {};
    window_class.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC; //TODO: Try CS_OWNDC
    window_class.lpfnWndProc = window_callback;
    window_class.hInstance = instance;
    window_class.hIcon = LoadIcon(instance, IDI_APPLICATION); //default icon
    window_class.lpszClassName = "SGL Window";
    window_class.hCursor = LoadCursor(0, IDC_ARROW);    //default cursor
    //ShowCursor(0);
    
    if(RegisterClassA(&window_class))
    {
        RECT window_rect = {0 ,0, win32_window->width, win32_window->height};
        DWORD style = WS_OVERLAPPEDWINDOW|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
        AdjustWindowRect(&window_rect, style, 0);
        
        win32_window->handle = CreateWindowExA( WS_EX_WINDOWEDGE/*|WS_EX_COMPOSITED*/, //WS_EX_TOPMOST|WS_EX_LAYERED, //TODO: Experiment with this 
                                                window_class.lpszClassName, 
                                                win32_window->title,
                                                style, //TODO: Experiment with this 
                                                CW_USEDEFAULT,
                                                CW_USEDEFAULT,
                                                window_rect.right -window_rect.left,
                                                window_rect.bottom -window_rect.top ,
                                                0, 0, instance, 0);
    }
    else
    {
        //TODO: Some sort of error reporting to user.
    }
}