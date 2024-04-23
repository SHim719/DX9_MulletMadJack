#include "Ventilador.h"

#include "GameInstance.h"

#include "CUi_SpecialHit.h"


CVentilador::CVentilador(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CVentilador::CVentilador(const CVentilador& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CVentilador::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CVentilador::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	if (nullptr != pArg)
		m_iTexture_Index = *(_uint*)pArg;

	m_pTransformCom->Set_RotationSpeed(To_Radian(480.f));
	return S_OK;
}

void CVentilador::PriorityTick(_float fTimeDelta)
{
}

void CVentilador::Tick(_float fTimeDelta)
{
	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_LOOK), fTimeDelta);
}

void CVentilador::LateTick(_float fTimeDelta)
{
	if (m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 3.f))
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CVentilador::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	m_pBoxCollider->Render();

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CVentilador::OnTriggerEnter(CGameObject* pOther)
{
	// Blood Effect
	CGameObject* pHitBlood = m_pGameInstance->Add_Clone(LEVEL_STATIC, L"Effect", L"Prototype_HitBlood");
	pHitBlood->Get_Transform()->Set_Position(m_pTransformCom->Get_Pos());
	pHitBlood->Get_Transform()->Set_Scale({ 5.f, 5.f, 1.f });

	CUi_SpecialHit::SpecialHit_Desc Arg;
	Arg.Hit = eSpecialHit::FINISHED;
	Arg.iCount = 8;
	m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_SpecialHit"), eUiRenderType::Render_NonBlend, &Arg);

	pOther->Set_Destroy(true);
}

HRESULT CVentilador::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Ventilador_Textures"), TEXT("Ventilador_Textures"), nullptr));
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


CVentilador* CVentilador::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVentilador* pInstance = new CVentilador(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVentilador"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVentilador::Clone(void* pArg)
{
	CVentilador* pInstance = new CVentilador(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CVentilador"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVentilador::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBoxCollider);
}
