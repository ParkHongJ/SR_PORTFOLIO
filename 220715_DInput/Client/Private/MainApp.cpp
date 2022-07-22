#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"

using namespace Client;

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	JSON_Value *rootValue;
	JSON_Object *rootObject;
	rootValue = json_value_init_object();
	rootObject = json_value_get_object(rootValue);

	json_object_set_string(rootObject, "Title", "Inception");

	// ��ü�� Ű�� �߰��ϰ� ���� ����
	json_object_set_number(rootObject, "Year", 2010);
	json_object_set_number(rootObject, "Runtime", 148);

	// ��ü�� Ű�� �߰��ϰ� ���ڿ� ����
	json_object_set_string(rootObject, "Genre", "Sci-Fi");
	json_object_set_string(rootObject, "Director", "Christopher Nolan");

	json_object_set_value(rootObject, "Actors", json_value_init_array());
	// ��ü���� �迭 �����͸� ������
	JSON_Array *actors = json_object_get_array(rootObject, "Actors");
	// �迭�� ���ڿ� ��� �߰�
	json_array_append_string(actors, "Leonardo DiCaprio");
	json_array_append_string(actors, "Joseph Gordon-Levitt");
	json_array_append_string(actors, "Ellen Page");
	json_array_append_string(actors, "Tom Hardy");
	json_array_append_string(actors, "Ken Watanabe");

	// ��ü�� Ű�� �߰��ϰ� ���� ����
	json_object_set_number(rootObject, "imdbRating", 8.8);
	// ��ü�� Ű�� �߰��ϰ� �� �� ����
	json_object_set_boolean(rootObject, "KoreaRelease", true);

	// JSON_Value�� ����� �б� ���� ���ڿ�(pretty)�� ���� �� ���Ͽ� ����
	json_serialize_to_file_pretty(rootValue, "example.json");

	json_value_free(rootValue);    // JSON_Value�� �Ҵ�� ���� �޸� ����


	GRAPHICDESC			GraphicDesc;
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

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
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

	static float f = 0.0f;
	static int counter = 0;

	//----------------------------------------------------------------------------------------------------------------------------
	ImGui::Begin("FPS");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	//----------------------------------------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------------------------------------
	ImGui::ShowDemoWindow();
	//----------------------------------------------------------------------------------------------------------------------------

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("Hong", u8"������Ʈ�̸�"))
			{
				CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);

				if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_HONG))))
					MSG_BOX(L"���½���");

				Safe_Release(pGameInstance);
			}
			if (ImGui::MenuItem("Sae", u8"������Ʈ�̸�")) 
			{
				CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);

				if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_LOGO))))
					MSG_BOX(L"���½���");

				Safe_Release(pGameInstance);
			}
			if (ImGui::MenuItem("Hyeuk", u8"������Ʈ�̸�")) 
			{
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);

				if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SJH))))
					MSG_BOX(L"���½���");

				Safe_Release(pGameInstance);
			}
			if (ImGui::MenuItem("Kyuu", u8"������Ʈ�̸�"))
			{

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Tool", u8"��������")) 
			{

			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	m_pGameInstance->Render_End();

//#ifdef _DEBUG
//	++m_iNumDraw;
//
//	if (m_fTimeAcc >= 1.f)
//	{
//		wsprintf(m_szFPS, TEXT("fps : %d"), m_iNumDraw);
//		m_iNumDraw = 0;
//		m_fTimeAcc = 0.f;
//	}
//
//	SetWindowText(g_hWnd, m_szFPS);
//#endif // _DEBUG

	

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* Ư�� ������ �Ҵ��ϱ� ���� �ε������� ���� �Ҵ�Ǵ� ���·� ����. */

	/* �Ҵ��� ������ �����Ŵ����� ������ �� �ֶǷ�. gksek. */
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

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Collider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		CCollider::Create(m_pGraphic_Device))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{

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
	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();

}
