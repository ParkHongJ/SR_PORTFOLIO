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
	HRESULT Set_Player_Active(const _tchar* pTag, CGameObject* pPlayer) {
		for (auto& pair : m_PlayerActive)
			if (pair.first == pTag) {
				pair.second = pPlayer->IsActive();
				return S_OK;
			}

		if (nullptr == pPlayer)
			return E_FAIL;

		m_PlayerActive.emplace(pTag, pPlayer->IsActive());

		return S_OK;
	}

	_bool IsPlayerActive(const _tchar* pTag);

private:
	CGameInstance* m_pGameInstance = nullptr;

	map<const _tchar*, _bool> m_PlayerActive;
	typedef map<const _tchar*, _bool> PLAYERACTIVE;

public:
	virtual void Free() override;
};
END