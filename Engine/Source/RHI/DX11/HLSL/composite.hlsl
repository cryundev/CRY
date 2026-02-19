Texture2D    psTexture  : register( t0 );
SamplerState SampleType : register( s0 );

struct PixelIn
{
    float4 clipPos  : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

PixelIn VS( uint vertexId : SV_VertexID )
{
    PixelIn output;

    const float2 clipPos[ 3 ] =
    {
        float2( -1.0f, -1.0f ),
        float2( -1.0f,  3.0f ),
        float2(  3.0f, -1.0f ),
    };

    const float2 texCoord[ 3 ] =
    {
        float2( 0.0f,  1.0f ),
        float2( 0.0f, -1.0f ),
        float2( 2.0f,  1.0f ),
    };

    output.clipPos  = float4( clipPos[ vertexId ], 0.0f, 1.0f );
    output.texCoord = texCoord[ vertexId ];

    return output;
}

float4 PS( PixelIn input ) : SV_TARGET
{
    return psTexture.SampleLevel( SampleType, input.texCoord, 0.0f );
}
