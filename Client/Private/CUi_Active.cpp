#include "CUi_Active.h"

CUi_Active::CUi_Active(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CUi_Active::CUi_Active(const CUi_Active& rhs)
	: CUi(rhs)
{
}

HRESULT CUi_Active::Initialize_Prototype()
{
	return E_NOTIMPL;
}

void CUi_Active::PriorityTick(_float fTimeDelta)
{
}

void CUi_Active::Tick(_float fTimeDelta)
{
}

void CUi_Active::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Active::Render()
{
	return E_NOTIMPL;
}

void CUi_Active::Free()
{
	__super::Free();
}
