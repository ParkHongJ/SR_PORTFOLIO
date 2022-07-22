#include "..\Public\VIBuffer_Topdee.h"

CVIBuffer_Topdee::CVIBuffer_Topdee(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Topdee::CVIBuffer_Topdee(const CVIBuffer_Topdee & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Topdee::Initialize_Prototype()
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 7;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 /*| D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1)*/;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 5;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*		pVertices = nullptr;


	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexture = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexture = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexture = _float2(1.f, 1.f);	

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexture = _float2(0.f, 1.f);

	//======================================================
	//¶ó
	pVertices[4].vPosition = _float3(1.f, 0.25f, 0.f);
	pVertices[4].vTexture = _float2(1.f, 1.f);
	//¾÷
	pVertices[5].vPosition = _float3(0.f, 1.f, 0.f);
	pVertices[5].vTexture = _float2(1.f, 1.f);
	//·è
	pVertices[6].vPosition = _float3(0.f, -0.5f, 1.f);
	pVertices[6].vTexture = _float2(1.f, 1.f);
	//======================================================

	m_pVB->Unlock();

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;
	//==========================
	pIndices[2]._0 = 1;
	pIndices[2]._1 = 4;
	pIndices[2]._2 = 2;

	pIndices[3]._0 = 0;
	pIndices[3]._1 = 5;
	pIndices[3]._2 = 1;

	pIndices[4]._0 = 2;
	pIndices[4]._1 = 6;
	pIndices[4]._2 = 3;
	m_pIB->Unlock();
	
	return S_OK;
}

HRESULT CVIBuffer_Topdee::Initialize(void * pArg)
{
	return S_OK;
}

CVIBuffer_Topdee * CVIBuffer_Topdee::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Topdee*			pInstance = new CVIBuffer_Topdee(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Topdee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Topdee::Clone(void * pArg)
{
	CVIBuffer_Topdee*			pInstance = new CVIBuffer_Topdee(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_Topdee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Topdee::Free()
{
	__super::Free();
}
