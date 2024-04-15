#pragma once
#include "Base.h"

BEGIN(Engine)

class CText : public CBase
{
public:
	using Text_Info = struct TextInformation
	{
		WCHAR* Text;
		RECT Rect = { 0, 0, 0, 0 };
		int Length = { 0 };
		D3DCOLOR RGBA = D3DCOLOR_RGBA(255, 255, 255, 255);

	};
private:
	CText(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXFONT Font);
	virtual ~CText() = default;


public:
	void Print_Text(Text_Info TextInfo);


private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device;
	LPD3DXFONT m_pFont;
	vector<Text_Info> m_TutorialTextlist;


public:
	static CText* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXFONT Font);
	virtual void Free() override;
};

END
