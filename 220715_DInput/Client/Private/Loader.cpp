#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "BackGround.h"
#include "Terrain.h"
#include "Monster.h"
#include "Player.h"
#include "Topdee.h"
#include "MyBox.h"
#include "Monster_Pig.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlayLevel();
		break;
	case LEVEL_HONG:
		pLoader->Loading_ForHongLevel();
		break;
	case LEVEL_GYUH:
		pLoader->Loading_ForGyuHLevel();
		break;
	case LEVEL_SENI:
		pLoader->Loading_ForSENILevel();
		break;
	case LEVEL_SJH:
		pLoader->Loading_ForSJHLevel();
		break;
		
	default:
		MSG_BOX(L"������ �����ϴ�");
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CS());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);

	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	lstrcpy(m_szLoadingText, TEXT("��ü������ �ε����Դϴ�. "));

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* ������ü���� �ε��Ѵ�. */	

	/* For.Prototype_GameObject_BackGround */ 
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;	

	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε����Դϴ�. "));
	/* �ؽ��ĸ� �ε��Ѵ�. */

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Default"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("���� �ε����Դϴ�. "));	
	/* �𵨸� �ε��Ѵ�. */


	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����Ϥ���.  "));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("��ü������ �ε����Դϴ�. "));

	/* ������ü���� �ε��Ѵ�. */
	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"), 
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε����Դϴ�. "));
	/* �ؽ��ĸ� �ε��Ѵ�. */

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.tga"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Player */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Player.png")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("���� �ε����Դϴ�. "));
	/* �𵨸� �ε��Ѵ�. */

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 100, 100))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����Ϥ���.  "));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT Client::CLoader::Loading_ForHongLevel()
{
	return S_OK;
}
HRESULT CLoader::Loading_ForSENILevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("��ü������ �ε����Դϴ�. "));

	/* ������ü���� �ε��Ѵ�. */
	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Pig"),
		CMonster_Pig::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε����Դϴ�. "));
	/* �ؽ��ĸ� �ε��Ѵ�. */
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SENI, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.tga"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SENI, TEXT("Prototype_Component_Texture_Monster_Pig"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/pigSpr_7.png")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("���� �ε����Դϴ�. "));
	/* �𵨸� �ε��Ѵ�. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SENI, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 100, 100))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����Ϥ���.  "));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT Client::CLoader::Loading_ForGyuHLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Topdee*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Topdee"),
		CTopdee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* �ؽ��ĸ� �ε��Ѵ�. */
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.tga"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GYUH, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"),13))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("���� �ε����Դϴ�. "));
	/* �𵨸� �ε��Ѵ�. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 100, 100))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Gyu Loading Complete."));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForSJHLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype..."));
	/* Loading Prototype */

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"), CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"), CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));
	/* Loading Texture */

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.tga"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Player */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Player.png")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));
	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"), CVIBuffer_Terrain::Create(m_pGraphic_Device, 50, 50))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Success!"));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLoader*		pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed To Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CriticalSection);

	CloseHandle(m_hThread);

	Safe_Release(m_pGraphic_Device);
}
