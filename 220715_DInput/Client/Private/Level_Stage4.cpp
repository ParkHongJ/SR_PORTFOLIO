#include "stdafx.h"
#include "..\Public\Level_Stage4.h"

#include "GameInstance.h"
#include "Camera_Free.h"

#include "Level_Loading.h"
#include "GameMgr.h"
#include "ParticleMgr.h"
#include "Toodee.h"
#include "Topdee.h"
CLevel_Stage4::CLevel_Stage4(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}
HRESULT CLevel_Stage4::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CGameMgr::Get_Instance()->Initialize(LEVEL_STAGE4);
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Toodee(TEXT("Layer_Toodee"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Topdee(TEXT("Layer_topdee"))))
		return E_FAIL;
	LoadGameObject();

	CParticleMgr::Get_Instance()->Initialize(LEVEL_STAGE4);

	CGameMgr::Get_Instance()->Open_Level_Append_ObstaclePos(LEVEL_STAGE4, L"Layer_Hole", true);
	CGameMgr::Get_Instance()->Open_Level_Append_ObstaclePos(LEVEL_STAGE4, L"Layer_Wall", false);

#pragma region BGM
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->PlayBGM(TEXT("jungleToodeeSnd.wav"), C_FMOD::CHANNELID::BGM1, SOUND_DEFAULT);
	pGameInstance->PlayBGM(TEXT("jungleTopdeeSnd.wav"), C_FMOD::CHANNELID::BGM2, SOUND_DEFAULT);

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

void CLevel_Stage4::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameMgr::Get_Instance()->Tick(fTimeDelta);
	if (CGameMgr::Get_Instance()->Key_Down(DIK_F4))
	{
		//여기서 씬 넘겨줘야함
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		pGameInstance->StopAll();

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device,
			LEVEL_STAGE4))))
			MSG_BOX(L"레벨 오픈 실패");

		Safe_Release(pGameInstance);
	}
	if (CGameMgr::Get_Instance()->Get_Object_Data(L"Portal_NextLevel")) {
		//여기서 씬 넘겨줘야함
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		pGameInstance->StopAll();

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device,
			LEVEL_STAGE5))))
			MSG_BOX(L"레벨 오픈 실패");

		CGameMgr::Get_Instance()->m_bLoadFinish = false;
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

HRESULT CLevel_Stage4::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Level_4"));

	return S_OK;
}

void CLevel_Stage4::LoadGameObject()
{
	HANDLE hFile = CreateFile(L"../Bin/Data/LEVEL_4.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0; //String length

	OBJ_INFO m_OBJInfo = {};

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

HRESULT CLevel_Stage4::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;

	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fFovy = D3DXToRadian(53.0f);
	CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.0f;

	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Camera_Free"),
		LEVEL_STAGE4, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage4::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_uint Level = LEVEL_STAGE4;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"),
		LEVEL_STAGE4, pLayerTag, &Level)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Sky"),
		LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage4::Ready_Layer_Toodee(const _tchar * pLayerTag, void * pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CToodee::PLAYER_INFO Info;
	Info.iNumLevel = LEVEL_STAGE4;
	Info.vPos = _float3(1.5f, 1.f, 1.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Toodee"),
		LEVEL_STAGE4, pLayerTag, &Info)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage4::Ready_Layer_Topdee(const _tchar * pLayerTag, void * pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTopdee::PLAYER_INFO Info;
	Info.iNumLevel = LEVEL_STAGE4;
	Info.vPos = _float3(25.f, 1.f, 3.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Topdee"),
		LEVEL_STAGE4, pLayerTag, &Info)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage4::Ready_Layer_Object(const _tchar * pPrototypeTag, const _tchar * pLayerTag, void * pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(pPrototypeTag,
		LEVEL_STAGE4, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Stage4 * CLevel_Stage4::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Stage4*		pInstance = new CLevel_Stage4(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CLevel_Stage4"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage4::Free()
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
