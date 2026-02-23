#include "EngineDLL.h"
#include "Source/Utility/UtilRay.h"


//---------------------------------------------------------------------------------------------------------------------
/// PickActorAtScreen
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API CRIdentity::id_t PickActorAtScreen( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH )
{
    return UtilRay::PickActorAtScreen( PixelX, PixelY, ViewportW, ViewportH );
}
