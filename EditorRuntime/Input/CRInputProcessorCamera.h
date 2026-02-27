#pragma once


#include "Source/Core/CRTypes.h"
#include <Windows.h>


//---------------------------------------------------------------------------------------------------------------------
/// Win32 viewport input bridge (camera).
//---------------------------------------------------------------------------------------------------------------------
namespace CREditorRuntimeWin32Input
{
    /// Handle mouse-related window message.
    void OnMouseMessage( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );

    /// Handle keyboard-related window message.
    void OnKeyboardMessage( WPARAM wParam, LPARAM lParam, bool bPressed );

    /// Release cached input states on focus loss.
    void OnKillFocus( HWND hWnd );

    /// Tick.
    void TickCamera( f32 DeltaTime );
}
