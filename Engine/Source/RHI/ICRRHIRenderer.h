#pragma once


#include "CRConstBufferStructures.h"
#include "CRRenderElement.h"
#include "Source/Core/Containers/CRContainerInc.h"
#include "Source/Core/Math/CRMath.h"


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

    /// Add render element.
    virtual CRRenderElementHandle AddRenderElement( const CRRenderElement& RenderElement ) = 0;

    /// Remove render element.
    virtual void RemoveRenderElement( const CRRenderElementHandle& Handle ) = 0;

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

    /// Set ambient light color and intensity (xyz = color, w = intensity).
    virtual void SetAmbientLight( const CRVector4D& ColorAndIntensity ) {}

    /// Replace active directional lights.
    virtual void SetDirectionalLights( const CRArray< CRDirectionalLightData >& Lights ) {}

    /// Set one directional light at index.
    virtual void SetDirectionalLight( u32 Index, const CRDirectionalLightData& Light ) {}

    /// Replace active point lights.
    virtual void SetPointLights( const CRArray< CRPointLightData >& Lights ) {}

    /// Set one point light at index.
    virtual void SetPointLight( u32 Index, const CRPointLightData& Light ) {}

    /// Replace active spot lights.
    virtual void SetSpotLights( const CRArray< CRSpotLightData >& Lights ) {}

    /// Set one spot light at index.
    virtual void SetSpotLight( u32 Index, const CRSpotLightData& Light ) {}

    /// Clear all active lights (keeps ambient).
    virtual void ClearLights() {}
};
