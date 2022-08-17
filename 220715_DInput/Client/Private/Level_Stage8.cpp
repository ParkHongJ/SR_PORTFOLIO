#include "stdafx.h"
#include "..\Public\Level_Stage8.h"

#include "GameInstance.h"
#include "Camera_Free.h"

#include "Level_Loading.h"
#include "GameMgr.h"
#include "ParticleMgr.h"
#include "Toodee.h"
#include "Topdee.h"
#include "Terrain.h"

CLevel_Stage8::CLevel_Stage8(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Stage8::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	ObjInfo objInfo1;
	objInfo1.iNumLevel = LEVEL_STAGE8;
	objInfo1.iDirection = 0;
	if (FAILED(Ready_Layer_Object(TEXT("Prototype_GameObject_FadeObject"), TEXT("Layer_Fade"), &objInfo1)))
		return E_FAIL;

	CGameMgr::Get_Instance()->Initialize(LEVEL_STAGE8);
	CGameMgr::Get_Instance()->SetFadeObj(LEVEL_STAGE8);


	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Toodee(TEXT("Layer_Toodee"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Topdee(TEXT("Layer_topdee"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	LoadGameObject();
	CGameMgr::Get_Instance()->SetGameMode(true);

	ObjInfo objInfo3;
	objInfo3.iNumLevel = LEVEL_STAGE8;
	objInfo3.iDirection = 0;
	objInfo3.vPos = _float3(-15.f, .5f, 15.5f);

	if (FAILED(Ready_Layer_Object(L"Prototype_GameObject_Wave", L"Layer_Wave", &objInfo3)))
		return E_FAIL;

	CParticleMgr::Get_Instance()->Initialize(LEVEL_STAGE8);

	CGameMgr::Get_Instance()->Open_Level_Append_ObstaclePos(LEVEL_STAGE8, L"Layer_Hole", true);
	CGameMgr::Get_Instance()->Open_Level_Append_ObstaclePos(LEVEL_STAGE8, L"Layer_Wall", false);

#pragma region BGM
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->PlayBGM(TEXT("scrollingMusicSnd.wav"), C_FMOD::CHANNELID::BGM1, SOUND_DEFAULT);
	pGameInstance->PlayBGM(TEXT("scrollingMusicSnd.wav"), C_FMOD::CHANNELID::BGM2, SOUND_DEFAULT);

	m_iMod = CGameMgr::Get_Instance()->GetMode();

	pGameInstance->InitMute();

	if (CGameMgr::TOODEE == CGameMgr::Get_Instance()->GetMode())
		pGameInstance->Mute(C_FMOD::CHANNELID::BGM2);
	else if (CGameMgr::TOPDEE == CGameMgr::Get_Instance()->GetMode())
		pGameInstance->Mute(C_FMOD::CHANNELID::BGM1);

	Safe_Release(pGameInstance);
#pragma endregion

	return S_OK;
}

void CLevel_Stage8::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameMgr::Get_Instance()->Tick(fTimeDelta);

	if (CGameMgr::Get_Instance()->Key_Down(DIK_F4))
	{
		//���⼭ �� �Ѱ������
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		pGameInstance->StopAll();

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device,
			LEVEL_STAGE9))))
			MSG_BOX(L"���� ���� ����");

		CGameMgr::Get_Instance()->m_bLoadFinish = false;
		Safe_Release(pGameInstance);
	}

	if (CGameMgr::Get_Instance()->Key_Down(DIK_F5))
	{
		//���⼭ �� �Ѱ������
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		pGameInstance->StopAll();

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device,
			LEVEL_STAGE8))))
			MSG_BOX(L"���� ���� ����");

		CGameMgr::Get_Instance()->m_bLoadFinish = false;
		Safe_Release(pGameInstance);
	}

	if (CGameMgr::Get_Instance()->Get_Object_Data(L"Portal_NextLevel")) {
		//���⼭ �� �Ѱ������
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		pGameInstance->StopAll();

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device,
			LEVEL_STAGE9))))
			MSG_BOX(L"���� ���� ����");

		CGameMgr::Get_Instance()->m_bLoadFinish = FALSE;
		Safe_Release(pGameInstance);
	}
#pragma region BGM
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (m_iMod != CGameMgr::Get_Instance()->GetMode()) {
		if (CGameMgr::TOODEE == CGameMgr::Get_Instance()->GetMode()) {
			pGameInstance->Mute(C_FMOD::CHANNELID::BGM1);
			pGameInstance->Mute(C_FMOD::CHANNELID::BGM2);
		}
		else if (CGameMgr::TOPDEE == CGameMgr::Get_Instance()->GetMode()) {
			pGameInstance->Mute(C_FMOD::CHANNELID::BGM2);
			pGameInstance->Mute(C_FMOD::CHANNELID::BGM1);
		}

		m_iMod = CGameMgr::Get_Instance()->GetMode();
	}

	Safe_Release(pGameInstance);
#pragma endregion
}

HRESULT CLevel_Stage8::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Run Fast! Go!"));

	return S_OK;
}

void CLevel_Stage8::LoadGameObject()
{
	HANDLE hFile = CreateFile(L"../Bin/Data/LEVEL_8.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0; //String length

	OBJ_INFO m_OBJInfo = {};
	while (true)
	{
		// Key �� �ε�
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		_tchar*	pFirst = nullptr;
		pFirst = new _tchar[dwStrByte];
		ReadFile(hFile, pFirst, dwStrByte, &dwByte, nullptr);
		pFirst[dwByte / sizeof(_tchar)] = 0;

		//Key�� �ε�
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
		//Very Importent
		if (!wcscmp(iter->first->pPrototypeTag.c_str(), L"Prototype_GameObject_BreakingBlock"))
		{
			Ready_Layer_Object(iter->first->pPrototypeTag.c_str(), L"Layer_Breaking", iter->second);
		}
		else
			Ready_Layer_Object(iter->first->pPrototypeTag.c_str(), iter->first->pLayerTag.c_str(), iter->second);
		++iter;
	}
	CloseHandle(hFile);
}

HRESULT CLevel_Stage8::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;

	CameraDesc.vEye = _float3(14.5f, 16.7f, 7.9f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fFovy = D3DXToRadian(53.0f);
	CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.0f;
	CameraDesc.iLevel = LEVEL_STAGE8;
	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Camera_Free"), 
		LEVEL_STAGE8, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage8::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTerrain::TERRAINDESC Desc;
	Desc.iNumLevel = LEVEL_STAGE8;
	Desc.vPos = { 15.f,0.f,8.f };

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"), LEVEL_STAGE8, pLayerTag, &Desc)))
		return E_FAIL;


	Desc.vPos.x += 29.f;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"), LEVEL_STAGE8, pLayerTag, &Desc)))
		return E_FAIL;

	Desc.vPos.x += 29.f;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"), LEVEL_STAGE8, pLayerTag, &Desc)))
		return E_FAIL;
	//vPos.x += 29.f;
	/*if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"), LEVEL_STAGE8, pLayerTag, vPos)))
		return E_FAIL;
	vPos.x += 29.f;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"), LEVEL_STAGE8, pLayerTag, vPos)))
		return E_FAIL;*/
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Sky"),
		LEVEL_STATIC, pLayerTag)))
		return E_FAIL;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage8::Ready_Layer_Toodee(const _tchar * pLayerTag, void * pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CToodee::PLAYER_INFO Info;
	Info.iNumLevel = LEVEL_STAGE8;
	Info.vPos = _float3(7.5f, 0.5f, 1.5f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Toodee"),
		LEVEL_STAGE8, pLayerTag, &Info)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage8::Ready_Layer_Topdee(const _tchar * pLayerTag, void * pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTopdee::PLAYER_INFO Info;
	Info.iNumLevel = LEVEL_STAGE8;
	Info.vPos = _float3(4.f, 0.5f, 12.5f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Topdee"),
		LEVEL_STAGE8, pLayerTag, &Info)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage8::Ready_Layer_Object(const _tchar * pPrototypeTag, const _tchar * pLayerTag, void * pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(pPrototypeTag,
		LEVEL_STAGE8, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Stage8 * CLevel_Stage8::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Stage8*		pInstance = new CLevel_Stage8(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CLevel_Stage8"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage8::Free()
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
