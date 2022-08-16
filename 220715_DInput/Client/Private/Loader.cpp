#include "stdafx.h"
#include "..\Public\Loader.h"

#include "ButtonBlock_Center.h"
#include "Particle_Button.h"
#include "Particle_Spark.h"
#include "BreakingBlock.h"
#include "Particle_Warp.h"
#include "Thunder_Cloud.h"
#include "ElectricBlock.h"
#include "GameInstance.h"
#include "GravityBlock.h"
#include "ButtonBlock.h"
#include "Monster_Pig.h"
#include "ParticleMgr.h"
#include "Camera_Free.h"
#include "BackGround.h"
#include "WarpBlock.h"
#include "Particle.h"
#include "KeyBlock.h"
#include "Terrain.h"
#include "Monster.h"
#include "Player.h"
#include "Topdee.h"
#include "Toodee.h"
#include "Tookee.h"
#include "Portal.h"
#include "Turret.h"
#include "Bullet.h"
#include "Button.h"
#include "Block.h"
#include "Spike.h"
#include "Cloud.h"
#include "Wall.h"
#include "Hole.h"
#include "Sky.h"
#include "Key.h"
#include "MetalBlock.h"
#include "Wave.h"

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
	case LEVEL_STAGE2:
		pLoader->Loading_ForLEVEL2(LEVEL_STAGE2);
		break;
	case LEVEL_STAGE3:
		pLoader->Loading_ForLEVEL3(LEVEL_STAGE3);
		break;
	case LEVEL_STAGE4:
		pLoader->Loading_ForLEVEL4(LEVEL_STAGE4);
		break;
	case LEVEL_STAGE5:
		pLoader->Loading_ForLEVEL5(LEVEL_STAGE5);
		break;
	case LEVEL_STAGE6:
		pLoader->Loading_ForLEVEL6(LEVEL_STAGE6);
		break;
	case LEVEL_STAGE7:
		pLoader->Loading_ForLEVEL7(LEVEL_STAGE7);
		break;
	case LEVEL_STAGE8:
		pLoader->Loading_ForLEVEL8(LEVEL_STAGE8);
		break;
	case LEVEL_HONG:
		pLoader->Loading_ForHongLevel();
		break;
	case LEVEL_GYUH:
		pLoader->Loading_ForGyuHLevel();
		break;
	case LEVEL_GYUTEST:
		pLoader->Loading_ForGyuTestLevel();
		break;
	case LEVEL_SENI:
		pLoader->Loading_ForSENILevel(LEVEL_SENI);
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

	/* For.Prototype_GameObject_Wave*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wave"),
		CWave::Create(m_pGraphic_Device))))
		return E_FAIL;

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

	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
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

	/* For.Prototype_GameObject_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GravityBlock"),
		CGravityBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		CParticle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Button"),
		CParticle_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tookee */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tookee"),
		CTookee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button"),
		CButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ButtonBlock"),
		CButtonBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ElectricBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ElectricBlock"),
		CElectricBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_WarpBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WarpBlock"),
		CWarpBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));

	/*------------------
	-----텍스쳐 생성-----
	------------------*/
	
	/* For.Prototype_Component_Texture_Wave */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Wave"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Wave/Default%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"), 22))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_0.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 38))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee_Died */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Toodee_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeDiedSpr/toodeeDiedSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Portal_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSwirlSpr/Toodee_Swirl_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Monster_Pig"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MonsterPig/pigSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_NormalBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_NormalBlock"),
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

	/* For.Prototype_Component_Texture_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_GravityBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/gravBlockSpr/GravlBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Portal_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Portal/Topdee_Portal_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/buttonWallPartSpr_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Sky.dds")))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Button */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Button/buttonSpr_0%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_ButtonBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Button/ButtonBox_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_BreakingBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_BreakingBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Breaking/BreakingSpr_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Tookee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Tookee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Tookee/Tookee_%d.png"), 42))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ElectricBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_ElectricBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/ElectricBlock/Electric_Block_%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_WarpBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_WarpBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Warp_Block/Warp_Block_%d.dds"), 4))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));

	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))//, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

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
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype..."));

	/*----------------
	--프로토타입 생성--
	----------------*/
	/* For.Prototype_GameObject_BreakingBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BreakingBlock"),
		CBreakingBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
		return E_FAIL;

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

	/* For.Prototype_GameObject_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GravityBlock"),
		CGravityBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		CParticle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button"),
		CButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ButtonBlock"),
		CButtonBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tookee */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tookee"),
		CTookee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_WarpBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WarpBlock"),
		CWarpBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ElectricBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ElectricBlock"),
		CElectricBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Spark"),
		CParticle_Spark::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Thunder_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Thunder_Cloud"),
		CThunder_Cloud::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_MetalBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MetalBlock"),
		CMetalBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Button"),
		CParticle_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));

	/*------------------
	-----텍스쳐 생성-----
	------------------*/
	
	/* For.Prototype_Component_Texture_WarpBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_WarpBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Warp_Block/Warp_Block_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Rain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Rain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Rain_Thunder/Rain_Thunder_%d.png"), 13))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"), 22))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_3.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 38))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Toodee_Died */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Toodee_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeDiedSpr/toodeeDiedSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Portal_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSwirlSpr/Toodee_Swirl_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Monster_Pig"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MonsterPig/pigSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_NormalBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_NormalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/NormalBox.dds")))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_Stage1_Wall */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Stage1_Wall"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/Stage2Wall/Stage2_Wall_%d.dds"), 26))))
	//	return E_FAIL;

	/* For.Prototype_Component_Texture_Stage1_Wall */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Stage1_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/Stage5Wall/Stage5_Wall_%d.dds"), 20))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_KeyBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/LockedWall/LockedWall_%d.dds"), 1))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_BreakingBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_BreakingBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Breaking/BreakingSpr_0.dds")))))
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
	/* For.Prototype_Component_Texture_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_GravityBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/gravBlockSpr/GravlBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Portal_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Portal/Topdee_Portal_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Button */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Button/buttonSpr_0%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_ButtonBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Button/ButtonBox_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Tookee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Tookee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Tookee/Tookee_%d.png"), 42))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Thunder_Cloud"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Cloud/Thunder_Cloud/Thunder_Cloud_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Thunder_Cloud_Shadow */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Thunder_Cloud_Shadow"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Cloud/Thunder_Cloud/Thunder_Cloud_Shadow/Thunder_Cloud_Shadow_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle_Spark"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle_Spark.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ElectricBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_ElectricBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/ElectricBlock/Electric_Block_%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MetalBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_MetalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/MetalBlock/MetalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/buttonWallPartSpr_%d.png"), 4))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));

	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))//, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"),
		CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("Success!"));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForSENILevel(_uint iNumLevel)
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

	/* For.Prototype_GameObject_Toodee*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Toodee"),
		CToodee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Topdee*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Topdee"),
		CTopdee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button"),
		CButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ButtonBlock"),
		CButtonBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ButtonBlock_Center */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ButtonBlock_Center"),
		CButtonBlock_Center::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cube */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"),
		CBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		CParticle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Button"),
		CParticle_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet"),
		CBullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_BreakingBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BreakingBlock"),
		CBreakingBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));
	
	/*-----------
	--Texture--
	------------*/

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_0.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"), 22))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 38))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee_Died */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeDiedSpr/toodeeDiedSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee_PreLoader"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_PreLoader.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Monster_Pig"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MonsterPig/pigSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Button/buttonSpr_0%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_ButtonBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Button/ButtonBox_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ButtonBlock_Center */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_ButtonBlock_Center"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Button/ButtonBox_Center_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/buttonWallPartSpr_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Fire/firePelletSpr_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_WarpBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_WarpBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Warp_Block/Warp_Block_%d.dds"), 4))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_BreakingBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_BreakingBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Breaking/BreakingSpr_%d.dds"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));
	
	/* Loading Model */
	
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Success!"));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForGyuHLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;
	/* For.Prototype_GameObject_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GravityBlock"),
		CGravityBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button"),
		CButton::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_GameObject_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_GameObject_Key */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_GameObject_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KeyBox"),
		CKeyBlock::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_GameObject_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ButtonBlock"),
		CButtonBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Button"),
		CParticle_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Topdee*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Topdee"),
		CTopdee::Create(m_pGraphic_Device))))
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

	/* For.Prototype_GameObject_WarpBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WarpBlock"),
		CWarpBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ElectricBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ElectricBlock"),
		CElectricBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Wall */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall"),
		CWall::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Hole */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Spike */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Spike"),
		CSpike::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet"),
		CBullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		CParticle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Spark"),
		CParticle_Spark::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Thunder_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Thunder_Cloud"),
		CThunder_Cloud::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Button"),
		CParticle_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Texture */

	/* For.Prototype_Component_Texture_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/buttonWallPartSpr_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Portal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Portal/portalSpr_%d.png"), 11))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_GravityBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/gravBlockSpr/GravlBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_1.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"),22))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 38))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_NormalBlock */ 
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_NormalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/NormalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Stage1_Wall */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Stage1_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/DefaultWall_%d.dds"),6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Topdee_PreLoader"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_PreLoader.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Hole */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Hole/Hole_%d.png"),2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Spike */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Spike"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Spike/SpikesSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_WarpBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_WarpBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Warp_Block/Warp_Block_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ElectricBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_ElectricBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/ElectricBlock/Electric_Block_%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Fire/firePelletSpr_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Thunder_Cloud"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Cloud/Thunder_Cloud/Thunder_Cloud_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Thunder_Cloud_Shadow */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Thunder_Cloud_Shadow"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Cloud/Thunder_Cloud/Thunder_Cloud_Shadow/Thunder_Cloud_Shadow_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle_Spark"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle_Spark.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee_Died */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Toodee_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeDiedSpr/toodeeDiedSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Rain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Rain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Rain_Thunder/Rain_Thunder_%d.png"), 13))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_KeyBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/LockedWall/LockedWall_%d.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Key */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Key"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Key/keySpr_%d.png"), 12))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/buttonWallPartSpr_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Button */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Button/buttonSpr_0%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_ButtonBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Button/ButtonBox_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Portal_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Portal/Topdee_Portal_%d.png"), 17))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Portal_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Portal_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSwirlSpr/Toodee_Swirl_%d.png"), 17))))
		return E_FAIL;

	//================================ TextureEnd
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"), CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Gyu Loading Complete."));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForGyuTestLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype..."));

	/*----------------
	--프로토타입 생성--
	----------------*/
	/* For.Prototype_GameObject_BreakingBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BreakingBlock"),
		CBreakingBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
		return E_FAIL;

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

	/* For.Prototype_GameObject_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GravityBlock"),
		CGravityBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		CParticle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button"),
		CButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ButtonBlock"),
		CButtonBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tookee */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tookee"),
		CTookee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_WarpBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WarpBlock"),
		CWarpBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ElectricBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ElectricBlock"),
		CElectricBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Spark"),
		CParticle_Spark::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Thunder_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Thunder_Cloud"),
		CThunder_Cloud::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_MetalBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MetalBlock"),
		CMetalBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Button"),
		CParticle_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));

	/*------------------
	-----텍스쳐 생성-----
	------------------*/

	/* For.Prototype_Component_Texture_WarpBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_WarpBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Warp_Block/Warp_Block_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Rain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Rain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Rain_Thunder/Rain_Thunder_%d.png"), 13))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"), 22))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_4.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 38))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Toodee_Died */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Toodee_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeDiedSpr/toodeeDiedSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Portal_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSwirlSpr/Toodee_Swirl_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Monster_Pig"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MonsterPig/pigSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_NormalBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_NormalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/NormalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Stage1_Wall */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Stage1_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/Stage2Wall/Stage2_Wall_%d.dds"), 26))))
		return E_FAIL;

	
	/* For.Prototype_Component_Texture_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_KeyBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/LockedWall/LockedWall_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_BreakingBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_BreakingBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Breaking/BreakingSpr_0.dds")))))
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
	/* For.Prototype_Component_Texture_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_GravityBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/gravBlockSpr/GravlBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Portal_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Portal/Topdee_Portal_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Button */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Button/buttonSpr_0%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_ButtonBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Button/ButtonBox_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Tookee */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Tookee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Tookee/Tookee_%d.png"), 42))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Thunder_Cloud"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Cloud/Thunder_Cloud/Thunder_Cloud_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Thunder_Cloud_Shadow */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Thunder_Cloud_Shadow"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Cloud/Thunder_Cloud/Thunder_Cloud_Shadow/Thunder_Cloud_Shadow_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle_Spark"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle_Spark.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ElectricBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_ElectricBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/ElectricBlock/Electric_Block_%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MetalBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_MetalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/MetalBlock/MetalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Particle_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/buttonWallPartSpr_%d.png"), 4))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));

	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))//, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"),
		CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("Success!"));

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

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));
	/* Loading Texture */

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_0.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Sky.dds")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));
	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SJH, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SJH, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"),
		CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Success!"));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForLEVEL2(_uint iNumLevel)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype..."));

	/*----------------
	--프로토타입 생성--
	----------------*/
	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;
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

	/* For.Prototype_GameObject_Topdee*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Topdee"),
		CTopdee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cube */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"),
		CBlock::Create(m_pGraphic_Device))))
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

	/* For.Prototype_GameObject_Wall */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall"),
		CWall::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		CParticle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Button"),
		CParticle_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button"),
		CButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ButtonBlock"),
		CButtonBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet"),
		CBullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Pig"),
		CMonster_Pig::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Spike */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Spike"),
		CSpike::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Key */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KeyBox"),
		CKeyBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));

	/*------------------
	-----텍스쳐 생성-----
	------------------*/

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"), 22))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_1.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 38))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee_Died */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeDiedSpr/toodeeDiedSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSwirlSpr/Toodee_Swirl_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_NormalBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_NormalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/NormalBox.dds")))))
		return E_FAIL;

	/* 이거 수정해라 */
	/* For.Prototype_Component_Texture_Stage1_Wall */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Stage1_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/Stage2Wall/Stage2_Wall_%d.dds"), 26))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Button/buttonSpr_0%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_ButtonBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Button/ButtonBox_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Portal/portalSpr_%d.png"), 11))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee_PreLoader"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_PreLoader.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Hole */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Hole/Hole_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Monster_Pig"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MonsterPig/pigSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Cloud"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Cloud/cloudSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Portal/Topdee_Portal_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/buttonWallPartSpr_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Fire/firePelletSpr_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Key */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Key"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Key/keySpr_%d.png"), 12))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Spike */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Spike"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Spike/SpikesSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_KeyBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/LockedWall/LockedWall_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Sky.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));

	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))//, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"),
		CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("Success!"));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForLEVEL3(_uint iNumLevel)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype..."));

	/*----------------
	--프로토타입 생성--
	----------------*/
	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;
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

	/* For.Prototype_GameObject_Topdee*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Topdee"),
		CTopdee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Hole */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Wall */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall"),
		CWall::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		CParticle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Button"),
		CParticle_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet"),
		CBullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GravityBlock"),
		CGravityBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));

	/*------------------
	-----텍스쳐 생성-----
	------------------*/

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"), 22))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_0.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 38))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee_Died */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeDiedSpr/toodeeDiedSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSwirlSpr/Toodee_Swirl_%d.png"), 17))))
		return E_FAIL;

	/* 이거 수정해라 */
	/* For.Prototype_Component_Texture_Stage1_Wall */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Stage1_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/DefaultWall_%d.dds"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Portal/portalSpr_%d.png"), 11))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee_PreLoader"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_PreLoader.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Hole */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Hole/Hole_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Portal/Topdee_Portal_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Fire/firePelletSpr_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/buttonWallPartSpr_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Sky.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_GravityBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/gravBlockSpr/GravlBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));

	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))//, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"),
		CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("Success!"));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForLEVEL4(_uint iNumLevel)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype..."));

	/*----------------
	--프로토타입 생성--
	----------------*/

	/* For.Prototype_GameObject_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Spark"),
		CParticle_Spark::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_MetalBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MetalBlock"),
		CMetalBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GravityBlock"),
		CGravityBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Turret */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Turret"),
		CTurret::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
		return E_FAIL; 

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

	/* For.Prototype_GameObject_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Hole */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
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

	/* For.Prototype_GameObject_Particle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		CParticle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Button"),
		CParticle_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_GameObject_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet"),
		CBullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button"),
		CButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ButtonBlock"),
		CButtonBlock::Create(m_pGraphic_Device))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));

	/*------------------
	-----텍스쳐 생성-----
	------------------*/

	/* For.Prototype_Component_Texture_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Spark"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle_Spark.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MetalBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_MetalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/MetalBlock/MetalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"), 22))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_2.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 38))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee_Died */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeDiedSpr/toodeeDiedSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSwirlSpr/Toodee_Swirl_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Monster_Pig"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MonsterPig/pigSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_NormalBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_NormalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/NormalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Stage4_Wall */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Stage1_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/theme3Wall/Theme3Wall_%d.dds"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_KeyBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/LockedWall/LockedWall_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Turret */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Turret"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Turret_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Fire/firePelletSpr_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/buttonWallPartSpr_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Portal/portalSpr_%d.png"), 11))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee_PreLoader"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_PreLoader.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Hole */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Hole/Hole_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Cloud"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Cloud/cloudSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Key */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Key"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Key/keySpr_%d.png"), 12))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Spike */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Spike"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Spike/SpikesSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_GravityBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/gravBlockSpr/GravlBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Portal/Topdee_Portal_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Sky.dds")))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Button/buttonSpr_0%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_ButtonBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Button/ButtonBox_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));

	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))//, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"),
		CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("Success!"));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForLEVEL5(_uint iNumLevel)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype..."));

	/*----------------
	--프로토타입 생성--
	----------------*/
	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;
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

	/* For.Prototype_GameObject_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Hole */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
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

	/* For.Prototype_GameObject_Particle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		CParticle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Button"),
		CParticle_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_GameObject_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet"),
		CBullet::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_GameObject_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button"),
		CButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ButtonBlock"),
		CButtonBlock::Create(m_pGraphic_Device))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));

	/*------------------
	-----텍스쳐 생성-----
	------------------*/

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"), 22))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_1.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 38))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee_Died */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeDiedSpr/toodeeDiedSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSwirlSpr/Toodee_Swirl_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Monster_Pig"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MonsterPig/pigSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_NormalBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_NormalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/NormalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Stage4_Wall */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Stage1_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/Stage2WallTemp/Stage2_Wall_0.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_KeyBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/LockedWall/LockedWall_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Turret */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Turret"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Turret_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Fire/firePelletSpr_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Portal/portalSpr_%d.png"), 11))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee_PreLoader"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_PreLoader.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Hole */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Hole/Hole_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Cloud"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Cloud/cloudSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Key */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Key"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Key/keySpr_%d.png"), 12))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Spike */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Spike"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Spike/SpikesSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_GravityBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/gravBlockSpr/GravlBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Portal/Topdee_Portal_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/buttonWallPartSpr_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Sky.dds")))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Button/buttonSpr_0%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_ButtonBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Button/ButtonBox_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));

	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))//, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"),
		CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("Success!"));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForLEVEL6(_uint iNumLevel)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype..."));

	/*----------------
	--프로토타입 생성--
	----------------*/
	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_WarpBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WarpBlock"),
		CWarpBlock::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_GameObject_Turret */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Turret"),
		CTurret::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Turret */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Turret"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Turret_%d.png"), 4))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_WarpBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_WarpBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Warp_Block/Warp_Block_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tookee */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tookee"),
		CTookee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

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

	/* For.Prototype_GameObject_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GravityBlock"),
		CGravityBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		CParticle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button"),
		CButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ButtonBlock"),
		CButtonBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Spark"),
		CParticle_Spark::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_MetalBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MetalBlock"),
		CMetalBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Button"),
		CParticle_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Spark"),
		CParticle_Spark::Create(m_pGraphic_Device))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));

	/*------------------
	-----텍스쳐 생성-----
	------------------*/

	/* For.Prototype_Component_Texture_Tookee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Tookee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Tookee/Tookee_%d.png"), 42))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"), 22))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_4.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 38))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee_Died */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeDiedSpr/toodeeDiedSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSwirlSpr/Toodee_Swirl_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_NormalBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_NormalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/NormalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Stage1_Wall */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Stage1_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/Stage4Wall/theme4WallSpr_%d.dds"), 11))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_KeyBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/LockedWall/LockedWall_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Turret */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Turret"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Turret_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Fire/firePelletSpr_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Portal/portalSpr_%d.png"), 11))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee_PreLoader"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_PreLoader.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Hole */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Hole/Hole_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Key */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Key"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Key/keySpr_%d.png"), 12))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_GravityBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/gravBlockSpr/GravlBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Portal/Topdee_Portal_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Button/buttonSpr_0%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_ButtonBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Button/ButtonBox_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Spark"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle_Spark.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MetalBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_MetalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/MetalBlock/MetalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/buttonWallPartSpr_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Sky.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));

	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))//, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"),
		CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("Success!"));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForLEVEL7(_uint iNumLevel)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype..."));

	/*----------------
	--프로토타입 생성--
	----------------*/
	/* For.Prototype_GameObject_WarpBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WarpBlock"),
		CWarpBlock::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_WarpBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_WarpBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Warp_Block/Warp_Block_%d.dds"), 4))))
		return E_FAIL;
	/* For.Prototype_GameObject_BreakingBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BreakingBlock"),
		CBreakingBlock::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_BreakingBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_BreakingBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Breaking/BreakingSpr_%d.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;
	/* For.Prototype_GameObject_Tookee */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tookee"),
		CTookee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

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

	/* For.Prototype_GameObject_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GravityBlock"),
		CGravityBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		CParticle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button"),
		CButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ButtonBlock"),
		CButtonBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Spark"),
		CParticle_Spark::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_MetalBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MetalBlock"),
		CMetalBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Button"),
		CParticle_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Spark"),
		CParticle_Spark::Create(m_pGraphic_Device))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));

	/*------------------
	-----텍스쳐 생성-----
	------------------*/

	/* For.Prototype_Component_Texture_Tookee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Tookee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Tookee/Tookee_%d.png"), 42))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"), 22))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_4.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 38))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Toodee_Died */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeDiedSpr/toodeeDiedSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSwirlSpr/Toodee_Swirl_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_NormalBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_NormalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/NormalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Stage1_Wall */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Stage1_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/Stage4Wall/theme4WallSpr_%d.dds"), 11))))
		return E_FAIL;
	 
	/* For.Prototype_Component_Texture_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_KeyBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/LockedWall/LockedWall_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Turret */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Turret"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Turret_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Fire/firePelletSpr_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Portal/portalSpr_%d.png"), 11))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee_PreLoader"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_PreLoader.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Hole */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Hole/Hole_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Key */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Key"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Key/keySpr_%d.png"), 12))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_GravityBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_GravityBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/gravBlockSpr/GravlBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Portal/Topdee_Portal_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Button/buttonSpr_0%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_ButtonBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Button/ButtonBox_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Spark */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Spark"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle_Spark.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MetalBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_MetalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/MetalBlock/MetalBox.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/buttonWallPartSpr_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Sky.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));

	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))//, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"),
		CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("Success!"));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForLEVEL8(_uint iNumLevel)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype..."));

	/*----------------
	--프로토타입 생성--
	----------------*/
	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;
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

	/* For.Prototype_GameObject_Topdee*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Topdee"),
		CTopdee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cube */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"),
		CBlock::Create(m_pGraphic_Device))))
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

	/* For.Prototype_GameObject_Wall */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall"),
		CWall::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		CParticle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Button"),
		CParticle_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button"),
		CButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ButtonBlock"),
		CButtonBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet"),
		CBullet::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Pig"),
		CMonster_Pig::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Spike */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Spike"),
		CSpike::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Key */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KeyBox"),
		CKeyBlock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Warp"),
		CParticle_Warp::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Texture..."));

	/*------------------
	-----텍스쳐 생성-----
	------------------*/

	/* For.Prototype_Component_Texture_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Spr/Topdee_Spr_%d.png"), 22))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/GamePlay/BackGround_GamePlay_3.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSpr/toodeeSpr_%d.png"), 38))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toodee_Died */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Toodee_Died"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeDiedSpr/toodeeDiedSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Toodee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Toodee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Toodee/toodeeSwirlSpr/Toodee_Swirl_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_NormalBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_NormalBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/NormalBox.dds")))))
		return E_FAIL;

	/* 이거 수정해라 */
	/* For.Prototype_Component_Texture_Stage1_Wall */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Stage1_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Wall/Stage5Wall/Stage5_Wall_%d.dds"), 20))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Button/buttonSpr_0%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ButtonBlock */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_ButtonBlock"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Button/ButtonBox_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Portal/portalSpr_%d.png"), 11))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Topdee_PreLoader */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Topdee_PreLoader"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_PreLoader.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Hole */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Hole"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Hole/Hole_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster_Pig */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Monster_Pig"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MonsterPig/pigSpr_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Cloud"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Cloud/cloudSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Portal_Topdee */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Portal_Topdee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/TopdeeTexture/Topdee_Portal/Topdee_Portal_%d.png"), 17))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/Particle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Button */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/buttonWallPartSpr_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Turret/Fire/firePelletSpr_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Key */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Key"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Key/keySpr_%d.png"), 12))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Spike */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Spike"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Spike/SpikesSpr_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_KeyBox */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_KeyBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/LockedWall/LockedWall_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Sky.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Warp */
	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_Texture_Particle_Warp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Particle/BlockExpSpr/BlockExpSpr_%d.png"), 9))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Model..."));

	/* Loading Model */

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 30, 16))))//, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(iNumLevel, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"),
		CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
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


