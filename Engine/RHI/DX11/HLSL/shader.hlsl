struct PixelIn
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};


matrix transform : register( c0 );


PixelIn VS( float4 position : POSITION, float2 texCoord : TEXCOORD )
{
	PixelIn output;
    position.w = 1.0f;
	output.position = mul( position, transform );
	output.texCoord = texCoord;

	return output;
}


Texture2D    psTexture  : register( t0 );
SamplerState SampleType : register( s0 );


float4 PS( PixelIn input ) : SV_TARGET
{
	float4 textureColor = float4( 1.f, 1.f, 1.f, 1.f );
	textureColor = psTexture.Sample( SampleType, input.texCoord );

	return textureColor;
}