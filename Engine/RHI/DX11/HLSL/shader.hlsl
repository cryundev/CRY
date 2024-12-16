struct PixelIn
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

PixelIn VS( float4 position : POSITION, float2 texCoord : TEXCOORD )
{
	PixelIn output;
	output.position = position;
	output.texCoord = texCoord;

	return output;
}

Texture2D    psTexture  : register( t0 );
SamplerState SampleType : register( s0 );

float4 PS( PixelIn input ) : SV_TARGET
{
	float4 textureColor = float4( 1.f, 1.f, 1.f, 1.f );
	textureColor = psTexture.Sample( SampleType, input.texCoord );
	textureColor = pow( textureColor, 1.f/2.2f );

	return textureColor;
}