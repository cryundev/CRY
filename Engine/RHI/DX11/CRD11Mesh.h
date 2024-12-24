#pragma once


#include "CRD11ShaderResourceTexture.h"
#include "CRD11Types.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Mesh
//---------------------------------------------------------------------------------------------------------------------
class CRD11Mesh
{
private:
    CRD11VertexBufferWPtr VertexBuffer;
    CRD11IndexBufferWPtr  IndexBuffer;
    CRD11InputLayoutWPtr  InputLayout;

    CRD11ShaderResourceTexture Texture;
    CRD11SamplerStateWPtr SamplerState;

    CRD11VertexShaderWPtr VertexShader;
    CRD11PixelShaderWPtr  PixelShader;
    
public:
    /// Constructor
    CRD11Mesh() = default;

    /// Destructor
    virtual ~CRD11Mesh() = default;

    /// Initialize.
    void Initialize();

    /// Set in the rendering pipeline.
    void SetInRenderingPipeline() const;

    /// Draw.
    void Draw() const;
};
