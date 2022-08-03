#include "stdafx.h"
#include "..\Public\Hong.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Level_Loading.h"
#include "GameObject.h"
#include "GameMgr.h"
#include <winnt.h>
int CHong::iDir_Select = 0;
int CHong::iLevel_Select = 0;
CHong::CHong(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CHong::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	D3DXCreateSphere(m_pGraphic_Device, 3.0f, 30, 10, &m_pSphereMesh, NULL);
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

	ImGui::ShowDemoWindow();
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
		"Prototype_GameObject_GravityBlock"
	};
	ImGui::ListBox("Prototypes", &item_current, Prototypes, IM_ARRAYSIZE(Prototypes), 6);

	if (ImGui::BeginListBox("Layers"))
	{
		for (int n = 0; n < IM_ARRAYSIZE(Layers); n++)
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

				delete szTemp;

				//Prototypes
				strSize = MultiByteToWideChar(CP_ACP, 0, Prototypes[item_current], -1, NULL, NULL);
				szTemp = new WCHAR[strSize];
				
				MultiByteToWideChar(CP_ACP, 0, Prototypes[item_current], (_uint)strlen(Prototypes[item_current]) + 1, (LPWSTR)szTemp, strSize);
				pTaginfo->pPrototypeTag = szTemp;

				delete szTemp;

				if (FAILED(Ready_Layer_Block(pTaginfo->pPrototypeTag.c_str(), pTaginfo->pLayerTag.c_str(), m_vPosition)))
					return S_OK;

				OBJ_INFO* tObjInfo = new OBJ_INFO;
				tObjInfo->vPos = m_vPosition;
				tObjInfo->iDirection = iDir_Select;
				tObjInfo->iNumLevel = iLevel_Select + 1;

				m_pObjects.insert({ pTaginfo, tObjInfo });
			}
		}
		ImGui::EndListBox();
	}
	// Using the _simplified_ one-liner Combo() api here
	// See "Combo" section for examples of how to use the more flexible BeginCombo()/EndCombo() api.
	const char* Directions[] = { "UP", "RIGHT", "DOWN", "LEFT" };
	ImGui::Combo("Direction", &iDir_Select, Directions, IM_ARRAYSIZE(Directions));
	
	const char* Levels[] = { "LEVEL_STAGE1", "LEVEL_STAGE2", "LEVEL_STAGE3", "LEVEL_STAGE4" };
	ImGui::Combo("Level", &iLevel_Select, Levels, IM_ARRAYSIZE(Levels));

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
	//	
	//}

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

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"), LEVEL_HONG, pLayerTag)))
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
	HANDLE		hFile = CreateFile(L"../Bin/Data/LEVEL_TEST.txt", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

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
	HANDLE hFile = CreateFile(L"../Bin/Data/LEVEL_TEST.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

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
		//// Key 값 로드
		//ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		//_tchar*	pFirst = nullptr;
		//pFirst = new _tchar[dwStrByte];
		//ReadFile(hFile, pFirst, dwStrByte, &dwByte, nullptr);
		//pFirst[dwByte / sizeof(_tchar)] = 0;

		////Key값 로드
		//ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		//_tchar*	pSecond = nullptr;
		//pSecond = new _tchar[dwStrByte];
		//ReadFile(hFile, pSecond, dwStrByte, &dwByte, nullptr);
		//pSecond[dwByte / sizeof(_tchar)] = 0;

		//_float3 vPos = {};
		//ReadFile(hFile, vPos, sizeof(_float3), &dwByte, nullptr);

		//if (0 == dwByte)
		//{
		//	Safe_Delete_Array(pFirst);
		//	Safe_Delete_Array(pSecond);
		//	break;
		//}
		//m_TestMap.insert(make_pair(make_pair(pFirst, pSecond), vPos));

		//Safe_Delete_Array(pFirst);
		//Safe_Delete_Array(pSecond);

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
		Ready_Layer_Block(iter->first->pPrototypeTag.c_str(), iter->first->pLayerTag.c_str(), iter->second);
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
}
