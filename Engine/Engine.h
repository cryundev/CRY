#pragma once


#include "Source/Core/CRSmartPtrMacro.h"
#include "Source/Object/Camera/CRCamera.h"
#include <windows.h>


extern CRSharedPtr< CRCamera > GCamera;


namespace CREngine
{
    /// Initialize.
    void Initialize( HWND hWnd, unsigned int Width, unsigned int Height );
    
    /// Call per message loop.
    void MessageLoop(); 
}
