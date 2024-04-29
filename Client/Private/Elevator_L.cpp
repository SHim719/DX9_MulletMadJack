#include "Elevator_L.h"

#include "GameInstance.h"

CElevator_L::CElevator_L(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CElevator_L::CElevator_L(const CElevator_L& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CElevator_L::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CElevator_L::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;
	m_iTexture_Index = 0;

	m_strTag = "Elevator";

	m_pTransformCom->Set_Speed(10.f);
	return S_OK;
}

void CElevator_L::PriorityTick(_float fTimeDelta)
{
}

void CElevator_L::Tick(_float fTimeDelta)
{
	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());

	if (m_bOpening)
	{
		m_fDestroyTime -= fTimeDelta;
		m_pTransformCom->Go_Left(fTimeDelta);
		if (m_fDestroyTime <= 0.f)
			m_bDestroyed = true;
	}
}

void CElevator_L::LateTick(_float fTimeDelta)
{
	if (m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 2.f))
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CElevator_L::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CElevator_L::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_Elevator_L*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Elevator_L_Default"), TEXT("VIBuffer"), this));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Elevator_Textures"), TEXT("Elevator_Textures"), nullptr));
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

void CElevator_L::OnTriggerEnter(CGameObject* pOther)
{
	m_bOpening = true;
	m_pBoxCollider->Set_Active(false);
}

CElevator_L* CElevator_L::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CElevator_L* pInstance = new CElevator_L(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CElevator_L"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CElevator_L::Clone(void* pArg)
{
	CElevator_L* pInstance = new CElevator_L(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CElevator_L"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CElevator_L::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBoxCollider);
}
