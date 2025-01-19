#include "CRD11Mesh.h"
#include "CRD11.h"
#include "CRD11RenderingPipeline.h"
#include "CRD11ResourceManager.h"
#include "Core/CRD11CompiledShader.h"
#include "Core/CRD11Device.h"
#include "Core/CRD11IndexBuffer.h"
#include "Core/CRD11InputLayout.h"
#include "Core/CRD11PixelShader.h"
#include "Core/CRD11SamplerState.h"
#include "Core/CRD11ShaderResourceView.h"
#include "Core/CRD11VertexBuffer.h"
#include "Core/CRD11VertexShader.h"
#include "Core/CRVertex.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize primitive.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Mesh::InitializePrimitive( const CRName& Name, const CRPrimitiveData& PrimitiveData )
{
    CRArray< CRVertex > vertice;
    CRVertex::LoadFromPrimitiveData( PrimitiveData, vertice );
    
    VertexBuffer = GD11RM.GetVertexBuffer( Name );
    VertexBuffer.lock()->Create( D3D11_USAGE_DEFAULT, 0, vertice.data(), sizeof( CRVertex ), vertice.size() );
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize material.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Mesh::InitializeMaterial()
{
    CRD11CompiledShader compiledVS;
    compiledVS.Create( L"RHI/DX11/HLSL/shader.hlsl", "VS", "vs_5_0" );

    VertexShader = GD11RM.GetVertexShader( "Diffuse" );
    VertexShader.lock()->Create( compiledVS.GetObjectPtr() );

    D3D11_INPUT_ELEMENT_DESC elements[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    InputLayout = GD11RM.GetInputLayout( "Diffuse" );
    InputLayout.lock()->Create( elements, ARRAYSIZE( elements ), compiledVS.GetObjectPtr() );
    
    CRD11CompiledShader compiledPS;
    compiledPS.Create( L"RHI/DX11/HLSL/shader.hlsl", "PS", "ps_5_0" );

    PixelShader = GD11RM.GetPixelShader( "Diffuse" );
    PixelShader.lock()->Create( compiledPS.GetObjectPtr() );

    Texture.Create( "../Asset/cryun_icon.png" );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set in the rendering pipeline.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Mesh::SetInRenderingPipeline() const
{
    if ( !VertexBuffer.expired() )
    {
        GD11RP.SetVertexBuffer( VertexBuffer.lock()->GetObjectPtr(), 0, VertexBuffer.lock()->GetStride(), 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    }

    if ( !IndexBuffer.expired() )
    {
        GD11RP.SetIndexBuffer( IndexBuffer.lock()->GetObjectPtr(), 0 );
    }

    if ( !InputLayout.expired() )
    {
        GD11RP.SetInputLayout( InputLayout.lock()->GetObjectPtr(), 0 );
    }

    if ( !Texture.GetShaderResourceView().expired() )
    {
        GD11RP.SetShaderResourceView( Texture.GetShaderResourceView().lock()->GetObjectPtr(), 0, ED11RenderingPipelineStage::PS );
    }

    if ( !Texture.GetSamplerState().expired() )
    {
        GD11RP.SetSamplerState( Texture.GetSamplerState().lock()->GetObjectPtr(), 0 );
    }

    if ( !VertexShader.expired() )
    {
        GD11RP.SetVertexShader( VertexShader.lock()->GetObjectPtr() );
    }

    if ( !PixelShader.expired() )
    {
        GD11RP.SetPixelShader( PixelShader.lock()->GetObjectPtr() );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Mesh::Draw() const
{
    if ( VertexBuffer.expired() ) return;

    if ( IndexBuffer.expired() )
    {
        GD11.GetDeviceContext()->Draw( VertexBuffer.lock()->GetCount(), 0 );
    }
    else if ( !VertexBuffer.expired() )
    {
        GD11.GetDeviceContext()->DrawIndexed( IndexBuffer.lock()->GetCount(), 0, 0 );
    }
}
