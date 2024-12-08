struct PixelIn
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
};

PixelIn VS( float4 position : SV_POSITION, float4 color : COLOR )
{
	PixelIn output;
	output.position = position;
	output.color    = color;

	return output;
}