#pragma once


#include "CRD11ShaderResourceTexture.h"
#include "CRD11Types.h"
#include "Core/Containers/CRContainerInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Material
//---------------------------------------------------------------------------------------------------------------------
class CRD11Material
{
private:
    CRD11VertexShaderWPtr                 VertexShader;
    CRD11PixelShaderWPtr                  PixelShader;
    CRD11InputLayoutWPtr                  InputLayout;
    CRArray< CRD11ShaderResourceTexture > Textures;
    
public:
    /// Constructor.
    CRD11Material() = default;

    /// Initialize.
    void Initialize();

    /// Set in the rendering pipeline.
    void SetInRenderingPipeline() const;
};
