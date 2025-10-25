inline float4 ReverseColor(Texture2D BaseTexture, SamplerState Sampler, float2 TexCoord)
{
    float4 vOut;
    
    vOut = BaseTexture.Sample(Sampler, TexCoord);
    vOut != vOut;
    
    return vOut;
}