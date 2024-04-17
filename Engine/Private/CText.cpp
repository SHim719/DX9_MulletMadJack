#include "CText.h"

CText::CText(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXFONT Font, LPD3DXFONT BigFont)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pFont(Font)
	, m_pBigFont(BigFont)
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pFont);
	Safe_AddRef(m_pBigFont);
}

void CText::Print_Text(Text_Info TextInfo)
{
	m_pFont->DrawText(nullptr, TextInfo.Text, TextInfo.Length, &TextInfo.Rect,
		DT_TOP | DT_LEFT | DT_WORDBREAK,
		TextInfo.RGBA);
}

void CText::Print_Big_Text(Text_Info BigTextInfo)
{
	m_pBigFont->DrawText(nullptr, BigTextInfo.Text, BigTextInfo.Length, 
		&BigTextInfo.Rect, DT_TOP | DT_LEFT | DT_WORDBREAK, BigTextInfo.RGBA);
}

CText* CText::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXFONT Font, LPD3DXFONT BigFont)
{
	return new CText(pGraphic_Device, Font, BigFont);
}

void CText::Free()
{
	Safe_Release(m_pBigFont);
	Safe_Release(m_pFont);
	Safe_Release(m_pGraphic_Device);
}
