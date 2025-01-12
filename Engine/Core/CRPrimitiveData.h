#pragma once


#include "ICRAsset.h"
#include "Containers/CRContainerInc.h"
#include "Math/CRMath.h"


class CRPrimitiveData : public ICRAsset
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
    /// Constructor.
    CRPrimitiveData() = default;

    /// Destructor.
    virtual ~CRPrimitiveData() override {}

    /// Save to file.
    virtual void Save( const CRString& Path ) override;

    /// Load from file.
    virtual void Load( const CRString& Path ) override;
    
    /// Initialize.
    void Initialize( int InVertexCount );

     
};
