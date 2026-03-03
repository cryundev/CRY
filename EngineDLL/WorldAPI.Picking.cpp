#include "EngineDLL.h"
#include "../EditorRuntime/EditorRuntime.h"


//---------------------------------------------------------------------------------------------------------------------
/// PickActorAtScreen
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API CRIdentity::id_t PickActorAtScreen( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH )
{
    return CREditorRuntime::PickActorOrBeginGizmoDrag( PixelX, PixelY, ViewportW, ViewportH );
}
