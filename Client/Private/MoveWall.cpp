#include "MoveWall.h"

#include "GameInstance.h"


CMoveWall::CMoveWall(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CMoveWall::CMoveWall(const CMoveWall& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CMoveWall::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CMoveWall::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	if (nullptr != pArg)
		m_iTexture_Index = *(_uint*)pArg;

	m_strTag = "MoveWall";

	return S_OK;
}

void CMoveWall::PriorityTick(_float fTimeDelta)
{
}

void CMoveWall::Tick(_float fTimeDelta)
{
	m_pVIBuffer->Tick(fTimeDelta);

	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
}

void CMoveWall::LateTick(_float fTimeDelta)
{
	if (m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 20.f))
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMoveWall::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBuffer->Render();


	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	return S_OK;
}

HRESULT CMoveWall::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBuffer = dynamic_cast<CVIBuffer_MoveWall*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_MoveWall_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pTexture = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Wall_Textures"), TEXT("Wall_Textures"), nullptr));
	if (nullptr == m_pTexture)
		return E_FAIL;

	CBoxCollider::BOXCOLLISION_DESC desc;
	desc.vScale = { 1.f, 1.f, 1.f };
	desc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &desc));
	if (nullptr == m_pBoxCollider)
		return E_FAIL;


	return S_OK;
}


CMoveWall* CMoveWall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMoveWall* pInstance = new CMoveWall(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMoveWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMoveWall::Clone(void* pArg)
{
	CMoveWall* pInstance = new CMoveWall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMoveWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoveWall::Free()
{
	__super::Free();

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBoxCollider);
}
