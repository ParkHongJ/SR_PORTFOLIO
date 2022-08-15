#include "stdafx.h"
#include "..\Public\Level_SENI.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Level_Loading.h"
#include "ParticleMgr.h"
#include "Topdee.h"
#include "Toodee.h"
#include "Hong.h"
#include "GameMgr.h"


CLevel_SENI::CLevel_SENI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_SENI::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Topdee(TEXT("Layer_Topdee"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Toodee(TEXT("Layer_Toodee"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button(TEXT("Layer_Button"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_ButtonBlock(TEXT("Layer_ButtonBlock"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BreakingBlock(TEXT("Layer_BreakingBlock"))))
		return E_FAIL;

	ObjInfo objInfo;
	objInfo.vPos = _float3(3.f, 1.f, 3.f);
	objInfo.iNumLevel = LEVEL_SENI;
	/*if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Monster_Pig", L"Layer_Monster_Pig", &objInfo)))
		return E_FAIL;*/

	CParticleMgr::Get_Instance()->Initialize(LEVEL_SENI);

	return S_OK;
}

void CLevel_SENI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (CGameMgr::Get_Instance()->Key_Down(DIK_F4))
	{
		//여기서 씬 넘겨줘야함
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device,
			LEVEL_SENI))))
			MSG_BOX(L"레벨 오픈 실패");

		Safe_Release(pGameInstance);
	}
}

HRESULT CLevel_SENI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("세니레벨임"));

	return S_OK;
}

HRESULT CLevel_SENI::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;

	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.0f;

	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Camera_Free"), LEVEL_SENI, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_SENI::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_uint Level = LEVEL_SENI;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"),
		LEVEL_SENI, pLayerTag, &Level)))
		return E_FAIL;
	
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_SENI::Ready_Layer_Button(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CHong::OBJ_INFO Info;
	Info.iNumLevel = LEVEL_SENI;
	Info.vPos = _float3(15.5f, 0.f, 10.5f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Button"),
		LEVEL_SENI, pLayerTag, &Info)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_SENI::Ready_Layer_ButtonBlock(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CHong::OBJ_INFO Info;
	Info.iNumLevel = LEVEL_SENI;
	Info.vPos = _float3(20.5f, 0.5f, 10.5f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ButtonBlock"),
		LEVEL_SENI, pLayerTag, &Info)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_SENI::Ready_Layer_BreakingBlock(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CHong::OBJ_INFO Info;
	Info.iNumLevel = LEVEL_SENI;
	Info.vPos = _float3(3.f, 0.5f, 2.5f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BreakingBlock"),
		LEVEL_SENI, pLayerTag, &Info)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	
	return S_OK;
}

HRESULT CLevel_SENI::Ready_Layer_Topdee(const _tchar * pLayerTag, void* pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTopdee::PLAYER_INFO Info;
	Info.iNumLevel = LEVEL_SENI;
	Info.vPos = _float3(15.5f, 0.5f, 11.5f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Topdee"),
		LEVEL_SENI, pLayerTag, &Info)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_SENI::Ready_Layer_Toodee(const _tchar * pLayerTag, void * pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CToodee::PLAYER_INFO Info;
	Info.iNumLevel = LEVEL_SENI;
	Info.vPos = _float3(3.f, 1.f, 3.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Toodee"),
		LEVEL_SENI, pLayerTag, &Info)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_SENI::Ready_Layer_Object(const _tchar * pPrototypeTag, const _tchar * pLayerTag, void * pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(pPrototypeTag, LEVEL_SENI, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

CLevel_SENI * CLevel_SENI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_SENI*		pInstance = new CLevel_SENI(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CLevel_SENI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_SENI::Free()
{
	__super::Free();
	for (auto Pair : m_pObjects)
	{
		Pair.first->pPrototypeTag.clear();
		Pair.first->pLayerTag.clear();
		delete Pair.first;
		delete Pair.second;
	}
	m_pObjects.clear();
	CParticleMgr::Get_Instance()->Destroy_Instance();
}


