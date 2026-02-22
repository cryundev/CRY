#include "EngineDLL.h"
#include "Source/Core/CRTypes.h"


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// Mouse state cache for editor viewport bridge.
//---------------------------------------------------------------------------------------------------------------------
struct CRViewportMouseState
{
    i32 PixelX = 0;
    i32 PixelY = 0;

    i32 ViewportWidth  = 1;
    i32 ViewportHeight = 1;

    f32 NdcX = 0.0f;
    f32 NdcY = 0.0f;

    bool bLeftPressed = false;
};

CRViewportMouseState GViewportMouseState;

//---------------------------------------------------------------------------------------------------------------------
/// Convert pixel coordinates to NDC.
//---------------------------------------------------------------------------------------------------------------------
bool TryConvertToNdc( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH, f32& OutNdcX, f32& OutNdcY )
{
    if ( ViewportW <= 0 || ViewportH <= 0 ) return false;

    OutNdcX = ( (f32)PixelX / (f32)ViewportW ) * 2.0f - 1.0f;
    OutNdcY = 1.0f - ( (f32)PixelY / (f32)ViewportH ) * 2.0f;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Store latest pointer state.
//---------------------------------------------------------------------------------------------------------------------
void StorePointerState( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH, f32 NdcX, f32 NdcY )
{
    GViewportMouseState.PixelX = PixelX;
    GViewportMouseState.PixelY = PixelY;

    GViewportMouseState.ViewportWidth  = ViewportW;
    GViewportMouseState.ViewportHeight = ViewportH;

    GViewportMouseState.NdcX = NdcX;
    GViewportMouseState.NdcY = NdcY;
}
}


//---------------------------------------------------------------------------------------------------------------------
/// OnViewportMouseMove
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API void OnViewportMouseMove( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH )
{
    f32 ndcX = 0.0f;
    f32 ndcY = 0.0f;

    if ( !TryConvertToNdc( PixelX, PixelY, ViewportW, ViewportH, ndcX, ndcY ) ) return;

    StorePointerState( PixelX, PixelY, ViewportW, ViewportH, ndcX, ndcY );
}

//---------------------------------------------------------------------------------------------------------------------
/// OnViewportMouseButton
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API void OnViewportMouseButton( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH, i32 Button, bool bPressed )
{
    f32 ndcX = 0.0f;
    f32 ndcY = 0.0f;

    if ( !TryConvertToNdc( PixelX, PixelY, ViewportW, ViewportH, ndcX, ndcY ) ) return;

    StorePointerState( PixelX, PixelY, ViewportW, ViewportH, ndcX, ndcY );

    if ( Button == 0 )
    {
        GViewportMouseState.bLeftPressed = bPressed;
    }
}
