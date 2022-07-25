#include "..\Public\VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_dwFVF(rhs.m_dwFVF)
	, m_ePrimitiveType(rhs.m_ePrimitiveType)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_pIB(rhs.m_pIB)
	, m_iIndexSizeofPrimitive(rhs.m_iIndexSizeofPrimitive)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_pVerticesPos(rhs.m_pVerticesPos)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);

}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);

	m_pGraphic_Device->SetIndices(m_pIB);

	m_pGraphic_Device->SetFVF(m_dwFVF);	

	// m_pGraphic_Device->DrawPrimitive(m_ePrimitiveType, 0, m_iNumPrimitive);
	m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

	return S_OK;
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexSizeofPrimitive * m_iNumPrimitive, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;	

	return S_OK;
}


void CVIBuffer::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pVerticesPos);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
