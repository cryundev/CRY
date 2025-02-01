#pragma once


#include "CRTime.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRFrameUpdator
//---------------------------------------------------------------------------------------------------------------------
class CRFrameUpdator
{
private:
    int   FPS                = 30;
    float AccumulatedSeconds = 0.f;
    
public:
    /// Initialize.
    void Initialize( int InFPS );

    /// Update.
    bool Update( float DeltaSeconds = 0.f );
};
