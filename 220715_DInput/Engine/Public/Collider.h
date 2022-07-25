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
	/*그냥 사각충돌 (밀어내기없음)*/
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
	//나중에 박스,원, 캡슐 등으로 바뀔수있어서 따로뺌
protected:
	_float3 m_vPos;

	//나중에 박스,원, 캡슐 등으로 바뀔수있어서 따로뺌
	
};

END