#include "..\Public\VIBuffer_LayDown.h"

CVIBuffer_LayDown::CVIBuffer_LayDown(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_LayDown::CVIBuffer_LayDown(const CVIBuffer_LayDown & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_LayDown::Initialize_Prototype()
{
	
	
	return S_OK;
}

HRESULT CVIBuffer_LayDown::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_RectDesc, pArg, sizeof(RECTDESC));
	else
	{
		m_RectDesc.vSize = { 1.f,1.f,1.f };
	}
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 /*| D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1)*/;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*		pVertices = nullptr;


	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	_float3 vSize = m_RectDesc.vSize * 0.5f;

	/*pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);*/
	pVertices[0].vPosition = _float3(-vSize.x, 0.f, vSize.y);
	pVertices[0].vTexture = _float2(0.f, 0.f);

	/*pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);*/
	pVertices[1].vPosition = _float3(vSize.x, 0.f, vSize.y);
	pVertices[1].vTexture = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(vSize.x, 0.f, -vSize.y);
	pVertices[2].vTexture = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-vSize.x,0.f, -vSize.y);
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

CVIBuffer_LayDown * CVIBuffer_LayDown::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_LayDown*			pInstance = new CVIBuffer_LayDown(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_LayDown"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_LayDown::Clone(void * pArg)
{
	CVIBuffer_LayDown*			pInstance = new CVIBuffer_LayDown(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_LayDown"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_LayDown::Free()
{
	__super::Free();
}
