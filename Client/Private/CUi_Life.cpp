#include "CUi_Life.h"

CUi_Life::CUi_Life(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CUi_Life::CUi_Life(const CUi_Life& rhs)
	: CUi(rhs)
	, m_fLifeTime(rhs.m_fLifeTime)
{
}

void CUi_Life::Cal_Life_Blink(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	m_iBlink += fTimeDelta;

	if (m_fLifeTime < 0)
	{
		m_bDead = true;
	}

	if (m_fLifeTime < 0.5f)
	{
		m_bBlink = true;
	}
}

bool CUi_Life::Cal_BlinkRender(_float BlinkGap)
{
	if (m_bBlink)
	{
		if (m_iBlink > BlinkGap)
		{
			m_iBlink = 0.f;
			return true;
		}
		return false;
	}

	return true;
}


void CUi_Life::Free()
{
	__super::Free();
}
