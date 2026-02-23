#include "CRInputProcessorPicking.h"
#include "Source/Utility/Log/CRLog.h"
#include "Source/Utility/UtilRay.h"
#include <string>


//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
void CRInputProcessorPicking::Initialize()
{
}

//---------------------------------------------------------------------------------------------------------------------
/// Handle mouse message.
//---------------------------------------------------------------------------------------------------------------------
void CRInputProcessorPicking::OnMouseMessage( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    (void)wParam;

    if ( Message != WM_LBUTTONDOWN ) return;

    RECT clientRect = {};
    if ( !GetClientRect( hWnd, &clientRect ) ) return;

    const i32 viewportW = clientRect.right - clientRect.left;
    const i32 viewportH = clientRect.bottom - clientRect.top;
    if ( viewportW <= 0 || viewportH <= 0 ) return;

    const i32 pixelX = (i32)(i16)LOWORD( lParam );
    const i32 pixelY = (i32)(i16)HIWORD( lParam );

    const CRIdentity::id_t pickedActorId = UtilRay::PickActorAtScreen( pixelX, pixelY, viewportW, viewportH );
    
    _LogPickResult( pixelX, pixelY, pickedActorId );
}

//---------------------------------------------------------------------------------------------------------------------
/// Log pick result.
//---------------------------------------------------------------------------------------------------------------------
void CRInputProcessorPicking::_LogPickResult( i32 PixelX, i32 PixelY, const CRIdentity::id_t& PickedActorId )
{
    if ( CRIdentity::IsValid( PickedActorId ) )
    {
        GLog.AddLog( "Win32 pick hit: Id=" + std::to_string( (u64)PickedActorId ) + " | Pixel=(" + std::to_string( PixelX ) + "," + std::to_string( PixelY ) + ")" );
    }
    else
    {
        GLog.AddLog( "Win32 pick miss | Pixel=(" + std::to_string( PixelX ) + "," + std::to_string( PixelY ) + ")" );
    }
}
