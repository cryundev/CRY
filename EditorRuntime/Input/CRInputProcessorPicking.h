#pragma once


#include "Source/Core/CRTypes.h"
#include <Windows.h>


//---------------------------------------------------------------------------------------------------------------------
/// Win32 viewport input bridge (picking).
//---------------------------------------------------------------------------------------------------------------------
namespace CREditorRuntimeWin32Input
{
    /// Handle mouse message.
    void OnPickingMouseMessage( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );
}
