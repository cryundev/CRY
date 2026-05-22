#include "CRD11RenderState.h"
#include "CRD11.h"
#include "CRD11RenderingPipeline.h"
#include "CRD11ResourceManager.h"
#include "Resource/CRD11BlendState.h"
#include "Resource/CRD11DepthStencilState.h"
#include "Resource/CRD11RasterizerState.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize render state resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderState::Initialize()
{
    Release();

    _CreateDepthWriteState();
    _CreateDepthOffState();
    _CreateSolidCullBackState();
    _CreateSolidCullNoneState();
    _CreateOpaqueBlendState();

    ApplySceneOpaque();
}

//---------------------------------------------------------------------------------------------------------------------
/// Release render state resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderState::Release()
{
    if ( !DepthWrite   .expired() ) DepthWrite   .lock()->SetObjectPtr( nullptr );
    if ( !DepthOff     .expired() ) DepthOff     .lock()->SetObjectPtr( nullptr );
    if ( !SolidCullBack.expired() ) SolidCullBack.lock()->SetObjectPtr( nullptr );
    if ( !SolidCullNone.expired() ) SolidCullNone.lock()->SetObjectPtr( nullptr );
    if ( !OpaqueBlend  .expired() ) OpaqueBlend  .lock()->SetObjectPtr( nullptr );
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply scene opaque render state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderState::ApplySceneOpaque()
{
    _BindStates( DepthWrite, SolidCullBack, OpaqueBlend );
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply post process render state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderState::ApplyPostProcess()
{
    _BindStates( DepthOff, SolidCullNone, OpaqueBlend );
}

//---------------------------------------------------------------------------------------------------------------------
/// Create depth write state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderState::_CreateDepthWriteState()
{
    DepthWrite = GD11RM.GetDepthStencilState( "RenderState_DepthWrite" );
    if ( DepthWrite.expired() ) return;

    D3D11_DEPTH_STENCIL_DESC dsd;
    ZeroMemory( &dsd, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

    dsd.DepthEnable    = true;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsd.DepthFunc      = D3D11_COMPARISON_LESS;

    DepthWrite.lock()->Create( dsd );
}

//---------------------------------------------------------------------------------------------------------------------
/// Create depth off state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderState::_CreateDepthOffState()
{
    DepthOff = GD11RM.GetDepthStencilState( "RenderState_DepthOff" );
    if ( DepthOff.expired() ) return;

    D3D11_DEPTH_STENCIL_DESC dsd;
    ZeroMemory( &dsd, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

    dsd.DepthEnable    = false;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsd.DepthFunc      = D3D11_COMPARISON_ALWAYS;

    DepthOff.lock()->Create( dsd );
}

//---------------------------------------------------------------------------------------------------------------------
/// Create solid cull back state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderState::_CreateSolidCullBackState()
{
    SolidCullBack = GD11RM.GetRasterizerState( "RenderState_SolidCullBack" );
    if ( SolidCullBack.expired() ) return;

    D3D11_RASTERIZER_DESC rd;
    ZeroMemory( &rd, sizeof( D3D11_RASTERIZER_DESC ) );

    rd.CullMode              = D3D11_CULL_BACK;
    rd.FillMode              = D3D11_FILL_SOLID;
    rd.FrontCounterClockwise = false;

    SolidCullBack.lock()->Create( rd );
}

//---------------------------------------------------------------------------------------------------------------------
/// Create solid cull none state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderState::_CreateSolidCullNoneState()
{
    SolidCullNone = GD11RM.GetRasterizerState( "RenderState_SolidCullNone" );
    if ( SolidCullNone.expired() ) return;

    D3D11_RASTERIZER_DESC rd;
    ZeroMemory( &rd, sizeof( D3D11_RASTERIZER_DESC ) );

    rd.CullMode              = D3D11_CULL_NONE;
    rd.FillMode              = D3D11_FILL_SOLID;
    rd.FrontCounterClockwise = false;

    SolidCullNone.lock()->Create( rd );
}

//---------------------------------------------------------------------------------------------------------------------
/// Create opaque blend state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderState::_CreateOpaqueBlendState()
{
    OpaqueBlend = GD11RM.GetBlendState( "RenderState_OpaqueBlend" );
    if ( OpaqueBlend.expired() ) return;

    D3D11_BLEND_DESC bd;
    ZeroMemory( &bd, sizeof( D3D11_BLEND_DESC ) );

    bd.RenderTarget[ 0 ].BlendEnable           = false;
    bd.RenderTarget[ 0 ].SrcBlend              = D3D11_BLEND_ONE;
    bd.RenderTarget[ 0 ].DestBlend             = D3D11_BLEND_ZERO;
    bd.RenderTarget[ 0 ].BlendOp               = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[ 0 ].SrcBlendAlpha         = D3D11_BLEND_ONE;
    bd.RenderTarget[ 0 ].DestBlendAlpha        = D3D11_BLEND_ZERO;
    bd.RenderTarget[ 0 ].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[ 0 ].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    OpaqueBlend.lock()->Create( bd );
}

//---------------------------------------------------------------------------------------------------------------------
/// Bind state objects.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderState::_BindStates( const CRD11DepthStencilStateWPtr& DepthState, const CRD11RasterizerStateWPtr& RasterizerState, const CRD11BlendStateWPtr& BlendState )
{
    if ( !DepthState     .expired() ) GD11RP.SetDepthStencilState( DepthState     .lock()->GetObjectPtr() );
    if ( !RasterizerState.expired() ) GD11RP.SetRasterizerState  ( RasterizerState.lock()->GetObjectPtr() );
    if ( !BlendState     .expired() ) GD11RP.SetBlendState       ( BlendState     .lock()->GetObjectPtr() );
}
