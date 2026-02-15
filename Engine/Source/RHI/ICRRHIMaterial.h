#pragma once


#include "Source/Core/Strings/CRStringInc.h"


class ICRRHIMaterial
{
public:
    /// Constructor
    ICRRHIMaterial() = default;

    /// Destructor
    virtual ~ICRRHIMaterial() = 0 {}

public:
    /// Initialize material.
    virtual void Initialize( const CRName& Name ) = 0;

    /// Set in the rendering pipeline.
    virtual void SetInRenderingPipeline() const = 0;
};
