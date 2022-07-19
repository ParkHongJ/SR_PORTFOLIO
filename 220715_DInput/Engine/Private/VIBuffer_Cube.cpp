#include "..\Public\VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cube::Initialize_Prototype()
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 8;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 /*| D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1)*/;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*		pVertices = nullptr;

	
 	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[0].vTexture = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[1].vTexture = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[2].vTexture = _float2(1.f, 1.f);	

	pVertices[3].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[3].vTexture = _float2(0.f, 1.f);

	pVertices[4].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[4].vTexture = _float2(0.f, 0.f);

	pVertices[5].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[5].vTexture = _float2(1.f, 0.f);

	pVertices[6].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[6].vTexture = _float2(1.f, 1.f);

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[7].vTexture = _float2(0.f, 1.f);

	m_pVB->Unlock();

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
	m_eIndexFormat = D3DFMT_INDEX32;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	pIndices[2]._0 = 3;
	pIndices[2]._1 = 2;
	pIndices[2]._2 = 6;

	pIndices[3]._0 = 3;
	pIndices[3]._1 = 6;
	pIndices[3]._2 = 6;

	pIndices[4]._0 = 2;
	pIndices[4]._1 = 1;
	pIndices[4]._2 = 5;

	pIndices[5]._0 = 2;
	pIndices[5]._1 = 5;
	pIndices[5]._2 = 6;

	pIndices[6]._0 = 1;
	pIndices[6]._1 = 0;
	pIndices[6]._2 = 4;

	pIndices[7]._0 = 1;
	pIndices[7]._1 = 4;
	pIndices[7]._2 = 5;

	pIndices[8]._0 = 0;
	pIndices[8]._1 = 3;
	pIndices[8]._2 = 7;

	pIndices[9]._0 = 0;
	pIndices[9]._1 = 7;
	pIndices[9]._2 = 4;

	pIndices[10]._0 = 7;
	pIndices[10]._1 = 6;
	pIndices[10]._2 = 5;

	pIndices[11]._0 = 7;
	pIndices[11]._1 = 5;
	pIndices[11]._2 = 4;


	m_pIB->Unlock();
	
	return S_OK;
}

HRESULT CVIBuffer_Cube::Initialize(void * pArg)
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 8;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 /*| D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1)*/;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*		pVertices = nullptr;


	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[0].vTexture = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[1].vTexture = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[2].vTexture = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[3].vTexture = _float2(0.f, 1.f);

	pVertices[4].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[4].vTexture = _float2(0.f, 0.f);

	pVertices[5].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[5].vTexture = _float2(1.f, 0.f);

	pVertices[6].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[6].vTexture = _float2(1.f, 1.f);

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[7].vTexture = _float2(0.f, 1.f);

	m_pVB->Unlock();

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
	m_eIndexFormat = D3DFMT_INDEX32;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	pIndices[2]._0 = 3;
	pIndices[2]._1 = 2;
	pIndices[2]._2 = 6;

	pIndices[3]._0 = 3;
	pIndices[3]._1 = 6;
	pIndices[3]._2 = 6;

	pIndices[4]._0 = 2;
	pIndices[4]._1 = 1;
	pIndices[4]._2 = 5;

	pIndices[5]._0 = 2;
	pIndices[5]._1 = 5;
	pIndices[5]._2 = 6;

	pIndices[6]._0 = 1;
	pIndices[6]._1 = 0;
	pIndices[6]._2 = 4;

	pIndices[7]._0 = 1;
	pIndices[7]._1 = 4;
	pIndices[7]._2 = 5;

	pIndices[8]._0 = 0;
	pIndices[8]._1 = 3;
	pIndices[8]._2 = 7;

	pIndices[9]._0 = 0;
	pIndices[9]._1 = 7;
	pIndices[9]._2 = 4;

	pIndices[10]._0 = 7;
	pIndices[10]._1 = 6;
	pIndices[10]._2 = 5;

	pIndices[11]._0 = 7;
	pIndices[11]._1 = 5;
	pIndices[11]._2 = 4;


	m_pIB->Unlock();

	return S_OK;
}

CVIBuffer_Cube * CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cube*			pInstance = new CVIBuffer_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Cube::Clone(void * pArg)
{
	CVIBuffer_Cube*			pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();
}
