#include "../Public/Graphic_Device.h"

CGraphic_Device::CGraphic_Device()
	: m_pSDK{ nullptr }
	, m_pDevice{ nullptr }
	, m_pSprite{ nullptr }
	, m_pFont{ nullptr }
{	

}


HRESULT CGraphic_Device::Initialize(const GRAPHIC_DESC& GraphicDesc, _Out_ LPDIRECT3DDEVICE9* ppOut)
{
	//return E_FAIL;
	
	// 1. 장치를 조사하는 객체 생성
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	// 2. 장치 조사(지원 수준)

	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : 장치에 대한 정보(지원 수준)를 얻어오는 함수
	// D3DADAPTER_DEFAULT : 정보를 얻으려는 기본 그래픽 카드를 의미

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		MSG_BOX(L"GetDeviceCaps Failed");
		return E_FAIL;
	}
	
	// 3. 장치를 제어할 수 있는 객체 생성
	// 버텍스 프로세싱 = 정점 변환 + 조명 연산

	DWORD		vp(0);

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(GraphicDesc, d3dpp);

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GraphicDesc.hWnd, vp, &d3dpp, &m_pDevice)))
	{
		MSG_BOX(L"CreateDevice Failed");
		return E_FAIL;
	}

	// 스프라이트 객체 생성

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		MSG_BOX(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	// 폰트 객체 생성
	D3DXFONT_DESCW		tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");

	// D3DXCreateFont : 얻어온 폰트 핸들을 토대로 폰트 객체를 생성하는 함수
	// D3DXCreateFontIndirect : 폰트 구조체 정보를 토대로 폰트 객체를 생성하는 함수

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		MSG_BOX(L"D3DXCreateFontIndirect Failed");
		return E_FAIL;
	}

	*ppOut = m_pDevice;

	Safe_AddRef(m_pDevice);
	
	return S_OK;
}

// 후면 버퍼의 동작 원리

void CGraphic_Device::Render_Begin()
{
	m_pDevice->Clear(0, // 렉트의 개수
					nullptr, // 렉트 배열의 첫 번째 주소
					D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,	// 그리기(타겟, 후면), 스텐실, 깊이버퍼의 값을 모두 비움 
					D3DCOLOR_ARGB(0, 0, 0, 0), // 후면 버퍼 텍스처의 색상
					1.f,	// 깊어 버퍼 초기화 값
					0);		// 스텐실 버퍼 초기화 값

	m_pDevice->BeginScene();	
}

void CGraphic_Device::Render_End(HWND hWnd)
{	
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);

	// Present : 후면 버퍼가 전면 버퍼로 텍스처를 전환시켜 화면에 출력할 수 있게 만들어줌
	// 1, 2 인자 : 렉트 주소, 스왑체인 D3DSWAPEFFECT_COPY로 작성되지 않는한 NULL
	// 4인자 : 스왑체인 D3DSWAPEFFECT_COPY로 작성되지 않는한 NULL
}


void CGraphic_Device::Set_Parameters(const GRAPHIC_DESC& GraphicDesc, _Out_ D3DPRESENT_PARAMETERS & d3dpp)
{
	d3dpp.BackBufferWidth = GraphicDesc.iWinSizeX;
	d3dpp.BackBufferHeight = GraphicDesc.iWinSizeY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// D3DSWAPEFFECT_DISCARD : 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP : 버퍼 하나로 뒤집으면서 사용하는 방식
	// D3DSWAPEFFECT_COPY : 더블 버퍼링과 유사한 방식

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// 장치를 사용할 윈도우 핸들 지정 
	d3dpp.hDeviceWindow = GraphicDesc.hWnd;

	// 창 모드 실행 or 전체 화면 모드 실행
	d3dpp.Windowed = GraphicDesc.isWindowed;		// 창 모드로 실행

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 모니터
	// 전체 화면 모드일 경우 모니터 재생율 관리를 어떻게 할 것인가
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// 재생율과 시연의 간격 결정
	// D3DPRESENT_INTERVAL_IMMEDIATE : 즉시 시연
	// D3DPRESENT_INTERVAL_DEFAULT : 적절한 간격을 DX가 알아서 결정(보통 모니터 재생율을 따라감)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

CGraphic_Device * CGraphic_Device::Create(const GRAPHIC_DESC & GraphicDesc, _Out_ LPDIRECT3DDEVICE9* ppOut)
{
	CGraphic_Device*	pInstance = new CGraphic_Device();

	if (FAILED(pInstance->Initialize(GraphicDesc, ppOut)))
	{
		MSG_BOX(TEXT("Failed to Created : CGraphic_Device"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CGraphic_Device::Free()
{
	if (nullptr != m_pFont)
		m_pFont->Release();

	if (nullptr != m_pSprite)
		m_pSprite->Release();

	unsigned int iRefCnt = { 0 };

	if (nullptr != m_pDevice)
	{
		iRefCnt = m_pDevice->Release();

		if (0 != iRefCnt)
			MSG_BOX(TEXT("Failed to Deleted : IDirect3DDevice9"));
	}

	if (nullptr != m_pSDK)
		m_pSDK->Release();
}
