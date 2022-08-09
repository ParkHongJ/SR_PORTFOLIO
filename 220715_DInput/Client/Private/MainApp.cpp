#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "GameMgr.h"
#include "Loading_BackGround.h"

using namespace Client;

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	GRAPHICDESC GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWinMode = GRAPHICDESC::MODE_WIN;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, g_hInst, GraphicDesc, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(Ready_Default_SamplerState()))
		return E_FAIL;

	if (FAILED(Ready_Default_RenderState()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_LoadingLevel()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	if (FAILED(CGameMgr::Get_Instance()->Initialize()))
		return E_FAIL;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(m_pGraphic_Device);

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return ;

#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
#endif // _DEBUG
	m_pGameInstance->Tick_Engine(fTimeDelta);

	CGameMgr::Get_Instance()->Tick(fTimeDelta);

	_bool bTopdeeTurn{ false };
	if (CGameMgr::GAMEMODE::TOPDEE == CGameMgr::Get_Instance()->GetMode())
		bTopdeeTurn = true;
	else
		bTopdeeTurn = false;
	if (!m_pCollider->Collision_Ray_Top(CCollider::INTEREACTION, bTopdeeTurn))	//만약 블록에서 레이를 검출당하지 않았다면 플레이어도 체크
		m_pCollider->Collision_Ray_Top(CCollider::PLAYER, bTopdeeTurn);


	//구충돌로 비교하면서 Stay 호출
	//사각충돌 비교하면서 Stay 방향 호출
	m_pCollider->Collision_RectEx(CCollider::PLAYER, CCollider::BLOCK, fTimeDelta);
	m_pCollider->Collision_RectEx(CCollider::PLAYER, CCollider::INTEREACTION, fTimeDelta);
	m_pCollider->Collision_RectEx(CCollider::MONSTER, CCollider::BLOCK, fTimeDelta);
	m_pCollider->Collision_RectEx(CCollider::MONSTER, CCollider::INTEREACTION, fTimeDelta);

	//사각충돌 비교하면서 상호작용블럭
	m_pCollider->Collision_RectEx(CCollider::INTEREACTION, CCollider::BLOCK, fTimeDelta);
	m_pCollider->Collision_RectEx(CCollider::INTEREACTION, CCollider::INTEREACTION, fTimeDelta);
							  
	//Object Collision		   
	m_pCollider->Collision_RectEx(CCollider::OBJECT, CCollider::BLOCK, fTimeDelta);
	m_pCollider->Collision_RectEx(CCollider::OBJECT, CCollider::INTEREACTION, fTimeDelta);

	//구충돌로 비교하면서 Stay 호출
	//Player Collision
	m_pCollider->Collision_SphereEx(CCollider::PLAYER, CCollider::BULLET, fTimeDelta);
	m_pCollider->Collision_SphereEx(CCollider::PLAYER, CCollider::OBJECT, fTimeDelta);
	m_pCollider->Collision_SphereEx(CCollider::PLAYER, CCollider::MONSTER, fTimeDelta);
								
	//Monster Collision			
	m_pCollider->Collision_SphereEx(CCollider::MONSTER, CCollider::OBJECT, fTimeDelta);
	m_pCollider->Collision_SphereEx(CCollider::MONSTER, CCollider::BULLET, fTimeDelta);
								
	//Bullet Collision		
	m_pCollider->Collision_SphereEx(CCollider::BULLET, CCollider::BLOCK, fTimeDelta);
	m_pCollider->Collision_SphereEx(CCollider::BULLET, CCollider::INTEREACTION, fTimeDelta);

	//구충돌로 비교하면서 OnTrigger호출
	m_pCollider->Collision_TriggerXXX(CCollider::PLAYER, CCollider::PORTAL, fTimeDelta);
	m_pCollider->EndEx();
}

HRESULT CMainApp::Render()
{

	if (nullptr == m_pGameInstance)
		return E_FAIL;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	m_pGameInstance->Render_Begin();

	m_pRenderer->Draw();

	m_pGameInstance->Render_Level();

	ImGui::Begin("FPS");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	/*if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("Hong", u8"툴씬"))
			{
				CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);

				if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_HONG))))
					MSG_BOX(L"레벨 오픈 실패");

				Safe_Release(pGameInstance);
			}
			if (ImGui::MenuItem("Sae", u8"박스 버튼"))
			{
				CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);

				if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SENI))))
					MSG_BOX(L"레벨 오픈 실패");

				Safe_Release(pGameInstance);
			}
			if (ImGui::MenuItem("Hyeuk", u8"투디"))
			{
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);

				if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SJH))))
					MSG_BOX(L"레벨 오픈 실패");

				Safe_Release(pGameInstance);
			}
			if (ImGui::MenuItem("Kyuu", u8"탑디"))
			{
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);

				if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GYUH))))
					MSG_BOX(L"레벨 오픈 실패");

				Safe_Release(pGameInstance);
			}
			if (ImGui::MenuItem("STAGE1", u8"STAGE1"))
			{
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);

				if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_STAGE1))))
					MSG_BOX(L"레벨 오픈 실패");

				Safe_Release(pGameInstance);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}*/

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	m_pGameInstance->Render_End();
	
	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, eLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Default_SamplerState()
{
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	return S_OK;
}

HRESULT CMainApp::Ready_Default_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Toodee_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Toodee_Rect"), CVIBuffer_Toodee_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Terrain_Cube*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_Cube"), CVIBuffer_Terrain_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Topdee*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Topdee"), CVIBuffer_Topdee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_Component_Collider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		m_pCollider = CCollider::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), 
		CBoxCollider::Create(m_pGraphic_Device))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{

	return S_OK;
}

HRESULT CMainApp::Ready_LoadingLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Loading_BackGround"), CLoading_BackGround::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Texture_Loading_Cloud"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Loading/SpinCloud/SpinCloud_%d.png"),10))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Loading_Inside */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Texture_Loading_Inside"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Loading/InsideCloud/InsideCloud_%d.png"),2))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Loading_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Texture_Loading_BackGround"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Loading/LoadingBackground.png")))))
		return E_FAIL;
	Safe_Release(pGameInstance);

	
	
	
	return S_OK;
}

CMainApp * CMainApp::Create()
{	
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CMainApp"));
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pRenderer);

	m_pCollider->Clear_RayList();
	
	Safe_Release(m_pCollider);
	Safe_Release(m_pGameInstance);

	CGameMgr::Destroy_Instance();

	CGameInstance::Release_Engine();

}
