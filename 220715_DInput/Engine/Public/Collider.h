#pragma once
#include "Component.h"
BEGIN(Engine)
union COLLIDER_ID
{
	struct {
		_uint Left_ID;
		_uint Right_ID;
	};
	LONGLONG ID;
};
class ENGINE_DLL CCollider : public CComponent
{
public:
	enum COLLISIONGROUP {PLAYER, BULLET, MONSTER, BLOCK, COLLISION_END};

private:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Add_CollisionGroup(COLLISIONGROUP eCollisionGroup, class CGameObject* pGameObject);
	HRESULT Add_CollisionGroup(COLLISIONGROUP eCollisionGroup, class CBoxCollider* pBoxCollider);
	HRESULT Collision_Rect(COLLISIONGROUP eSourGroup, COLLISIONGROUP eDestGroup, _float fTimeDelta);
	HRESULT Collision_Sphere(COLLISIONGROUP eSourGroup, COLLISIONGROUP eDestGroup, _float fTimeDelta);
	HRESULT End();
	bool Check_Sphere(class CGameObject* pSour, class CGameObject* pDest);
	bool Check_Rect(class CGameObject* pSour, class CGameObject* pDest);
	bool Check_RectEx(class CGameObject* pSour, class CGameObject* pDest, float *pX, float* pZ);
//	bool Check_RectEx(class CGameObject* pSour, class CGameObject* pDest, float *pX, float* pZ);

private:
	list<class CGameObject*>				m_CollisionObjects[COLLISION_END];
	typedef list<class CGameObject*>		COLLISIONOBJECTS;
	/*list<class CBoxCollider*>				m_CollisionObjects[COLLISION_END];
	typedef list<class CBoxCollider*>		COLLISIONOBJECTS;*/
	unordered_map<LONGLONG, bool> m_ColInfo;
public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

public:
	HRESULT Render();
protected:
	_float3 m_vPos;

	
};

END