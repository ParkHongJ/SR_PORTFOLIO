#pragma once
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CBoxCollider : public CComponent
{
public:
	static _uint g_iNextID;
public:
	enum STATETYPE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
	typedef struct BoxColliderDesc
	{
		_float3 vPos;
		_float3 vSize;
		_float fRadius;
		bool	bIsTrigger;
	}BOXDESC;
public:
	CBoxCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBoxCollider(const CBoxCollider& rhs);
	virtual ~CBoxCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Render(_float4x4 matWorld);//For Debug

	_float3 GetMin();
	_float3 GetMax();
	void Tick(const _float4x4& matWorld);
	_float GetRadius() { return m_fRadius; }
	_uint GetID() { return m_iID; }
public:
	static CBoxCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

public:
	const BOXDESC GetBoxDesc() { return m_BoxDesc; }
/*------------
--AABB�� ����--
-------------*/
private:
	_float3 m_fMin;
	_float3 m_fMax;

	BOXDESC m_BoxDesc;
/*--------------
--���浹�� ����--
--------------*/
private:
	_float m_fRadius;
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
	_uint					m_iID;
	
};

END