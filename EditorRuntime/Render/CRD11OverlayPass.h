#pragma once

#include "../Actor/CRGizmoActor.h"
#include "Source/RHI/DX11/Passes/CRD11RenderPass.h"
#include "Source/RHI/DX11/CRD11BindingConstantBuffer.h"
#include "Source/RHI/DX11/CRD11Types.h"
#include "Source/Core/Math/CRMath.h"
#include "Source/Core/CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CROverlayVSConstants
//---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) CROverlayVSConstants
{
    CRMatrix   GizmoTransform        = CRMatrix::Identity;
    CRVector4D GizmoColor            = CRVector4D( 1.0f, 1.0f, 1.0f, 1.0f );
    CRVector4D GizmoPivot            = CRVector4D( 0.0f, 0.0f, 0.0f, 1.0f );
    f32        GizmoPixelSize        = 96.0f;
    f32        ViewportHeight        = 1080.0f;
    f32        ProjectionCotHalfFovY = 1.0f;
    f32        AxisType              = 0.0f;
    f32        _Pad0                 = 0.0f;
};

static_assert( sizeof( CROverlayVSConstants ) % 16 == 0, "CROverlayVSConstants size must be 16-byte aligned." );


//---------------------------------------------------------------------------------------------------------------------
/// CROverlayCameraParams
//---------------------------------------------------------------------------------------------------------------------
struct CROverlayCameraParams
{
    f32 ViewportHeight        = 1080.0f;
    f32 ProjectionCotHalfFovY = 1.0f;
};


//---------------------------------------------------------------------------------------------------------------------
/// CROverlayPassConfig
//---------------------------------------------------------------------------------------------------------------------
struct CROverlayPassConfig
{
    CRName     DepthStateName    = {};
    CRName     RasterStateName   = {};
    CRName     ShaderName        = {};
    CRString   ConstantBufferName;
    CRPath     ShaderFileName    = CRPath( "gizmo.hlsl" );
    CRString   LogPrefix;
    CRVector4D DefaultColor      = CRVector4D( 1.0f, 1.0f, 1.0f, 1.0f );
    f32        DefaultPixelSize  = 96.0f;
};


//---------------------------------------------------------------------------------------------------------------------
/// CRD11OverlayPass
//---------------------------------------------------------------------------------------------------------------------
class CRD11OverlayPass : public CRD11RenderPass
{
private:
    CRD11DepthStencilStateWPtr DepthState;
    CRD11RasterizerStateWPtr   RasterState;
    CRD11VertexShaderWPtr      VertexShader;
    CRD11PixelShaderWPtr       PixelShader;
    CRD11InputLayoutWPtr       InputLayout;

    CRD11BindingConstantBuffer< CROverlayVSConstants > Buffer;
    bool bHasCapturedPipelineState = false;
    bool bReady                    = false;

public:
    /// Initialize pass resources.
    virtual void Initialize( u32 Width, u32 Height ) override final;

    /// Resize pass resources.
    virtual void Resize( u32 Width, u32 Height ) override final;

    /// Prepare overlay pass render state.
    virtual void OnPreDraw() override final;

    /// Draw overlay elements.
    virtual void OnDrawRenderElements( const CRPackedArray< CRRenderElement >& RenderElements ) override final;

    /// Restore pass-local state.
    virtual void OnPostDraw() override final;

    /// Release pass resources.
    virtual void Release() override final;

protected:
    /// Get overlay pipeline/resource config.
    virtual CROverlayPassConfig GetOverlayConfig() const = 0;

    /// Check whether draw should run this frame.
    virtual bool CanDrawOverlay() const = 0;

    /// Draw overlay-specific elements.
    virtual void DrawOverlayElements() = 0;

    /// Build common camera parameters for pixel-sized overlay rendering.
    CROverlayCameraParams BuildCameraParams() const;

    /// Draw one overlay mesh with common constant-buffer setup.
    void DrawOverlayMesh( const CRGizmoRenderElement& Element, const CROverlayCameraParams& CameraParams, f32 DefaultPixelSize );

private:
    /// Create depth/rasterizer states.
    bool _CreateStates( const CROverlayPassConfig& Config );

    /// Create shader and input layout resources.
    bool _CreateShadersAndLayout( const CROverlayPassConfig& Config );

    /// Bind back-buffer render target.
    bool _BindSceneTargets() const;
};
