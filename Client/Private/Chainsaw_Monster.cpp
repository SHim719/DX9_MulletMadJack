#include "Chainsaw_Monster.h"
#include "GameInstance.h"
#include "PlayerManager.h"

CChainsaw_Monster::CChainsaw_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPawn{ pGraphic_Device }
{
}

CChainsaw_Monster::CChainsaw_Monster(const CChainsaw_Monster& rhs)
	: CPawn{ rhs }
{
}

HRESULT CChainsaw_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChainsaw_Monster::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Add_Textures()))
		return E_FAIL;

	m_pCamera = dynamic_cast<CFPS_Camera*>(m_pGameInstance->Get_Instance()->Get_CurCamera());
	m_pTarget = CPlayer_Manager::Get_Instance()->Get_Player();
	Safe_AddRef(m_pTarget);

	_float3 Scale = { 1.3f, 1.3f, 1.f };
	m_pTransformCom->Set_Scale(Scale);

	_float3 vPos = { 0.f, 0.65f, 2.7f };
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPos);

	m_pRigidbody->Set_Friction(0.f);
	m_pRigidbody->Set_Velocity({ 0.f, 0.f, 0.f });
	m_pRigidbody->Set_Gravity({ 0.f, 0.f, 0.f });

	m_pAnimationCom->Play_Animation(TEXT("Idle_Up"), 0.1f, true);

	m_strTag = "Monster";

	return S_OK;
}

void CChainsaw_Monster::PriorityTick(_float fTimeDelta)
{
}

void CChainsaw_Monster::Tick(_float fTimeDelta)
{
	Process_State(fTimeDelta);

	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
	m_pRigidbody->Update(fTimeDelta);
	m_pAnimationCom->Update(fTimeDelta);
}

void CChainsaw_Monster::LateTick(_float fTimeDelta)
{
	m_pTransformCom->Set_Billboard_Matrix(m_pCamera->Get_Billboard_Matrix());

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

	m_pBoxCollider->Render();

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CChainsaw_Monster::On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)
{
	if (STATE_DEATH == m_eState)
		return;

	_float4x4   WorldMatrixInverse = m_pTransformCom->Get_WorldMatrix_Inverse();
	_float3     vHitLocalPos = *D3DXVec3TransformCoord(&_float3(), &fHitWorldPos, &WorldMatrixInverse);

	ENEMYHIT_DESC* pDesc = (ENEMYHIT_DESC*)pArg;
	pDesc->fHitWorldPos = fHitWorldPos;
	pDesc->fDist = fDist;

	if (Check_HeadShot(vHitLocalPos))
	{
		pDesc->eHitType = HEAD_SHOT;
		Hit(pDesc);
	}
	else if (Check_EggShot(vHitLocalPos))
	{
		pDesc->eHitType = EGG_SHOT;
		Hit(pDesc);
	}
	else if (Check_BodyShot(vHitLocalPos))
	{
		pDesc->eHitType = BODY_SHOT;
		Hit(pDesc);
	}
}

_bool CChainsaw_Monster::Check_HeadShot(_float3 vHitLocalPos)
{
	return -0.05f < vHitLocalPos.x && vHitLocalPos.x < 0.05f && 0.18f <= vHitLocalPos.y && vHitLocalPos.y < 0.3f;
}

_bool CChainsaw_Monster::Check_BodyShot(_float3 vHitLocalPos)
{
	return (-0.15f < vHitLocalPos.x && vHitLocalPos.x < 0.15f) &&
		((0.f <= vHitLocalPos.y && vHitLocalPos.y < 0.18f) || (-0.5f < vHitLocalPos.y && vHitLocalPos.y < -0.1f));
}

_bool CChainsaw_Monster::Check_EggShot(_float3 vHitLocalPos)
{
	return -0.1f < vHitLocalPos.x && vHitLocalPos.x < 0.1f && -0.1f <= vHitLocalPos.y && vHitLocalPos.y < 0.f;
}

void CChainsaw_Monster::Hit(void* pArg)
{
	ENEMYHIT_DESC* pDesc = (ENEMYHIT_DESC*)pArg;

	switch (pDesc->eHitType)
	{
	case CPawn::HEAD_SHOT:
		m_fHp -= 3.f;
		break;

	case CPawn::BODY_SHOT:
		m_fHp -= 3.f;

		/*if (m_fHp > 0.f)
			m_pAnimationCom->Play_Animation(TEXT("Hit"), 0.1f, false);*/

		break;

	case CPawn::EGG_SHOT:
		m_fHp -= 3.f;
		break;
	}

	if (m_fHp <= 0.f)
		SetState_Death(pDesc);
}

void CChainsaw_Monster::Process_State(_float fTimeDelta)
{
	switch (m_eState)
	{
	case CChainsaw_Monster::STATE_IDLE:
		State_Idle();
		break;

	case CChainsaw_Monster::STATE_MOVE:
		State_Move();
		break;

	case CChainsaw_Monster::STATE_PUSHED:
		State_Pushed();
		break;

	case CChainsaw_Monster::STATE_SLASH:
		State_Slash();
		break;

	case CChainsaw_Monster::STATE_JUMP:
		State_Jump();
		break;

	case CChainsaw_Monster::STATE_DEATH:
		State_Death(fTimeDelta);
		break;
	}
}

void CChainsaw_Monster::State_Idle()
{
	_float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));

	if (fTargetDist < m_fPerceptionDist)
	{
		SetState_Move();
	}
	else
	{
		if (m_pAnimationCom->IsEndAnim())
		{
			SetState_Idle();
		}
	}
}

void CChainsaw_Monster::State_Move()
{
	_float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));

	if (m_bPerceivedPlayer && fTargetDist < m_fRange)
	{
		SetState_Slash();

		return;
	}
	else if (m_bPerceivedPlayer && m_fPerceptionDist <= fTargetDist && fTargetDist < 9.f)
	{
		SetState_Jump();
	}
	else if (fTargetDist > 9.f)
	{
		SetState_Idle();

		return;
	}

	_float3 vToTargetDir = (m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos());
	vToTargetDir.y = 0.f;
	D3DXVec3Normalize(&vToTargetDir, &vToTargetDir);

	m_pRigidbody->Set_Velocity(vToTargetDir * m_fSpeed);
}

void CChainsaw_Monster::State_Pushed()
{
	if (0.025f >= D3DXVec3Length(&m_pRigidbody->Get_Velocity()) && !m_bPushRecovery)
	{
		m_pRigidbody->Set_Friction(0.f);

		m_pAnimationCom->Play_Animation(TEXT("Pushed_Recovery"), 0.1f, false);

		m_bPushRecovery = true;
	}

	if (m_pAnimationCom->IsEndAnim())
	{
		SetState_Slash();

		m_bPushRecovery = false;
	}
}

void CChainsaw_Monster::State_Slash()
{
	if (m_pAnimationCom->IsEndAnim())
	{
		_float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));

		if (fTargetDist < m_fRange)
		{
			SetState_Slash();
		}
		else if (m_fRange <= fTargetDist && fTargetDist < 9.f)
		{
			SetState_Move();
		}
		else if (m_fPerceptionDist <= fTargetDist && fTargetDist < 9.f)
		{
			SetState_Jump();
		}
		else if (fTargetDist >= 9.f)
		{
			SetState_Idle();
		}
	}
}

void CChainsaw_Monster::State_Jump()
{
	_float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));

	if (m_pAnimationCom->IsEndAnim())
	{
		if (fTargetDist < m_fRange)
		{
			SetState_Slash();
		}
		else if (m_fRange <= fTargetDist && fTargetDist < m_fPerceptionDist)
		{
			SetState_Move();
		}
		else if (m_fPerceptionDist <= fTargetDist && fTargetDist < 9.f)
		{
			SetState_Jump();
		}
		else if (fTargetDist >= 9.f)
		{
			SetState_Idle();
		}
	}
}

void CChainsaw_Monster::State_Death(_float fTimeDelta)
{
	if (m_pAnimationCom->IsEndAnim())
	{
		m_fDeathTime -= fTimeDelta;

		if (m_fDeathTime <= 0.f)
			m_bDestroyed = true;
	}
}

void CChainsaw_Monster::SetState_Idle()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_IDLE;
	m_bPerceivedPlayer = false;

	m_pAnimationCom->Play_Animation(L"Idle_Up", 0.15f, true);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CChainsaw_Monster::SetState_Move()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_MOVE;
	m_bPerceivedPlayer = true;

	m_pAnimationCom->Play_Animation(L"Walk", 0.1f, true);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CChainsaw_Monster::SetState_Pushed(_float3 vLook)
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_PUSHED;

	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);

	m_pRigidbody->Set_Velocity(vLook * 4.f);
	m_pRigidbody->Set_Friction(4.f);

	m_pAnimationCom->Play_Animation(L"Pushed", 100.f, false);
}

void CChainsaw_Monster::SetState_Slash()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_SLASH;

	m_pAnimationCom->Play_Animation(L"Slash", 0.1f, false);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CChainsaw_Monster::SetState_Jump()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_JUMP;
	m_pAnimationCom->Play_Animation(L"Jump", 0.05f, false);

	_float3 vLook = m_pTarget->Get_Transform()->Get_Look();
	//vLook = { vLook.x, 0.f, vLook.z };

	m_pRigidbody->Set_Velocity(-vLook * 4);
	m_pRigidbody->Set_Gravity({ 0.f, -1.f, 0.f });
}

void CChainsaw_Monster::SetState_Death(ENEMYHIT_DESC* pDesc)
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_DEATH;
	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));

	switch (pDesc->eHitType)
	{
	case CPawn::HEAD_SHOT:
		m_pAnimationCom->Play_Animation(TEXT("Death_Headshot"), 0.1f, false);
		break;

	case CPawn::BODY_SHOT:
		m_pAnimationCom->Play_Animation(TEXT("Death_Bodyshot"), 0.1f, false);
		break;

	case CPawn::EGG_SHOT:
		m_pAnimationCom->Play_Animation(TEXT("Death_Groinshot"), 0.1f, false);
		break;
	}
}

HRESULT CChainsaw_Monster::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

	m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component(LEVEL_STATIC, TEXT("Animation_Default"), TEXT("Animation"), this));

	m_pRigidbody = dynamic_cast<CRigidbody*>(Add_Component(LEVEL_STATIC, TEXT("Rigidbody_Default"), TEXT("Rigidbody"), m_pTransformCom));

	CBoxCollider::BOXCOLLISION_DESC pDesc;
	pDesc.vScale = { 0.5f, 1.f, 0.5f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

	return S_OK;
}

HRESULT CChainsaw_Monster::Add_Textures()
{
	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Headshot"), TEXT("Death_Headshot"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Slash"), TEXT("Slash"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Bodyshot"), TEXT("Death_Bodyshot"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Pushed"), TEXT("Pushed"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Pushed_Recovery"), TEXT("Pushed_Recovery"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_HeadExplode_Backward"), TEXT("HeadExplode_Backward"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Death_Push_Floor"), TEXT("Death_Push_Floor"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Start_Push_Floor"), TEXT("Start_Push_Floor"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Death_Push_Wall"), TEXT("Death_Push_Wall"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_GetUp"), TEXT("GetUp"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Groinshot"), TEXT("Death_Groinshot"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Hit"), TEXT("Hit"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Idle_Up"), TEXT("Idle_Up"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Jump"), TEXT("Jump"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Death_Shotgun"), TEXT("Death_Shotgun"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Chainsaw_Monster_Walk"), TEXT("Walk"))))
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

CChainsaw_Monster* CChainsaw_Monster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CChainsaw_Monster* pInstance = new CChainsaw_Monster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to create: CChainsaw_Monster"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CChainsaw_Monster::Clone(void* pArg)
{
	CChainsaw_Monster* pInstance = new CChainsaw_Monster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to create: CChainsaw_Monster"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChainsaw_Monster::Free()
{
	__super::Free();

	Safe_Release(m_pTarget);
	Safe_Release(m_pAnimationCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRigidbody);
}
