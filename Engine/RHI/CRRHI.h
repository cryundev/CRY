#pragma once


#include <windows.h>


namespace CRRHI
{
    /// Initialize RHI.
    void Initialize( HWND hWnd, unsigned int Width, unsigned int Height );

    // Render frame RHI.
    void RenderFrame();    
}
