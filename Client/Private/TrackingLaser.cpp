#include "TrackingLaser.h"


CTrackingLaser::CTrackingLaser(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CTrackingLaser::CTrackingLaser(const CTrackingLaser& rhs)
	:CGameObject(rhs)
{
}

HRESULT CTrackingLaser::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrackingLaser::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Add_Texture()))
		return E_FAIL;

	Initialize_Arg(pArg);

	Initialize_Order();

	m_pAnimationCom->Play_Animation(TEXT("TrackingLaser"), 0.1f, true);

	m_eState = TrackingLaserState::Warning;

	m_strTag = "TrackingLaser";

	return S_OK;
}

void CTrackingLaser::PriorityTick(_float fTimeDelta)
{
	Prepair_Order();
}

void CTrackingLaser::Tick(_float fTimeDelta)
{
	Execute_Order();

	m_fLife -= fTimeDelta;
	if (m_fLife < 0)
	{
		CGameObject* pObj = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"LandMine", L"Prototype_LandMine");
		pObj->Get_Transform()->Set_State(CTransform::STATE_POSITION, &m_vTargetPos);

		m_bDestroyed = true;
	}
	AdjustAlpha(fTimeDelta);

}

void CTrackingLaser::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

void CTrackingLaser::BeginRenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (m_eState == TrackingLaserState::Warning)
	{
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR,
			D3DCOLOR_RGBA(255, 0, 0, 255));
	}
	else
	{
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR,
			D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}

HRESULT CTrackingLaser::Render()
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

void CTrackingLaser::EndRenderState()
{
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CTrackingLaser::OnTriggerEnter(CGameObject* pOther)
{

}

void CTrackingLaser::Prepair_Order()
{
	if (m_eState == TrackingLaserState::Warning) {
		m_vTargetPos = m_pTarget->Get_Transform()->Get_Pos();
		m_vTargetPos.y -= 0.5f;
	}
}

void CTrackingLaser::Execute_Order()
{
	switch (m_eAttackOrder)
	{
		case CBeholder::ENDORDER:
			return;
		case CBeholder::PLAYERTRACKING:
			PlayerTracking_Order();
			return;
		default:
			return;
	}
}

void CTrackingLaser::PlayerTracking_Order()
{
	_float3 vDir = m_vTargetPos + m_vMasterPos;
	
	m_pTransformCom->Set_Position(vDir/2);


	//x¶û z Ãà acos
	
	_float3 vAngleATAN = m_vMasterPos - m_vTargetPos;
	_float3 vAngleATANfabs = { fabs(vAngleATAN.x), fabs(vAngleATAN.y), fabs(vAngleATAN.z) };
	

	_float fSide = (_float)sqrt(pow(vAngleATANfabs.x,2) + pow(vAngleATANfabs.z,2));

	_float fAngleY = atan2(vAngleATAN.x, vAngleATAN.z);
	_float fAngleX = atan2(fSide, vAngleATANfabs.y);

	fAngleY = D3DXToDegree(fAngleY);
	fAngleX = D3DXToDegree(fAngleX);

	m_pTransformCom->Rotation_XYZ({ fAngleX, fAngleY, 0.f });
	//m_pTransformCom->Set_Scale({ 1.f, 10.f, 1.f});

	_float fLength = (_float)sqrt(pow(fabs(m_vTargetPos.x - m_vMasterPos.x), 2) + pow(fabs(m_vTargetPos.y - m_vMasterPos.y), 2) + pow(fabs(m_vTargetPos.z - m_vMasterPos.z), 2));

	m_pTransformCom->Set_Scale({ 0.1f,fLength*1.1f, 0.1f });
}

HRESULT CTrackingLaser::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Box*>(__super::Add_Component
	(LEVEL_STATIC, TEXT("VIBuffer_Box_Default"), TEXT("VIBuffer")));

	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component
	(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

	m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component
	(LEVEL_STATIC, TEXT("Animation_Default"), TEXT("Animation"), this));

	Set_Collider();
	return S_OK;
}

HRESULT CTrackingLaser::Add_Texture()
{
	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_BOSS, TEXT("Texture_TrackingLaser"), TEXT("TrackingLaser"))))
		return E_FAIL;

	return S_OK;
}

void CTrackingLaser::Initialize_Arg(void* pArg)
{
	CBeholder::BeholderAttackOrder* pOrder = (CBeholder::BeholderAttackOrder*)pArg;

	m_eAttackOrder = pOrder->eOrder;
	m_vMasterPos = pOrder->vMasterPos;
	m_vMasterPos.y -= 0.2f;
	/*TrackingLaserArg* Arg = (TrackingLaserArg*)pArg;
	SansGasterFirePos FirePos = Arg->FirePos;*/
	Arg_InitializeSetPosScale();
}

void CTrackingLaser::Initialize_Order()
{
	switch(m_eAttackOrder)
	{
		case CBeholder::ENDORDER:
			return;
		case CBeholder::PLAYERTRACKING:
			m_pTarget = CPlayer_Manager::Get_Instance()->Get_Player();
			if(m_eState == TrackingLaserState::Warning)
				m_vTargetPos = m_pTarget->Get_Transform()->Get_Pos();
			return;
		default:
			return;
	}
	return;
}

void CTrackingLaser::Arg_InitializeSetPosScale()
{
	_float3 Scale{};
	_float3 Pos{};
	Scale = { 0.1f, 10.f, 0.1f };
	Pos = { 0.f, 0.f, 10.f};
	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_Position(Pos);
}

void CTrackingLaser::Set_Collider()
{
	CBoxCollider::BOXCOLLISION_DESC pDesc;
	pDesc.vScale = { 0.f, 0.f, 0.f };
	pDesc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component
	(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));
}

void CTrackingLaser::AdjustAlpha(_float fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;
	if (m_fLife > 0.5)
	{
		m_eState = TrackingLaserState::Warning;
		if (m_fAlphaTime > 0.05)
		{
			m_fAlphaTime = 0;
			if (m_iAlpha >= 235)
			{
				m_iAlpha = 255;
			}
			else
				m_iAlpha += 5;
		}
	}
	else if (m_fLife > 0.2)
	{
		m_eState = TrackingLaserState::Fire;
		m_iAlpha = 255;
	}
	else if (m_fLife <= 0.1)
	{
		m_eState = TrackingLaserState::End;
		if (m_fAlphaTime > 0.08)
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

void CTrackingLaser::AdjustScale(_float fTimeDelta)
{
	//if (m_eState == TrackingLaserState::Warning)
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

CTrackingLaser* CTrackingLaser::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrackingLaser* pInstance = new CTrackingLaser(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to create: CTrackingLaser"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrackingLaser::Clone(void* pArg)
{
	CTrackingLaser* pInstance = new CTrackingLaser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to create clone: CTrackingLaser"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrackingLaser::Free()
{
	__super::Free();

	Safe_Release(m_pAnimationCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBoxCollider);
}
