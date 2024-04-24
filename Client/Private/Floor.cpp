#include "Floor.h"

#include "GameInstance.h"

CFloor::CFloor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CFloor::CFloor(const CFloor& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CFloor::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CFloor::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	if (nullptr != pArg)
		m_iTexture_Index = *(_uint*)pArg;

	m_strTag = "Floor";

	return S_OK;
}

void CFloor::PriorityTick(_float fTimeDelta)
{
}

void CFloor::Tick(_float fTimeDelta)
{
	//m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
}

void CFloor::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CFloor::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CFloor::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_RectXY_Default"), TEXT("VIBuffer"), this));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Floor_Textures"), TEXT("Floor_Textures"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;


	CBoxCollider::BOXCOLLISION_DESC desc;
	desc.vScale = { 1.f, 1.f, 1.f };
	desc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &desc));
	if (nullptr == m_pBoxCollider)
		return E_FAIL;

	return S_OK;
}

void CFloor::OnCollisionEnter(CGameObject* pOther)
{
	CRigidbody* pRigidbody = dynamic_cast<CRigidbody*>(pOther->Find_Component(L"Rigidbody"));
	if (pRigidbody)
	{
		pRigidbody->Set_OnGround();
	}
}

CFloor* CFloor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFloor* pInstance = new CFloor(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CFloor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFloor::Clone(void* pArg)
{
	CFloor* pInstance = new CFloor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CFloor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFloor::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBoxCollider);
}
