#include "stdafx.h"
#include "..\Public\GameMgr.h"

IMPLEMENT_SINGLETON(CGameMgr)

CGameMgr::CGameMgr()
{
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
