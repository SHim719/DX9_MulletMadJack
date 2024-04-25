#include "Drone_Monster.h"
#include "GameInstance.h"
#include "PlayerManager.h"
#include "CUi_SpecialHit.h"
#include "FPS_Camera.h"

CDrone_Monster::CDrone_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPawn{ pGraphic_Device }
{
}

CDrone_Monster::CDrone_Monster(const CDrone_Monster& rhs)
	: CPawn{ rhs }
{
}

HRESULT CDrone_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDrone_Monster::Initialize(void* pArg)
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

	m_pAnimationCom->Play_Animation(TEXT("Move"), 0.1f, true);
	m_pRigidbody->Set_UseGravity(false);

	m_strTag = "Monster";

	m_fHp = 8.f;
	m_fFlyTime = 1.5f;
	m_fSpeed = 2.f;
	return S_OK;
}

void CDrone_Monster::PriorityTick(_float fTimeDelta)
{
	m_bThisFrameHit = false;
}

void CDrone_Monster::Tick(_float fTimeDelta)
{
	Process_State(fTimeDelta);

	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
	m_pRigidbody->Update(fTimeDelta);
	m_pAnimationCom->Update(fTimeDelta);
}

void CDrone_Monster::LateTick(_float fTimeDelta)
{
	if (!m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 2.f))
		return;

	m_pTransformCom->Set_Billboard_Matrix(m_pCamera->Get_Billboard_Matrix());

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CDrone_Monster::Render()
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

_bool CDrone_Monster::On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)
{
	if (STATE_DEATH == m_eState || STATE_PUSHED == m_eState)
		return false;

	if (m_bThisFrameHit)
		return true;
	if (CPlayer_Manager::Get_Instance()->Get_WeaponType() == CPlayer::KATANA && CPlayer_Manager::Get_Instance()->Get_PlayerToTarget(m_pTransformCom->Get_Pos()) > 1.5f)
		return false;

	_float4x4   WorldMatrixInverse = m_pTransformCom->Get_WorldMatrix_Inverse();
	_float3     vHitLocalPos = *D3DXVec3TransformCoord(&_float3(), &fHitWorldPos, &WorldMatrixInverse);

	if ((-0.4f <= vHitLocalPos.x && vHitLocalPos.x <= 0.4f) && (-0.4f <= vHitLocalPos.y && vHitLocalPos.y <= 0.4f))
	{
		ENEMYHIT_DESC* pDesc = (ENEMYHIT_DESC*)pArg;
		pDesc->fHitWorldPos = fHitWorldPos;
		pDesc->fDist = fDist;

		Hit(pDesc);
		return true;
	}


	return false;
}

void CDrone_Monster::OnCollisionEnter(CGameObject* pOther)
{
	if ("Player" == pOther->Get_Tag() && m_eState == STATE_RUSH)
	{
		_float fDamage = 3.f;
		pOther->Hit(&fDamage);

		SetState_Bound();
	}
}

void CDrone_Monster::Hit(void* pArg)
{
	ENEMYHIT_DESC* pDesc = (ENEMYHIT_DESC*)pArg;

	m_bThisFrameHit = true;

	CGameObject* pHitBlood = m_pGameInstance->Add_Clone(LEVEL_STATIC, L"Effect", L"Prototype_HitBlood");
	pHitBlood->Get_Transform()->Set_Position(pDesc->fHitWorldPos);

	m_fHp -= 4.f;

	if (CPlayer_Manager::Get_Instance()->Get_WeaponType() == CPlayer::KATANA)
	{
		m_pGameInstance->Play(L"Katana_Cutting_Flesh", false);
		m_pGameInstance->SetVolume(L"Katana_Cutting_Flesh", 0.5f);
		m_pGameInstance->Play(L"Blood_Splatter", false);
		m_pGameInstance->SetVolume(L"Blood_Splatter", 0.5f);
	}

	if (m_fHp <= 0.f)
		SetState_Death(pDesc);
}

void CDrone_Monster::Process_State(_float fTimeDelta)
{
	switch (m_eState)
	{
	case CDrone_Monster::STATE_IDLE:
		State_Idle();
		break;
	case CDrone_Monster::STATE_MOVE:
		State_Move();
		break;
	case CDrone_Monster::STATE_RUSH:
		State_Rush();
		break;
	case CDrone_Monster::STATE_BOUND:
		State_Bound(fTimeDelta);
		break;
	case CDrone_Monster::STATE_PUSHED:
		State_Pushed(fTimeDelta);
		break;
	case CDrone_Monster::STATE_DEATH:
		State_Death();
		break;
	}
}

void CDrone_Monster::State_Idle()
{
	_float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));

	if (fTargetDist < m_fPerceptionDist)
	{
		SetState_Move();
	}
}

void CDrone_Monster::State_Move()
{
	_float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));

	if (fTargetDist < 2.f)
	{
		SetState_Rush();
	}
	else if (fTargetDist >= m_fPerceptionDist)
	{
		SetState_Idle();
		return;
	}

	_float3 vToTargetDir = (m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos());
	D3DXVec3Normalize(&vToTargetDir, &vToTargetDir);

	m_pRigidbody->Set_Velocity(vToTargetDir * m_fSpeed);
}

void CDrone_Monster::State_Rush()
{
	_float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));

	if (fTargetDist > m_fPerceptionDist)
	{
		SetState_Idle();
		return;
	}

	_float3 vToTargetDir = (m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos());
	D3DXVec3Normalize(&vToTargetDir, &vToTargetDir);

	m_pRigidbody->Set_Velocity(vToTargetDir * m_fSpeed * 0.5f);
}

void CDrone_Monster::State_Bound(_float fTimeDelta)
{
	m_fBoundTimeAcc += fTimeDelta;
	if (m_fBoundTimeAcc >= m_fBoundTime)
		SetState_Idle();
}

void CDrone_Monster::State_Pushed(_float fTimeDelta)
{
	m_fFlyTimeAcc += fTimeDelta;

	if (m_fFlyTimeAcc >= m_fFlyTime)
	{
		SetState_Death(nullptr);
	}
}


void CDrone_Monster::State_Death()
{
	if (m_pAnimationCom->IsEndAnim())
	{
		CGameObject* pEffect = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Effect", L"Prototype_Explosion");
		pEffect->Get_Transform()->Set_Position(m_pTransformCom->Get_Pos());
		pEffect->Get_Transform()->Set_Scale({ 3.f, 3.f, 1.f });

		m_pGameInstance->Play(L"Drone_Explosion", false);
		m_pGameInstance->SetVolume(L"Drone_Explosion", 0.5f);

		Call_MonsterDieUi(eMonsterGrade::Middle);

		CUi_SpecialHit::SpecialHit_Desc Arg;
		Arg.Hit = eSpecialHit::FINISHED;
		Arg.iCount = 4;
		m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_SpecialHit"), eUiRenderType::Render_NonBlend, &Arg);
		m_bDestroyed = true;
	}
		
	
		
}

void CDrone_Monster::SetState_Idle()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_IDLE;

	m_pAnimationCom->Play_Animation(L"Move", 0.1f, true);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CDrone_Monster::SetState_Move()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_MOVE;

	m_pAnimationCom->Play_Animation(L"Move", 0.1f, true);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CDrone_Monster::SetState_Rush()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_RUSH;
	m_pGameInstance->Play(L"Drone_Attack", false);
	m_pGameInstance->SetVolume(L"Drone_Attack", 0.3f);

	m_pAnimationCom->Play_Animation(L"Rush", 0.1f, true);
}

void CDrone_Monster::SetState_Bound()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_BOUND;

	m_pAnimationCom->Play_Animation(L"Bound", 0.1f, false);

	_float3 vVelocity = m_pRigidbody->Get_Velocity();
	m_pRigidbody->Set_Velocity(-vVelocity * 2.f);
	m_fBoundTimeAcc = 0.f;
}

void CDrone_Monster::SetState_Pushed(_float3 vLook)
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_PUSHED;

	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);

	m_pRigidbody->Set_Velocity(vLook * 4.f);

	m_pAnimationCom->Play_Animation(L"Pushed", 51313.f, false);
}

void CDrone_Monster::SetState_Death(ENEMYHIT_DESC* pDesc)
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_DEATH;
	m_pGameInstance->Play(L"Drone_Death", false);
	m_pGameInstance->SetVolume(L"Drone_Death", 0.5f);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
	m_pAnimationCom->Play_Animation(L"Death", 0.1f, false);
}

HRESULT CDrone_Monster::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

	m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component(LEVEL_STATIC, TEXT("Animation_Default"), TEXT("Animation"), this));

	m_pRigidbody = dynamic_cast<CRigidbody*>(Add_Component(LEVEL_STATIC, TEXT("Rigidbody_Default"), TEXT("Rigidbody"), m_pTransformCom));

	CBoxCollider::BOXCOLLISION_DESC pDesc;
	pDesc.vScale = { 0.4f, 0.4f, 0.4f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

	return S_OK;
}

HRESULT CDrone_Monster::Add_Textures()
{
	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Death"), TEXT("Death"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Move"), TEXT("Move"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Bound"), TEXT("Bound"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Attack"), TEXT("Rush"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Pushed"), TEXT("Pushed"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDrone_Monster::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CDrone_Monster::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CDrone_Monster* CDrone_Monster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDrone_Monster* pInstance = new CDrone_Monster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to create: CDrone_Monster"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDrone_Monster::Clone(void* pArg)
{
	CDrone_Monster* pInstance = new CDrone_Monster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to create: CDrone_Monster"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDrone_Monster::Free()
{
	__super::Free();

	Safe_Release(m_pTarget);
	Safe_Release(m_pAnimationCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRigidbody);
}
