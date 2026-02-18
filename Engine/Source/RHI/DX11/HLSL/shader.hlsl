// =============================================================================
// Vertex Shader Input / Output
// =============================================================================

struct PixelIn
{
    float4 clipPos     : SV_POSITION;
    float2 texCoord    : TEXCOORD0;
    float3 worldPos    : TEXCOORD1;
    float3 worldNormal : NORMAL;
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


// =============================================================================
// Vertex Shader
// =============================================================================

PixelIn VS( float4 position : POSITION, float2 texCoord : TEXCOORD, float3 normal : NORMAL )
{
    PixelIn output;

    float4 worldPos = mul( position, transform );
    output.worldPos = worldPos.xyz;

    output.clipPos  = mul( worldPos, view );
    output.clipPos  = mul( output.clipPos, projection );
    output.texCoord = texCoord;

    output.worldNormal = normalize( mul( normal, (float3x3)transform ) );

    return output;
}


// =============================================================================
// Pixel Shader Resources
// =============================================================================

Texture2D    psTexture  : register( t0 );
SamplerState SampleType : register( s0 );


// =============================================================================
// Pixel Shader Constant Buffers
// =============================================================================

#define MAX_DIRECTIONAL_LIGHTS 8
#define MAX_POINT_LIGHTS       8
#define MAX_SPOT_LIGHTS        8

struct DirectionalLightData
{
    float4 direction;  // xyz = light travel direction
    float4 color;      // xyz = RGB, w = intensity
};

struct PointLightData
{
    float4 position;   // xyz = world position
    float4 color;      // xyz = RGB, w = intensity
    float  range;
    float3 _pad;
};

struct SpotLightData
{
    float4 position;   // xyz = world position
    float4 direction;  // xyz = light travel direction
    float4 color;      // xyz = RGB, w = intensity
    float  range;
    float  spotInner;
    float  spotOuter;
    float  _pad;
};

cbuffer LightsBuffer : register( b0 )
{
    float4               ambientColor;
    uint                 directionalCount;
    uint                 pointCount;
    uint                 spotCount;
    uint                 _lightPad;
    DirectionalLightData directionalLights[ MAX_DIRECTIONAL_LIGHTS ];
    PointLightData       pointLights[ MAX_POINT_LIGHTS ];
    SpotLightData        spotLights[ MAX_SPOT_LIGHTS ];
};

cbuffer MaterialPropertiesBuffer : register( b1 )
{
    float4 materialDiffuse;
    float4 materialSpecular;
};

cbuffer CameraPropertiesBuffer : register( b2 )
{
    float4 cameraPosition;
};


// =============================================================================
// Lighting helpers
// =============================================================================

float3 EvaluateLitSurface( float3 lightRgb,
                           float  lightIntensity,
                           float3 N,
                           float3 L,
                           float3 V,
                           float3 matDiffuse,
                           float4 matSpecular,
                           float  attenuation )
{
    if ( attenuation <= 0.0f ) return float3( 0.0f, 0.0f, 0.0f );

    float  NdotL   = max( dot( N, L ), 0.0f );
    float3 diffuse = lightRgb * lightIntensity * matDiffuse * NdotL;

    float3 H         = normalize( L + V );
    float  NdotH     = max( dot( N, H ), 0.0f );
    float  shininess = max( matSpecular.w, 1.0f );
    float3 specular  = lightRgb * lightIntensity
                     * matSpecular.rgb
                     * pow( NdotH, shininess )
                     * ( NdotL > 0.0f ? 1.0f : 0.0f );

    return ( diffuse + specular ) * attenuation;
}

float3 EvaluateDirectionalLight( DirectionalLightData light,
                                 float3               worldPos,
                                 float3               N,
                                 float3               V,
                                 float3               matDiffuse,
                                 float4               matSpecular )
{
    float3 L = normalize( -light.direction.xyz );
    return EvaluateLitSurface( light.color.rgb, light.color.w, N, L, V, matDiffuse, matSpecular, 1.0f );
}

float3 EvaluatePointLight( PointLightData light,
                           float3         worldPos,
                           float3         N,
                           float3         V,
                           float3         matDiffuse,
                           float4         matSpecular )
{
    float3 toLight = light.position.xyz - worldPos;
    float  dist    = length( toLight );
    float3 L       = toLight / ( dist + 1e-5f );

    float  r = saturate( dist / max( light.range, 1e-5f ) );
    float  attenuation = saturate( ( 1.0f - r * r ) * ( 1.0f - r * r ) );

    return EvaluateLitSurface( light.color.rgb, light.color.w, N, L, V, matDiffuse, matSpecular, attenuation );
}

float3 EvaluateSpotLight( SpotLightData light,
                          float3        worldPos,
                          float3        N,
                          float3        V,
                          float3        matDiffuse,
                          float4        matSpecular )
{
    float3 toLight = light.position.xyz - worldPos;
    float  dist    = length( toLight );
    float3 L       = toLight / ( dist + 1e-5f );

    float  r = saturate( dist / max( light.range, 1e-5f ) );
    float  attenuation = saturate( ( 1.0f - r * r ) * ( 1.0f - r * r ) );

    float  cosAngle   = dot( -L, normalize( light.direction.xyz ) );
    float  spotFactor = smoothstep( light.spotOuter, light.spotInner, cosAngle );
    attenuation *= spotFactor;

    return EvaluateLitSurface( light.color.rgb, light.color.w, N, L, V, matDiffuse, matSpecular, attenuation );
}


// =============================================================================
// Pixel Shader
// =============================================================================

float4 PS( PixelIn input ) : SV_TARGET
{
    float4 textureColor = psTexture.Sample( SampleType, input.texCoord );

    float3 N = normalize( input.worldNormal );
    float3 V = normalize( cameraPosition.xyz - input.worldPos );

    float3 litDiffuse = materialDiffuse.rgb * textureColor.rgb;
    float3 result = ambientColor.rgb * ambientColor.w * materialDiffuse.rgb;

    for ( uint i = 0u; i < directionalCount; ++i )
    {
        result += EvaluateDirectionalLight( directionalLights[ i ], input.worldPos, N, V, litDiffuse, materialSpecular );
    }

    for ( uint i = 0u; i < pointCount; ++i )
    {
        result += EvaluatePointLight( pointLights[ i ], input.worldPos, N, V, litDiffuse, materialSpecular );
    }

    for ( uint i = 0u; i < spotCount; ++i )
    {
        result += EvaluateSpotLight( spotLights[ i ], input.worldPos, N, V, litDiffuse, materialSpecular );
    }

    return float4( saturate( result ), textureColor.a * materialDiffuse.w );
}
