#include "CRD11ScenePass.h"
#include "../CRD11.h"
#include "../CRD11RenderingPipeline.h"
#include "../Resource/CRD11Device.h"
#include "Source/RHI/ICRRHIMaterial.h"
#include "Source/RHI/ICRRHIMesh.h"
#include "Source/Object/Component/CRTransformComponent.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIRenderer.h"
#include "Source/Utility/Generic/CRGeneric.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize pass resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ScenePass::Initialize( u32 Width, u32 Height )
{
    u32 sceneWidth  = Width;
    u32 sceneHeight = Height;
    
    DXGI_FORMAT sceneFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

    if ( IDXGISwapChain* swapChain = GD11.GetSwapChain() )
    {
        ID3D11Texture2D* backBufferTexture = nullptr;
        HRESULT hr = swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&backBufferTexture );
        if ( !CRGeneric::CheckError( hr ) && backBufferTexture )
        {
            D3D11_TEXTURE2D_DESC backBufferDesc;
            ZeroMemory( &backBufferDesc, sizeof( D3D11_TEXTURE2D_DESC ) );
            backBufferTexture->GetDesc( &backBufferDesc );

            sceneWidth  = backBufferDesc.Width;
            sceneHeight = backBufferDesc.Height;
            sceneFormat = backBufferDesc.Format;

            backBufferTexture->Release();
        }
    }

    SceneRenderTarget.Create( "SceneColor", sceneWidth, sceneHeight, sceneFormat );
    SceneDepthStencilBuffer.Create( sceneWidth, sceneHeight );
}

//---------------------------------------------------------------------------------------------------------------------
/// Resize pass resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ScenePass::Resize( u32 Width, u32 Height )
{
    Initialize( Width, Height );
}

//---------------------------------------------------------------------------------------------------------------------
/// Clear scene render target and depth.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ScenePass::OnClearRenderTarget( const float ClearColor[ 4 ] )
{
    _SetInRenderingPipeline();
    _ClearRenderTargets( ClearColor );
}

//---------------------------------------------------------------------------------------------------------------------
/// Bind scene render target before geometry draw.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ScenePass::OnPreDraw()
{
    _SetInRenderingPipeline();
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw render elements.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ScenePass::OnDrawRenderElements( const CRPackedArray< CRRenderElement >& RenderElements )
{
    ICRRHIRenderer* renderer = GRHI.GetRenderer();
    if ( !renderer ) return;

    RenderElements.ForEachActive( [ renderer ] ( const CRRenderElementHandle&, const CRRenderElement& Element )
    {
        ICRRHIMeshSPtr     mesh     = Element.Mesh    .lock();
        ICRRHIMaterialSPtr material = Element.Material.lock();
        
        if ( !mesh || !material ) return;

        CRMatrix worldMatrix = CRMatrix::Identity;

        if ( CRTransformComponent* transform = CRTransformComponent::Get( Element.OwnerId ) )
        {
            transform->UpdateComponent( 0.0f );
            worldMatrix = transform->GetLocalMatrix();
        }

        renderer->UpdateTransformBuffer( worldMatrix );

        mesh    ->SetInRenderingPipeline();
        material->SetInRenderingPipeline();
        
        mesh->Draw();
    } );
}

//---------------------------------------------------------------------------------------------------------------------
/// Release pass resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ScenePass::Release()
{
    SceneRenderTarget.Release();
    SceneDepthStencilBuffer.Release();
}

//---------------------------------------------------------------------------------------------------------------------
/// Bind scene render target and depth buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ScenePass::_SetInRenderingPipeline() const
{
    if ( !SceneRenderTarget.IsValid() ) return;

    ID3D11DepthStencilView* depthStencilView = SceneDepthStencilBuffer.GetView();

    GD11RP.SetRenderTargetView( SceneRenderTarget.GetRTV(), depthStencilView );

    if ( ID3D11DepthStencilState* depthStencilState = SceneDepthStencilBuffer.GetState() )
    {
        GD11RP.SetDepthStencilState( depthStencilState );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Clear scene color and depth.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ScenePass::_ClearRenderTargets( const float Color[ 4 ] ) const
{
    if ( !SceneRenderTarget.IsValid() ) return;

    GD11.GetDeviceContext()->ClearRenderTargetView( SceneRenderTarget.GetRTV(), Color );

    if ( ID3D11DepthStencilView* depthStencilView = SceneDepthStencilBuffer.GetView() )
    {
        GD11.GetDeviceContext()->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
    }
}
