#include "CRFrameUpdator.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
void CRFrameUpdator::Initialize( int InFPS )
{
    FPS = InFPS;
}

//---------------------------------------------------------------------------------------------------------------------
/// Update.
//---------------------------------------------------------------------------------------------------------------------
bool CRFrameUpdator::Update( float DeltaSeconds )
{
    AccumulatedSeconds += DeltaSeconds;

    bool bElapsed = AccumulatedSeconds > ( 1.0f / (float)( FPS ) );

    if ( bElapsed )
    {
        AccumulatedSeconds -= ( 1.0f / (float)( FPS ) );
    }

    return bElapsed;
}
