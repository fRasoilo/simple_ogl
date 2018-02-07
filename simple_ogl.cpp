
#include "stdio.h"

#include "simple_ogl.h"

Win32Window main_window = {};

internal LRESULT CALLBACK
Win32_WindowCallback_(HWND   Window,UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    switch(Message)
    {
        case WM_CREATE:
        {
            OutputDebugStringA("WM_CREATE\n");
            //Win32SetupOpenGLWindow(Window);            
            return(0);
        }break;
            
        case WM_CLOSE:
        {
            main_window.running = false;
        }break;
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            //NOTE(filipe): Painting is done here
            /*if(global_ogl.RenderingContext)
              {
              display(&gwGUI);
              } 
            */
            //TODO(filipe): This seems like an alright fix for now.
            // Check how its done in the other source code.            
            RECT GLRect;
            RECT MainRect;
            GetClientRect(main_window.handle, &MainRect);
            //GetClientRect(GlobalGLWindow, &GLRect);            
            EndPaint(Window, &Paint);                   
        }break;
        case WM_SIZE:
        {
            /*
              OutputDebugStringA("Resize!\n");
              //TODO(filipe): Resize;
              if(global_ogl.RenderingContext)
              {
              MainWindowWidth = (int) LOWORD(LParam);
              SetWindowPos(GlobalGLWindow, 0,
              0 , 0,
              MainWindowWidth,
              MainWindowHeight,
              SWP_NOZORDER|SWP_NOCOPYBITS); //NOTE(filipe):  when the window is resized all the static controls move to their new position and then the rest of the window is repainted. This lack of synchronization appears as flicker.
              //You can avoid this by passing the SWP_NOCOPYBITS flag to SetWindowPos
              //Refer to http://stackoverflow.com/questions/20362622/static-controls-slightly-flicker-when-main-window-is-resized
              MainWindowHeight = (int) HIWORD(LParam); //Wont change for now.
              reshape(MainWindowWidth, MainWindowHeight);
              }
            */
        }break;
        case WM_MOUSEMOVE:
        {
            // get mouse coordinates from Windows
            float mouseX = LOWORD(LParam);
            float mouseY = HIWORD(LParam);
            //gwGUI.IO.MousePos = {mouseX, mouseY};
            RECT ClientRect;
            GetClientRect(main_window.handle, &ClientRect);
            //i32 AdjustedX = MainWindowWidth - ClientRect.right;
            //i32 AdjustedY = MainWindowHeight - ClientRect.bottom;
        }break;
        case WM_KEYDOWN:
        {
            switch(WParam)
            {
            }
        }break;
        
        default:
        {
            //NOTE(filipe): Default window handling.
            Result = DefWindowProcA(Window, Message, WParam, LParam);
        }break;
    }
    return(Result);
}

internal void
Win32_ProcessMessages_(void)
{
    MSG Message;
    while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
    {
        //NOTE(filipe): For now lets just translate and dispatch all message types
        //              trough Windows.
        TranslateMessage(&Message);
        //NOTE(filipe): This will pipe the messages to the Win32MainWindowCallback func
        DispatchMessageA(&Message);               
    }
}

internal void
Win32GLRender()
{
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    SwapBuffers(main_window.device_context);
}

int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{

    sgl_win32_window_setup(&main_window, 800, 600, "SimpleOGL");
    sgl_win32_window_create(&main_window,Instance,Win32_WindowCallback_);
    sgl_win32_window_ogl_setup(&main_window);

    //Loop
    while(main_window.running)
    {
        Win32_ProcessMessages_();
        Win32GLRender();
    }    

    printf("Hey there!\n");
    return 0;
}