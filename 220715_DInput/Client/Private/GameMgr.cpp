#include "stdafx.h"
#include "..\Public\GameMgr.h"
#include "KeyMgr.h"
IMPLEMENT_SINGLETON(CGameMgr)

CGameMgr::CGameMgr()
{
}

void CGameMgr::Tick(_float fTimeDelta)
{
	if (CKeyMgr::Get_Instance()->Key_Down('X'))//X키를 누르면
	{
		m_eGameMode == TOODEE ? m_eGameMode = TOPDEE : m_eGameMode = TOODEE;
	}
}

void CGameMgr::LateTick(_float fTimeDelta)
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

HRESULT CGameMgr::Initialize()
{
	//게임모드의 초기화
	m_eGameMode = TOODEE;
	return S_OK;
}
