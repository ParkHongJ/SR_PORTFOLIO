#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Monster_Pig.h"
#include "Camera_Free.h"
#include "BackGround.h"
#include "Terrain.h"
#include "Monster.h"
#include "Player.h"
#include "Topdee.h"
#include "Toodee.h"
#include "Portal.h"
#include "Turret.h"
#include "Bullet.h"
#include "Portal.h"
#include "Block.h"
#include "Spike.h"
#include "Cloud.h"
#include "Wall.h"
#include "Hole.h"
#include "Sky.h"
#include "Key.h"
#include "KeyBlock.h"


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
	case LEVEL_STAGE1:
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
		MSG_BOX(L"레벨이 없습니다");
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
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩중입니다. "));

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/*  객체원형 로드한다. */	

	/* For.Prototype_GameObject_BackGround */ 
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;	

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다. "));
	/* 텍스쳐를 로드한다. */

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Default"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Logo/Title.jpg")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다. "));	
	/* 모델을 로드한다. */


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다.  "));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype..."));
	
	/*----------------
	--프로토타입 생성--
	----------------*/

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"), 
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Toodee*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Toodee"), 
		CToodee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Pig"),
		CMonster_Pig::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"), 
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Topdee*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Topdee"),
		CTopdee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cube */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"),
		CBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Turret */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Turret"),
		CTurret::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet"),
		CBullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Hole */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cloud"),
		CCloud::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_GameObject_Key */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Wall */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall"),
		CWall::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KeyBox"),
		CKeyBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Spike */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Spike"),
		CSpike::Create(m_pGraphic_Device))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));

	/*------------------
	-----텍스쳐 생성-----
	------------------*/

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_0.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 73))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Monster_Pig"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MonsterPig/pigSpr_%d.png"), 9))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_ElectricBlock */ 기존 박스.
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_HONG, TEXT("Prototype_Component_Texture_ElectricBlock"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/DefaultWall_%d.dds"), 6))))
	//	return E_FAIL;

	/* For.Prototype_Component_Texture_ElectricBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_ElectricBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/NormalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Stage1_Wall */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Stage1_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/DefaultWall_%d.dds"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_KeyBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/LockedWall/LockedWall_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Turret */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Turret"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Turret_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Fire/firePelletSpr_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Portal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Portal/portalSpr_%d.png"), 11))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Topdee_PreLoader"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_PreLoader.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Hole */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Hole/Hole_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Cloud"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Cloud/cloudSpr_%d.png"), 6))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Key */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Key"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Key/keySpr_%d.png"), 12))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Spike */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Spike"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Spike/SpikesSpr_%d.png"), 6))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));

	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))//, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;
	
	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_HONG, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;*/

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"),
		CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("Success!"));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForHongLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/*----------------
	--프로토타입 생성--
	----------------*/

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Toodee*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Toodee"),
		CToodee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Pig"),
		CMonster_Pig::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Topdee*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Topdee"),
		CTopdee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cube */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"),
		CBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Turret */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Turret"),
		CTurret::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet"),
		CBullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Hole */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cloud"),
		CCloud::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_GameObject_Key */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Wall */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall"),
		CWall::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KeyBox"),
		CKeyBlock::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_GameObject_Spike */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Spike"),
		CSpike::Create(m_pGraphic_Device))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));

	/*------------------
	-----텍스쳐 생성-----
	------------------*/

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.tga"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 73))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Monster_Pig"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MonsterPig/pigSpr_%d.png"), 9))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_ElectricBlock */ 기존 박스.
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_HONG, TEXT("Prototype_Component_Texture_ElectricBlock"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/DefaultWall_%d.dds"), 6))))
	//	return E_FAIL;

	/* For.Prototype_Component_Texture_ElectricBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_ElectricBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/NormalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Stage1_Wall */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Stage1_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/DefaultWall_%d.dds"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_KeyBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/LockedWall/LockedWall_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Turret */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Turret"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Turret_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Fire/firePelletSpr_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Portal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Portal/portalSpr_%d.png"), 11))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Topdee_PreLoader"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_PreLoader.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Hole */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Hole/Hole_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Cloud"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Cloud/cloudSpr_%d.png"), 6))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Key */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Key"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Key/keySpr_%d.png"), 12))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Spike */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Spike"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Spike/SpikesSpr_%d.png"), 6))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));

	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))//, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_HONG, TEXT("Prototype_Component_VIBuffer_Cube"),
	CVIBuffer_Cube::Create(m_pGraphic_Device))))
	return E_FAIL;*/

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"),
		CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("Success!"));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForSENILevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype..."));

	/*-----------
	--Prototype--
	------------*/

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Pig"),
		CMonster_Pig::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_BreakingBlock */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BreakingBlock"),
	//	CBreakingBlock::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));
	
	/*-----------
	--Texture--
	------------*/

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.tga"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SENI, TEXT("Prototype_Component_Texture_Monster_Pig"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MonsterPig/pigSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_BreakingBlock */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_SENI, TEXT("Prototype_Component_Texture_BreakingBlock"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/BreakingBlock/breakingSpr_%d.png"), 2))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));
	
	/* Loading Model */
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("ˇSuccess!"));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForGyuHLevel()
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

	/* For.Prototype_GameObject_Cube */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"),
		CBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Wall */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall"),
		CWall::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Hole */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Hole */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Spike */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Spike"),
		CSpike::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	/* ??˝???¸? ˇ?????´?. */
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GYUH, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"),17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ElectricBlock */ 
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_HONG, TEXT("Prototype_Component_Texture_ElectricBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/NormalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Stage1_Wall */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_HONG, TEXT("Prototype_Component_Texture_Stage1_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/DefaultWall_%d.dds"),6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GYUH, TEXT("Prototype_Component_Texture_Topdee_PreLoader"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_PreLoader.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Hole */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Hole/Hole_%d.png"),2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Spike */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Spike"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Spike/SpikesSpr_%d.png"), 6))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("¸đ?¨?? ˇ????ß??´?´?. "));
	/* ¸đ?¨¸? ˇ?????´?. */
	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 32, 18))))
		return E_FAIL;*/

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_HONG, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_HONG, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"), CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Gyu Loading Complete."));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForSJHLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype..."));
	/* Loading Prototype */

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Toodee*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Toodee"),
		CToodee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cube */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"),
		CBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Spike */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Spike"),
		CSpike::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));
	/* Loading Texture */

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.tga"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ElectricBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_HONG, TEXT("Prototype_Component_Texture_ElectricBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/DefaultWall_%d.dds"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SJH, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 73))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Spike */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Spike"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Spike/SpikesSpr_%d.png"), 6))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));
	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))
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
