#pragma once


#include "CRD11BindingConstantBuffer.h"
#include "CRD11DepthStencilBuffer.h"
#include "CRD11Include.h"
#include "CRD11Types.h"
#include "Source/RHI/CRRHITypes.h"
#include "Source/RHI/CRConstBufferStructures.h"
#include "Source/RHI/ICRRHIRenderer.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Renderer
//---------------------------------------------------------------------------------------------------------------------
class CRD11Renderer : public ICRRHIRenderer
{
private:
    unsigned int ViewportWidth  = 1920;
    unsigned int ViewportHeight = 1080;

    CRPackedArray< CRRenderElement > RenderElements;

    CRD11BindingConstantBuffer< CRMatrix           > TransformBuffer;
    CRD11BindingConstantBuffer< CRViewProjection   > ViewProjectionBuffer;
    CRD11BindingConstantBuffer< CRLightsBuffer     > LightsBuffer;
    CRD11BindingConstantBuffer< CRCameraProperties > CameraPropertiesBuffer;

    CRLightsBuffer LightsData;

    CRD11RenderTargetViewSPtr RenderTargetView;
    CRD11RasterizerStateWPtr  RasterizerState;

    CRD11DepthStencilBuffer DepthStencilBuffer;

public:
    /// Constructor
    CRD11Renderer() = default;

    /// Destructor
    virtual ~CRD11Renderer() override = default;

    /// Initialize renderer.
    virtual void Initialize( u32 Width, u32 Height ) override;

    /// Add render element.
    virtual CRRenderElementHandle AddRenderElement( const CRRenderElement& RenderElement ) override;

    /// Remove render element.
    virtual void RemoveRenderElement( const CRRenderElementHandle& Handle ) override;

    /// Draw.
    virtual void Draw() override;

    /// Clear render target.
    virtual void ClearRenderTarget() const override;

    /// Present.
    virtual void Present() const override;

    /// Get viewport width.
    virtual unsigned int GetViewportWidth() const override { return ViewportWidth; }

    /// Get viewport height.
    virtual unsigned int GetViewportHeight() const override { return ViewportHeight; }

public:
    /// Update transform buffer.
    virtual void UpdateTransformBuffer( const CRMatrix& Matrix ) override;

    /// Update view projection buffer.
    virtual void UpdateViewProjectionBuffer( const CRMatrix& ViewMatrix, const CRMatrix& ProjectionMatrix ) override;

    /// Set ambient light.
    virtual void SetAmbientLight( const CRVector4D& ColorAndIntensity ) override;

    /// Replace active directional lights.
    virtual void SetDirectionalLights( const CRArray< CRDirectionalLightData >& Lights ) override;

    /// Set one directional light at index.
    virtual void SetDirectionalLight( u32 Index, const CRDirectionalLightData& Light ) override;

    /// Replace active point lights.
    virtual void SetPointLights( const CRArray< CRPointLightData >& Lights ) override;

    /// Set one point light at index.
    virtual void SetPointLight( u32 Index, const CRPointLightData& Light ) override;

    /// Replace active spot lights.
    virtual void SetSpotLights( const CRArray< CRSpotLightData >& Lights ) override;

    /// Set one spot light at index.
    virtual void SetSpotLight( u32 Index, const CRSpotLightData& Light ) override;

    /// Clear all active lights.
    virtual void ClearLights() override;

private:
    /// Initialize render target.
    void _InitializeRenderTarget();

    /// Initialize viewport.
    void _InitializeViewport( f32 Width, f32 Height ) const;

    /// Flush CPU-side LightsData to the GPU constant buffer.
    void _FlushLightsBuffer();
};
