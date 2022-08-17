#include "stdafx.h"
#include "Level_SJH.h"

#include "GameInstance.h"
#include "Camera_Free.h"

#include "Level_Loading.h"
#include "GameMgr.h"
#include "ParticleMgr.h"

CLevel_SJH::CLevel_SJH(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_SJH::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Test_Cube()))
		return E_FAIL;

	return S_OK;
}

void CLevel_SJH::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

HRESULT CLevel_SJH::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("In_Level_SJH"));

	return S_OK;
}

HRESULT CLevel_SJH::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC CameraDesc;

	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fFovy = D3DXToRadian(53.0f);
	CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.0f;

	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Camera_Free"), LEVEL_SJH, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_SJH::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"), LEVEL_SJH, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Sky"), LEVEL_SJH, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_SJH::Ready_Layer_Object(const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg /*= nullptr*/)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(pPrototypeTag, LEVEL_STAGE1, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_SJH::Test_Cube()
{
	ObjInfo objInfo;

	objInfo.iNumLevel = LEVEL_STAGE1;
	objInfo.iDirection = 0;

	_float Temp = 15.5f;

	//========== 15

	/* Left Top  */
	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 9;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 1;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 29;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 6; i < 23; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 5;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 30;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 1;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	/* Right Top  */
	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 3;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 14

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 13

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 12

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 11

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 10

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 9

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 24;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(6.5f, 0.5f, Temp);
	objInfo.iTex = 5;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(7.5f, 0.5f, Temp);
	objInfo.iTex = 7;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 8

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 7

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 6

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 5

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 4

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 3

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 2

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 1

	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 8;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 0;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 2;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	Temp -= 1.f;
	//========== 0

	/* Right Bottom  */
	objInfo.vPos = _float3(28.5f, 0.5f, Temp);
	objInfo.iTex = 6;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 1; i < 6; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 4;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(5.5f, 0.5f, Temp);
	objInfo.iTex = 28;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 6; i < 23; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 5;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	objInfo.vPos = _float3(23.5f, 0.5f, Temp);
	objInfo.iTex = 27;
	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	for (_uint i = 24; i < 28; ++i) {
		objInfo.vPos = _float3((_float)i + 0.5f, 0.5f, Temp);
		objInfo.iTex = 4;
		if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
			return E_FAIL;
	}

	/* Left Bottom  */
	objInfo.vPos = _float3(0.5f, 0.5f, Temp);
	objInfo.iNumLevel = LEVEL_STAGE1;
	objInfo.iDirection = 0;
	objInfo.iTex = 12;

	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wall", L"Layer_Wall", &objInfo)))
		return E_FAIL;

	//==========

	return S_OK;
}

CLevel_SJH * CLevel_SJH::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_SJH*		pInstance = new CLevel_SJH(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CLevel_SJH"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_SJH::Free()
{
	__super::Free();
}