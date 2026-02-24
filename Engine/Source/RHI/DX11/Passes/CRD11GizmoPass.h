#pragma once


#include "CRD11RenderPass.h"
#include "../CRD11BindingConstantBuffer.h"
#include "../CRD11Types.h"
#include "Source/Core/Math/CRMath.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRGizmoVSData
//---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) CRGizmoVSData
{
    CRMatrix   GizmoTransform = CRMatrix::Identity;
    CRVector4D GizmoColor     = CRVector4D( 1.0f, 1.0f, 0.0f, 1.0f );
};

static_assert( sizeof( CRGizmoVSData ) % 16 == 0, "CRGizmoVSData size must be 16-byte aligned." );


//---------------------------------------------------------------------------------------------------------------------
/// CRD11GizmoPass
//---------------------------------------------------------------------------------------------------------------------
class CRD11GizmoPass : public CRD11RenderPass
{
private:
    CRD11DepthStencilStateWPtr DepthState;
    CRD11RasterizerStateWPtr   RasterState;
    CRD11VertexShaderWPtr      VertexShader;
    CRD11PixelShaderWPtr       PixelShader;
    CRD11InputLayoutWPtr       InputLayout;
    
    CRD11BindingConstantBuffer< CRGizmoVSData > Buffer;
    bool bHasCapturedPipelineState = false;

public:
    //-----------------------------------------------------------------------------------------------------------------
    /// ICRRHIRenderPass
    //-----------------------------------------------------------------------------------------------------------------
    /// Get pass name.
    virtual CRName GetPassName() const override { return "GizmoPass"; }

    /// Initialize pass resources.
    virtual void Initialize( u32 Width, u32 Height ) override;

    /// Resize pass resources.
    virtual void Resize( u32 Width, u32 Height ) override;

    /// Prepare gizmo pass render state.
    virtual void OnPreDraw() override;

    /// Draw gizmo elements.
    virtual void OnDrawRenderElements( const CRPackedArray< CRRenderElement >& RenderElements ) override;

    /// Restore pass-local state.
    virtual void OnPostDraw() override;

    /// Release pass resources.
    virtual void Release() override;

private:
    /// Create depth/rasterizer states.
    bool _CreateStates();

    /// Create gizmo shader and input layout resources.
    bool _CreateShadersAndLayout();

    /// Bind scene render/depth targets for overlay drawing.
    bool _BindSceneTargets() const;

    /// Draw gizmo elements from gizmo system (Phase2-1 stub).
    void _DrawGizmoElementsFromSystem();

};
