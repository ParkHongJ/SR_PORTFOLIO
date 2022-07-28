#include "..\Public\BoxCollider.h"
#include "Transform.h"


_uint CBoxCollider::g_iNextID = 0;

CBoxCollider::CBoxCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}
CBoxCollider::CBoxCollider(const CBoxCollider& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_dwFVF(rhs.m_dwFVF)
	, m_pIB(rhs.m_pIB)
	, m_iIndexSizeofPrimitive(rhs.m_iIndexSizeofPrimitive)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_fMin(rhs.m_fMin)
	, m_fMax(rhs.m_fMax)
	, m_iID(g_iNextID++)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CBoxCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoxCollider::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_BoxDesc, pArg, sizeof(BOXDESC));
	else
	{
		m_BoxDesc.vPos = { 0.f, 0.f, 0.f };
		m_BoxDesc.vSize = { 1.f, 1.f, 1.f };
		m_BoxDesc.bIsTrigger = false;
	}
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 8;
	m_dwFVF = D3DFVF_XYZ;

	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * 12, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXTEX*	pVertices = nullptr;

	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	//pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	
	_float3 vSize = m_BoxDesc.vSize * 0.5f;

	pVertices[0].vPosition = _float3(m_BoxDesc.vPos.x - vSize.x, 
									 m_BoxDesc.vPos.y + vSize.y,
									 m_BoxDesc.vPos.z - vSize.z);


	//pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);

	pVertices[1].vPosition = _float3(m_BoxDesc.vPos.x + vSize.x,
		m_BoxDesc.vPos.y + vSize.y,
		m_BoxDesc.vPos.z - vSize.z);

	//pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);

	pVertices[2].vPosition = _float3(m_BoxDesc.vPos.x + vSize.x,
		m_BoxDesc.vPos.y - vSize.y,
		m_BoxDesc.vPos.z - vSize.z);

	//m_fMin = pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);

	m_fMin = pVertices[3].vPosition = _float3(m_BoxDesc.vPos.x - vSize.x,
		m_BoxDesc.vPos.y - vSize.y,
		m_BoxDesc.vPos.z - vSize.z);

	//pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);

	pVertices[4].vPosition = _float3(m_BoxDesc.vPos.x - vSize.x,
		m_BoxDesc.vPos.y + vSize.y,
		m_BoxDesc.vPos.z + vSize.z);


	//m_fMax = pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);

	m_fMax = pVertices[5].vPosition = _float3(m_BoxDesc.vPos.x + vSize.x,
		m_BoxDesc.vPos.y + vSize.y,
		m_BoxDesc.vPos.z + vSize.z);

	//pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);

	pVertices[6].vPosition = _float3(m_BoxDesc.vPos.x + vSize.x,
		m_BoxDesc.vPos.y - vSize.y,
		m_BoxDesc.vPos.z + vSize.z);

	//pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);

	pVertices[7].vPosition = _float3(m_BoxDesc.vPos.x - vSize.x,
		m_BoxDesc.vPos.y - vSize.y,
		m_BoxDesc.vPos.z + vSize.z);

	m_pVB->Unlock();

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);

	m_eIndexFormat = D3DFMT_INDEX16;

	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexSizeofPrimitive * 12, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
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

HRESULT CBoxCollider::Render(_float4x4 matWorld)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);

	m_pGraphic_Device->SetIndices(m_pIB);

	m_pGraphic_Device->SetFVF(m_dwFVF);

	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	return S_OK;
}

_float3 CBoxCollider::GetMin()
{
	return m_fMin;
}

_float3 CBoxCollider::GetMax()
{
	return m_fMax;
}

CBoxCollider * CBoxCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoxCollider*			pInstance = new CBoxCollider(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBoxCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CBoxCollider::Clone(void * pArg)
{
	CBoxCollider*			pInstance = new CBoxCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBoxCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoxCollider::Free()
{
	__super::Free();
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
