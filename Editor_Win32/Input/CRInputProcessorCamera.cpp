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
    
    if ( k.W ) GCamera->Transform.Location += GCamera->Transform.GetUp()    * MoveDistancePerSecond * DeltaTime;
    if ( k.S ) GCamera->Transform.Location -= GCamera->Transform.GetUp()    * MoveDistancePerSecond * DeltaTime;
    if ( k.A ) GCamera->Transform.Location += GCamera->Transform.GetRight() * MoveDistancePerSecond * DeltaTime;
    if ( k.D ) GCamera->Transform.Location -= GCamera->Transform.GetRight() * MoveDistancePerSecond * DeltaTime;
    
    auto m = GMouse.GetState();
    
    MouseTracker.Update( m );
    
    if ( m.positionMode == DirectX::Mouse::MODE_RELATIVE )
    {
        if ( m.x > 0 || m.y > 0 )
        {
            int i = 0;
        }
        GCamera->Transform.Rotate( GCamera->Transform.GetUp(),    m.x * RotateDistancePerSecond * DeltaTime );
        GCamera->Transform.Rotate( GCamera->Transform.GetRight(), m.y * RotateDistancePerSecond * DeltaTime );
    }
    
    GMouse.SetMode( m.leftButton ? DirectX::Mouse::MODE_RELATIVE : DirectX::Mouse::MODE_ABSOLUTE );
}
