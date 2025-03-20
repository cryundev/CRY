#include "CRInputProcessorCamera.h"
#include "Editor_Win32.h"
#include "Engine.h"
#include "Source/World/CRWorld.h"


//---------------------------------------------------------------------------------------------------------------------
/// Tick.
//---------------------------------------------------------------------------------------------------------------------
void CRInputProcessorCamera::Tick( float DeltaTime )
{
    auto k = GKeyboard.GetState();
    
    KeyboardTracker.Update( k );
    
    if ( k.W ) GWorld->GetCamera()->GetTransform()->Location += GWorld->GetCamera()->GetTransform()->GetUp()    * MoveDistancePerSecond * DeltaTime;
    if ( k.S ) GWorld->GetCamera()->GetTransform()->Location -= GWorld->GetCamera()->GetTransform()->GetUp()    * MoveDistancePerSecond * DeltaTime;
    if ( k.A ) GWorld->GetCamera()->GetTransform()->Location += GWorld->GetCamera()->GetTransform()->GetRight() * MoveDistancePerSecond * DeltaTime;
    if ( k.D ) GWorld->GetCamera()->GetTransform()->Location -= GWorld->GetCamera()->GetTransform()->GetRight() * MoveDistancePerSecond * DeltaTime;
    
    auto m = GMouse.GetState();
    
    MouseTracker.Update( m );
    
    if ( m.positionMode == DirectX::Mouse::MODE_RELATIVE )
    {
        if ( m.x > 0 || m.y > 0 )
        {
            int i = 0;
        }
        GWorld->GetCamera()->GetTransform()->Rotate( GWorld->GetCamera()->GetTransform()->GetUp(),    m.x * RotateDistancePerSecond * DeltaTime );
        GWorld->GetCamera()->GetTransform()->Rotate( GWorld->GetCamera()->GetTransform()->GetRight(), m.y * RotateDistancePerSecond * DeltaTime );
    }
    
    GMouse.SetMode( m.leftButton ? DirectX::Mouse::MODE_RELATIVE : DirectX::Mouse::MODE_ABSOLUTE );
}
