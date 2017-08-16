Texture2D shaderTexture;
SamplerState SampleType;
cbuffer PixelBuffer
{
    float4 pixelColor;
};
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};
float4 FontPixelShader(PixelInputType input) : SV_TARGET
{
    float4 color;
	
	
    // Sample the texture pixel at this location.
    color = shaderTexture.Sample(SampleType, input.tex);
    color=color*pixelColor;

    return color;
}