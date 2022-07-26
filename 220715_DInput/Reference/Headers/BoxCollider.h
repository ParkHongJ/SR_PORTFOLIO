#pragma once
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CBoxCollider : public CComponent
{
public:
	enum STATETYPE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	CBoxCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBoxCollider(const CBoxCollider& rhs);
	virtual ~CBoxCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Render(_float4x4 matWorld);//For Debug

	_float3 GetMin() { return m_fMin; }
	_float3 GetMax() { return m_fMax; }

public:
	static CBoxCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

/*------------
--AABB용 변수--
-------------*/
private:
	_float3 m_fMin;
	_float3 m_fMax;

/*------------------
--그리기용 멤버변수--
------------------*/
private:
	LPDIRECT3DVERTEXBUFFER9	m_pVB = nullptr;
	_uint					m_iStride = 0; /* 정점하나의 byte크기 .*/
	_uint					m_iNumVertices = 0;
	_ulong					m_dwFVF = 0;

private:
	LPDIRECT3DINDEXBUFFER9	m_pIB = nullptr;
	_uint					m_iIndexSizeofPrimitive = 0;
	D3DFORMAT				m_eIndexFormat;

	
};

END