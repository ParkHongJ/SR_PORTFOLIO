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
	enum COLLISIONGROUP { PLAYER, BULLET, MONSTER, BLOCK, INTEREACTION, PORTAL, OBJECT, COLLISION_END };
	enum DIRECTION { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_END };

private:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Add_CollisionGroup(COLLISIONGROUP eCollisionGroup, class CBoxCollider* pBoxCollider, class CTransform* pTransform);
	HRESULT Collision_RectEx(COLLISIONGROUP eSourGroup, COLLISIONGROUP eDestGroup, _float fTimeDelta);
	HRESULT Collision_SphereEx(COLLISIONGROUP eSourGroup, COLLISIONGROUP eDestGroup, _float fTimeDelta);
	HRESULT Collision_TriggerXXX(COLLISIONGROUP eSourGroup, COLLISIONGROUP eDestGroup, _float fTimeDelta);
	HRESULT EndEx();
	bool Check_SphereEx(class CBoxCollider* pSourCol, class CTransform* pSourTrans, class CBoxCollider* pDestCol, class CTransform* pDestTrans);
	bool Check_RectEx(class CBoxCollider* pSourCol, class CTransform* pSourTrans, class CBoxCollider* pDestCol, class CTransform* pDestTrans, float *pX, float* pZ);

private:
	list <pair<class CBoxCollider*, class CTransform* >> m_pCollisionObjects[COLLISION_END];
	typedef list<pair<class CBoxCollider*, class CTransform*>>		COLLISIONOBJECTS;
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