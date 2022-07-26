#pragma once

/* V : Vertex */
/* I : Index */

/* 실제 모델들의 부모가 되는 클래스다. */
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();
public:
	virtual _bool Picking(class CTransform* pTransform, _float3* pOut) { return true; }
	virtual _float Compute_Height(_float3 vTargetPos) { return 0; }




protected:
	LPDIRECT3DVERTEXBUFFER9	m_pVB = nullptr;
	_uint					m_iStride = 0; /* 정점하나의 byte크기 .*/
	_uint					m_iNumVertices = 0;
	_ulong					m_dwFVF = 0;
	D3DPRIMITIVETYPE		m_ePrimitiveType;
	_uint					m_iNumPrimitive = 0;

protected:
	LPDIRECT3DINDEXBUFFER9	m_pIB = nullptr;
	_uint					m_iIndexSizeofPrimitive = 0;
	D3DFORMAT				m_eIndexFormat;
	_float3*				m_pVerticesPos = nullptr;


protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END