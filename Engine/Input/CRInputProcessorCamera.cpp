#include "CRInputProcessorCamera.h"
#include "Engine.h"


//---------------------------------------------------------------------------------------------------------------------
/// Tick.
//---------------------------------------------------------------------------------------------------------------------
void CRInputProcessorCamera::Tick( float DeltaTime ) const
{
    auto k = GKeyboard.GetState();

    if ( k.W ) GCamera.Transform.Location -= GCamera.Transform.GetUp()    * MoveDistancePerSecond * DeltaTime;
    if ( k.S ) GCamera.Transform.Location += GCamera.Transform.GetUp()    * MoveDistancePerSecond * DeltaTime;
    if ( k.A ) GCamera.Transform.Location -= GCamera.Transform.GetRight() * MoveDistancePerSecond * DeltaTime;
    if ( k.D ) GCamera.Transform.Location += GCamera.Transform.GetRight() * MoveDistancePerSecond * DeltaTime;
}
