
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture			g_Texture;
float			g_time;

float2			g_fOffset;
float			g_fRadian;

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

/* ���ؽ� ���̴�. */
/* ������ �⺻���� ��ȯ�۾��� �����Ѵ�. */
/* ���������� ������ �����Ѵ�. */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;

	/* ������ ��ġ�� ��������� ���Ѵ�. */
	/* ������ ��ġ�� ������� ���Ѵ�. */
	/* ������ ��ġ�� ��������� ���Ѵ�. */

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);	
	Out.vTexUV = In.vTexUV;	
	/*Out.vTexUV = In.vTexUV + float2(g_time * .25f, 0.f);*/
	return Out;
}

/* w.������ ������. */
/* ����Ʈ ��ȯ. */
/* �����Ͷ����� (�ȼ��� �����Ѵ�.) */

struct PS_IN
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;	
};

/* �ȼ� ���̴�. */
/* �ȼ��� ���� ���������� �����Ѵ�. */
struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out;

	Out.vColor = tex2D(DefaultSampler, In.vTexUV);

	return Out;
}


PS_OUT PS_PORTAL(PS_IN In)
{
	PS_OUT		Out;

	Out.vColor = tex2D(DefaultSampler, In.vTexUV);
	Out.vColor.rgb += 0.4f;
	//Out.vColor.a = 1.f;
	//Out.vColor.gb = Out.vColor.r;
	//Out.vColor.a = 0.5f;

	/*vector(1.f, 1.f, In.vTexUV.y, 1.f)*/;

	return Out;
}

PS_OUT PS_WAVE(PS_IN In)
{
	PS_OUT		Out;
	float2 Trans = In.vTexUV + g_time * 0.3f;
	float4 Noise = tex2D(DefaultSampler1, Trans);

	float2 UV = In.vTexUV + Noise.xy * 0.05f;
	float4 OutColor = tex2D(DefaultSampler, UV);
	Out.vColor = OutColor;
	return Out;
}

PS_OUT PS_FADE(PS_IN In)
{
	PS_OUT Out;

	float fDistance = length(g_fOffset - In.vTexUV.xy);
	if (fDistance > g_fRadian)
	{
		Out.vColor = float4(0.f, 0.f, 0.f, 1.f);
	}
	else
	{
		clip(-1);
		Out.vColor = float4(0.f, 0.f, 0.f, 1.f);
	}
	return Out;
}

PS_OUT PS_DEFAULT(PS_IN In)
{
	PS_OUT Out;

	clip(-1);
	Out.vColor = float4(0.f, 0.f, 0.f, 1.f);

	return Out;
}

technique DefaultTecnique
{
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Portal
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_PORTAL();
	}

	pass Wave
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_WAVE();
	}

	pass DefaultFade
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_DEFAULT();
	}

	pass Fade
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_FADE();
	}
}
