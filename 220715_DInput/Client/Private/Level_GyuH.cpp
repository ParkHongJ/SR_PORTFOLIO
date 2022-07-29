#include "stdafx.h"
#include "..\Public\Level_GyuH.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "GameMgr.h"
CLevel_GyuH::CLevel_GyuH(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_GyuH::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	LoadGameObject();
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	_float3 vInitPos{ 15.5f,0.5f,11.5f };
	if (FAILED(Ready_Layer_Topdee(TEXT("Layer_Topdee"), vInitPos)))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	 vInitPos= { 10.5f,0.5f,11.5f };
	for (_uint i = 0; i < 3; ++i) {
		vInitPos.z -= 1.0f;
		vInitPos.y = 0.3f;
		if (FAILED(Ready_Layer_Hole(TEXT("Layer_Hole"), vInitPos)))
			return E_FAIL;
	}
	vInitPos = { 7.f,0.5f,10.5f };
	for (_uint i = 0; i < 3; ++i) {
		vInitPos.z -= 1.0f;
		if (FAILED(Ready_Layer_Spike(TEXT("Layer_Spike"), vInitPos)))
			return E_FAIL;
	}
	CGameMgr::Get_Instance()->Open_Level_Append_ObstaclePos(LEVEL_GYUH, L"Layer_Hole",	true);
	CGameMgr::Get_Instance()->Open_Level_Append_ObstaclePos(LEVEL_STATIC, L"Layer_Spike", false);
	return S_OK;

}

void CLevel_GyuH::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	CGameMgr::Get_Instance()->Tick(fTimeDelta);

}
HRESULT CLevel_GyuH::Ready_Layer_Block(const _tchar * pLayerTag, void * pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Cube"), LEVEL_SJH, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;

}

HRESULT CLevel_GyuH::Ready_Layer_Hole(const _tchar * pLayerTag,void* pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Hole"), LEVEL_GYUH, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GyuH::Ready_Layer_Spike(const _tchar * pLayerTag, void * pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Spike"), LEVEL_STATIC, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}


void CLevel_GyuH::LoadGameObject()
{
	HANDLE		hFile = CreateFile(L"../Bin/data/Map.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD dwByte = 0;

	while (true)
	{
		_float3 vBlockPos = {};
		ReadFile(hFile, vBlockPos, sizeof(_float3), &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}
		m_list.push_back(vBlockPos);
	}

	CloseHandle(hFile);

	for (auto& iter : m_list)
	{
		m_vPosition = iter;
		Ready_Layer_Block(L"Layer_Cube", m_vPosition);
	}
}

HRESULT CLevel_GyuH::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("규현레벨임"));

	return S_OK;
}

HRESULT CLevel_GyuH::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Camera_Free")
		, LEVEL_GYUH, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GyuH::Ready_Layer_Topdee(const _tchar * pLayerTag,void* pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Topdee"), 
		LEVEL_GYUH, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GyuH::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"), 
		LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_GyuH * CLevel_GyuH::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GyuH*		pInstance = new CLevel_GyuH(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CLevel_GyuH"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GyuH::Free()
{
	__super::Free();
}
