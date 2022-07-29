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

public:
	enum GAMEMODE
	{
		TOODEE, TOPDEE, THIRDEE, MODE_END
	};
private:
	CGameMgr();
	virtual ~CGameMgr() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	GAMEMODE GetMode() { return m_eGameMode; }
public:
	HRESULT Set_Player_Active(const _tchar* pTag, CGameObject* pPlayer);
	_bool IsPlayerActive(const _tchar* pTag);

private:
	CGameInstance* m_pGameInstance = nullptr;

	map<const _tchar*, _bool> m_PlayerActive;
	typedef map<const _tchar*, _bool> PLAYERACTIVE;

	GAMEMODE m_eGameMode = MODE_END;
public:
	virtual void Free() override;
};
END