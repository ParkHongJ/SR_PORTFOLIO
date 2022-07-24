#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "BackGround.h"
#include "Level_Loading.h"


CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;


	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
				
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SENI))))
			return;
			
		Safe_Release(pGameInstance);
	}
}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("로고레벨임"));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CBackGround::BACKDESC			BackDesc;
	ZeroMemory(&BackDesc, sizeof(CBackGround::BACKDESC));

	BackDesc.iSizeX = g_iWinSizeX;
	BackDesc.iSizeY = g_iWinSizeY;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BackGround"), LEVEL_LOGO, pLayerTag, &BackDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Logo*		pInstance = new CLevel_Logo(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}


