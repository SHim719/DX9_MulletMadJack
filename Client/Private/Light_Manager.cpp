#include "Light_Manager.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

void CLight_Manager::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(m_pGraphic_Device);
}

void CLight_Manager::Reset_Light()
{
	memset(m_bArrUseLight, 0, sizeof(m_bArrUseLight));
}

_int CLight_Manager::Set_Light(const D3DLIGHT9& lightDesc)
{
	for (_int i = 0; i < LIGHT_MAX; ++i)
	{
		if (!m_bArrUseLight[i])
		{
			m_bArrUseLight[i] = true;
			m_pGraphic_Device->SetLight(i, &lightDesc);
			return i;
		}
	}
	return -1;
}

void CLight_Manager::Remove_Light(_uint iLightIdx)
{
	m_bArrUseLight[iLightIdx] = false;
}

HRESULT CLight_Manager::Set_Material(const D3DMATERIAL9& material)
{
	return m_pGraphic_Device->SetMaterial(&material);
}

void CLight_Manager::Free()
{
	Safe_Release(m_pGraphic_Device);
	Destroy_Instance();
}
