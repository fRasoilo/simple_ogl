#pragma once


//Simple OGL --------------

//=============================================================================
// Summary :
//
// simple_ogl is small library that provides a quick and simple way to create an 
// OpenGL ready Window.
//
// It is a single file header that should be very easy to drop-in and start using in any pre-existing project.
//
//
//=============================================================================
// Revision History :
//
//  
//
//===============================================================================  
//  You MUST define SIMPLE_OGL_IMPLEMENTATION
//
//  in exactly *one* C or C++ file that includes this header, BEFORE the include
//  like so:
//
//  #define SIMPLE_OGL_IMPLEMENTATION
//  #include "simple_ogl.h"
//
//  All other files should just include "simple_ogl.h" without the define
//=============================================================================
// Basic Usage / Quick Start :
//
// 
//
//===============================================================================
// API  : API reference can be found further down.
//        In the following sections (search for) :
//===============================================================================  

//@TODO: Need to link to OpenGL32.lib



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

//[INTERNAL] OpenGL Enums and Types not found in gl.h
//@NOTE: gl.h can be found in almost all win32 systems while the other headers are not usually present.

typedef char			GLchar;
typedef ptrdiff_t		GLsizeiptr;
typedef uint64_t    	GLuint64;

//Enums in glcorearb.h from https://www.khronos.org/registry/OpenGL/index_gl.php#headers
#define GL_TEXTURE0								0x84C0
#define GL_TEXTURE1								0x84C1
#define GL_TEXTURE_RECTANGLE					0x84F5
#define GL_RGBA_INTEGER							0x8D99
#define GL_HALF_FLOAT							0x140B
#define GL_RGBA16UI								0x8D76
#define GL_RGBA16F								0x881A
#define GL_VERTEX_SHADER						0x8B31
#define GL_FRAGMENT_SHADER						0x8B30
#define GL_ARRAY_BUFFER							0x8892
#define GL_ELEMENT_ARRAY_BUFFER					0x8893
#define GL_STATIC_DRAW							0x88E4
#define GL_WRITE_ONLY							0x88B9
#define GL_QUERY_RESULT							0x8866
#define GL_TIME_ELAPSED							0x88BF
#define GL_TIMESTAMP							0x8E28
#define GL_FRAMEBUFFER_SRGB						0x8DB9

// WGL Enums -- in wglext.h from https://www.khronos.org/registry/OpenGL/index_gl.php#headers
#ifdef _WIN32

    #define WGL_CONTEXT_MAJOR_VERSION_ARB			0x2091
    #define WGL_CONTEXT_MINOR_VERSION_ARB			0x2092
    #define WGL_CONTEXT_PROFILE_MASK_ARB			0x9126
    #define WGL_CONTEXT_CORE_PROFILE_BIT_ARB		0x00000001
    #define WGL_DRAW_TO_WINDOW_ARB					0x2001
    #define WGL_ACCELERATION_ARB					0x2003
    #define WGL_SUPPORT_OPENGL_ARB					0x2010
    #define WGL_DOUBLE_BUFFER_ARB					0x2011
    #define WGL_PIXEL_TYPE_ARB						0x2013
    #define WGL_COLOR_BITS_ARB						0x2014
    #define WGL_ALPHA_BITS_ARB						0x201B
    #define WGL_FULL_ACCELERATION_ARB				0x2027
    #define WGL_TYPE_RGBA_ARB						0x202B
    #define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB		0x20A9
    #define WGL_DEPTH_BITS_ARB                      0x2022
    #define WGL_STENCIL_BITS_ARB                    0x2023

#endif //_WIN32

//@TODO: Explain this bit more
//Macro for Declaring OpenGL Function Pointers
#define DECLARE_GL_FUNC_PTR(return_type, func_name, params) typedef return_type func_name##_func_signature params; \
                                                            func_name##_func_signature *func_name = nullptr; 

//Macro for getting the adress of the given OpenGL function and checking we have a valid address (not null).
#define GET_GL_FUNC_SAFE(name) name = (name##_func_signature *)wglGetProcAddress(#name); \
                               SGL_Assert(name);



//
//--Win32 Specific --

//@TODO: The default Window32 callback will need to link to User32.lib
//@TODO: SwapBuffers will need to link to Gdi32.lib

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <gl/gl.h>

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


//[INTERNAL] - Declaring Win32 specific OpenGL function pointers.
DECLARE_GL_FUNC_PTR(BOOL ,wglChoosePixelFormatARB, (HDC , const int *, const FLOAT *, UINT , int *, UINT *))
DECLARE_GL_FUNC_PTR(HGLRC ,wglCreateContextAttribsARB, (HDC , HGLRC , const int *))





internal void 
sgl_win32_window_setup(Win32Window* window,char* title = "SimpleOGL Window", int32 width = 1920, int32 height = 1080, bool32 full_screen = 0)
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

//@TODO: Check the name on this.
internal bool32
sgl_win32_window_ogl_setup(Win32Window* window, int32 major_version = 0, int32 minor_version = 0)
{
    window->device_context = GetDC(window->handle);

     //Setup Pixel Format
    PIXELFORMATDESCRIPTOR desired_pixel_format = {};
    desired_pixel_format.nSize = sizeof(desired_pixel_format);
    desired_pixel_format.nVersion = 1;   
    desired_pixel_format.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
    desired_pixel_format.iPixelType = PFD_TYPE_RGBA;
    desired_pixel_format.cColorBits = 32; //@TODO: Allow to be specified by user
    desired_pixel_format.cDepthBits = 8; //@TODO: Allow to be specified by user
    desired_pixel_format.cStencilBits = 8;//@TODO: Allow to be specified by user
    desired_pixel_format.cAlphaBits   = 8;//@TODO: Allow to be specified by user
    desired_pixel_format.iLayerType = PFD_MAIN_PLANE;

    int32 suggested_format_index = ChoosePixelFormat(window->device_context, &desired_pixel_format);

    PIXELFORMATDESCRIPTOR suggested_pixel_format;
    DescribePixelFormat(window->device_context, suggested_format_index, 
                        sizeof(suggested_pixel_format), &suggested_pixel_format);
    SetPixelFormat(window->device_context, suggested_format_index, &suggested_pixel_format);
    
    window->rendering_context = wglCreateContext(window->device_context);
    if(wglMakeCurrent(window->device_context, window->rendering_context))
    {
        //We can now create a modern OpenGL context.

        //@TODO: Load OpenGL extensions and functions.
        GET_GL_FUNC_SAFE(wglCreateContextAttribsARB)

        if(wglCreateContextAttribsARB)
        {
            const int context_attrib_list[] =
            {
                //WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                //WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0, 0 //End
            };
            
            HGLRC share_context = 0;
            HGLRC modern_gl_context = wglCreateContextAttribsARB(window->device_context,share_context,context_attrib_list );
        
            if(modern_gl_context)
            {
                if(wglMakeCurrent(window->device_context, modern_gl_context))
                {
                    wglDeleteContext(window->rendering_context);
                    window->rendering_context = modern_gl_context;
                }
            }
        }
        else
        {
            //@TODO: Could not create modern gl context, old version of OGL
        }

        //@TODO: Get OpenGL context info
        char* vendor     = (char *)glGetString(GL_VENDOR);
        char* renderer   = (char *)glGetString(GL_RENDERER);
        char* version    = (char *)glGetString(GL_VERSION);
        char* extensions = (char *)glGetString(GL_EXTENSIONS);
    }
    else
    {
        SGL_Assert(!"We should not be here!");
    }

    ReleaseDC(window->handle, window->device_context);

    return true;
}

#endif //_WIN32
