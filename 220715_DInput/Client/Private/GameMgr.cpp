#include "stdafx.h"
#include "..\Public\GameMgr.h"

#include "GameInstance.h"

IMPLEMENT_SINGLETON(CGameMgr)

CGameMgr::CGameMgr()
{
}

HRESULT CGameMgr::Initialize()
{
	//게임모드의 초기화
	m_eGameMode = TOODEE;
	return S_OK;
}

void CGameMgr::Tick(_float fTimeDelta)
{
}

void CGameMgr::LateTick(_float fTimeDelta)
{
}

void CGameMgr::Set_Player_Active(const _tchar * pTag, CGameObject * pPlayer)
{
	if (Key_Down(DIK_X))//X키를 누르면
		{
			m_eGameMode == TOODEE ? m_eGameMode = TOPDEE : m_eGameMode = TOODEE;
		}

	// for (auto& pair : m_Data)
	// 	if (pair.first == pTag) {
	// 		pair.second = pPlayer->IsActive();
	// 		return;
	// 	}

	// m_Data.emplace(pTag, pPlayer->IsActive());

	// return;
}

_bool CGameMgr::IsPlayerActive(const _tchar* pTag)
{
	for (auto& pair : m_Data)
		if (pair.first == pTag)
			return pair.second;
}

_bool CGameMgr::Key_Pressing(_uchar KeyInput)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKState(KeyInput) & 0x80) {
		m_bKeyState[KeyInput] = true;
		Safe_Release(pGameInstance);
		return true;
	}

	Safe_Release(pGameInstance);
	return false;
}

_bool CGameMgr::Key_Down(_uchar KeyInput)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (!m_bKeyState[KeyInput] && (pGameInstance->Get_DIKState(KeyInput) & 0x80)) {
		m_bKeyState[KeyInput] = true;
		Safe_Release(pGameInstance);
		return true;
	}

	for (_uint i = 0; i < 256; ++i) {
		if (m_bKeyState[i] && !(pGameInstance->Get_DIKState(i) & 0x80)) {
			m_bKeyState[i] = false;
		}
	}

	Safe_Release(pGameInstance);
	return false;
}

void CGameMgr::Free()
{
	m_Data.clear();
}