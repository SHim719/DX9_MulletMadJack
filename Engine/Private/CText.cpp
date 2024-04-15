#include "CText.h"

CText::CText(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXFONT Font)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pFont(Font)
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pFont);
}

void CText::Print_Text(Text_Info TextInfo)
{
	m_pFont->DrawText(nullptr, TextInfo.Text, TextInfo.Length, &TextInfo.Rect,
		DT_TOP | DT_LEFT | DT_WORDBREAK,
		TextInfo.RGBA);
}

CText* CText::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXFONT Font)
{
	return new CText(pGraphic_Device, Font);
}

void CText::Free()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pGraphic_Device);
	
}
