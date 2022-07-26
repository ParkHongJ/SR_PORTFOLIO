#pragma once

#include "Base.h"

BEGIN(Engine)
//KKK ¿ø·¡ enginedll ¾Æ´Ô.
class  ENGINE_DLL CLayer final : public CBase
{
public:
	CLayer();
	virtual ~CLayer() = default;
public:
	class CComponent* Get_Component(const _tchar* pComponentTag, _uint iLayerIndex);

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);

	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	list<class CGameObject*>* KKK_Get_List();
private:
	list<class CGameObject*>				m_GameObjects;
	typedef list<class CGameObject*>		GAMEOBJECTS;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END