#include "stdafx.h"
#include "..\Public\Hong.h"
#include "KeyMgr.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Level_Loading.h"
#include "GameObject.h"
#include <winnt.h>
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

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_Block(TEXT("Layer_Cube"))))
		return E_FAIL;*/

	//LoadGameObject();


	D3DXCreateSphere(m_pGraphic_Device, 3.0f, 30, 10, &m_pSphereMesh, NULL );   
	return S_OK;
}

void CHong::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
	{
		m_vPosition.x -= 1.f;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
	{
		m_vPosition.x += 1.f;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
	{
		m_vPosition.z += 1.f;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN))
	{
		m_vPosition.z -= 1.f;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		if (FAILED(Ready_Layer_Block(m_SelectPrototypes, m_SelectLayer, m_vPosition)))
			return;
		m_list.push_back(m_vPosition);

		m_TestMap.insert(make_pair(make_pair(m_SelectPrototypes, m_SelectLayer), m_vPosition));
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
		"Layer_Monster_Pig",
		"Layer_Monster_Turret",
		"Layer_Portal",
		"Layer_Cloud",
		"Layer_Hole",
		"Layer_KeyBox",
		"Layer_Key"
	};
	const char* Prototypes[] =
	{
		"Prototype_GameObject_Topdee",
		"Prototype_GameObject_Toodee",
		"Prototype_GameObject_Cube",
		"Prototype_GameObject_Monster_Pig",
		"Prototype_GameObject_Turret",
		"Prototype_GameObject_Portal",
		"Prototype_GameObject_Cloud",
		"Prototype_GameObject_Hole",
		"Prototype_GameObject_KeyBox",
		"Prototype_GameObject_Key"
	};
	ImGui::ListBox("Prototypes", &item_current, Prototypes, IM_ARRAYSIZE(Prototypes), 6);

	if (ImGui::BeginListBox("Layers"))
	{
		for (int n = 0; n < IM_ARRAYSIZE(Layers); n++)
		{
			const bool is_selected = (item_current == n);
			if (ImGui::Selectable(Layers[n], is_selected))
				item_current = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();

				//Layers
				int strSize = MultiByteToWideChar(CP_ACP, 0, Layers[item_current], -1, NULL, NULL);
				//wchar_t 메모리 할당
				const _tchar* szTemp = new WCHAR[strSize];
				//형 변환
				MultiByteToWideChar(CP_ACP, 0, Layers[item_current], strlen(Layers[item_current]) + 1, (LPWSTR)szTemp, strSize);
				m_SelectLayer = szTemp;
				
				//Prototypes
				strSize = MultiByteToWideChar(CP_ACP, 0, Prototypes[item_current], -1, NULL, NULL);
				szTemp = new WCHAR[strSize];
				MultiByteToWideChar(CP_ACP, 0, Prototypes[item_current], strlen(Prototypes[item_current]) + 1, (LPWSTR)szTemp, strSize);
				m_SelectPrototypes = szTemp;
			}
		}
		ImGui::EndListBox();
		if (ImGui::Button("Save")) SaveGameObject(); ImGui::SameLine();
		if (ImGui::Button("Load")) LoadGameObject();
	}


	//CreateMap();
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

HRESULT CHong::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, pLayerTag)))
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

HRESULT CHong::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (_uint i = 0; i < 3; ++i)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Monster"), LEVEL_HONG, pLayerTag)))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CHong::Ready_Layer_Block(const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(pPrototypeTag, LEVEL_HONG, pLayerTag, pArg)))
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
	HANDLE		hFile = CreateFile(L"../Bin/Data/temp.dat", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	
	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte = 0;
	DWORD	dwStrByte = 0;

	/*	Map<<Prototype, Layer>, vPosition> */
	
	auto iter = m_TestMap.begin();

	while (iter != m_TestMap.end())
	{
		// Key값 저장
		dwStrByte =  sizeof(_tchar) * wcslen(iter->first.first);//sizeof(iter->first.first);
		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, iter->first.first, dwStrByte, &dwByte, nullptr);
		// Key값 저장
		dwStrByte = sizeof(_tchar) * wcslen(iter->first.second);
		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, iter->first.second, dwStrByte, &dwByte, nullptr);

		// Second값 저장
		WriteFile(hFile, iter->second, sizeof(_float3), &dwByte, nullptr);
		++iter;
	}
	CloseHandle(hFile);
}

void CHong::LoadGameObject()
{
	HANDLE hFile = CreateFile(L"../Bin/Data/temp.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

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
	m_Test.clear();

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
			Safe_Delete_Array(pFirst);
			Safe_Delete_Array(pSecond);
			break;
		}
		m_TestMap.insert(make_pair(make_pair(pFirst, pSecond), vPos));
	}

	auto iter = m_TestMap.begin();

	while (iter != m_TestMap.end())
	{
		Ready_Layer_Block(iter->first.first, iter->first.second, iter->second);
		++iter;
	}
	CloseHandle(hFile);
}

void CHong::Free()
{
	__super::Free();
	CKeyMgr::Get_Instance()->Destroy_Instance();
	m_pSphereMesh->Release();
}
