#pragma once


#include "CRTime.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRFrameUpdator
//---------------------------------------------------------------------------------------------------------------------
class CRFrameUpdator
{
private:
    CRTime Time;
    int    FPS = 30;
    
public:
    /// Initialize.
    void Initialize( int InFPS );

    /// Update.
    bool Update();
};
