#include "CGasterLaser.h"


CGasterLaser::CGasterLaser(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CGasterLaser::CGasterLaser(const CGasterLaser& rhs)
	:CGameObject(rhs)
{
}

HRESULT CGasterLaser::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGasterLaser::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Add_Texture()))
		return E_FAIL;

	Initialize_Arg(pArg);

	m_pAnimationCom->Play_Animation(TEXT("GasterLaser"), 0.1f, true);

	m_eState = GasterLaserState::Warning;

	m_strTag = "GasterLaser";

	return S_OK;
}

void CGasterLaser::PriorityTick(_float fTimeDelta)
{
}

void CGasterLaser::Tick(_float fTimeDelta)
{
	m_fLife -= fTimeDelta;
	if (m_fLife < 0)
	{
		m_bDestroyed = true;
	}
	AdjustAlpha(fTimeDelta);

}

void CGasterLaser::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

void CGasterLaser::BeginRenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (m_eState == GasterLaserState::Warning)
	{
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR,
			D3DCOLOR_RGBA(255, 0, 0, m_iAlpha));
	}
	else
	{
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR,
			D3DCOLOR_RGBA(255, 255, 255, m_iAlpha));
	}
}

HRESULT CGasterLaser::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pAnimationCom->Render();

	BeginRenderState();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (m_pBoxCollider)
		m_pBoxCollider->Render();

	EndRenderState();
	
	return S_OK;
}

void CGasterLaser::EndRenderState()
{
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CGasterLaser::OnTriggerEnter(CGameObject* pOther)
{
}

HRESULT CGasterLaser::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Box*>(__super::Add_Component
	(LEVEL_STATIC, TEXT("VIBuffer_Box_Default"), TEXT("VIBuffer")));

	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component
	(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

	m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component
	(LEVEL_STATIC, TEXT("Animation_Default"), TEXT("Animation"), this));

	CBoxCollider::BOXCOLLISION_DESC pDesc;
	pDesc.vScale = { 0.25f, 0.25f, 0.5f };
	pDesc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component
	(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

	return S_OK;
}

HRESULT CGasterLaser::Add_Texture()
{
	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_GasterLaser"), TEXT("GasterLaser"))))
		return E_FAIL;

	return S_OK;
}

void CGasterLaser::Initialize_Arg(void* pArg)
{
	GasterLaserArg* Arg = (GasterLaserArg*)pArg;
	SansGasterFirePos FirePos = Arg->FirePos;
	Arg_InitializeSetPosScale(FirePos, Arg->Pos);
}

void CGasterLaser::Arg_InitializeSetPosScale(SansGasterFirePos FirePos, _float3 Pos)
{
	_float3 Scale{};
	switch (FirePos)
	{
	case SansGasterFirePos::Straight:
		Scale = { 0.8f, 0.8f, 20.f };
		Pos += { 0.f, 0.f, -10.05f };
		m_pTransformCom->Set_Scale(Scale);
		m_pTransformCom->Set_Position(Pos);
		break;
	case SansGasterFirePos::BackWard:
		Scale = { 0.8f, 0.8f, 20.f };
		Pos += { 0.f, 0.f, 10.05f };
		m_pTransformCom->Set_Scale(Scale);
		m_pTransformCom->Set_Position(Pos);
		break;
	case SansGasterFirePos::Left:
		Scale = { 20.f, 0.8f, 0.8f };
		Pos += { 10.05f, 0.f, 0.f };
		m_pTransformCom->Set_Scale(Scale);
		m_pTransformCom->Set_Position(Pos);
		break;
	case SansGasterFirePos::Right:
		Scale = { 20.f, 0.8f, 0.8f };
		Pos += {-10.05f, 0.f, 0.f};
		m_pTransformCom->Set_Scale(Scale);
		m_pTransformCom->Set_Position(Pos);
		break;
	case SansGasterFirePos::Down:
		Scale = { 0.8f, 20.f, 0.8f };
		Pos += {0.f, 10.05f, 0.f};
		m_pTransformCom->Set_Scale(Scale);
		m_pTransformCom->Set_Position(Pos);
		break;
	case SansGasterFirePos::Up:
		Scale = { 0.8f, 20.f, 0.8f };
		Pos += {0.f, -10.05f, 0.f};
		m_pTransformCom->Set_Scale(Scale);
		m_pTransformCom->Set_Position(Pos);
		break;
	case SansGasterFirePos::End:
		break;
	default:
		break;
	}
}

void CGasterLaser::AdjustAlpha(_float fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;

	if (m_fLife > 0.6)
	{
		m_eState = GasterLaserState::Warning;
		if (m_fAlphaTime > 0.1)
		{
			m_fAlphaTime = 0;
			if (m_iAlpha >= 235)
			{
				m_iAlpha = 255;
			}
			else
				m_iAlpha += 15;
		}
	}

	else if (m_fLife > 0.3)
	{
		m_eState = GasterLaserState::Fire;
		m_iAlpha = 255;
	}

	else if (m_fLife <= 0.3)
	{
		m_eState = GasterLaserState::End;
		if (m_fAlphaTime > 0.05)
		{
			m_fAlphaTime = 0;
			if (m_iAlpha <= 50)
			{
				m_iAlpha = 0;
			}
			else
				m_iAlpha -= 50;
		}
	}
}

void CGasterLaser::AdjustScale(_float fTimeDelta)
{
	//if (m_eState == GasterLaserState::Warning)
	//{
	//	m_fScaleTime += fTimeDelta;

	//	//if (m_fScaleTime > 0.1)
	//	//{
	//	//	m_fScaleTime = 0.f;
	//	//	m_fScale.x += 1;
	//	//	m_pTransformCom->Set_Scale(m_fScale);
	//	//	//_float3 Pos = m_pTransformCom->Get_Pos();
	//	//	//Pos.x -= 0.5;
	//	//	//m_pTransformCom->Set_PosX(Pos.x);
	//	//}

	//	if (m_fScaleTime > 0.1)
	//	{
	//		m_fScaleTime = 0.f;
	//		m_fScale.y += 1;
	//		m_pTransformCom->Set_Scale(m_fScale);
	//		//_float3 Pos = m_pTransformCom->Get_Pos();
	//		//Pos.y += 0.5;
	//		//m_pTransformCom->Set_PosY(Pos.y);
	//	}

	//	//if (m_fScaleTime > 0.1)
	//	//{
	//	//	m_fScaleTime = 0.f;
	//	//	m_fScale.z += 1;
	//	//	m_pTransformCom->Set_Scale(m_fScale);
	//	//	_float3 Pos = m_pTransformCom->Get_Pos();
	//	//	Pos.z -= 0.5;
	//	//	m_pTransformCom->Set_PosZ(Pos.z);
	//	//}
	//}
}

CGasterLaser* CGasterLaser::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGasterLaser* pInstance = new CGasterLaser(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to create: CGasterLaser"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGasterLaser::Clone(void* pArg)
{
	CGasterLaser* pInstance = new CGasterLaser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to create clone: CGasterLaser"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGasterLaser::Free()
{
	__super::Free();

	Safe_Release(m_pAnimationCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBoxCollider);
}
