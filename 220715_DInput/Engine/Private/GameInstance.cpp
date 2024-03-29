#include "..\Public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pInput_Device(CInput_Device::Get_Instance())
	, m_pFMOD(C_FMOD::Get_Instance())
	, m_pLevel_Manager(CLevel_Manager::Get_Instance())
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pTimer_Manager(CTimer_Manager::Get_Instance())
	, m_pPicking(CPicking::Get_Instance())
{
	Safe_AddRef(m_pPicking);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pFMOD);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pGraphic_Device);
}


HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, HINSTANCE hInst, const GRAPHICDESC& GraphicDesc, IDirect3DDevice9** ppOut)
{
	/* 엔진을 구동하기위한 초기화작업을 거치낟. */
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 그래픽디바이스 초기화. */
	if (FAILED(m_pGraphic_Device->InitDevice(GraphicDesc.hWnd, GraphicDesc.iWinSizeX, GraphicDesc.iWinSizeY, GraphicDesc.eWinMode, ppOut)))
		return E_FAIL;

	/* 입력 초기화. */
	if (FAILED(m_pInput_Device->Initialize(hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	/* 사운드 초기화 */
	if (FAILED(m_pFMOD->Initialize()))
		return E_FAIL;

	/* 피킹 초기화*/
	if (FAILED(m_pPicking->Initialize(GraphicDesc.hWnd, GraphicDesc.iWinSizeX, GraphicDesc.iWinSizeY, *ppOut)))
		return E_FAIL;

	/* 사운드 초기화. */

	if (FAILED(m_pObject_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pObject_Manager || 
		nullptr == m_pInput_Device)
		return;

	m_pInput_Device->Update();

	m_pObject_Manager->Tick(fTimeDelta);

	m_pPicking->Tick();

	m_pObject_Manager->LateTick(fTimeDelta);

	m_pLevel_Manager->Tick(fTimeDelta);

	m_pFMOD->Tick(fTimeDelta);
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return;
	//이거 수정해라
	//m_pComponent_Manager->Clear(iLevelIndex);
 	m_pObject_Manager->Clear(iLevelIndex);
}

void CGameInstance::Render_Begin(void)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_Begin();
}

void CGameInstance::Render_End(HWND hWnd)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_End(hWnd);
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

HRESULT CGameInstance::Render_Level()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Render();	
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);	
}

HRESULT CGameInstance::Add_GameObjectToLayer(const _tchar * pPrototypeTag, _uint iLevelIndex, const _tchar * pLayerTag, void * pArg)
{
 	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObjectToLayer(pPrototypeTag, iLevelIndex, pLayerTag, pArg);
}


list<class CGameObject*>* CGameInstance::GetLayer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	//Hong
	if (nullptr == m_pObject_Manager)
		return nullptr;

	list<class CGameObject*>* pList = m_pObject_Manager->GetLayer(iLevelIndex, pLayerTag);
	if (pList == nullptr)
		return nullptr;
	else
		return pList;
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);	
}

_float CGameInstance::Get_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;
	return m_pTimer_Manager->Add_Timer(pTimerTag);
}

HRESULT CGameInstance::Update_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Update_Timer(pTimerTag);
}

_char CGameInstance::Get_DIKState(_uchar eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKState(eKeyID);	
}

_char CGameInstance::Get_DIMKeyState(DIMK eMouseKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMKeyState(eMouseKeyID);
}

_long CGameInstance::Get_DIMMoveState(DIMM eMouseMoveID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMMoveState(eMouseMoveID);
}

int CGameInstance::VolumeUp(_uint eID, _float _vol)
{
	if (nullptr == m_pFMOD)
		return 0;

	return m_pFMOD->VolumeUp(eID, _vol);
}

int CGameInstance::VolumeDown(_uint eID, _float _vol)
{
	if (nullptr == m_pFMOD)
		return 0;

	return m_pFMOD->VolumeDown(eID, _vol);
}

int CGameInstance::BGMVolumeUp(_float _vol, _uint eID)
{
	if (nullptr == m_pFMOD)
		return 0;

	return m_pFMOD->BGMVolumeUp(_vol, eID);
}

int CGameInstance::BGMVolumeDown(_float _vol, _uint eID)
{
	if (nullptr == m_pFMOD)
		return 0;

	return m_pFMOD->BGMVolumeDown(_vol, eID);
}

int CGameInstance::Pause(_uint eID)
{
	if (nullptr == m_pFMOD)
		return 0;

	return m_pFMOD->Pause(eID);
}

int CGameInstance::Mute(_uint eID)
{
	if (nullptr == m_pFMOD)
		return 0;

	return m_pFMOD->Mute(eID);
}

void CGameInstance::PlayEffect(_tchar * pSoundKey, _uint eID, _float _vol)
{
	if (nullptr == m_pFMOD)
		return;

	return m_pFMOD->PlayEffect(pSoundKey, eID, _vol);
}

void CGameInstance::PlayBGM(_tchar * pSoundKey, _uint eID, _float _vol)
{
	if (nullptr == m_pFMOD)
		return;

	return m_pFMOD->PlayBGM(pSoundKey, eID, _vol);
}

void CGameInstance::StopSound(_uint eID)
{
	if (nullptr == m_pFMOD)
		return;

	return m_pFMOD->StopSound(eID);
}

void CGameInstance::StopAll()
{
	if (nullptr == m_pFMOD)
		return;

	return m_pFMOD->StopAll();
}

void CGameInstance::Release_Engine()
{
	CGameInstance::Get_Instance()->Destroy_Instance();

	CComponent_Manager::Get_Instance()->Destroy_Instance();

	CObject_Manager::Get_Instance()->Destroy_Instance();

	CLevel_Manager::Get_Instance()->Destroy_Instance();

	CTimer_Manager::Get_Instance()->Destroy_Instance();

	CPicking::Get_Instance()->Destroy_Instance();

	C_FMOD::Get_Instance()->Destroy_Instance();

	CInput_Device::Get_Instance()->Destroy_Instance();
	
	CGraphic_Device::Get_Instance()->Destroy_Instance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pPicking);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pFMOD);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
}
