#include "Chainsaw_Monster.h"

#include "GameInstance.h"

CChainsaw_Monster::CChainsaw_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPawn{ pGraphic_Device }
{
}

CChainsaw_Monster::CChainsaw_Monster(const CChainsaw_Monster& rhs)
	: CPawn{ rhs }
	, IsPlaying(false)
	, m_bSlashing(false)
	, m_bDead(false)
	, m_bWalking(false)
{
}

HRESULT CChainsaw_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChainsaw_Monster::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_Chainsaw_Monster_Desc, pArg, sizeof m_Chainsaw_Monster_Desc);

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Add_Textures()))
		return E_FAIL;

	m_pFPS_Camera = dynamic_cast<CFPS_Camera*>(m_pGameInstance->Get_Instance()->Get_CurCamera());

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_Chainsaw_Monster_Desc.vPosition);

	m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Idle_Up"), 0.1f, true);

	return S_OK;
}

void CChainsaw_Monster::PriorityTick(_float fTimeDelta)
{
}

void CChainsaw_Monster::Tick(_float fTimeDelta)
{
	Decide_Pawn_Motions(fTimeDelta);

	Set_Motions(fTimeDelta);

	m_pAnimationCom->Update(fTimeDelta, IsPlaying, m_bDead);
}

void CChainsaw_Monster::LateTick(_float fTimeDelta)
{
	m_pTransformCom->Set_Billboard_Matrix(m_pFPS_Camera->Get_Billboard_Matrix());

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CChainsaw_Monster::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pAnimationCom->Render();

	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CChainsaw_Monster::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), &m_Chainsaw_Monster_Desc));

	m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animation"), TEXT("Animation"), this));

	return S_OK;
}

HRESULT CChainsaw_Monster::Add_Textures()
{
	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Headshot"), TEXT("Chainsaw_Monster_Headshot"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Slash"), TEXT("Chainsaw_Monster_Slash"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Death_Machinegun"), TEXT("Chainsaw_Monster_Death_Machinegun"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Blocking"), TEXT("Chainsaw_Monster_Blocking"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_HeadExplode_Backward"), TEXT("Chainsaw_Monster_HeadExplode_Backward"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Death_Push_Floor"), TEXT("Chainsaw_Monster_Death_Push_Floor"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Start_Push_Floor"), TEXT("Chainsaw_Monster_Start_Push_Floor"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Death_Push_Wall"), TEXT("Chainsaw_Monster_Start_Push_Floor"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_GetUp"), TEXT("Chainsaw_Monster_GetUp"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_GroinShot"), TEXT("Chainsaw_Monster_GroinShot"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_HeadExplode_Forward"), TEXT("Chainsaw_Monster_HeadExplode_Forward"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Hit"), TEXT("Chainsaw_Monster_Hit"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Idle_Down"), TEXT("Chainsaw_Monster_Idle_Down"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Idle_Up"), TEXT("Chainsaw_Monster_Idle_Up"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Death_Shot_Floor"), TEXT("Chainsaw_Monster_Death_Shot_Floor"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Jump"), TEXT("Chainsaw_Monster_Jump"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Death_Shotgun"), TEXT("Chainsaw_Monster_Death_Shotgun"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Walk"), TEXT("Chainsaw_Monster_Walk"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Wall"), TEXT("Chainsaw_Monster_Wall"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CChainsaw_Monster::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CChainsaw_Monster::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CChainsaw_Monster::Set_Motions(_float fTimeDelta)
{
}

void CChainsaw_Monster::Decide_Pawn_Motions(_float fTimeDelta)
{
	Pawn_Slashing_Motion(fTimeDelta);
}

void CChainsaw_Monster::Pawn_Slashing_Motion(_float fTimeDelta)
{
	m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION);

	m_fSlashing_TimeGap += fTimeDelta;

	if (D3DXVec3Length(&(m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION) - m_Chainsaw_Monster_Desc.vPosition)) >= 5.f)
		return;

	if (D3DXVec3Length(&(m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION) - m_Chainsaw_Monster_Desc.vPosition)) < 5.f
		&& !m_bDead && !m_bWalking && !IsPlaying)
	{
		m_eState = STATE_SLASH;

		m_bSlashing = true;
		IsPlaying = m_bSlashing;

		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Slash"), 0.1f, false);
	}
}

CChainsaw_Monster* CChainsaw_Monster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CChainsaw_Monster* pInstance = new CChainsaw_Monster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to create: CDrone_Monster"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CChainsaw_Monster::Clone(void* pArg)
{
	CChainsaw_Monster* pInstance = new CChainsaw_Monster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to create: CDrone_Monster"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChainsaw_Monster::Free()
{
	__super::Free();

	Safe_Release(m_pAnimationCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
