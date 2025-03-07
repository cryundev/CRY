#pragma once


#include "ICRAsset.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Containers/CRContainerInc.h"
#include "Source/Core/Math/CRMath.h"
#include "Source/Core/Strings/CRStringInc.h"


class CRPrimitiveAsset : public ICRAsset
{
public:
    CRArray< CRVector   > Positions;
    CRArray< CRVector   > Normals;
    CRArray< CRVector   > Tangents;
    CRArray< CRVector   > Binormals;
    CRArray< CRVector   > Colors;
    CRArray< CRVector2D > UVs;    
    CRArray< u32        > Indices;

    int VertexCount = 0;

public:
    /// Constructor.
    CRPrimitiveAsset() = default;

    /// Destructor.
    virtual ~CRPrimitiveAsset() override {}

    /// Save to file.
    virtual void Save( const CRString& Path ) override;

    /// Load from file.
    virtual void Load( const CRString& Path ) override;
    
    /// Initialize.
    void Initialize( int InVertexCount );
};
