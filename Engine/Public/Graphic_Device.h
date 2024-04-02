#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{	
private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	HRESULT Initialize(const GRAPHIC_DESC& GraphicDesc, _Out_ LPDIRECT3DDEVICE9* ppOut);
	void Render_Begin();
	void Render_End(HWND hWnd = 0);

private:
	LPDIRECT3D9			m_pSDK;		// 장치를 조사하는 객체
	LPDIRECT3DDEVICE9	m_pDevice;	// 그래픽 장치를 통해 렌더링을 제어하는 객체
	LPD3DXSPRITE		m_pSprite;	// dx 상에서 2d 이미지 출력을 담당하는 객체
	LPD3DXFONT			m_pFont;	// dx 상에서 폰트를 출력을 담당하는 객체

private:
	void		Set_Parameters(const GRAPHIC_DESC& GraphicDesc, _Out_ D3DPRESENT_PARAMETERS& d3dpp);

public:
	static CGraphic_Device* Create(const GRAPHIC_DESC& GraphicDesc, _Out_ LPDIRECT3DDEVICE9* ppOut);
	virtual void Free() override;
};

END