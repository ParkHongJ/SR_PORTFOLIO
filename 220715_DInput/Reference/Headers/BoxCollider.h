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
	HRESULT Render();//For Debug

	_float3 GetMin() { return m_fMin; }
	_float3 GetMax() { return m_fMax; }

public:
	static CBoxCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

	/*void Set_State(STATETYPE eState, const _float3& State) {
		memcpy(&m_WorldMatrix.m[eState][0], &State, sizeof(_float3));		
	}*/
	void Set_Scale(_float3& vScale);


/*------------
--AABB�� ����--
-------------*/
private:
	_float3 m_fMin;
	_float3 m_fMax;

/*------------------
--�׸���� �������--
------------------*/
private:
	LPDIRECT3DVERTEXBUFFER9	m_pVB = nullptr;
	_uint					m_iStride = 0; /* �����ϳ��� byteũ�� .*/
	_uint					m_iNumVertices = 0;
	_ulong					m_dwFVF = 0;

private:
	LPDIRECT3DINDEXBUFFER9	m_pIB = nullptr;
	_uint					m_iIndexSizeofPrimitive = 0;
	D3DFORMAT				m_eIndexFormat;

	
};

END