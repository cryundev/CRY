#pragma once


#include "Containers/CRContainerInc.h"
#include "Math/CRMath.h"


struct CRPrimitiveData
{
public:
    CRArray< CRVector     > Positions;
    CRArray< CRVector     > Normals;
    CRArray< CRVector     > Tangents;
    CRArray< CRVector     > Binormals;
    CRArray< CRVector     > Colors;
    CRArray< CRVector2D   > UVs;    
    CRArray< unsigned int > Indices;

    int VertexCount = 0;

public:
    /// Initialize.
    void Initialize( int InVertexCount )
    {
        VertexCount = InVertexCount;
        
        Positions = CRArray< CRVector >( InVertexCount, CRVector::Zero );
        Normals   = CRArray< CRVector >( InVertexCount, CRVector::Zero );
        Tangents  = CRArray< CRVector >( InVertexCount, CRVector::Zero );
        Binormals = CRArray< CRVector >( InVertexCount, CRVector::Zero );
        Colors    = CRArray< CRVector >( InVertexCount, CRVector::Zero );
        
        UVs = CRArray< CRVector2D >( InVertexCount, CRVector2D::Zero );

    }
};
