#pragma once


//---------------------------------------------------------------------------------------------------------------------
/// CRInputProcessorCamera
//---------------------------------------------------------------------------------------------------------------------
class CRInputProcessorCamera
{
protected:
    float MoveDistancePerSecond = 1.f;
    
public:
    /// Tick.
    void Tick( float DeltaTime ) const;
};
