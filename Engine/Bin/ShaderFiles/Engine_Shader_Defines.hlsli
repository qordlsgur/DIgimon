
vector g_vMtrlDiffuse = 1.f;
vector g_vMtrlAmbient = 1.f;
vector g_vMtrlSpecular = 1.f;

sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = wrap;
    AddressV = wrap;
};

RasterizerState RS_Default
{
    FillMode = Solid;
    CullMode = Back;
};

RasterizerState RS_Wireframe
{
    FillMode = Wireframe;
};

RasterizerState RS_Cull_Front
{
    FillMode = Solid;
    CullMode = Front;
};

RasterizerState RS_Cull_Default
{
    FillMode = Solid;
    CullMode = None;
};

DepthStencilState DSS_Default
{
    DepthEnable = true;
    DepthWriteMask = all;
    DepthFunc = less_equal;
};

DepthStencilState DSS_None
{
    DepthEnable = false;
    DepthWriteMask = zero;
};

BlendState BS_None
{
    BlendEnable[0] = false;
 
};

BlendState BS_AlphaBlend
{
    BlendEnable[0] = true;
    BlendEnable[1] = true;

    SrcBlend = Src_Alpha;
    DestBlend = Inv_Src_Alpha;
    BlendOp = Add;
};

BlendState BS_Blend
{
    BlendEnable[0] = true;
    BlendEnable[1] = true;

    SrcBlend = one;
    DestBlend = one;
    BlendOp = Add;
};

