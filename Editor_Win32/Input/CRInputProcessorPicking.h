#pragma once


#include "Source/Core/CRTypes.h"
#include "Source/Core/Identify/CRIdentity.h"
#include <Windows.h>


//---------------------------------------------------------------------------------------------------------------------
/// CRInputProcessorPicking
//---------------------------------------------------------------------------------------------------------------------
class CRInputProcessorPicking
{
public:
    /// Initialize.
    void Initialize();

    /// Handle mouse message.
    void OnMouseMessage( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );

private:
    /// Log pick result.
    static void _LogPickResult( i32 PixelX, i32 PixelY, const CRIdentity::id_t& PickedActorId );
};
