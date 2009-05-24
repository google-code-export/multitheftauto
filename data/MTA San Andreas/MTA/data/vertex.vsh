struct VS_INPUT
{
    float4 Position   : POSITION;
    float2 Texture    : TEXCOORD0;
    float3 Normal     : NORMAL;
};


struct VS_OUTPUT
{
    float4 Position   : POSITION;
    float2 Texture    : TEXCOORD0;
};

struct VS_OUTPUT_LIGHTING
{
    float4 Position   : POSITION;
    float2 Texture    : TEXCOORD0;
    float3 Normal     : TEXCOORD1;
    float3 Position3D : TEXCOORD2;
};

float4x4 WorldViewProjLast;
float4x4 WorldViewProj;
float4x4 World;


VS_OUTPUT vs_mask( in VS_INPUT In )
{
    VS_OUTPUT Out;
    
    Out.Position = mul(In.Position, WorldViewProj);
    Out.Texture  = In.Texture;
    
    return Out;
}

VS_OUTPUT_LIGHTING vs_lighting( in VS_INPUT In )
{
    VS_OUTPUT_LIGHTING Out;
    
    Out.Position = mul(In.Position, WorldViewProj);
    Out.Texture  = In.Texture;
    
    Out.Normal = normalize(mul(In.Normal, (float3x3)World));
    Out.Position3D = mul(In.Position, World);
    
    return Out;
}
