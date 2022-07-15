#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
	: m_p3D(nullptr) , m_pDevice(nullptr), m_pSprite(nullptr), m_pFont(nullptr)
{
}

HRESULT CGraphic_Device::InitDevice(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY, GRAPHICDESC::WINMODE eWinMode, LPDIRECT3DDEVICE9* ppOut)
{

	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	
	if (FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		MSG_BOX(L"GetDeviceCaps Failed"); 
		return E_FAIL;
	}

	DWORD	vp = 0;	
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		

	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp, hWnd, iWinSizeX, iWinSizeY, eWinMode);

	if (FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_pDevice)))
	{
		MSG_BOX(L"CreateDevice Failed"); 
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		MSG_BOX(L"m_pSprite Failed");
		return E_FAIL;
	}

	D3DXFONT_DESCW	tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");
	
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		MSG_BOX(L"m_pFont Failed");
		return E_FAIL;
	}

	*ppOut = m_pDevice;

	Safe_AddRef(m_pDevice);

	return S_OK;
}

void CGraphic_Device::SetParameters(D3DPRESENT_PARAMETERS & d3dpp, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY, GRAPHICDESC::WINMODE eWinMode)
{
	d3dpp.BackBufferWidth = iWinSizeX;
	d3dpp.BackBufferHeight = iWinSizeY;

	// 후면 버퍼의 픽셀 포맷 , 32비트 포맷 사용
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	
	// 후면 버퍼의 개수
	d3dpp.BackBufferCount = 1;

	// 픽셀이 모여 발생하는 계단현상을 뭉개서 부드럽게 연출하는 기법(안티 엘리어싱)을 지원하는지 여부를 묻는 인자값
	// 자연스럽게 연출은 되지만 성능저하가 발생하기 때문에 사용하지 않을 예정
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// 복사가 아닌 교체 방식을 통해 그리기와 지우기를 하게끔 만들어준다.
	
	// D3DSWAPEFFECT_DISCARD : 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP : 버퍼 하나를 뒤집으면서 사용하는 방식
	// D3DSWAPEFFECT_COPY : 더블 버퍼링과 유사하게 복사하여 사용하는 방식

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;	// 장치를 사용할 윈도우 핸들 설정
	
	d3dpp.Windowed = eWinMode; // TRUE인 경우 창모드, FALSE 전체 화면 모드

	// 스텐실 버퍼 : 반사와 같은 물체를 표현하는 버퍼
	// 깊이 버퍼(Z버퍼) : 물체의 깊이 값을 저장하여 원근감을 표현하기 위한 버퍼
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 전체화면 시에 사용하는 인자값
	// 창 모드 일 경우 장치가 아닌 운영체제가 재생률을 관리함
	// 전체 화면으로 변경 시 모니터 재생율에 대한 설정
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	
	// 재생율과 시연의 간격 설정
	// D3DPRESENT_INTERVAL_IMMEDIATE : 즉시 시연
	// D3DPRESENT_INTERVAL_DEFAULT : 적절한 간격을 DIRECTX 알아서 결정, 보통 모니터 재생율을 따라감
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CGraphic_Device::Render_Begin(void)
{
	// 렌더링 과정 : 후면 버퍼를 먼저 비운다(지운다)-> 후면 버퍼에 그린다-> 후면버퍼에 연결된 서피스를 전면버퍼에 연결된 서피스와 교체하여 출력한다.

	// 1인자 : 지우고자 하는 렉트의 개수
	// 2인자 : 지우고자 하는 렉트 배열의 이름(nullptr인 경우 전체 화면 영역에 해당하는 렉터라는 의미)

	m_pDevice->Clear(0,
		nullptr,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 0, 0, 255),	// 백버퍼 색상
		1.f, // z버퍼의 초기화 값
		0);	 // 스텐실 버퍼의 초기화 값

	//여기서부터 후면버퍼에 그리기 시점을 알린다.
	m_pDevice->BeginScene();

}

void CGraphic_Device::Render_End(HWND hWnd)
{	
	m_pDevice->EndScene();

	// 후면버퍼에 연결되어 있는 서피스와 전면 버퍼에 연결된 서피스를 교환하는 과정
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
	// 1,2인자 : 스왑체인 기법이 D3DSWAPEFFECT_COPY가 아닌 이상 NULL
	// 3인자 : 출력 대상 윈도우 핸들, 기본 nullptr인 경우 d3dpp.hDeviceWindow을 참조
	// 4인자 :  스왑체인 기법이 D3DSWAPEFFECT_COPY가 아닌 이상 NULL

}

void CGraphic_Device::Free()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
	Safe_Release(m_p3D);
}
