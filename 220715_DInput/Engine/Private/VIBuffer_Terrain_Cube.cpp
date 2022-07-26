#include "VIBuffer_Terrain_Cube.h"

CVIBuffer_Terrain_Cube::CVIBuffer_Terrain_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Terrain_Cube::CVIBuffer_Terrain_Cube(const CVIBuffer_Terrain_Cube & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Terrain_Cube::Initialize_Prototype()
{
	m_iStride = sizeof(VTXCUBETEX);
	m_iNumVertices = 8;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0) /*| D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1)*/ ;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXCUBETEX*	pVertices = nullptr;

	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexture = pVertices[0].vPosition;
	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexture = pVertices[1].vPosition;
	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexture = pVertices[2].vPosition;
	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexture = pVertices[3].vPosition;

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexture = pVertices[4].vPosition;
	pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexture = pVertices[5].vPosition;
	pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexture = pVertices[6].vPosition;
	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexture = pVertices[7].vPosition;


	m_pVB->Unlock();

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);

	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	/* +x */
	pIndices[0]._0 = 1; pIndices[0]._1 = 5; pIndices[0]._2 = 6;
	pIndices[1]._0 = 1; pIndices[1]._1 = 6; pIndices[1]._2 = 2;

	/* -x */
	pIndices[2]._0 = 4; pIndices[2]._1 = 0; pIndices[2]._2 = 3;
	pIndices[3]._0 = 4; pIndices[3]._1 = 3; pIndices[3]._2 = 7;

	/* +y */
	pIndices[4]._0 = 4; pIndices[4]._1 = 5; pIndices[4]._2 = 1;
	pIndices[5]._0 = 4; pIndices[5]._1 = 1; pIndices[5]._2 = 0;

	/* -y */
	pIndices[6]._0 = 3; pIndices[6]._1 = 2; pIndices[6]._2 = 6;
	pIndices[7]._0 = 3; pIndices[7]._1 = 6; pIndices[7]._2 = 7;

	/* +z */
	pIndices[8]._0 = 5; pIndices[8]._1 = 4; pIndices[8]._2 = 7;
	pIndices[9]._0 = 5; pIndices[9]._1 = 7; pIndices[9]._2 = 6;

	/* -z */
	pIndices[10]._0 = 0; pIndices[10]._1 = 1; pIndices[10]._2 = 2;
	pIndices[11]._0 = 0; pIndices[11]._1 = 2; pIndices[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Terrain_Cube::Initialize(void * pArg)
{
	return S_OK;
}

CVIBuffer_Terrain_Cube * CVIBuffer_Terrain_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Terrain_Cube* pInstance = new CVIBuffer_Terrain_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Terrain_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Terrain_Cube::Clone(void * pArg)
{
	CVIBuffer_Terrain_Cube* pInstance = new CVIBuffer_Terrain_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_Terrain_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain_Cube::Free()
{
	__super::Free();
}
