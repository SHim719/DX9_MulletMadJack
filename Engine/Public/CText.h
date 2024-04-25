#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CText : public CBase
{
public:
	using Text_Info = struct TextInformation
	{
		const WCHAR* Text;
		RECT Rect = { 0, 0, 0, 0 };
		int Length = { 0 };
		D3DCOLOR RGBA = D3DCOLOR_RGBA(255, 255, 255, 255);
		bool m_bPrint = { false };
	};
private:
	CText(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXFONT Font, LPD3DXFONT BigFont);
	virtual ~CText() = default;


public:
	void Print_Text(Text_Info TextInfo);
	void Print_Big_Text(Text_Info BigTextInfo);

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device;
	LPD3DXFONT m_pFont;
	LPD3DXFONT m_pBigFont;
	vector<Text_Info> m_TutorialTextlist;


public:
	static CText* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXFONT Font, LPD3DXFONT BigFont);
	virtual void Free() override;
};

END
