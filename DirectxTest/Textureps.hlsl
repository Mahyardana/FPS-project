Texture2D shaderTexture;
SamplerState SampleType;
cbuffer PixelBuffer
{
    float blurradius;
    float alpha;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};
float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
    float radius = 0;
    float4 textureColor = 0;
    float count = 0;
    for (float i = -radius; i <= radius; i++)
    {
        for (float j = -radius; j <= radius; j++)
        {
            if ((input.tex.x + i * 0.0005) >= 0 && (input.tex.y + j * 0.0005) >= 0)
            {
                textureColor += shaderTexture.Sample(SampleType, float2(input.tex.x + i * 0.0005, input.tex.y + j * 0.0005));
                count++;
            }
        }
    }
    textureColor /= count;
    textureColor.a *= alpha;
    return textureColor;
}

