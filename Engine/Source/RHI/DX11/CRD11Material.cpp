#include "CRD11Material.h"
#include "CRD11.h"
#include "CRD11RenderingPipeline.h"
#include "CRD11ResourceManager.h"
#include "Resource/CRD11InputLayout.h"
#include "Resource/CRD11PixelShader.h"
#include "Resource/CRD11SamplerState.h"
#include "Resource/CRD11ShaderResourceView.h"
#include "Resource/CRD11VertexShader.h"
#include "Source/Utility/Log/CRLog.h"
#include "Source/Utility/UtilPath.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize material.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Material::Initialize( const CRName& Name )
{
    VertexShader = GD11RM.GetVertexShader( "Diffuse" );
    PixelShader  = GD11RM.GetPixelShader ( "Diffuse" );
    InputLayout  = GD11RM.GetInputLayout ( "Diffuse" );

    CRName bufferName = Name + "_MaterialProperties";
    PropertiesBuffer.Create( bufferName, (u32)( EConstBufferSlotPS::MaterialProperties ), ED11RenderingPipelineStage::PS );

    Textures.clear();

    const CRPath texturePath = UtilPath::ResolveExistingEnginePath( "Asset/cryun_icon.png" );
    if ( texturePath.empty() )
    {
        GLog.AddLog( "[CRD11Material::Initialize] Failed to find cryun_icon.png." );
    }

    CRD11ShaderResourceTexture texture;
    texture.Create( texturePath.lexically_normal() );

    Textures.push_back( texture );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set in the rendering pipeline.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Material::SetInRenderingPipeline() const
{
    if ( !InputLayout.expired() )
    {
        GD11RP.SetInputLayout( InputLayout.lock()->GetObjectPtr() );
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

    PropertiesBuffer.SetInRenderingPipeline();
}

//---------------------------------------------------------------------------------------------------------------------
/// Set diffuse color (RGB) and alpha (A).
//---------------------------------------------------------------------------------------------------------------------
void CRD11Material::SetDiffuseColor( const CRVector4D& Color )
{
    PropertiesBuffer.Edit().DiffuseColor = Color;
}

//---------------------------------------------------------------------------------------------------------------------
/// Set specular color (RGB) and shininess (A).
//---------------------------------------------------------------------------------------------------------------------
void CRD11Material::SetSpecularColor( const CRVector4D& Color )
{
    PropertiesBuffer.Edit().SpecularColor = Color;
}
