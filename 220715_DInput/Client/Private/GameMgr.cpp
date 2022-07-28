#include "stdafx.h"
#include "..\Public\GameMgr.h"

IMPLEMENT_SINGLETON(CGameMgr)

CGameMgr::CGameMgr()
{
}

HRESULT CGameMgr::Set_Player_Active(const _tchar * pTag, CGameObject * pPlayer)
{
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

_bool CGameMgr::IsPlayerActive(const _tchar* pTag)
{
	for (auto& pair : m_PlayerActive)
		if (pair.first == pTag)
			return pair.second;
}

void CGameMgr::Free()
{
}
