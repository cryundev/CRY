#pragma once


class ICRRHIRenderer
{
public:
    /// Constructor
    ICRRHIRenderer() = default;

    /// Destructor
    virtual ~ICRRHIRenderer() = 0 {}

public:
    /// Initialize renderer.
    virtual void Initialize( unsigned int Width, unsigned int Height ) = 0;

    /// Add render mesh.
    virtual void AddRenderMesh( const ICRRHIMeshWPtr& Mesh ) = 0;

    /// Draw.
    virtual void Draw() = 0;

    /// Clear render target.
    virtual void ClearRenderTarget() const = 0;

    /// Present.
    virtual void Present() const = 0;

    /// Get viewport width.
    virtual unsigned int GetViewportWidth() const = 0;

    /// Get viewport height.
    virtual unsigned int GetViewportHeight() const = 0;

public:
    /// Update transform buffer.
    virtual void UpdateTransformBuffer( const CRMatrix& Matrix ) {}

    /// Update view projection buffer.
    virtual void UpdateViewProjectionBuffer( const CRMatrix& ViewMatrix, const CRMatrix& ProjectionMatrix ) {}
};