#pragma once


#include "Extras/DirectXTK/Inc/Keyboard.h"
#include "Extras/DirectXTK/Inc/Mouse.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRInputProcessorCamera
//---------------------------------------------------------------------------------------------------------------------
class CRInputProcessorCamera
{
protected:
    float MoveDistancePerSecond   = 0.001f;
    float RotateDistancePerSecond = 1.f;

    DirectX::Keyboard::KeyboardStateTracker KeyboardTracker;
    DirectX::Mouse::ButtonStateTracker      MouseTracker;

    
public:
    /// Tick.
    void Tick( float DeltaTime );
};
