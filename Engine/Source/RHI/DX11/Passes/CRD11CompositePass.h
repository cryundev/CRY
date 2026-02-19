#pragma once


#include "CRD11RenderPass.h"
#include "../CRD11Types.h"

//---------------------------------------------------------------------------------------------------------------------
/// CRD11CompositePass
//---------------------------------------------------------------------------------------------------------------------
class CRD11CompositePass : public CRD11RenderPass
{
private:
    CRD11RenderTargetViewSPtr BackBufferRenderTargetView;
    CRD11VertexShaderWPtr     CompositeVertexShader;
    CRD11PixelShaderWPtr      CompositePixelShader;
    CRD11SamplerStateWPtr     CompositeSamplerState;

public:
    //-----------------------------------------------------------------------------------------------------------------
    /// ICRRHIRenderPass
    //-----------------------------------------------------------------------------------------------------------------
    /// Get pass name.
    virtual CRName GetPassName() const override { return "CompositePass"; }

    /// Initialize pass resources.
    virtual void Initialize( u32 Width, u32 Height ) override;

    /// Resize pass resources.
    virtual void Resize( u32 Width, u32 Height ) override;

    /// Draw scene color to back buffer after draw.
    virtual void OnPostDraw() override;

    /// Release pass resources.
    virtual void Release() override;

private:
    /// Create back buffer render target view.
    bool _CreateBackBufferRenderTargetView();

    /// Create composite shaders.
    bool _CreateCompositeShaders();

    /// Create sampler state.
    bool _CreateSamplerState();
};
