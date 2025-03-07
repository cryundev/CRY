#include "CRInputProcessorCamera.h"
#include "Editor_Win32.h"
#include "Engine.h"


//---------------------------------------------------------------------------------------------------------------------
/// Tick.
//---------------------------------------------------------------------------------------------------------------------
void CRInputProcessorCamera::Tick( float DeltaTime )
{
    auto k = GKeyboard.GetState();
    
    KeyboardTracker.Update( k );
    
    if ( k.W ) GCamera->GetTransform().Location += GCamera->GetTransform().GetUp()    * MoveDistancePerSecond * DeltaTime;
    if ( k.S ) GCamera->GetTransform().Location -= GCamera->GetTransform().GetUp()    * MoveDistancePerSecond * DeltaTime;
    if ( k.A ) GCamera->GetTransform().Location += GCamera->GetTransform().GetRight() * MoveDistancePerSecond * DeltaTime;
    if ( k.D ) GCamera->GetTransform().Location -= GCamera->GetTransform().GetRight() * MoveDistancePerSecond * DeltaTime;
    
    auto m = GMouse.GetState();
    
    MouseTracker.Update( m );
    
    if ( m.positionMode == DirectX::Mouse::MODE_RELATIVE )
    {
        if ( m.x > 0 || m.y > 0 )
        {
            int i = 0;
        }
        GCamera->GetTransform().Rotate( GCamera->GetTransform().GetUp(),    m.x * RotateDistancePerSecond * DeltaTime );
        GCamera->GetTransform().Rotate( GCamera->GetTransform().GetRight(), m.y * RotateDistancePerSecond * DeltaTime );
    }
    
    GMouse.SetMode( m.leftButton ? DirectX::Mouse::MODE_RELATIVE : DirectX::Mouse::MODE_ABSOLUTE );
}
