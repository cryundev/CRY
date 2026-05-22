#pragma once


#include "CRD11Include.h"
#include "CRD11Types.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11RenderState
//---------------------------------------------------------------------------------------------------------------------
class CRD11RenderState
{
private:
    CRD11DepthStencilStateWPtr DepthWrite;     // Depth-Stencil 쓰기 여부
    CRD11DepthStencilStateWPtr DepthOff;       // Depth-Stencil 비활성화
    CRD11RasterizerStateWPtr   SolidCullBack;  // 후면 컬링
    CRD11RasterizerStateWPtr   SolidCullNone;  // 컬링 없음
    CRD11BlendStateWPtr        OpaqueBlend;    // 불투명 블렌딩

public:
    /// Initialize render state resources.
    void Initialize();

    /// Release render state resources.
    void Release();

    /// Apply scene opaque render state.
    void ApplySceneOpaque();

    /// Apply post process render state.
    void ApplyPostProcess();

private:
    /// Create depth write state.
    void _CreateDepthWriteState();

    /// Create depth off state.
    void _CreateDepthOffState();

    /// Create solid cull back state.
    void _CreateSolidCullBackState();

    /// Create solid cull none state.
    void _CreateSolidCullNoneState();

    /// Create opaque blend state.
    void _CreateOpaqueBlendState();

    /// Bind state objects.
    void _BindStates( const CRD11DepthStencilStateWPtr& DepthState, const CRD11RasterizerStateWPtr& RasterizerState, const CRD11BlendStateWPtr& BlendState );
};
