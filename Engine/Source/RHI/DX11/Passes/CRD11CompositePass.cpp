#include "CRD11CompositePass.h"
#include "../CRD11.h"
#include "../CRD11RenderingPipeline.h"
#include "../CRD11ResourceManager.h"
#include "../Resource/CRD11CompiledShader.h"
#include "../Resource/CRD11Device.h"
#include "../Resource/CRD11PixelShader.h"
#include "../Resource/CRD11RenderTargetView.h"
#include "../Resource/CRD11SamplerState.h"
#include "../Resource/CRD11ShaderResourceView.h"
#include "../Resource/CRD11VertexShader.h"
#include "Source/Utility/Generic/CRGeneric.h"
#include "Source/Utility/Log/CRLog.h"
#include "Source/Utility/UtilPath.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize pass resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11CompositePass::Initialize( u32 Width, u32 Height )
{
    Resize( Width, Height );
}

//---------------------------------------------------------------------------------------------------------------------
/// Resize pass resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11CompositePass::Resize( u32 /*Width*/, u32 /*Height*/ )
{
    Release();

    _CreateBackBufferRenderTargetView();
    _CreateCompositeShaders();
    _CreateSamplerState();
}

//---------------------------------------------------------------------------------------------------------------------
/// Release pass resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11CompositePass::Release()
{
    if ( BackBufferRenderTargetView ) BackBufferRenderTargetView->SetObjectPtr( nullptr );
    
    if ( !CompositeVertexShader.expired() ) CompositeVertexShader.lock()->SetObjectPtr( nullptr );
    if ( !CompositePixelShader .expired() ) CompositePixelShader .lock()->SetObjectPtr( nullptr );
    if ( !CompositeSamplerState.expired() ) CompositeSamplerState.lock()->SetObjectPtr( nullptr );
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw scene SRV to back buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11CompositePass::OnPostDraw()
{
    if ( !BackBufferRenderTargetView ) return;
    if ( CompositeVertexShader.expired() ) return;
    if ( CompositePixelShader .expired() ) return;
    if ( CompositeSamplerState.expired() ) return;

    CRD11ShaderResourceViewSPtr sceneSRV = GD11RM.GetShaderResourceView( "SceneColor_SRV" );
    if ( !sceneSRV || !sceneSRV->GetObjectPtr() ) return;

    ID3D11VertexShader* compositeVS = CompositeVertexShader.lock()->GetObjectPtr();
    ID3D11PixelShader*  compositePS = CompositePixelShader .lock()->GetObjectPtr();
    ID3D11SamplerState* compositeSS = CompositeSamplerState.lock()->GetObjectPtr();
    if ( !compositeVS || !compositePS || !compositeSS ) return;

    GD11RP.SetRenderTargetView( BackBufferRenderTargetView->GetObjectPtr(), nullptr );
    GD11RP.SetInputLayout( nullptr );
    
    GD11RP.SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    GD11RP.SetVertexShader( compositeVS );
    GD11RP.SetPixelShader ( compositePS );
    
    GD11RP.SetShaderResourceView( sceneSRV->GetObjectPtr(), 0, ED11RenderingPipelineStage::PS );    
    GD11RP.SetSamplerState( compositeSS, 0 );

    GD11.GetDeviceContext()->Draw( 3, 0 );

    GD11RP.SetShaderResourceView( nullptr, 0, ED11RenderingPipelineStage::PS );
}

//---------------------------------------------------------------------------------------------------------------------
/// Create back buffer render target view.
//---------------------------------------------------------------------------------------------------------------------
bool CRD11CompositePass::_CreateBackBufferRenderTargetView()
{
    IDXGISwapChain* swapChain = GD11.GetSwapChain();
    if ( !swapChain ) return false;

    ID3D11Texture2D* backBufferTexture = nullptr;
    
    HRESULT hr = swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&backBufferTexture );

    if ( CRGeneric::CheckError( hr ) || !backBufferTexture )
    {
        GLog.AddLog( "[CRD11CompositePass] Failed to get back buffer texture." );
        
        return false;
    }

    BackBufferRenderTargetView = GD11RM.GetRenderTargetView( "BackBuffer" );
    
    if ( BackBufferRenderTargetView )
    {
        BackBufferRenderTargetView->Create( backBufferTexture, nullptr );
    }
    else
    {
        GLog.AddLog( "[CRD11CompositePass] Failed to get back buffer RTV resource." );
    }
    
    backBufferTexture->Release();

    return BackBufferRenderTargetView != nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/// Create composite shaders.
//---------------------------------------------------------------------------------------------------------------------
bool CRD11CompositePass::_CreateCompositeShaders()
{
    const CRPath shaderPath = UtilPath::ResolveExistingEnginePath( "Engine/Source/RHI/DX11/HLSL/composite.hlsl" );
    if ( shaderPath.empty() )
    {
        GLog.AddLog( "[CRD11CompositePass] Failed to find composite shader file." );
        
        return false;
    }

    CRD11CompiledShader compiledVS;
    CRD11CompiledShader compiledPS;
    
    compiledVS.Create( shaderPath.wstring(), "VS", "vs_5_0" );
    compiledPS.Create( shaderPath.wstring(), "PS", "ps_5_0" );
    
    if ( !compiledVS.GetObjectPtr() || !compiledPS.GetObjectPtr() )
    {
        GLog.AddLog( "[CRD11CompositePass] Failed to compile composite shader." );
        
        return false;
    }

    CompositeVertexShader = GD11RM.GetVertexShader( "Composite" );
    CompositePixelShader  = GD11RM.GetPixelShader ( "Composite" );
    
    if ( CompositeVertexShader.expired() || CompositePixelShader.expired() )
    {
        GLog.AddLog( "[CRD11CompositePass] Failed to get composite shader resources." );
        
        return false;
    }

    CompositeVertexShader.lock()->Create( compiledVS.GetObjectPtr() );
    CompositePixelShader .lock()->Create( compiledPS.GetObjectPtr() );

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Create sampler state.
//---------------------------------------------------------------------------------------------------------------------
bool CRD11CompositePass::_CreateSamplerState()
{
    CompositeSamplerState = GD11RM.GetSamplerState( "CompositeLinearClamp" );
    if ( CompositeSamplerState.expired() )
    {
        GLog.AddLog( "[CRD11CompositePass] Failed to get composite sampler state resource." );
        
        return false;
    }

    D3D11_SAMPLER_DESC sd;
    ZeroMemory( &sd, sizeof( D3D11_SAMPLER_DESC ) );

    sd.Filter         = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sd.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sd.MinLOD         = 0.0f;
    sd.MaxLOD         = D3D11_FLOAT32_MAX;

    CompositeSamplerState.lock()->Create( sd );

    return true;
}
