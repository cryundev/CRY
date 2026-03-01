// =============================================================================
// Vertex Shader Input / Output
// =============================================================================
struct PixelIn
{
    float4 clipPos : SV_POSITION;
    float4 color   : COLOR0;
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
    float4 gizmoPivot;
    float  gizmoPixelSize;
    float  viewportHeight;
    float  projectionCotHalfFovY; // projection._22 == cot( fovY * 0.5f )
    float  axisType;
    float  _pad0;
};


// =============================================================================
// Vertex Shader
// =============================================================================
PixelIn VS( float4 position : POSITION, float2 texCoord : TEXCOORD, float3 normal : NORMAL )
{
    PixelIn output;

    float4 pivotView = mul( float4( gizmoPivot.xyz, 1.0f ), view );

    float safeDistance = max( abs( pivotView.z ), 0.01f );
    float safeProjectionCotHalfFovY = max( abs( projectionCotHalfFovY ), 1.0e-6f );
    float safeViewportHeight = max( viewportHeight, 1.0f );

    float scale = ( 2.0f * safeDistance / safeProjectionCotHalfFovY ) * ( gizmoPixelSize / safeViewportHeight );
    scale = clamp( scale, 0.0001f, 32.0f );

    float4 localPos = float4( position.xyz * scale, 1.0f );
    float4 worldPos = mul( localPos, gizmoTransform );

    output.clipPos = mul( worldPos, view );
    output.clipPos = mul( output.clipPos, projection );
    output.color = gizmoColor;

    return output;
}


// =============================================================================
// Pixel Shader
// =============================================================================
float4 PS( PixelIn input ) : SV_TARGET
{
    return input.color;
}
