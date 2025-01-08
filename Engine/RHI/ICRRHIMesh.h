#pragma once


class ICRRHIMesh
{
public:
    /// Constructor
    ICRRHIMesh() = default;

    /// Destructor
    virtual ~ICRRHIMesh() = 0 {}

public:
    /// Initialize.
    virtual void Initialize() = 0;

    /// Set in the rendering pipeline.
    virtual void SetInRenderingPipeline() const = 0;

    /// Draw.
    virtual void Draw() const = 0;
};