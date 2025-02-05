#pragma once


#include "Source/Core/Strings/CRStringInc.h"


class CRPrimitiveAsset;


class ICRRHIMesh
{
public:
    /// Constructor
    ICRRHIMesh() = default;

    /// Destructor
    virtual ~ICRRHIMesh() = 0 {}

public:
    /// Initialize primitive.
    virtual void InitializePrimitive( const CRName& Name, const CRPrimitiveAsset& PrimitiveData ) = 0;

    /// Initialize material.
    virtual void InitializeMaterial() = 0;

    /// Set in the rendering pipeline.
    virtual void SetInRenderingPipeline() const = 0;

    /// Draw.
    virtual void Draw() const = 0;
};
