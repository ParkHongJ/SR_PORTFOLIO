#include "stdafx.h"
#include "..\Public\Level_GyuTest.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "ParticleMgr.h"
#include "GameMgr.h"
#include "Topdee.h"
#include "Toodee.h"
#include "Hong.h"

CLevel_GyuTest::CLevel_GyuTest(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_GyuTest::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	CGameMgr::Get_Instance()->Initialize(LEVEL_STAGE1);
	//LoadGameObject();
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	CTopdee::PLAYER_INFO Info;
	Info.iNumLevel = LEVEL_STAGE1;
	Info.vPos = _float3(3.f, 1.f, 10.f);
	if (FAILED(Ready_Layer_Topdee(TEXT("Layer_Topdee"), &Info)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Toodee(TEXT("Layer_Toodee"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Particle_Spark(TEXT("Layer_Particle_Spark"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	/*CHong::OBJ_INFO ObjInfo;
	ObjInfo.vPos = { 10.f,1.f,10.f };*/
	if (FAILED(Ready_Layer_ElectricBlock(TEXT("Layer_Cube"))))
		return E_FAIL;
	


	CHong::OBJ_INFO ObjInfo2;
	ObjInfo2.vPos = { 12.f,1.f,10.f };
	ObjInfo2.iNumLevel = (_uint)LEVEL_STAGE1;
	//if (FAILED(Ready_Layer_GravityBlock(TEXT("Layer_Cube"), &ObjInfo2)))
	//	return E_FAIL;
	ObjInfo objInfo3;
	objInfo3.vPos = _float3(13.5f, .5f, 4.5f);
	objInfo3.iNumLevel = LEVEL_STAGE1;
	objInfo3.iDirection = 2;

	ObjInfo objInfo4;
	objInfo4.vPos = _float3(6.5f, .5f, 4.5f);
	objInfo4.iNumLevel = LEVEL_STAGE1;
	objInfo4.iDirection = 2;

	ObjInfo objInfo5;
	objInfo5.vPos = _float3(8.5f, .5f, 4.5f);
	objInfo5.iNumLevel = LEVEL_STAGE1;
	

	//if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_WarpBlock", L"Layer_Cube", &objInfo3)))
	//return E_FAIL;
	//if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_WarpBlock", L"Layer_Cube", &objInfo4)))
	//return E_FAIL;
	
	
	CParticleMgr::Get_Instance()->Initialize(LEVEL_STAGE1);

	return S_OK;

}

void CLevel_GyuTest::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	CGameMgr::Get_Instance()->Tick(fTimeDelta);
}

HRESULT CLevel_GyuTest::Ready_Layer_Wall(const _tchar * pLayerTag, void * pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Wall"), LEVEL_STAGE1, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;

}

HRESULT CLevel_GyuTest::Ready_Layer_WarpBlock(const _tchar * pLayerTag, void * pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_WarpBlock"), LEVEL_STAGE1, pLayerTag, pArg)))
		return E_FAIL;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GyuTest::Ready_Layer_Toodee(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CToodee::PLAYER_INFO Info;
	Info.iNumLevel = LEVEL_STAGE1;
	Info.vPos = _float3(25.f, 1.f, 1.5f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Toodee"),
		LEVEL_STAGE1, pLayerTag, &Info)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GyuTest::Ready_Layer_Particle_Spark(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Particle_Spark"),
		LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GyuTest::Ready_Layer_Object(const _tchar * pPrototypeTag, const _tchar * pLayerTag, void * pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(pPrototypeTag, LEVEL_STAGE1, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GyuTest::Ready_Layer_ElectricBlock(const _tchar * pLayerTag, void * pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CHong::OBJ_INFO ObjInfo;
	ObjInfo.vPos = { 10.5f,0.5f,10.5f };
	ObjInfo.iNumLevel = (_uint)LEVEL_STAGE1;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ElectricBlock"), LEVEL_STAGE1, pLayerTag, &ObjInfo)))
		return E_FAIL;
	//CHong::OBJ_INFO ObjInfo2;
	//ObjInfo2.vPos = { 12.5f,.5f,10.5f };
	//ObjInfo2.iNumLevel = (_uint)LEVEL_STAGE1;
	//if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ElectricBlock"), LEVEL_STAGE1, pLayerTag, &ObjInfo2)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GyuTest::Ready_Layer_GravityBlock(const _tchar * pLayerTag, void * pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_GravityBlock"), LEVEL_STAGE1, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

void CLevel_GyuTest::LoadGameObject()
{
	HANDLE hFile = CreateFile(L"../Bin/Data/TEST.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0; //String length

	OBJ_INFO m_OBJInfo = {};
	/*	Map<<Prototype, Layer>, vPosition> */
	while (true)
	{
		// Key 값 로드
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		_tchar*	pFirst = nullptr;
		pFirst = new _tchar[dwStrByte];
		ReadFile(hFile, pFirst, dwStrByte, &dwByte, nullptr);
		pFirst[dwByte / sizeof(_tchar)] = 0;

		//Key값 로드
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		_tchar*	pSecond = nullptr;
		pSecond = new _tchar[dwStrByte];
		ReadFile(hFile, pSecond, dwStrByte, &dwByte, nullptr);
		pSecond[dwByte / sizeof(_tchar)] = 0;

		TagInfo* tagInfo = new TagInfo;
		tagInfo->pPrototypeTag = pFirst;
		Safe_Delete_Array(pFirst);
		tagInfo->pLayerTag = pSecond;
		Safe_Delete_Array(pSecond);

		ReadFile(hFile, &m_OBJInfo.vPos, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &m_OBJInfo.iNumLevel, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &m_OBJInfo.iDirection, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &m_OBJInfo.iTex, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(tagInfo);
			Safe_Delete_Array(pFirst);
			Safe_Delete_Array(pSecond);
			break;
		}

		ObjInfo* objInfo = new ObjInfo;
		objInfo->vPos = m_OBJInfo.vPos;
		objInfo->iNumLevel = m_OBJInfo.iNumLevel;
		objInfo->iDirection = m_OBJInfo.iDirection;
		objInfo->iTex = m_OBJInfo.iTex;

		m_pObjects.insert({ tagInfo, objInfo });
	}
	auto iter = m_pObjects.begin();

	while (iter != m_pObjects.end())
	{
		Ready_Layer_Object(iter->first->pPrototypeTag.c_str(), iter->first->pLayerTag.c_str(), iter->second);
		++iter;
	}
	CloseHandle(hFile);
}

HRESULT CLevel_GyuTest::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("규현레벨임"));

	return S_OK;
}

HRESULT CLevel_GyuTest::Ready_Layer_WarpBox(const _tchar * pLayerTag, void * pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_WarpBlock"), LEVEL_STAGE1, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GyuTest::Ready_Layer_Camera(const _tchar * pLayerTag)
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
		, LEVEL_STAGE1, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GyuTest::Ready_Layer_Topdee(const _tchar * pLayerTag,void* pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Topdee"), 
		LEVEL_STAGE1, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GyuTest::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"), 
		LEVEL_STAGE1, pLayerTag)))
		return E_FAIL;

	_float3 vPos{ 14.5f,0.5f,11.f };
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Thunder_Cloud"),
		LEVEL_STAGE1, pLayerTag,vPos)))
		return E_FAIL;

	 vPos = { 8.5f,0.5f,11.f };
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Thunder_Cloud"),
		LEVEL_STAGE1, pLayerTag, vPos)))
		return E_FAIL;
	
	 vPos = { 20.5f,0.5f,11.f };
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Thunder_Cloud"),
		LEVEL_STAGE1, pLayerTag, vPos)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_GyuTest * CLevel_GyuTest::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GyuTest*		pInstance = new CLevel_GyuTest(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CLevel_GyuTest"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GyuTest::Free()
{
	__super::Free();
	CParticleMgr::Get_Instance()->Destroy_Instance();
}
