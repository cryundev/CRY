#include "CRD11Mesh.h"
#include "CRD11.h"
#include "CRD11RenderingPipeline.h"
#include "CRD11ResourceManager.h"
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
void CRD11Mesh::InitializePrimitive( const CRName& Name, const CRPrimitiveAsset& PrimitiveData )
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
    VertexShader = GD11RM.GetVertexShader( "Diffuse" );
    PixelShader  = GD11RM.GetPixelShader ( "Diffuse" );
    InputLayout  = GD11RM.GetInputLayout ( "Diffuse" );

    CRD11ShaderResourceTexture texture;
    texture.Create( "../Asset/cryun_icon.png" );

    Textures.push_back( texture );;
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

    for ( int i = 0; i < Textures.size(); ++i )
    {
        if ( !Textures[ i ].GetShaderResourceView().expired() )
        {
            GD11RP.SetShaderResourceView( Textures[ i ].GetShaderResourceView().lock()->GetObjectPtr(), i, ED11RenderingPipelineStage::PS );
        }

        if ( !Textures[ i ].GetSamplerState().expired() )
        {
            GD11RP.SetSamplerState( Textures[ i ].GetSamplerState().lock()->GetObjectPtr(), i );
        }
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
