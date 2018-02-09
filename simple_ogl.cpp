#define SIMPLE_OGL_IMPLEMENTATION
#define SGL_DEFAULT_EXAMPLE 

#include "simple_ogl.h"


int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{

    HMODULE module = GetModuleHandle(0);
    sgl_window(&default_main_window,Instance,sgl_win32_window_callback);
    //Default Example
    sgl_init_default_state();
    //Loop
    while(default_main_window.running)
    {
        sgl_win32_process_msgs();
        sgl_default_render(&default_main_window);
    }   
    return 0;
}