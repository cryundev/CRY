#include "CRD11Material.h"
#include "CRD11.h"
#include "CRD11RenderingPipeline.h"
#include "CRD11ResourceManager.h"
#include "Resource/CRD11InputLayout.h"
#include "Resource/CRD11PixelShader.h"
#include "Resource/CRD11SamplerState.h"
#include "Resource/CRD11ShaderResourceView.h"
#include "Resource/CRD11VertexShader.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize material.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Material::Initialize( const CRName& )
{
    VertexShader = GD11RM.GetVertexShader( "Diffuse" );
    PixelShader  = GD11RM.GetPixelShader ( "Diffuse" );
    InputLayout  = GD11RM.GetInputLayout ( "Diffuse" );

    Textures.clear();

    CRD11ShaderResourceTexture texture;
    texture.Create( "../Asset/cryun_icon.png" );

    Textures.push_back( texture );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set in the rendering pipeline.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Material::SetInRenderingPipeline() const
{
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
