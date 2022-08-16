
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
textureCUBE		g_Texture;

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
	float3		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;	
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

	return Out;
}

/* w.������ ������. */
/* ����Ʈ ��ȯ. */
/* �����Ͷ����� (�ȼ��� �����Ѵ�.) */

struct PS_IN
{
	float4		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;	
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

	Out.vColor = texCUBE(DefaultSampler, In.vTexUV);
	//Out.vColor.rg = Out.vColor.b;
	/*Out.vColor.b += 0.5f;*/
	/*Out.vColor.a = 0.5f;*/

	/*vector(1.f, 1.f, In.vTexUV.y, 1.f)*/;

	return Out;
}

PS_OUT PS_InHole(PS_IN In)
{
	PS_OUT		Out;

	Out.vColor = texCUBE(DefaultSampler, In.vTexUV);
	Out.vColor.rgb -= 0.3f;
	Out.vColor.a += 0.1f;
	return Out;
}

PS_OUT PS_WarpWork(PS_IN In)
{
	PS_OUT		Out;

	Out.vColor = texCUBE(DefaultSampler, In.vTexUV);
	Out.vColor.rg += 0.3f;
	Out.vColor.b += 0.5f;
	Out.vColor.a += 0.1f;
	return Out;
}

technique DefaultTecnique
{
	pass Default
	{
		CULLMODE = NONE;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	pass InHole
	{
		CULLMODE = NONE;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_InHole();
	}

	pass WarpWork
	{
		CULLMODE = NONE;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_WarpWork();
	}
}