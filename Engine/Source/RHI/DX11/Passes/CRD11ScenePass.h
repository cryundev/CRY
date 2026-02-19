#pragma once


#include "CRD11RenderPass.h"
#include "../CRD11DepthStencilBuffer.h"
#include "../CRD11RenderTarget2D.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11ScenePass
//---------------------------------------------------------------------------------------------------------------------
class CRD11ScenePass : public CRD11RenderPass
{
private:
    CRD11RenderTarget2D SceneRenderTarget;
    CRD11DepthStencilBuffer SceneDepthStencilBuffer;

public:
    //-----------------------------------------------------------------------------------------------------------------
    /// ICRRHIRenderPass
    //-----------------------------------------------------------------------------------------------------------------
    /// Get pass name.
    virtual CRName GetPassName() const override { return "ScenePass"; }

    /// Initialize pass resources.
    virtual void Initialize( u32 Width, u32 Height ) override;

    /// Resize pass resources.
    virtual void Resize( u32 Width, u32 Height ) override;

    /// Clear scene render target and depth.
    virtual void OnClearRenderTarget( const float ClearColor[ 4 ] ) override;

    /// Bind scene render target before draw.
    virtual void OnPreDraw() override;

    /// Draw render elements.
    virtual void OnDrawRenderElements( const CRPackedArray< CRRenderElement >& RenderElements ) override;

    /// Release pass resources.
    virtual void Release() override;

private:
    /// Set scene render/depth state in rendering pipeline.
    void _SetInRenderingPipeline() const;

    /// Clear scene render targets.
    void _ClearRenderTargets( const float Color[ 4 ] ) const;
};
