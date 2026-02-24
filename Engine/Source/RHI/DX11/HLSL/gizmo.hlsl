// =============================================================================
// Vertex Shader Input / Output
// =============================================================================

struct PixelIn
{
    float4 clipPos : SV_POSITION;
};


// =============================================================================
// Vertex Shader Constant Buffers
// =============================================================================

cbuffer TransformBuffer : register( b0 )
{
    matrix transform;
};

cbuffer ViewProjectionBuffer : register( b1 )
{
    matrix view;
    matrix projection;
};

cbuffer GizmoBuffer : register( b2 )
{
    matrix gizmoTransform;
    float4 gizmoColor;
};


// =============================================================================
// Vertex Shader
// =============================================================================

PixelIn VS( float4 position : POSITION, float2 texCoord : TEXCOORD, float3 normal : NORMAL )
{
    PixelIn output;

    float4 worldPos = mul( position, gizmoTransform );

    output.clipPos = mul( worldPos, view );
    output.clipPos = mul( output.clipPos, projection );

    return output;
}


// =============================================================================
// Pixel Shader
// =============================================================================

float4 PS( PixelIn input ) : SV_TARGET
{
    return gizmoColor;
}

