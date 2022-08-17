#include "stdafx.h"
#include "..\Public\Hong.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Level_Loading.h"
#include "GameObject.h"
#include "GameMgr.h"
#include <winnt.h>
#include "Wall.h"
#include "ParticleMgr.h"
#include "Topdee.h"
#include "Toodee.h"

int CHong::iDir_Select = 0;
int CHong::iLevel_Select = 0;
int CHong::iTexNum = 0;
CHong::CHong(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CHong::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;


	CGameMgr::Get_Instance()->Initialize(LEVEL_STAGE1);
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	CTopdee::PLAYER_INFO Info;
	Info.iNumLevel = LEVEL_STAGE1;
	//Info.vPos = _float3(26.f, 1.f, 2.f);
	Info.vPos = _float3(8.f, 1.f, 13.5f);
	/*if (FAILED(Ready_Layer_Block(TEXT("Prototype_GameObject_Topdee"), L"Layer_Topdee",&Info)))
		return E_FAIL;

	CToodee::PLAYER_INFO Info2;
	Info2.iNumLevel = LEVEL_STAGE1;
	Info2.vPos = _float3(26.f, 1.f, 2.f);
	if (FAILED(Ready_Layer_Block(TEXT("Prototype_GameObject_Toodee"), L"Layer_Toodee", &Info2)))
		return E_FAIL;*/
	/*if (FAILED(Ready_Layer_Block(L"Prototype_GameObject_Wall", 
		L"Layer_Temp",
		_float3(10.f,2.5f,13.f))))
		return E_FAIL;*/

	/*CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	
	
	m_TempList = pGameInstance->GetLayer(LEVEL_STAGE1, L"Layer_Temp");
	
	
	
	Safe_Release(pGameInstance);*/
	D3DXCreateSphere(m_pGraphic_Device, 3.0f, 30, 10, &m_pSphereMesh, NULL);

	//LoadGameObject();
	CParticleMgr::Get_Instance()->Initialize(LEVEL_STAGE1);
	CGameMgr::Get_Instance()->Open_Level_Append_ObstaclePos(LEVEL_STAGE1, L"Layer_Hole", true);
	CGameMgr::Get_Instance()->Open_Level_Append_ObstaclePos(LEVEL_STAGE1, L"Layer_Wall", false);
	return S_OK;
}

void CHong::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (CGameMgr::Get_Instance()->Key_Down(DIK_LEFT))
	{
		m_vPosition.x -= m_fMoveSize;
	}
	if (CGameMgr::Get_Instance()->Key_Down(DIK_RIGHT))
	{
		m_vPosition.x += m_fMoveSize;
	}
	if (CGameMgr::Get_Instance()->Key_Down(DIK_UP))
	{
		m_vPosition.z += m_fMoveSize;
	}
	if (CGameMgr::Get_Instance()->Key_Down(DIK_DOWN))
	{
		m_vPosition.z -= m_fMoveSize;
	}
	if (CGameMgr::Get_Instance()->Get_Object_Data(L"Portal_NextLevel"))
	{
		//여기서 씬 넘겨줘야함
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device,
			LEVEL_GYUH))))
			MSG_BOX(L"레벨 오픈 실패");
		CGameMgr::Get_Instance()->m_bLoadFinish = false;
		Safe_Release(pGameInstance);
	}
	if (CGameMgr::Get_Instance()->Key_Down(DIK_F4))
	{
		//여기서 씬 넘겨줘야함
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device,
			LEVEL_GYUH))))
			MSG_BOX(L"레벨 오픈 실패");
		CGameMgr::Get_Instance()->m_bLoadFinish = false;
		Safe_Release(pGameInstance);
	}
}

HRESULT CHong::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	_float4x4 matWorld;
	D3DXMatrixIdentity(&matWorld);
	matWorld.m[0][0] = 0.1f;
	matWorld.m[1][1] = 0.1f;
	matWorld.m[2][2] = 0.1f;
	memcpy(&matWorld.m[3][0], &m_vPosition, sizeof(_float3));
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);

	m_pSphereMesh->DrawSubset(0);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	SetWindowText(g_hWnd, TEXT("홍준레벨임"));

	//ImGui::ShowDemoWindow();
	ImGui::Begin("SelectFolder");

	//큰 문제점. 서로의 쌍이 맞아야한다
	//틀리면 절대안댐
	static int item_current = 0;
	const char* Layers[] = {
		"Layer_topdee",
		"Layer_Toodee",
		"Layer_Cube",
		"Layer_Wall",
		"Layer_Spike",
		"Layer_Monster_Pig",
		"Layer_Monster_Turret",
		"Layer_Portal",
		"Layer_Cloud",
		"Layer_Hole",
		"Layer_KeyBox",
		"Layer_Key",
		"Layer_Cube",
		"Layer_ButtonBlock",
		"Layer_Button",
		"Thunder_Cloud",
		"Layer_Cube",
		"Layer_Cube",
		"Layer_Cube",
		"Layer_Cube"

	};
	const char* Prototypes[] =
	{
		"Prototype_GameObject_Topdee",
		"Prototype_GameObject_Toodee",
		"Prototype_GameObject_Cube",
		"Prototype_GameObject_Wall",
		"Prototype_GameObject_Spike",
		"Prototype_GameObject_Monster_Pig",
		"Prototype_GameObject_Turret",
		"Prototype_GameObject_Portal",
		"Prototype_GameObject_Cloud",
		"Prototype_GameObject_Hole",
		"Prototype_GameObject_KeyBox",
		"Prototype_GameObject_Key",
		"Prototype_GameObject_GravityBlock",
		"Prototype_GameObject_ButtonBlock",
		"Prototype_GameObject_Button",
		"Prototype_GameObject_Thunder_Cloud",
		"Prototype_GameObject_ElectricBlock",
		"Prototype_GameObject_WarpBlock",
		"Prototype_GameObject_MetalBlock",
		"Prototype_GameObject_BreakingBlock"
		
	};
	ImGui::ListBox("Prototypes", &item_current, Prototypes, IM_ARRAYSIZE(Prototypes), 6);
	const char* TexIndex[] =
	{
		"TEX_0","TEX_1","TEX_2","TEX_3","TEX_4","TEX_5",
		"TEX_6","TEX_7","TEX_8","TEX_9","TEX_10","TEX_11",
		"TEX_12","TEX_13","TEX_14","TEX_15","TEX_16","TEX_17"
		,"TEX_18","TEX_19","TEX_20","TEX_21","TEX_22","TEX_23"
		,"TEX_24","TEX_25","TEX_26","TEX_27","TEX_28","TEX_29"
		,"TEX_30","TEX_31","TEX_32","TEX_33","TEX_34","TEX_35"
		,"TEX_36","TEX_37","TEX_38","TEX_39","TEX_40"
	};
	if (ImGui::BeginListBox("Layers"))
	{
		for (int n = 0; n < IM_ARRAYSIZE(Prototypes); n++)
		{
			const bool is_selected = (item_current == n);
			if (ImGui::Selectable(Layers[n], is_selected))
			{
				item_current = n;
			}
			if (CGameMgr::Get_Instance()->Key_Down(DIK_SPACE))
			{
				//Layers
				int strSize = MultiByteToWideChar(CP_ACP, 0, Layers[item_current], -1, NULL, NULL);
				//wchar_t 메모리 할당
				const _tchar* szTemp = new WCHAR[strSize];
				//형 변환
				MultiByteToWideChar(CP_ACP, 0, Layers[item_current], (_uint)strlen(Layers[item_current]) + 1, (LPWSTR)szTemp, strSize);

				TagInfo* pTaginfo = new TagInfo;
				pTaginfo->pLayerTag = szTemp;

				Safe_Delete_Array(szTemp);

				//Prototypes
				strSize = MultiByteToWideChar(CP_ACP, 0, Prototypes[item_current], -1, NULL, NULL);
				szTemp = new WCHAR[strSize];
				
				MultiByteToWideChar(CP_ACP, 0, Prototypes[item_current], (_uint)strlen(Prototypes[item_current]) + 1, (LPWSTR)szTemp, strSize);
				pTaginfo->pPrototypeTag = szTemp;

				Safe_Delete_Array(szTemp);

				OBJ_INFO* tObjInfo = new OBJ_INFO;
				tObjInfo->vPos = m_vPosition;
				tObjInfo->iDirection = iDir_Select;
				tObjInfo->iNumLevel = iLevel_Select + 1;

				if (m_bIsCube)
				{
					tObjInfo->iTex = iTexNum;
				}
				else
				{
					tObjInfo->iTex = 0;
				}
				m_pObjects.insert({ pTaginfo, tObjInfo });

				OBJ_INFO tempObj;
				tempObj.vPos = tObjInfo->vPos;
				tempObj.iDirection = tObjInfo->iDirection;
				tempObj.iNumLevel = LEVEL_STAGE1;
				tempObj.iTex = iTexNum;
				//Test
				if (FAILED(Ready_Layer_Block(pTaginfo->pPrototypeTag.c_str(), pTaginfo->pLayerTag.c_str(), &tempObj)))
					MSG_BOX(L"생성 실패");
			}
		}
		ImGui::EndListBox();
	}

	if (ImGui::BeginListBox("Textures"))
	{
		for (int n = 0; n < IM_ARRAYSIZE(TexIndex); n++)
		{
			const bool is_selected = (iTexNum == n);
			if (ImGui::Selectable(TexIndex[n], is_selected))
			{
				iTexNum = n;
				//dynamic_cast<CWall*>(m_TempList->front())->SetTexture(iTexNum);
			}
		}
		ImGui::EndListBox();
	}
	// Using the _simplified_ one-liner Combo() api here
	// See "Combo" section for examples of how to use the more flexible BeginCombo()/EndCombo() api.
	const char* Directions[] = { "UP", "RIGHT", "DOWN", "LEFT" };
	ImGui::Combo("Direction", &iDir_Select, Directions, IM_ARRAYSIZE(Directions));
	
	const char* Levels[] = { "LEVEL_STAGE1", "LEVEL_STAGE2", "LEVEL_STAGE3", "LEVEL_STAGE4", "LEVEL_STAGE5"
							,"LEVEL_STAGE6", "LEVEL_STAGE7", "LEVEL_STAGE8", "LEVEL_STAGE9", "LEVEL_STAGE10" };
	ImGui::Combo("Level", &iLevel_Select, Levels, IM_ARRAYSIZE(Levels));
	
	ImGui::Checkbox("Wall?", &m_bIsCube); ImGui::SameLine();

	ImGui::Text("TextureIndex : %d", iTexNum);

	ImGui::DragFloat3("Position", m_vPosition, 0.1f, 0.0f, 200.f);
	ImGui::DragFloat("MoveSize", &m_fMoveSize, 0.01f, 0.0f, 200.f);
	
	if (ImGui::Button("Save")) SaveGameObject(); ImGui::SameLine();
	if (ImGui::Button("Load")) LoadGameObject();



	//if (ImGui::Button("Open"))
	//{
	//	TCHAR cpath[MAX_PATH] = L"";

	//	LPITEMIDLIST pDirList;
	//	BROWSEINFO browseInfo;
	//	browseInfo.hwndOwner = NULL;
	//	browseInfo.pidlRoot = NULL;
	//	browseInfo.lpszTitle = L"이미지들을 불러올 폴더를 선택해 주세요";
	//	browseInfo.pszDisplayName = cpath;
	//	browseInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	//	browseInfo.lpfn = NULL;
	//	browseInfo.lParam = 0;

	//	pDirList = SHBrowseForFolder(&browseInfo);
	//	if (pDirList != NULL)
	//	{
	//		BOOL bWorking = true;
	//		SHGetPathFromIDList(pDirList, cpath);
	//		TCHAR *return_path = cpath;


	//		/*FindFirstFile(cpath, &test);
	//		while (bWorking)
	//		{
	//			bWorking = FindNextFile();
	//		}*/
	//	}
	//	16
	//}31 18

	ImGui::End();

	return S_OK;
}

HRESULT CHong::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;

	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fFovy = D3DXToRadian(53.0f);
	CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Camera_Free"), LEVEL_HONG, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CHong::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	
	_uint iNumLevel{ 1 };
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"), LEVEL_HONG, pLayerTag, &iNumLevel)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CHong::Ready_Layer_Block(const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(pPrototypeTag, LEVEL_STAGE1, pLayerTag, pArg)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

CHong * CHong::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHong*		pInstance = new CHong(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CLevel_Hong"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHong::GetFiles(vector<_tchar*> &vList, _tchar* sPath, bool bAllDirectories)
{
	//_tchar* sTmp = sPath;// +string("\\*.*");
	//wcscat(sTmp, L"\\*.*");
	//WIN32_FIND_DATA fd;	
	//HANDLE hFind = FindFirstFile(sTmp, &fd);	
	//if (INVALID_HANDLE_VALUE != hFind)	
	//{		
	//	do		
	//	{			
	//		if ( fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)			
	//		{				
	//			if (bAllDirectories)				
	//			{					
	//				if (fd.cFileName[0] != '.')					
	//				{							
	//					sTmp = sPath + string("\\") + string(fd.cFileName);	
	//					GetFiles(vList, sTmp, bAllDirectories);					
	//				}				
	//			}			
	//		}			
	//		else			
	//		{				
	//			sTmp = sPath + string("\\") + string(fd.cFileName);				
	//			vList.push_back(sTmp);			
	//		}				
	//	} 
	//	while(FindNextFile(hFind, &fd));		
	//	FindClose(hFind);			
	//}	
}

void CHong::SaveGameObject()
{
	HANDLE		hFile = CreateFile(L"../Bin/Data/LEVEL_7.txt", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte = 0;
	DWORD	dwStrByte = 0;

	auto iter = m_pObjects.begin();

	while (iter != m_pObjects.end())
	{
		// Key값 저장
		dwStrByte = DWORD(sizeof(_tchar) * wcslen(iter->first->pPrototypeTag.c_str()));
		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, iter->first->pPrototypeTag.c_str(), dwStrByte, &dwByte, nullptr);
		// Key값 저장
		dwStrByte = DWORD(sizeof(_tchar) * wcslen(iter->first->pPrototypeTag.c_str()));
		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, iter->first->pLayerTag.c_str(), dwStrByte, &dwByte, nullptr);

		//Second값 저장
		WriteFile(hFile, iter->second->vPos, sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &iter->second->iNumLevel, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &iter->second->iDirection, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &iter->second->iTex, sizeof(_uint), &dwByte, nullptr);
		++iter;
	}
	CloseHandle(hFile);
}

void CHong::LoadGameObject()
{
	HANDLE hFile = CreateFile(L"../Bin/Data/LEVEL_5.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0; //String length

	//Clear Map
	for (auto& Pair : m_TestMap)
	{
		delete Pair.first.second;
		delete Pair.second;
	}
	m_TestMap.clear();

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
		OBJ_INFO tempObj;
		tempObj.vPos = iter->second->vPos;
		tempObj.iNumLevel = LEVEL_STAGE1;
		tempObj.iDirection = iter->second->iDirection;
		tempObj.iTex = iter->second->iTex;

		Ready_Layer_Block(iter->first->pPrototypeTag.c_str(), iter->first->pLayerTag.c_str(), &tempObj);
		++iter;
	}
	CloseHandle(hFile);
}

void CHong::Free()
{
	__super::Free();

	m_pSphereMesh->Release();

	for (auto Pair: m_pObjects)
	{
		Pair.first->pPrototypeTag.clear();
		Pair.first->pLayerTag.clear();
		delete Pair.first;
		delete Pair.second;
	}
	m_pObjects.clear();
	CParticleMgr::Get_Instance()->Destroy_Instance();
}
