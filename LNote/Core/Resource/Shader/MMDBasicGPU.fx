//
// Skinned Mesh Effect file 
// Copyright (c) 2000-2002 Microsoft Corporation. All rights reserved.
//

float4 lhtDir = {0.0f, 0.0f, -1.0f, 1.0f};    //light Direction 
float4 lightDiffuse = {0.6f, 0.6f, 0.6f, 1.0f}; // Light Diffuse
float4 MaterialAmbient : MATERIALAMBIENT = {0.1f, 0.1f, 0.1f, 1.0f};
float4 MaterialDiffuse : MATERIALDIFFUSE = {0.8f, 0.8f, 0.8f, 1.0f};

// Matrix Pallette
static const int MAX_MATRICES = 26;
float4x3    mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4    mViewProj : VIEWPROJECTION;

//-------------------------------------------------------------------------
// テスクチャ
//-------------------------------------------------------------------------
texture gTexture    : MATERIALTEXTURE;
sampler gTexSampler = sampler_state
{
    texture = < gTexture >;
    MINFILTER = NONE;
    MAGFILTER = NONE;
};

texture gToonTexture    : TOONTEXTURE;
sampler gToonSampler = sampler_state
{
    texture = < gToonTexture >;
    MINFILTER = NONE;
    MAGFILTER = NONE;
};


///////////////////////////////////////////////////////
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
    float3  Normal          : NORMAL;
    float3  Tex0            : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4  Pos     : POSITION;
    float4  Diffuse : COLOR;
    float2  Tex0    : TEXCOORD0;
};


float3 Diffuse(float3 Normal)
{
    float CosTheta;
    
    // N.L Clamped
    CosTheta = max(0.0f, dot(Normal, lhtDir.xyz));
       
    // propogate scalar result to vector
    return (CosTheta);
}


VS_OUTPUT VShade(VS_INPUT i, uniform int NumBones)
{
    VS_OUTPUT   o;
    float3      Pos = 0.0f;
    float3      Normal = 0.0f;    
    float       LastWeight = 0.0f;
     
    // Compensate for lack of UBYTE4 on Geforce3
    int4 IndexVector = D3DCOLORtoUBYTE4(i.BlendIndices);

    // cast the vectors to arrays for use in the for loop below
    float BlendWeightsArray[4] = (float[4])i.BlendWeights;
    int   IndexArray[4]        = (int[4])IndexVector;
    
    // calculate the pos/normal using the "normal" weights 
    //        and accumulate the weights to calculate the last weight
    for (int iBone = 0; iBone < NumBones-1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(i.Pos, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        Normal += mul(i.Normal, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 

    // Now that we have the calculated weight, add in the final influence
    Pos += (mul(i.Pos, mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight);
    Normal += (mul(i.Normal, mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight); 
    
    // transform position from world space into view and then projection space
    o.Pos = mul(float4(Pos.xyz, 1.0f), mViewProj);

    
    // 強制設定
	/*
	i.Pos.z += 500;
    */
	//Normal = i.Normal;
	//o.Pos = mul(float4(i.Pos.xyz, 1.0f), mViewProj);
    //o.Pos = mul(i.Pos, mViewProj);
	//Pos = mul(i.Pos, mWorldMatrixArray[0]);
    //o.Pos = mul(float4(Pos.xyz, 1.0f), mViewProj);

    // normalize normals
    Normal = normalize(Normal);

    // Shade (Ambient + etc.)
    //o.Diffuse.xyz = MaterialAmbient.xyz + Diffuse(Normal) * MaterialDiffuse.xyz;
    //o.Diffuse.w = 1.0f;
    
    if ( IndexArray[0] >= 26 || IndexArray[1] >= 26 )
    {
        o.Diffuse = float4( 1, 0, 0, 1 );
    }
    else
    {
        o.Diffuse = float4( 0, 0, 0, 1 );
    }

    // copy the input texture coordinate through
    o.Tex0  = i.Tex0.xy;
    
    float4 c = tex2Dlod(gToonSampler,float4(0, 0, 0, 1));

    return o;
}

//-------------------------------------------------------------------------
// PS
//-------------------------------------------------------------------------
float4 psBasic(
    float4 in_color_    : COLOR0,
    float2 in_uv_       : TEXCOORD0 ) : COLOR0
{
    /*
    /if ( abs(in_toon_uv_.y) > 0.1 ) { in_toon_uv_.y = 1.0; }
    
    float4 out_color = tex2D( gTexSampler, in_uv_ );
    
    out_color.rgb *= in_toon_uv_.y;
    */
    
    float4 out_color = tex2D(gTexSampler, in_uv_);//*tex2D(gToonSampler, in_toon_uv_);//*Diffuse;
    
    return out_color;//in_color_;//float4(1, 0, 0, 1);//// * 
}

int CurNumBones = 1;
VertexShader vsArray[4] = { compile vs_2_0 VShade(1), 
                            compile vs_2_0 VShade(2),
                            compile vs_2_0 VShade(3),
                            compile vs_2_0 VShade(4)
                          };


//////////////////////////////////////
// Techniques specs follow
//////////////////////////////////////
technique t0
{
    pass p0
    {
        VertexShader = compile vs_2_0 VShade(2);//(vsArray[CurNumBones]);
        PixelShader	 = compile ps_2_0 psBasic();
    }
}

