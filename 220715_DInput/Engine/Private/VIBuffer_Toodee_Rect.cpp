#include "VIBuffer_Toodee_Rect.h"

CVIBuffer_Toodee_Rect::CVIBuffer_Toodee_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Toodee_Rect::CVIBuffer_Toodee_Rect(const CVIBuffer_Toodee_Rect & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Toodee_Rect::Initialize_Prototype()
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 /*| D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1)*/;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*		pVertices = nullptr;


	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.f, 1.5f);
	pVertices[0].vTexture = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.f, 1.5f);
	pVertices[1].vTexture = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, 0.f, 0.f);
	pVertices[2].vTexture = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, 0.f, 0.f);
	pVertices[3].vTexture = _float2(0.f, 1.f);

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

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Toodee_Rect::Initialize(void * pArg)
{
	return S_OK;
}

CVIBuffer_Toodee_Rect * CVIBuffer_Toodee_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Toodee_Rect* pInstance = new CVIBuffer_Toodee_Rect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Toodee_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Toodee_Rect::Clone(void * pArg)
{
	CVIBuffer_Toodee_Rect* pInstance = new CVIBuffer_Toodee_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_Toodee_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Toodee_Rect::Free()
{
	__super::Free();
}
