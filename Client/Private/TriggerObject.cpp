#include "TriggerObject.h"

#include "GameInstance.h"

CTriggerObject::CTriggerObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CTriggerObject::CTriggerObject(const CTriggerObject& rhs)
	: CGameObject{ rhs }
{
}

void CTriggerObject::Tick(_float fTimeDelta)
{
}

void CTriggerObject::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CTriggerObject::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (m_pBoxCollider)
		m_pBoxCollider->Render();
	return S_OK;
}


void CTriggerObject::Free()
{
	__super::Free();

	Safe_Release(m_pBoxCollider);
}
