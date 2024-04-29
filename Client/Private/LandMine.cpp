#include "LandMine.h"
#include "PlayerManager.h"
#include "GameInstance.h"

CLandMine::CLandMine(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CLandMine::CLandMine(const CLandMine& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CLandMine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandMine::Initialize(void* pArg)
{

	bool bExplode = (bool)pArg;

	if(bExplode == true)
		m_eCurState = LandMineState::Explode;
	else
		m_eCurState = LandMineState::Idle;

	if (E_FAIL == Add_Components())
		return E_FAIL;

	m_pBoxCollider->Set_Scale({ 0.25f, 0.25f, 0.25f });
	return S_OK;
}

void CLandMine::PriorityTick(_float fTimeDelta)
{
	if (m_eCurState == LandMineState::Explode)
	{
		m_pTransformCom->Set_Scale({ 0.f, 0.f, 0.f });
		m_fExplodeTime -= fTimeDelta;
		if (m_fExplodeTime <= 0.f)
		{
			m_bDestroyed = true;
		}
	}

	if (m_eCurState == LandMineState::Explode && !m_bExplode)
	{

		m_pBoxCollider->Set_Scale({2.f, 2.f, 2.f});
		m_pBoxCollider->Set_Offset({ 0.f, 1.f, 0.f });

		CGameObject* pEffect = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Effect", L"Prototype_Explosion");
		pEffect->Get_Transform()->Set_Position(m_pTransformCom->Get_Pos());
		pEffect->Get_Transform()->Set_Scale({ 5.f, 5.f, 1.f });

		m_pGameInstance->Play(L"Drone_Explosion", false);
		m_pGameInstance->SetVolume(L"Drone_Explosion", 0.5f);
		if(CPlayer_Manager::Get_Instance()->Get_IsRoundPattern() == false)
			CPlayer_Manager::Get_Instance()->Set_Pattern(false);
		m_bExplode = true;
	}

}

void CLandMine::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;

	if (m_fLifeTime <= 0.f)
	{
		m_eCurState = LandMineState::Explode;
	}

	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
	m_pRigidbody->Update(fTimeDelta);
}

void CLandMine::LateTick(_float fTimeDelta)
{
	if (!m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 2.f))
		return;

	_float4x4	ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vScaled = m_pTransformCom->Get_Scale();

	_float3		vRight = *(_float3*)&ViewMatrix.m[0][0];
	_float3		vUp = *(_float3*)&ViewMatrix.m[1][0];
	_float3		vLook = *(_float3*)&ViewMatrix.m[2][0];

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, &(vRight * vScaled.x));
	m_pTransformCom->Set_State(CTransform::STATE_UP, &(vUp * vScaled.y));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, &(vLook * vScaled.z));

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CLandMine::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

HRESULT CLandMine::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("LandMine_Texture"), TEXT("LandMine_Texture"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	m_pRigidbody = dynamic_cast<CRigidbody*>(Add_Component(LEVEL_STATIC, TEXT("Rigidbody_Default"), TEXT("Rigidbody"), m_pTransformCom));
	if (nullptr == m_pRigidbody)
		return E_FAIL;

	m_pRigidbody->Set_Gravity({ 0.f, -10.f, 0.f });
	m_pRigidbody->Set_Friction(0.5f);

	CBoxCollider::BOXCOLLISION_DESC desc;
	desc.vScale = { 0.25f, 0.25f, 0.25f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &desc));
	if (nullptr == m_pBoxCollider)
		return E_FAIL;

	return S_OK;
}

void CLandMine::OnTriggerStay(CGameObject* pOther)
{
	if ("Player" == pOther->Get_Tag())
	{
		/*m_pGameInstance->Play(L"Player_Soda_Drink", false);
		m_pGameInstance->SetVolume(L"Player_Soda_Drink", 1.f);*/

		m_eCurState = LandMineState::Explode;
	}

	if ("Player" == pOther->Get_Tag() && m_eCurState == LandMineState::Explode)
	{
		CPlayer_Manager::Get_Instance()->Set_PlayerHP_Damaged(10);

	}

}

CLandMine* CLandMine::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLandMine* pInstance = new CLandMine(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLandMine"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLandMine::Clone(void* pArg)
{
	CLandMine* pInstance = new CLandMine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CLandMine"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLandMine::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRigidbody);
	Safe_Release(m_pBoxCollider);
}
