#include "CRVertex.h"


CRVertex GCRVTriangle[ 3 ] =
{
    { .Position = CRVector(  0.0f, 0.5f, 0.0f ), .Color = CRVector( 1.0f, 0.0f, 0.0f ) },
    { .Position = CRVector(  0.5f, 0.0f, 0.0f ), .Color = CRVector( 0.0f, 1.0f, 0.0f ) },
    { .Position = CRVector( -0.5f, 0.0f, 0.0f ), .Color = CRVector( 0.0f, 0.0f, 1.0f ) }
};

CRVertex GCRVTriangle2[ 3 ] =
{
    { .Position = CRVector(  0.0f, -0.5f, 0.0f ), .Color = CRVector( 1.0f, 0.0f, 0.0f ) },
    { .Position = CRVector( -0.5f,  0.0f, 0.0f ), .Color = CRVector( 0.0f, 1.0f, 0.0f ) },
    { .Position = CRVector(  0.5f,  0.0f, 0.0f ), .Color = CRVector( 0.0f, 0.0f, 1.0f ) }
};