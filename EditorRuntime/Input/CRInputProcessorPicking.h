#pragma once


#include "Source/Core/CRTypes.h"
#include <Windows.h>


//---------------------------------------------------------------------------------------------------------------------
/// CREditorRuntimeWin32Input
//---------------------------------------------------------------------------------------------------------------------
namespace CREditorRuntimeWin32Input
{
    /// Handle mouse message.
    void OnPickingMouseMessage( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );
}
