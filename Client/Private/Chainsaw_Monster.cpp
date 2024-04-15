#include "Chainsaw_Monster.h"

#include "GameInstance.h"

CChainsaw_Monster::CChainsaw_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPawn{ pGraphic_Device }
{
}

CChainsaw_Monster::CChainsaw_Monster(const CChainsaw_Monster& rhs)
	: CPawn{ rhs }
	, IsPlaying(false)
	, m_bIdle(true)
	, m_bDead(false)
	, m_bWalking(false)
	, m_bSlashing(false)
	, m_bBlock(false)
	, m_fWalking_TimeGap(0.f)
	, m_fSlashing_TimeGap(0.f)
	, m_fBlocking_TimeGap(0.f)
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

	_float3 Scale = { (_float)1.2f, (_float)1.2f, (_float)1.2f };
	m_pTransformCom->Set_Scale(Scale);

	m_pTransformCom->Set_Target(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION));

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

	m_pAnimationCom->Update(fTimeDelta, IsPlaying);

	if (m_bDead && !IsPlaying)
	{
		m_fCorpseDuration -= fTimeDelta;

		if (m_fCorpseDuration <= 0)
			m_bDestroyed = true;
	}
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

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_BodyShot"), TEXT("Chainsaw_Monster_BodyShot"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Blocking"), TEXT("Chainsaw_Monster_Blocking"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_HeadExplode_Backward"), TEXT("Chainsaw_Monster_HeadExplode_Backward"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Death_Push_Floor"), TEXT("Chainsaw_Monster_Death_Push_Floor"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Start_Push_Floor"), TEXT("Chainsaw_Monster_Start_Push_Floor"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chainsaw_Monster_Death_Push_Wall"), TEXT("Chainsaw_Monster_Death_Push_Wall"))))
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
	switch (m_eState)
	{
	case STATE_IDLE:
		break;

	case STATE_AIM:
		break;

	case STATE_WALK:
		m_pTransformCom->Go_Floor_Backward(fTimeDelta);
		break;

	case STATE_SLASH:
		break;

	case STATE_BLOCK:
		break;

	case STATE_HEADSHOT:
		break;

	case STATE_BODYSHOT:
		break;

	case STATE_GROINSHOT:
		break;

	default:
		break;
	}

	/*if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Headshot"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Slash"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_BodyShot"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Blocking"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_HeadExplode_Backward"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Death_Push_Floor"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Start_Push_Floor"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Death_Push_Wall"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_GetUp"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_GroinShot"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_HeadExplode_Forward"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Hit"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Idle_Down"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Idle_Up"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Death_Shot_Floor"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Jump"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Death_Shotgun"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Walk"), 0.1f, false);

	if (GetAsyncKeyState('1') & 0x8000)
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Wall"), 0.1f, false);*/
}

void CChainsaw_Monster::On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)
{
	srand(unsigned(time(nullptr)));

	_float4x4   WorldMatrixInverse = m_pTransformCom->Get_WorldMatrix_Inverse();
	_float3     vHitLocalPos = *D3DXVec3TransformCoord(&_float3(), &fHitWorldPos, &WorldMatrixInverse);

	if (-0.05f < vHitLocalPos.x && vHitLocalPos.x < 0.05f && 0.18f <= vHitLocalPos.y && vHitLocalPos.y < 0.3f)
		m_eState = STATE_HEADSHOT;

	else if (-0.1f < vHitLocalPos.x && vHitLocalPos.x < 0.1f && 0.f <= vHitLocalPos.y && vHitLocalPos.y < 0.18f && rand() % 2 == 0)
		m_eState = STATE_BLOCK;

	else if (-0.1f < vHitLocalPos.x && vHitLocalPos.x < 0.1f && 0.f <= vHitLocalPos.y && vHitLocalPos.y < 0.18f && rand() % 2 == 1)
		m_eState = STATE_BODYSHOT;

	else if (-0.1f < vHitLocalPos.x && vHitLocalPos.x < 0.1f && -0.1f <= vHitLocalPos.y && vHitLocalPos.y < 0.f)
		m_eState = STATE_GROINSHOT;

	else if (-0.1f < vHitLocalPos.x && vHitLocalPos.x < 0.1f && -0.5f < vHitLocalPos.y && vHitLocalPos.y < -0.1f)
		m_eState = STATE_BODYSHOT;
}

void CChainsaw_Monster::Decide_Pawn_Motions(_float fTimeDelta)
{
	Pawn_Slashing_Motion(fTimeDelta);
	Pawn_Walking_Motion(fTimeDelta);
	Pawn_Blocking_Motion(fTimeDelta);
	Pawn_Dying_Motion(fTimeDelta);
}

void CChainsaw_Monster::Pawn_Slashing_Motion(_float fTimeDelta)
{
	m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION);

	m_fSlashing_TimeGap += fTimeDelta;

	if (D3DXVec3Length(&(m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION) -
		m_pTransformCom->Get_State(CTransform::STATE_POSITION))) < 5.f)
	{
		m_bIdle = false;
	}

	if (D3DXVec3Length(&(m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION) -
		m_pTransformCom->Get_State(CTransform::STATE_POSITION))) < 1.f && !m_bDead && !IsPlaying)
	{
		m_eState = STATE_SLASH;

		m_bSlashing = true;
		IsPlaying = m_bSlashing;

		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Slash"), 0.1f, false);

		if (m_fSlashing_TimeGap > 1.f)
		{
			m_bSlashing = false;

			m_fSlashing_TimeGap = 0.f;
		}
	}

	if (D3DXVec3Length(&(m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION) -
		m_pTransformCom->Get_State(CTransform::STATE_POSITION))) >= 1.f)
	{
		m_bSlashing = false;
	}
}

void CChainsaw_Monster::Pawn_Walking_Motion(_float fTimeDelta)
{
	m_fWalking_TimeGap += fTimeDelta;

	if (D3DXVec3Length(&(m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION) -
		m_pTransformCom->Get_State(CTransform::STATE_POSITION))) >= 1.f && !m_bIdle)
	{
		m_bWalking = true;
	}

	if (m_bWalking && !m_bIdle && !m_bDead && !m_bSlashing && !IsPlaying)
	{
		m_eState = STATE_WALK;

		IsPlaying = m_bWalking;

		m_fWalking_TimeGap += fTimeDelta;

		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Walk"), 0.1f, false);

		if (m_fWalking_TimeGap > 1.f)
		{
			m_bWalking = false;

			m_eState = STATE_END;

			m_fWalking_TimeGap = 0.f;
		}
	}

	if (D3DXVec3Length(&(m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION) -
		m_pTransformCom->Get_State(CTransform::STATE_POSITION))) < 1.f)
	{
		m_bWalking = false;
	}
}

void CChainsaw_Monster::Pawn_Blocking_Motion(_float fTimeDelta)
{
	if (m_eState == STATE_BLOCK && !m_bDead)
	{
		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Blocking"), 0.1f, false);

		m_eState = STATE_END;
	}
}

void CChainsaw_Monster::Pawn_Dying_Motion(_float fTimeDelta)
{
	// 총알이 어디에 맞았는지에 따라 세부 모션을 구분할 것
	if (m_eState == STATE_HEADSHOT && !m_bDead)
	{
		m_bDead = true;

		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Headshot"), 0.1f, false);
	}

	if (m_eState == STATE_BODYSHOT && !m_bDead)
	{
		m_bDead = true;

		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_BodyShot"), 0.1f, false);
	}

	if (m_eState == STATE_GROINSHOT && !m_bDead)
	{
		m_bDead = true;

		m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_GroinShot"), 0.1f, false);
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
