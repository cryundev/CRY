#pragma once


#include "Source/Core/CRSmartPtrMacro.h"
#include "Source/Object/Camera/CRCamera.h"
#include "Source/Utility/Time/CRTime.h"
#include <windows.h>


extern CRSharedPtr< CRCamera > GCamera;
extern CRTime GFrameTime;


namespace CREngine
{
    /// Initialize.
    void Initialize( HWND hWnd, unsigned int Width, unsigned int Height );

    /// Tick.
    void Tick( float DeltaSeconds );

    /// Pre-render.
    bool PreRender( float DeltaSeconds );

    /// Render.
    void Render( float DeltaSeconds );

    /// Post-render.
    void PostRender( float DeltaSeconds );
}
