#pragma once


#include "../CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRVertex
//---------------------------------------------------------------------------------------------------------------------
struct CRVertex
{
    CRVector   Position;
    CRVector2D TexCoord;
    CRVector   Normal;
    
};


extern CRVertex GCRVTriangle[ 3 ];
extern CRVertex GCRVRect    [ 4 ];
