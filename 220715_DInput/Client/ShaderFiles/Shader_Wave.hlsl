#define PI 3.14159
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture			g_Texture;
float			g_time;

sampler DefaultSampler = sampler_state
{
	Texture = g_Texture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_Texture1;

sampler DefaultSampler1 = sampler_state
{
	Texture = g_Texture1;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

/* 버텍스 셰이더. */
/* 정점의 기본적인 변환작업을 수행한다. */
/* 정점정보의 변경을 수행한다. */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;

	/* 정점의 위치에 월드행렬을 곱한다. */
	/* 정점의 위치에 뷰행렬을 곱한다. */
	/* 정점의 위치에 투영행렬을 곱한다. */

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	return Out;
}

/* w.나누기 연산을. */
/* 뷰포트 변환. */
/* 래스터라이즈 (픽셀을 생성한다.) */

struct PS_IN
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

/* 픽셸 셰이더. */
/* 픽셀의 색을 최종적으로 결정한다. */
struct PS_OUT
{
	float4		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out;

	float2 Trans = In.vTexUV + g_time * 0.3f;
	float4 Noise = tex2D(DefaultSampler1, Trans);
	float2 UV = In.vTexUV + Noise.xy * 0.05f;

	float4 OutColor = tex2D(DefaultSampler, UV);
	Out.vColor = OutColor;
	return Out;
}

technique DefaultTecnique
{
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}
