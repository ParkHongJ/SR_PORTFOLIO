#include "stdafx.h"
#include "..\Public\Hong.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Level_Loading.h"
#include "GameObject.h"
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

	if (FAILED(Ready_Layer_Block(TEXT("Layer_Cube"))))
		return E_FAIL;


	return S_OK;
}

void CHong::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	/*if (GetKeyState(VK_UP) & 0x8000)
	{
		m_vPosition.z += 1.f;
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_vPosition.z -= 1.f;
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_vPosition.x -= 1.f;
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_vPosition.x += 1.f;
	}
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		Ready_Layer_Block(L"Layer_Cube");
	}*/
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	

	if ((pGameInstance->Get_DIKState(DIK_RIGHT) & 0x00))
	{
		m_vPosition.x += 1.f;
	}
	Safe_Release(pGameInstance);

}

HRESULT CHong::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("홍준레벨임"));
	_float4x4 ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	_float4x4		CamWorldMatrix;
	D3DXMatrixInverse(&CamWorldMatrix, nullptr, &ViewMatrix);
	_float3 vRight = *(_float3*)&CamWorldMatrix.m[0][0];
	_float3 vUp = *(_float3*)&CamWorldMatrix.m[1][0];
	_float3 vLook = *(_float3*)&CamWorldMatrix.m[2][0];
	_float3 vPos = *(_float3*)&CamWorldMatrix.m[3][0];
	ImGui::Begin("CameraController");

	ImGui::DragFloat3("vRight", vRight, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat3("vUp", vUp, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat3("vLook", vLook, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat3("vPos", vPos, 0.01f, -100.0f, 100.0f);
	ImGui::End();

	ImGui::Begin("Test");
	ImGui::DragFloat3("vPos", m_vPosition, 1.f, -100.0f, 100.0f);
	if (ImGui::Button("+X"))
	{
		m_vPosition.x += 1.f;
	}

	ImGui::End();

	ImGui::Begin("SelectFolder");


	ImGui::Text("Blah");
	const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
	static int item_current = 1;
	ImGui::ListBox("listbox", &item_current, items, IM_ARRAYSIZE(items), 4);

	if (ImGui::Button("Open"))
	{
		TCHAR cpath[MAX_PATH] = L"";

		LPITEMIDLIST pDirList;
		BROWSEINFO browseInfo;
		browseInfo.hwndOwner = NULL;
		browseInfo.pidlRoot = NULL;
		browseInfo.lpszTitle = L"이미지들을 불러올 폴더를 선택해 주세요";
		browseInfo.pszDisplayName = cpath;
		browseInfo.ulFlags = BIF_RETURNONLYFSDIRS;
		browseInfo.lpfn = NULL;
		browseInfo.lParam = 0;

		pDirList = SHBrowseForFolder(&browseInfo);
		if (pDirList != NULL)
		{
			BOOL bWorking = true;
			SHGetPathFromIDList(pDirList, cpath);
			TCHAR *return_path = cpath;


			/*FindFirstFile(cpath, &test);
			while (bWorking)
			{
				bWorking = FindNextFile();
			}*/
		}
		
	}

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
	CameraDesc.fFovy = D3DXToRadian(60.0f);
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

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Player"), LEVEL_HONG, pLayerTag)))
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


HRESULT CHong::Ready_Layer_Block(const _tchar* pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Cube"), LEVEL_HONG, pLayerTag, m_vPosition)))
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

void CHong::Free()
{
	__super::Free();
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

	HANDLE		hFile = CreateFile(경로, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return;



	//vector<TILE*>& vecTile = pTerrain->Get_VecTile();

	DWORD	dwByte = 0;

	/*typedef struct tagTile
	{
		D3DXVECTOR3	vPos;
		D3DXVECTOR3 vSize;

		BYTE		byOption;
		BYTE		byDrawID;

		int			iIndex;
		int			iParentIndex;

	}TILE;*/

	for (auto& iter : vecTile)
	{
		//WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);
	}

	CloseHandle(hFile);
}

