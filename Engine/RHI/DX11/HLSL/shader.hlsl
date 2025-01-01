struct PixelIn
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
    float3 normal   : NORMAL;
};


cbuffer TransformBuffer : register( b0 )
{
    matrix transform;
};

cbuffer ViewProjectionBuffer : register( b1 )
{
    matrix view;
    matrix projection;
};

PixelIn VS( float4 position : POSITION, float2 texCoord : TEXCOORD, float3 normal : NORMAL )
{
	PixelIn output;
	output.position = mul( position, transform );
    output.position = mul( output.position, view );
    output.position = mul( output.position, projection );
    
	output.texCoord = texCoord;
    output.normal   = normal; 

	return output;
}


Texture2D    psTexture  : register( t0 );
SamplerState SampleType : register( s0 );


cbuffer LightDirectionBuffer : register( b0 )
{
    float4 lightDirection;
};

cbuffer LightColorBuffer : register( b1 )
{
    float4 lightColor;
};


float4 PS( PixelIn input ) : SV_TARGET
{
	float4 textureColor = float4( 1.f, 1.f, 1.f, 1.f );
	textureColor = psTexture.Sample( SampleType, input.texCoord );

    float3 lightDir = normalize( -lightDirection );
    float lightIntensity = saturate( dot( lightDir, input.normal ) );
    
    lightIntensity = lightIntensity * 0.5 + 0.5;
    lightIntensity = pow( lightIntensity, 2.f );
    
	return textureColor * saturate( lightColor * lightIntensity );
}