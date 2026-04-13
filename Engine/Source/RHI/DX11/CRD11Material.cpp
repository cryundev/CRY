#include "CRD11Material.h"
#include "CRD11.h"
#include "CRD11MaterialShader.h"
#include "CRD11RenderingPipeline.h"
#include "CRD11ResourceManager.h"
#include "Resource/CRD11InputLayout.h"
#include "Resource/CRD11PixelShader.h"
#include "Resource/CRD11SamplerState.h"
#include "Resource/CRD11ShaderResourceView.h"
#include "Resource/CRD11VertexShader.h"
#include "Source/Utility/Log/CRLog.h"
#include "Source/Utility/UtilPath.h"
#include <filesystem>


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// Get the default material shader name.
//---------------------------------------------------------------------------------------------------------------------
const CRName& _GetDefaultMaterialShaderName()
{
    static const CRName defaultShaderName = "Diffuse";
    return defaultShaderName;
}

//---------------------------------------------------------------------------------------------------------------------
/// Convert a material texture semantic to an array index.
//---------------------------------------------------------------------------------------------------------------------
size_t _GetTextureSemanticIndex( ECRMaterialTextureSemantic Semantic )
{
    return static_cast< size_t >( Semantic );
}

//---------------------------------------------------------------------------------------------------------------------
/// Check whether a texture binding is used by the current shader.
//---------------------------------------------------------------------------------------------------------------------
bool _HasAnyShaderBinding( const CRD11MaterialTextureBinding& TextureBinding )
{
    return TextureBinding.ShaderResourceSlot >= 0
        || TextureBinding.SamplerSlot        >= 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// Resolve a texture path, preferring engine-relative assets.
//---------------------------------------------------------------------------------------------------------------------
CRPath _ResolveTexturePath( const CRPath& TexturePath )
{
    if ( TexturePath.empty() ) return {};
    if ( TexturePath.is_absolute() ) return TexturePath;

    const CRPath resolvedEnginePath = UtilPath::ResolveExistingEnginePath( TexturePath );
    if ( !resolvedEnginePath.empty() )
    {
        return resolvedEnginePath;
    }

    return TexturePath;
}
}


//---------------------------------------------------------------------------------------------------------------------
/// Initialize material.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Material::Initialize( const CRName& Name )
{
    CRName bufferName = Name + "_MaterialProperties";
    PropertiesBuffer.Create( bufferName, (u32)( EConstBufferSlotPS::MaterialProperties ), ED11RenderingPipelineStage::PS );

    Textures.clear();
    Textures.resize( _GetTextureSemanticIndex( ECRMaterialTextureSemantic::Max ) );

    SetShader( _GetDefaultMaterialShaderName() );

    _SetDefaultTexture( ECRMaterialTextureSemantic::Diffuse );
    _SetDefaultTexture( ECRMaterialTextureSemantic::Normal );
    _SetDefaultTexture( ECRMaterialTextureSemantic::Specular );
    _SetDefaultTexture( ECRMaterialTextureSemantic::Emissive );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set the shader used by this material.
//---------------------------------------------------------------------------------------------------------------------
bool CRD11Material::SetShader( const CRName& InShaderName )
{
    CRD11MaterialShaderSPtr materialShader = GD11RM.GetMaterialShader( InShaderName );
    if ( !materialShader || !materialShader->HasValidResources() )
    {
        GLog.AddLog( "[CRD11Material::SetShader] Failed to find registered material shader: " + InShaderName );
        return false;
    }

    MaterialShader = materialShader;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Set texture for a specific material semantic.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Material::SetTexture( ECRMaterialTextureSemantic Semantic, const CRPath& TexturePath )
{
    const size_t semanticIndex = _GetTextureSemanticIndex( Semantic );
    if ( semanticIndex >= Textures.size() ) return;

    if ( TexturePath.empty() )
    {
        _SetDefaultTexture( Semantic );
        return;
    }

    const CRPath resolvedTexturePath = _ResolveTexturePath( TexturePath );
    if ( resolvedTexturePath.empty() || !std::filesystem::exists( resolvedTexturePath ) )
    {
        GLog.AddLog( "[CRD11Material::SetTexture] Failed to find texture: " + resolvedTexturePath.string() );
        _SetDefaultTexture( Semantic );
        return;
    }

    Textures[ semanticIndex ].Create( resolvedTexturePath.lexically_normal() );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set in the rendering pipeline.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Material::SetInRenderingPipeline() const
{
    CRD11MaterialShaderSPtr materialShader = MaterialShader.lock();
    if ( !materialShader ) return;

    const CRD11InputLayoutWPtr& inputLayout = materialShader->GetInputLayout();
    if ( !inputLayout.expired() )
    {
        GD11RP.SetInputLayout( inputLayout.lock()->GetObjectPtr() );
    }

    const CRArray< CRD11MaterialTextureBinding >& textureBindings = materialShader->GetTextureBindings();
    const size_t bindingCount = textureBindings.size();

    for ( size_t semanticIndex = 0; semanticIndex < Textures.size() && semanticIndex < bindingCount; ++semanticIndex )
    {
        const CRD11MaterialTextureBinding& textureBinding = textureBindings[ semanticIndex ];
        if ( !_HasAnyShaderBinding( textureBinding ) )
        {
            continue;
        }

        ID3D11ShaderResourceView* shaderResourceView = nullptr;
        if ( !Textures[ semanticIndex ].GetShaderResourceView().expired() )
        {
            shaderResourceView = Textures[ semanticIndex ].GetShaderResourceView().lock()->GetObjectPtr();
        }

        ID3D11SamplerState* samplerState = nullptr;
        if ( !Textures[ semanticIndex ].GetSamplerState().expired() )
        {
            samplerState = Textures[ semanticIndex ].GetSamplerState().lock()->GetObjectPtr();
        }

        if ( textureBinding.ShaderResourceSlot >= 0 )
        {
            GD11RP.SetShaderResourceView( shaderResourceView, (u32)textureBinding.ShaderResourceSlot, ED11RenderingPipelineStage::PS );
        }

        if ( textureBinding.SamplerSlot >= 0 )
        {
            GD11RP.SetSamplerState( samplerState, (u32)textureBinding.SamplerSlot );
        }
    }

    const CRD11VertexShaderWPtr& vertexShader = materialShader->GetVertexShader();
    if ( !vertexShader.expired() )
    {
        GD11RP.SetVertexShader( vertexShader.lock()->GetObjectPtr() );
    }

    const CRD11PixelShaderWPtr& pixelShader = materialShader->GetPixelShader();
    if ( !pixelShader.expired() )
    {
        GD11RP.SetPixelShader( pixelShader.lock()->GetObjectPtr() );
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

//---------------------------------------------------------------------------------------------------------------------
/// Set a semantic back to its default texture.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Material::_SetDefaultTexture( ECRMaterialTextureSemantic Semantic )
{
    const size_t semanticIndex = _GetTextureSemanticIndex( Semantic );
    if ( semanticIndex >= Textures.size() ) return;

    CRD11ShaderResourceTexture& texture = Textures[ semanticIndex ];

    switch ( Semantic )
    {
    case ECRMaterialTextureSemantic::Diffuse:  texture.CreateSolidColor( "__CRY_DefaultDiffuse",  255, 255, 255, 255 ); return;
    case ECRMaterialTextureSemantic::Normal:   texture.CreateSolidColor( "__CRY_DefaultNormal",   128, 128, 255, 255 ); return;
    case ECRMaterialTextureSemantic::Specular: texture.CreateSolidColor( "__CRY_DefaultSpecular", 255, 255, 255, 255 ); return;
    case ECRMaterialTextureSemantic::Emissive: texture.CreateSolidColor( "__CRY_DefaultEmissive",   0,   0,   0, 255 ); return;
    default:                                   texture = CRD11ShaderResourceTexture();                                  return;
    }
}
