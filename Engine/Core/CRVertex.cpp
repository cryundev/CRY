#include "CRVertex.h"


CRVertex GCRVTriangle[ 3 ] =
{
    { .Position = CRVector(  0.0f, 0.5f, 0.0f ), .TexCoord = CRVector2D( 0.5f, 0.5f ), .Normal = CRVector( 0.0f, 0.0f, 1.0f ) },
    { .Position = CRVector(  0.5f, 0.0f, 0.0f ), .TexCoord = CRVector2D( 0.0f, 1.0f ), .Normal = CRVector( 0.0f, 0.0f, 1.0f ) },
    { .Position = CRVector( -0.5f, 0.0f, 0.0f ), .TexCoord = CRVector2D( 1.0f, 1.0f ), .Normal = CRVector( 0.0f, 0.0f, 1.0f ) },
};


CRVertex GCRVRect[ 4 ] =
{
    { .Position = CRVector( -0.5f,  0.5f, 0.0f ), .TexCoord = CRVector2D( 0.0f, 0.0f ), .Normal = CRVector( 0.0f, 0.0f, 1.0f ) },
    { .Position = CRVector(  0.5f,  0.5f, 0.0f ), .TexCoord = CRVector2D( 1.0f, 0.0f ), .Normal = CRVector( 0.0f, 0.0f, 1.0f ) },
    { .Position = CRVector( -0.5f, -0.5f, 0.0f ), .TexCoord = CRVector2D( 0.0f, 1.0f ), .Normal = CRVector( 0.0f, 0.0f, 1.0f ) },
    { .Position = CRVector(  0.5f, -0.5f, 0.0f ), .TexCoord = CRVector2D( 1.0f, 1.0f ), .Normal = CRVector( 0.0f, 0.0f, 1.0f ) },
};