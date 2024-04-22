#include "Effect.h"
#include "FPS_Camera.h"


CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CEffect::CEffect(const CEffect& rhs)
	: CGameObject { rhs }
{
}

void CEffect::PriorityTick(_float fTimeDelta)
{
}

void CEffect::Tick(_float fTimeDelta)
{
	if (m_pAnimation->IsEndAnim())
		m_bDestroyed = true;

	m_pAnimation->Update(fTimeDelta);
}

void CEffect::LateTick(_float fTimeDelta)
{

	m_pTransformCom->Set_Billboard_Matrix(static_cast<CFPS_Camera*>(m_pGameInstance->Get_CurCamera())->Get_Billboard_Matrix());

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}


HRESULT CEffect::Render()
{
	Begin_RenderState();

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pAnimation->Render();
	m_pVIBuffer->Render();

	End_RenderState();
	return S_OK;
}

HRESULT CEffect::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return S_OK;
}

HRESULT CEffect::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

void CEffect::Free()
{
	__super::Free();

	Safe_Release(m_pAnimation);
	Safe_Release(m_pVIBuffer);
}
