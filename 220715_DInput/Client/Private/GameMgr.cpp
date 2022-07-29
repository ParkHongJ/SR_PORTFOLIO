#include "stdafx.h"
#include "..\Public\GameMgr.h"

#include "GameInstance.h"
#include "KeyMgr.h"

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
	if (Key_Down(DIK_X)) {
		Player_Active();
	}
}

void CGameMgr::LateTick(_float fTimeDelta)
{
}

void CGameMgr::Set_Object_Data(const _tchar * pTag, _bool * bData)
{
	for (auto& pair : m_Data)
		if (pair.first == pTag) {
			pair.second = bData;
			return;
		}

	m_Data.emplace(pTag, bData);
	return;
}

_bool CGameMgr::Get_Object_Data(const _tchar * pTag)
{
	for (auto& pair : m_Data)
		if (pair.first == pTag) {
			return *(pair.second);
		}
}

void CGameMgr::Player_Active()
{
	m_eGameMode == TOODEE ? m_eGameMode = TOPDEE : m_eGameMode = TOODEE;
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