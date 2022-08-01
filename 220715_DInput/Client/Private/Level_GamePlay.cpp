#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Free.h"

#include "Level_Loading.h"
#include "GameMgr.h"
CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_Release(pGameInstance);

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Toodee(TEXT("Layer_Toodee"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Topdee(TEXT("Layer_topdee"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_Key(TEXT("Layer_Key"), _float3(10.f, 0.5f, 3.f))))
		return E_FAIL;

	if (FAILED(Ready_Layer_KeyBox(TEXT("Layer_KeyBox"))))
		return E_FAIL;*/
	/*	if (FAILED(Ready_Layer_Monster_Pig(TEXT("Layer_Monster_Pig"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Turret(TEXT("Layer_Monster_Turret"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Portal(TEXT("Layer_Portal"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Cloud(TEXT("Layer_Cloud"))))
		return E_FAIL;

	_float3 temp = { 15.5f,0.3f,4.5f };
	*/
	//이상없음
	/*if (FAILED(Ready_Layer_Hole(TEXT("Layer_Hole"), _float3(12.5f, 0.5f, 10.5f))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Wall(TEXT("Layer_Wall"), _float3(12.5f, 0.5f, 10.5f))))
		return E_FAIL;


	if (FAILED(Ready_Layer_Key(TEXT("Layer_Key"))))
		return E_FAIL;
		
	_float3 vInitPos = { 12.5f,0.5f,10.5f };
	for (_uint i = 0; i < 1; ++i) {
		vInitPos.z -= 1.0f;
		if (FAILED(Ready_Layer_Block((L"Layer_Cube"), vInitPos)))
			return E_FAIL;
	}
*/

	LoadGameObject();
	CGameMgr::Get_Instance()->Open_Level_Append_ObstaclePos(LEVEL_STAGE1, L"Layer_Hole", true);
	CGameMgr::Get_Instance()->Open_Level_Append_ObstaclePos(LEVEL_STAGE1, L"Layer_Wall", false);

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameMgr::Get_Instance()->Tick(fTimeDelta);

	/*if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_LOGO))))
			return;

		Safe_Release(pGameInstance);
	}*/
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("게임플레이레벨임"));

	//ImGui::Begin("GamePlay");
	//
	//const char* Obj[] = { "Player", "Monster", "Map" };
	//static int Obj_current_idx = 0; // Here we store our selection data as an index.
	//const char* combo_preview_value = Obj[Obj_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
	//if (ImGui::BeginCombo("GamePlay_Object", combo_preview_value))
	//{
	//	for (int n = 0; n < IM_ARRAYSIZE(Obj); n++)
	//	{
	//		const bool is_selected = (Obj_current_idx == n);

	//		if (ImGui::Selectable(Obj[n], is_selected))
	//			Obj_current_idx = n;

	//		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
	//		if (is_selected)
	//		{
	//			ImGui::SetItemDefaultFocus();
	//		}
	//	}
	//	ImGui::EndCombo();
	//}

	//ImGui::End();


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Camera_Free"), LEVEL_GAMEPLAY, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Toodee(const _tchar * pLayerTag, void* pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Toodee"), 
		LEVEL_SJH, pLayerTag, _float3(3.f, 1.f, 14.f))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Block(const _tchar* pLayerTag, void* pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Cube"), LEVEL_STAGE1, pLayerTag, pArg)))
		return E_FAIL;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster_Pig(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Monster_Pig"), LEVEL_SENI, pLayerTag)))
		return E_FAIL;


	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Topdee(const _tchar * pLayerTag, void* pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Topdee"),
		LEVEL_GYUH, pLayerTag, _float3(14.f, 1.f, 3.f))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Turret(const _tchar* pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Turret"),
		LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Bullet(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Bullet"),
		LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Layer_Portal(const _tchar* pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Portal"),
		LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

void CLevel_GamePlay::LoadGameObject()
{
	HANDLE hFile = CreateFile(L"../Bin/Data/temp.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0; //String length

						 //Clear Map
	/*for (auto& Pair : m_TestMap)
	{
		delete Pair.first.second;
		delete Pair.second;
	}
	m_TestMap.clear();*/

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

		_float3 vPos = {};
		ReadFile(hFile, vPos, sizeof(_float3), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pFirst);
			Safe_Delete(pSecond);
			break;
		}
		TEST_STRUCT* test = new TEST_STRUCT;
		test->pFirst = pFirst;
		test->pSecond = pSecond;
		/*wstring str1(pFirst);
		wstring str2(pFirst);*/
		

		m_TestMap.insert(make_pair(test, vPos));
		//Ready_Layer_Object(L"Prototype_GameObject_Cube", L"Layer_Cube", vPos);

		/*Safe_Delete(pFirst);
		Safe_Delete(pSecond);*/
		Safe_Delete_Array(pFirst);
		Safe_Delete_Array(pSecond);
	}
	CloseHandle(hFile);

	auto iter = m_TestMap.begin();

	while (iter != m_TestMap.end())
	{
		Ready_Layer_Object(iter->first->pFirst.c_str(), iter->first->pSecond.c_str(), iter->second);
		++iter;
	}
}

HRESULT CLevel_GamePlay::Ready_Layer_Hole(const _tchar* pLayerTag, void* pArg )
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Hole"), LEVEL_STAGE1, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Cloud(const _tchar* pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Cloud"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Key(const _tchar* pLayerTag, void* pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Key"), LEVEL_GAMEPLAY, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_Wall(const _tchar * pLayerTag, void * pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Wall"), LEVEL_STAGE1, pLayerTag, pArg)))
		return E_FAIL;
	Safe_Release(pGameInstance);

	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Layer_KeyBox(const _tchar* pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_KeyBox"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

	for (auto& Pair : m_TestMap)
	{
		delete Pair.first;
	}
	m_TestMap.clear();
}

HRESULT CLevel_GamePlay::Ready_Layer_Object(const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg /*= nullptr*/)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(pPrototypeTag, LEVEL_STAGE1, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}
