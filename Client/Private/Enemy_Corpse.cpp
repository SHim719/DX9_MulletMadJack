#include "Enemy_Corpse.h"

#include "GameInstance.h"
#include "PlayerManager.h"

#include "FPS_Camera.h"


CEnemy_Corpse::CEnemy_Corpse(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CEnemy_Corpse::CEnemy_Corpse(const CEnemy_Corpse& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CEnemy_Corpse::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CEnemy_Corpse::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	if (FAILED(Init_Animations()))
		return E_FAIL;


	ENEMYCORPSE_DESC* pDesc = (ENEMYCORPSE_DESC*)pArg;

	switch (pDesc->eType)
	{
	case WHITE_SUIT:
	{
		if (pDesc->isTop)
		{
			_float3 vScale = _float3(0.8f, 0.8f, 1.f);
			m_pAnimation->Play_Animation(L"WS_Up", 0.1f, false);
			m_pTransformCom->Set_Scale(vScale);
		}
		else
		{
			_float3 vScale = _float3(1.3f, 1.3f, 1.f);

			m_pAnimation->Play_Animation(L"WS_Bottom", 0.1f, false);
			m_pTransformCom->Set_Scale(vScale);
		}
		break;
	}
	case CHAINSAW:
	{
		_float3 vScale = _float3(1.3f, 1.3f, 1.f);
		if (pDesc->isTop)
		{
			m_pAnimation->Play_Animation(L"CS_Up", 0.1f, false);
		}
		else
		{
			m_pAnimation->Play_Animation(L"CS_Bottom", 0.1f, false);
		}
		m_pTransformCom->Set_Scale(vScale);
		break;
	}
	case DRONE:
		break;
	}

	if (2 == CPlayer_Manager::Get_Instance()->Get_SlashCount())
		m_bFlip = false;
	else
		m_bFlip = true;

	return S_OK;
}

void CEnemy_Corpse::PriorityTick(_float fTimeDelta)
{
}

void CEnemy_Corpse::Tick(_float fTimeDelta)
{
	m_fDeathTime -= fTimeDelta;
	if (m_fDeathTime <= 0.f)
	{
		m_bDestroyed = true;
		return;
	}
	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());

	m_pAnimation->Update(fTimeDelta);
	m_pRigidbody->Update(fTimeDelta);
}

void CEnemy_Corpse::LateTick(_float fTimeDelta)
{
	if (m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 2.f))
	{
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
		m_pTransformCom->Set_Billboard_Matrix(static_cast<CFPS_Camera*>(m_pGameInstance->Get_CurCamera())->Get_Billboard_Matrix());
	}
		
}

HRESULT CEnemy_Corpse::Render()
{
	Begin_RenderState();

	m_pAnimation->Render();

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	End_RenderState();

	return S_OK;
}

HRESULT CEnemy_Corpse::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	CBoxCollider::BOXCOLLISION_DESC desc;
	desc.vScale = { 0.5f, 0.5f, 0.5f };
	desc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &desc));
	if (nullptr == m_pBoxCollider)
		return E_FAIL;

	m_pAnimation = static_cast<CAnimation*>(Add_Component(LEVEL_STATIC, TEXT("Animation_Default"), TEXT("Animation"), this));
	if (nullptr == m_pAnimation)
		return E_FAIL;

	m_pRigidbody = static_cast<CRigidbody*>(Add_Component(LEVEL_STATIC, TEXT("Rigidbody_Default"), TEXT("Rigidbody"), m_pTransformCom));
	if (nullptr == m_pRigidbody)
		return E_FAIL;

	m_pRigidbody->Set_UseGravity(true);

	return S_OK;
}

HRESULT CEnemy_Corpse::Init_Animations()
{
	if (FAILED(m_pAnimation->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Katana_Left_Up"), TEXT("WS_Up"))))
		return E_FAIL;

	if (FAILED(m_pAnimation->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Katana_Left_Bottom"), TEXT("WS_Bottom"))))
		return E_FAIL;

	if (FAILED(m_pAnimation->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Katana_Left_Up"), TEXT("CS_Up"))))
		return E_FAIL;

	if (FAILED(m_pAnimation->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Katana_Left_Down"), TEXT("CS_Bottom"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnemy_Corpse::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (m_bFlip)
		static_cast<CVIBuffer_Rect*>(m_pVIBufferCom)->Flip_Horizontal();

	return S_OK;
}

HRESULT CEnemy_Corpse::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	if (m_bFlip)
		static_cast<CVIBuffer_Rect*>(m_pVIBufferCom)->Set_OriginTexCoord();

	return S_OK;
}


CEnemy_Corpse* CEnemy_Corpse::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnemy_Corpse* pInstance = new CEnemy_Corpse(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEnemy_Corpse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Corpse::Clone(void* pArg)
{
	CEnemy_Corpse* pInstance = new CEnemy_Corpse(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEnemy_Corpse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Corpse::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pAnimation);
	Safe_Release(m_pRigidbody);
	Safe_Release(m_pBoxCollider);
}
