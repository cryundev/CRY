#pragma once


#include "CRD11Types.h"
#include "Source/Core/Containers/CRContainerInc.h"
#include "Source/RHI/ICRRHIMaterial.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11MaterialTextureBinding
//---------------------------------------------------------------------------------------------------------------------
struct CRD11MaterialTextureBinding
{
    int ShaderResourceSlot = -1;
    int SamplerSlot        = -1;
};


//---------------------------------------------------------------------------------------------------------------------
/// CRD11MaterialShader
//---------------------------------------------------------------------------------------------------------------------
class CRD11MaterialShader
{
private:
    CRD11VertexShaderWPtr VertexShader;
    CRD11PixelShaderWPtr  PixelShader;
    CRD11InputLayoutWPtr  InputLayout;

    CRArray< CRD11MaterialTextureBinding > TextureBindings;

public:
    /// Constructor
    CRD11MaterialShader()
    {
        ClearTextureBindings();
    }

    /// Set shader resources.
    void SetShaders ( const CRD11VertexShaderSPtr& InVertexShader, const CRD11PixelShaderSPtr& InPixelShader, const CRD11InputLayoutSPtr& InInputLayout )
    {
        VertexShader = InVertexShader;
        PixelShader  = InPixelShader;
        InputLayout  = InInputLayout;
    }

    /// Clear texture bindings.
    void ClearTextureBindings()
    {
        TextureBindings.clear();
        TextureBindings.resize( static_cast< size_t >( ECRMaterialTextureSemantic::Max ) );
    }

    /// Set texture binding for a semantic.
    void SetTextureBinding( ECRMaterialTextureSemantic Semantic, int ShaderResourceSlot, int SamplerSlot )
    {
        const size_t semanticIndex = static_cast< size_t >( Semantic );
        if ( semanticIndex >= TextureBindings.size() ) return;

        TextureBindings[ semanticIndex ].ShaderResourceSlot = ShaderResourceSlot;
        TextureBindings[ semanticIndex ].SamplerSlot        = SamplerSlot;
    }

    /// Get texture bindings.
    const CRArray< CRD11MaterialTextureBinding >& GetTextureBindings() const
    {
        return TextureBindings;
    }

    /// Get vertex shader.
    const CRD11VertexShaderWPtr& GetVertexShader() const
    {
        return VertexShader;
    }

    /// Get pixel shader.
    const CRD11PixelShaderWPtr& GetPixelShader() const
    {
        return PixelShader;
    }

    /// Get input layout.
    const CRD11InputLayoutWPtr& GetInputLayout() const
    {
        return InputLayout;
    }

    /// Check whether all shader resources were registered.
    bool HasValidResources() const
    {
        return !VertexShader.expired() && !PixelShader.expired() && !InputLayout.expired();
    }
};
