#pragma once
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CCollider : public CComponent
{
private:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Add_CollisionGroup(class CGameObject* pGameObject);
	HRESULT OnCollision();
private:
	list<class CGameObject*>				m_CollisionObjects;
	typedef list<class CGameObject*>		COLLISIONOBJECTS;

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

public:
	HRESULT Render();
	//���߿� �ڽ�,��, ĸ�� ������ �ٲ���־ ���λ�
protected:
	_float3 m_vPos;

	//���߿� �ڽ�,��, ĸ�� ������ �ٲ���־ ���λ�
	
};

END