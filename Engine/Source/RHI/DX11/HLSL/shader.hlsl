// =============================================================================
// Vertex Shader Input / Output
// =============================================================================

struct PixelIn
{
    float4 clipPos     : SV_POSITION;
    float2 texCoord    : TEXCOORD0;
    float3 worldPos    : TEXCOORD1;
    float3 worldNormal : NORMAL;
    float3 worldTangent   : TEXCOORD2;
    float3 worldBitangent : TEXCOORD3;
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

float3 NormalizeSafe( float3 value, float3 fallback )
{
    float lengthSquared = dot( value, value );
    if ( lengthSquared <= 1e-8f )
    {
        return fallback;
    }

    return value * rsqrt( lengthSquared );
}

PixelIn VS( float4 position : POSITION, float2 texCoord : TEXCOORD, float3 normal : NORMAL, float3 tangent : TANGENT, float3 bitangent : BITANGENT )
{
    PixelIn output;

    float4 worldPos = mul( position, transform );
    output.worldPos = worldPos.xyz;

    output.clipPos  = mul( worldPos, view );
    output.clipPos  = mul( output.clipPos, projection );
    output.texCoord = texCoord;

    output.worldNormal = NormalizeSafe( mul( normal, (float3x3)transform ), float3( 0.0f, 0.0f, 1.0f ) );

    float3 transformedTangent   = mul( tangent,   (float3x3)transform );
    float3 transformedBitangent = mul( bitangent, (float3x3)transform );

    output.worldTangent = NormalizeSafe
    (
        transformedTangent - output.worldNormal * dot( transformedTangent, output.worldNormal ),
        float3( 1.0f, 0.0f, 0.0f )
    );

    float3 orthogonalBitangent = transformedBitangent
                               - output.worldNormal * dot( transformedBitangent, output.worldNormal )
                               - output.worldTangent * dot( transformedBitangent, output.worldTangent );

    output.worldBitangent = NormalizeSafe
    (
        orthogonalBitangent,
        NormalizeSafe( cross( output.worldNormal, output.worldTangent ), float3( 0.0f, 1.0f, 0.0f ) )
    );

    return output;
}


// =============================================================================
// Pixel Shader Resources
// =============================================================================

// Material semantic bindings for the default lit surface shader.
Texture2D    psTexture       : register( t0 );
Texture2D    psNormalTexture : register( t1 );
SamplerState SampleType      : register( s0 );


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
    float3 normalSample = psNormalTexture.Sample( SampleType, input.texCoord ).xyz * 2.0f - 1.0f;

    float3 N = NormalizeSafe( input.worldNormal, float3( 0.0f, 0.0f, 1.0f ) );
    float3 T = NormalizeSafe( input.worldTangent - N * dot( input.worldTangent, N ), float3( 1.0f, 0.0f, 0.0f ) );
    float3 rawB = input.worldBitangent
                - N * dot( input.worldBitangent, N )
                - T * dot( input.worldBitangent, T );
    float3 crossB = cross( N, T );
    float  handedness = ( dot( crossB, rawB ) < 0.0f ) ? -1.0f : 1.0f;
    float3 B = NormalizeSafe( crossB * handedness, NormalizeSafe( rawB, float3( 0.0f, 1.0f, 0.0f ) ) );

    float3x3 tbn = float3x3( T, B, N );
    N = NormalizeSafe( mul( normalSample, tbn ), N );

    float3 V = normalize( cameraPosition.xyz - input.worldPos );

    float3 litDiffuse = materialDiffuse.rgb * textureColor.rgb;
    float3 result = ambientColor.rgb * ambientColor.w * materialDiffuse.rgb;

    for ( uint directionalIndex = 0u; directionalIndex < directionalCount; ++directionalIndex )
    {
        result += EvaluateDirectionalLight( directionalLights[ directionalIndex ], input.worldPos, N, V, litDiffuse, materialSpecular );
    }

    for ( uint pointIndex = 0u; pointIndex < pointCount; ++pointIndex )
    {
        result += EvaluatePointLight( pointLights[ pointIndex ], input.worldPos, N, V, litDiffuse, materialSpecular );
    }

    for ( uint spotIndex = 0u; spotIndex < spotCount; ++spotIndex )
    {
        result += EvaluateSpotLight( spotLights[ spotIndex ], input.worldPos, N, V, litDiffuse, materialSpecular );
    }

    return float4( saturate( result ), textureColor.a * materialDiffuse.w );
}
