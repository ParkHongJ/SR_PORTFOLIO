#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
END

BEGIN(Client)
class CGameMgr final : public CBase
{
	DECLARE_SINGLETON(CGameMgr)

private:
	CGameMgr();
	virtual ~CGameMgr() = default;

public:
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	HRESULT Set_Player_Active(const _tchar* pTag, CGameObject* pPlayer);
	_bool IsPlayerActive(const _tchar* pTag);

private:
	CGameInstance* m_pGameInstance = nullptr;

	map<const _tchar*, _bool> m_PlayerActive;
	typedef map<const _tchar*, _bool> PLAYERACTIVE;

public:
	virtual void Free() override;
};
END