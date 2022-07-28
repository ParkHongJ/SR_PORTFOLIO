#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "Hong.h"
#include "Level_GyuH.h"
#include "Level_SENI.h"
#include "Level_SJH.h"
#include "Loader.h"
#include "Loading_BackGround.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	Ready_Layer_BackGround(TEXT("Layer_Loading_BackGround"));


	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pGraphic_Device, eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (true == m_pLoader->Get_Finished())
	{
		if (GetKeyState(VK_RETURN) & 0x8000)
		{
			CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			CLevel*		pNewLevel = nullptr;

			switch (m_eNextLevel)
			{
			case LEVEL_LOGO:
				pNewLevel = CLevel_Logo::Create(m_pGraphic_Device);
				break;
			case LEVEL_GAMEPLAY:
				pNewLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
				break;
			case LEVEL_HONG:
				pNewLevel = CHong::Create(m_pGraphic_Device);
				break;
			case LEVEL_GYUH:
				pNewLevel = CLevel_GyuH::Create(m_pGraphic_Device);
				break;
			case LEVEL_SENI:
				pNewLevel = CLevel_SENI::Create(m_pGraphic_Device);
				break;
			case LEVEL_SJH:
				pNewLevel = CLevel_SJH::Create(m_pGraphic_Device);
				break;
			}

			if (nullptr == pNewLevel)
				goto except;

			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pNewLevel)))
				goto except;

			except:
			Safe_Release(pGameInstance);
		}
	}
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());
	
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Loading_BackGround"), LEVEL_LOADING, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLevel_Loading*		pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed To Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;

}


void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);

}


