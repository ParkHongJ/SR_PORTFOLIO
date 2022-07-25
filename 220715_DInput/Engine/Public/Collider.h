#pragma once
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CCollider : public CComponent
{
public:
	enum COLLISIONGROUP {TOODEE, TOPDEE, MONSTER, BLOCK, COLLISION_END};
private:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Add_CollisionGroup(COLLISIONGROUP eCollisionGroup, class CGameObject* pGameObject);
	/*�׳� �簢�浹 (�о�����)*/
	HRESULT Collision_Rect(COLLISIONGROUP eSourGroup, COLLISIONGROUP eDestGroup);
	HRESULT End();
	bool Check_Rect(class CGameObject* pSour, class CGameObject* pDest);
private:
	list<class CGameObject*>				m_CollisionObjects[COLLISION_END];
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