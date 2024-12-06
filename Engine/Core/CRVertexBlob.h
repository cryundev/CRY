#pragma once


#include "../CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRVertex
//---------------------------------------------------------------------------------------------------------------------
struct CRVertex
{
    CRVector   Position;
    CRVector   Color;
    CRVector2D TexCoord;
};


//---------------------------------------------------------------------------------------------------------------------
/// CRVertexBlob
//---------------------------------------------------------------------------------------------------------------------
class CRVertexBlob
{
private:
    CRArray< CRVertex > Vertices;

public:
    /// Return vertices's first element pointer.
    const CRVertex* GetBlobPtr() const { return Vertices.data(); }
};
