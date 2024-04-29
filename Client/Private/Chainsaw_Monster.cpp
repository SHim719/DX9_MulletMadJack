#include "Chainsaw_Monster.h"
#include "GameInstance.h"
#include "PlayerManager.h"
#include "CUi_SpecialHit.h"
#include "FPS_Camera.h"
#include "Enemy_Corpse.h"

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

	m_pAnimationCom->Play_Animation(TEXT("Idle_Up"), 0.1f, true);

	m_strTag = "Monster";
	m_substrTag = "Chainsaw_Monster";

	m_fHp = 8.f;
	m_fSpeed = 2.f;
	m_fPerceptionDist = 4.f;
	return S_OK;
}

void CChainsaw_Monster::PriorityTick(_float fTimeDelta)
{
	m_bThisFrameHit = false; 
}

void CChainsaw_Monster::Tick(_float fTimeDelta)
{
	Process_State(fTimeDelta);

	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
	m_pRigidbody->Update(fTimeDelta);
	m_pAnimationCom->Update(fTimeDelta);

	m_pRigidbody->Set_Ground(false);
}

void CChainsaw_Monster::LateTick(_float fTimeDelta)
{
	if (!m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 2.f))
		return;

	m_pTransformCom->Set_Billboard_Matrix(m_pCamera->Get_Billboard_Matrix());

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CChainsaw_Monster::Render()
{
	if (STATE_EXECUTION == m_eState)
		return S_OK;

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

_bool CChainsaw_Monster::On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)
{
	if (STATE_DEATH == m_eState || STATE_FLY == m_eState || STATE_EXECUTION == m_eState
		|| STATE_FLYDEATH == m_eState)
		return false;

	if (m_bThisFrameHit)
		return true;


	CPlayer::WEAPON_TYPE ePlayerWeapon = CPlayer_Manager::Get_Instance()->Get_WeaponType();
	if (ePlayerWeapon == CPlayer::KATANA && CPlayer_Manager::Get_Instance()->Get_PlayerToTarget(m_pTransformCom->Get_Pos()) > 1.5f)
		return false;

	if (ePlayerWeapon == CPlayer::SHOTGUN && CPlayer_Manager::Get_Instance()->Get_PlayerToTarget(m_pTransformCom->Get_Pos()) > 5.f)
		return false;

	_float4x4   WorldMatrixInverse = m_pTransformCom->Get_WorldMatrix_Inverse();
	_float3     vHitLocalPos = *D3DXVec3TransformCoord(&_float3(), &fHitWorldPos, &WorldMatrixInverse);

	ENEMYHIT_DESC* pDesc = (ENEMYHIT_DESC*)pArg;
	pDesc->fHitWorldPos = fHitWorldPos;
	pDesc->fDist = fDist;

	if (Check_HeadShot(vHitLocalPos))
	{
		pDesc->eHitType = HEAD_SHOT;
		Hit(pDesc);
		return true;
	}
	else if (Check_EggShot(vHitLocalPos))
	{
		pDesc->eHitType = EGG_SHOT;
		Hit(pDesc);
		return true;
	}
	else if (Check_BodyShot(vHitLocalPos))
	{
		pDesc->eHitType = BODY_SHOT;
		Hit(pDesc);
		return true;
	}

	return false;
}

void CChainsaw_Monster::OnCollisionEnter(CGameObject* pOther)
{
	if ("Floor" == pOther->Get_Tag()
		&& m_pTransformCom->Get_Pos().y > pOther->Get_Transform()->Get_Pos().y)
	{
		m_pRigidbody->Set_Ground(true);
	}

	if ("Player" == pOther->Get_Tag() && m_eState == STATE_JUMP)
	{
		_float fDamage = 2.f;
		pOther->Hit(&fDamage);
	}

	if (STATE_FLY == m_eState && "Wall" == pOther->Get_Tag())
	{
		m_bWallColl = true;
	}
}

void CChainsaw_Monster::OnCollisionStay(CGameObject* pOther)
{
	if ("Floor" == pOther->Get_Tag()
		&& m_pTransformCom->Get_Pos().y > pOther->Get_Transform()->Get_Pos().y)
	{
		m_pRigidbody->Set_Ground(true);
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


	CGameObject* pHitBlood = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Effect", L"Prototype_HitBlood");
	pHitBlood->Get_Transform()->Set_Position(pDesc->fHitWorldPos);

	_bool bHitByKatana = CPlayer_Manager::Get_Instance()->Get_Player_WeaponType() == CPlayer::KATANA;
	if (CPlayer_Manager::Get_Instance()->Get_WeaponType() == CPlayer::KATANA) {
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Blood"), false);
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Blood"), true);
	}

	m_bThisFrameHit = true;

	switch (pDesc->eHitType)
	{
	case CPawn::HEAD_SHOT:
		m_fHp -= 8.f;
		break;

	case CPawn::BODY_SHOT:
		if (bHitByKatana)
		{
			//m_pGameInstance->Play(L"Chainsaw_Slashed", false);
			//m_pGameInstance->SetVolume(L"Chainsaw_Slashed", 1.f);
			//m_pGameInstance->Play(L"Chainsaw_Electrified", false);
			//m_pGameInstance->SetVolume(L"Chainsaw_Electrified", 1.f);
			//m_pGameInstance->Play(L"Katana_Hit_Metal", false);
			//m_pGameInstance->SetVolume(L"Katana_Hit_Metal", 1.f);

			m_pGameInstance->Play(L"Katana_Cutting_Flesh", false);
			m_pGameInstance->SetVolume(L"Katana_Cutting_Flesh", 0.5f);
			m_pGameInstance->Play(L"Blood_Splatter", false);
			m_pGameInstance->SetVolume(L"Blood_Splatter", 0.5f);

			m_fHp -= 8.f;
			m_bThisFrameHit = true;
		}
			
		else
			m_fHp -= 4.f;
		break;

	case CPawn::EGG_SHOT:
		m_fHp -= 8.f;
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

	case CChainsaw_Monster::STATE_GETUP:
		State_GetUp();
		break;
	case CChainsaw_Monster::STATE_EXECUTION:
		State_Execution();
		break;
	case CChainsaw_Monster::STATE_FLY:
		State_Fly(fTimeDelta);
		break;
	case CChainsaw_Monster::STATE_FLYDEATH:
		State_FlyDeath(fTimeDelta);
		break;
	case CChainsaw_Monster::STATE_AIR:
		State_Air();
		break;
	case CChainsaw_Monster::STATE_LANDING:
		State_Landing();
		break;
	case CChainsaw_Monster::STATE_DEATH:
		State_Death(fTimeDelta);
		break;
	}
}

void CChainsaw_Monster::State_Idle()
{
	_float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));

	//if (!m_bFirstMeet &&  (m_fPerceptionDist < fTargetDist && fTargetDist < 9.f))
	//{
	//	SetState_Jump();
	//}
	//else if (m_bFirstMeet || fTargetDist < m_fPerceptionDist)
	//{
	//	SetState_Move();
	//}
	if (fTargetDist < m_fPerceptionDist)
	{
		SetState_Move();
	}
	else if (m_fPerceptionDist <= fTargetDist && fTargetDist < 9.f)
	{
		SetState_Jump();
	}
	else
		SetState_Idle();
}

void CChainsaw_Monster::State_Move()
{
	_float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));

	if (fTargetDist < m_fRange)
	{
		SetState_Slash();

		return;
	}
	else if (m_fPerceptionDist < fTargetDist && fTargetDist < 9.f)
	{
		SetState_Jump();
		return;
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
		else if (m_fRange <= fTargetDist && fTargetDist < m_fPerceptionDist)
		{
			SetState_Move();
		}
		else if (m_fPerceptionDist <= fTargetDist && fTargetDist < 9.f)
		{
			SetState_Jump();
		}
		else
		{
			SetState_Idle();
		}	
	}
}

void CChainsaw_Monster::State_Jump()
{
	if (m_pRigidbody->IsGround() && m_pAnimationCom->IsEndAnim())
	{
		SetState_GetUp();
	}
}

void CChainsaw_Monster::State_GetUp()
{
	if (m_pAnimationCom->IsEndAnim())
	{
		_float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));

		if (fTargetDist < m_fRange)
		{
			SetState_Slash();
		}
		else if (m_fPerceptionDist >= fTargetDist && fTargetDist < 6.f)
		{
			SetState_Move();
		}
		else if (m_fPerceptionDist < fTargetDist && fTargetDist < 9.f)
		{
			SetState_Jump();
		}
	}
	
}

void CChainsaw_Monster::State_Execution()
{
}

void CChainsaw_Monster::State_Fly(_float fTimeDelta)
{
	m_fFlyTimeAcc += fTimeDelta;

	if (m_fFlyTimeAcc >= m_fFlyTime)
	{
		m_fFlyTimeAcc = 0.f;
		SetState_FlyDeath();
	}
}

void CChainsaw_Monster::State_FlyDeath(_float fTimeDelta)
{
	if (m_pAnimationCom->IsEndAnim())
	{
		m_fDeathTime -= fTimeDelta;
		if (m_fDeathTime <= 0.f)
		{
			m_bDestroyed = true;
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

void CChainsaw_Monster::State_Air()
{
	if (m_pRigidbody->IsGround())
		SetState_Landing();
}

void CChainsaw_Monster::State_Landing()
{
	if (m_pAnimationCom->IsEndAnim())
	{
		SetState_Move();
	}
}

void CChainsaw_Monster::SetState_Idle()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_IDLE;

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CChainsaw_Monster::SetState_Move()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_MOVE;

	m_pAnimationCom->Play_Animation(L"Walk", 0.1f, true);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CChainsaw_Monster::SetState_Pushed(_float3 vLook)
{
	if (STATE_DEATH == m_eState || STATE_EXECUTION == m_eState 
		|| STATE_FLY == m_eState || STATE_FLYDEATH == m_eState)
		return;

	m_eState = STATE_PUSHED;

	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);

	m_pRigidbody->Set_Velocity(vLook * 15.f);
	m_pRigidbody->Set_Friction(5.f);

	m_pAnimationCom->Play_Animation(L"Pushed", 100.f, false);

	m_bPushRecovery = false;
}

void CChainsaw_Monster::SetState_Slash()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_SLASH;
	m_pGameInstance->Play(L"Chainsaw_Attack", false);
	m_pGameInstance->SetVolume(L"Chainsaw_Attack", 0.3f);

	m_pAnimationCom->Play_Animation(L"Slash", 0.1f, false);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));

	_float fDamage = 1.f;
	m_pTarget->Hit(&fDamage);
}

void CChainsaw_Monster::SetState_Jump()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_JUMP;
	m_pGameInstance->Play(L"Chainsaw_Jump", false);
	m_pGameInstance->SetVolume(L"Chainsaw_Jump", 0.3f);

	m_pAnimationCom->Play_Animation(L"Jump", 0.05f, false);

	_float3 vToTargetDir = (m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos());
	vToTargetDir.y = 0.f;
	D3DXVec3Normalize(&vToTargetDir, &vToTargetDir);

	vToTargetDir *= 6.f;
	vToTargetDir.y = 4.f;

	m_pRigidbody->Set_Velocity(vToTargetDir);
}

void CChainsaw_Monster::SetState_GetUp()
{
	m_eState = STATE_GETUP;

	m_pAnimationCom->Play_Animation(L"GetUp", 0.15f, false);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

_bool CChainsaw_Monster::SetState_Execution()
{
	if (STATE_DEATH == m_eState || STATE_EXECUTION == m_eState
		|| STATE_FLY == m_eState || STATE_FLYDEATH == m_eState)
		return false;

	m_bCanIntersect = false;
	m_pBoxCollider->Set_Active(false);

	m_eState = STATE_EXECUTION;
	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
	m_pRigidbody->Set_UseGravity(false);

	return true;
}

void CChainsaw_Monster::SetState_Fly(_float3 vLook)
{
	if (STATE_DEATH == m_eState || STATE_FLY == m_eState || STATE_FLYDEATH == m_eState)
		return;

	m_eState = STATE_FLY;

	m_pAnimationCom->Play_Animation(L"Death_Fly", 0.1f, false);

	vLook.y = 0.f;
	m_pRigidbody->Set_Velocity(vLook * 10.f);


	m_pBoxCollider->Set_Active(true);
	m_fFlyTimeAcc = 0.f;
}

void CChainsaw_Monster::SetState_FlyDeath()
{
	if (STATE_DEATH == m_eState || STATE_EXECUTION == m_eState || STATE_FLYDEATH == m_eState)
		return;

	m_eState = STATE_FLYDEATH;

	if (m_bWallColl)
		m_pAnimationCom->Play_Animation(L"Death_Fly_Wall", 0.1f, false);
	else
		m_pAnimationCom->Play_Animation(L"Death_Fly_Floor", 0.1f, false);

	m_pBoxCollider->Set_Active(false);

	Call_MonsterDieUi(eMonsterGrade::Middle);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
	m_pRigidbody->Set_UseGravity(true);

}

void CChainsaw_Monster::SetState_Death(ENEMYHIT_DESC* pDesc)
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_DEATH;
	m_pGameInstance->Play(L"Chainsaw_Death", false);
	m_pGameInstance->SetVolume(L"Chainsaw_Death", 0.3f);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
	m_pRigidbody->Set_UseGravity(false);
	m_pBoxCollider->Set_Active(false);

	Call_MonsterDieUi(eMonsterGrade::Middle);
	
	CUi_SpecialHit::SpecialHit_Desc Arg;

	switch (CPlayer_Manager::Get_Instance()->Get_WeaponType())
	{ 
		case CPlayer::PISTOL :
			if (pDesc->eHitType == HEAD_SHOT) { 
				m_pAnimationCom->Play_Animation(TEXT("HeadExplode_Backward"), 0.1f, false); 
			
				Arg.Hit = eSpecialHit::HEADSHOT;
				Arg.iCount = 4;
				m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_SpecialHit"), eUiRenderType::Render_NonBlend, &Arg);
			}
			else if (pDesc->eHitType == EGG_SHOT) { m_pAnimationCom->Play_Animation(TEXT("Death_Groinshot"), 0.1f, false); }
			else m_pAnimationCom->Play_Animation(TEXT("Death_Bodyshot"), 0.1f, false);
			break;
		case CPlayer::SHOTGUN :
			m_pAnimationCom->Play_Animation(TEXT("Death_Shotgun"), 0.1f, false);
			break;

		case CPlayer::KATANA:
		{
			m_bDestroyed = true;
			CEnemy_Corpse::ENEMYCORPSE_DESC desc;
			desc.eType = CHAINSAW;
			desc.isTop = true;
			CGameObject* pCorpseUp = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Corpse", L"Prototype_Corpse", &desc);

			_float3 vOffset = 0.25f * m_pTarget->Get_Transform()->Get_GroundRight();

			if (1 == CPlayer_Manager::Get_Instance()->Get_SlashCount())
				pCorpseUp->Get_Transform()->Set_Position(m_pTransformCom->Get_Pos() - vOffset);
			else
				pCorpseUp->Get_Transform()->Set_Position(m_pTransformCom->Get_Pos() + vOffset);

			//pCorpseUp->Get_Transform()->Add_Pos({ 0.f, 0.3f, 0.f });
			static_cast<CBoxCollider*>(pCorpseUp->Find_Component(L"Collider"))->Set_Scale({ 0.5f, 0.5f, 0.5f });
			static_cast<CBoxCollider*>(pCorpseUp->Find_Component(L"Collider"))->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());

			//vOffset = 0.12f * m_pTarget->Get_Transform()->Get_GroundRight();
			desc.isTop = false;
			CGameObject* pCorpseDown = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Corpse", L"Prototype_Corpse", &desc);
			if (1 == CPlayer_Manager::Get_Instance()->Get_SlashCount())
				pCorpseDown->Get_Transform()->Set_Position(m_pTransformCom->Get_Pos() - vOffset);
			else
				pCorpseDown->Get_Transform()->Set_Position(m_pTransformCom->Get_Pos() + vOffset);
			static_cast<CBoxCollider*>(pCorpseDown->Find_Component(L"Collider"))->Set_Scale({ 1.3f, 1.3f, 1.3f });
			static_cast<CBoxCollider*>(pCorpseDown->Find_Component(L"Collider"))->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());

			CGameObject* pHitEffect
				= m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Effect", L"Prototype_HitBloodKatanaEffect");
			pHitEffect->Get_Transform()->Set_Pos(m_pTransformCom->Get_Pos());
		}
			break;
		default:
			m_pAnimationCom->Play_Animation(TEXT("Death_Bodyshot"), 0.1f, false);
			break;
	}

	Arg.Hit = eSpecialHit::FINISHED;
	Arg.iCount = 4;
	m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_SpecialHit"), eUiRenderType::Render_NonBlend, &Arg);
}

void CChainsaw_Monster::SetState_Air()
{
	m_eState = STATE_AIR;

	m_pAnimationCom->Play_Animation(L"Air", 0.1f, false);
}

void CChainsaw_Monster::SetState_Landing()
{
	m_eState = STATE_LANDING;

	m_pAnimationCom->Play_Animation(L"Landing", 0.1f, false);
}

HRESULT CChainsaw_Monster::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

	m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component(LEVEL_STATIC, TEXT("Animation_Default"), TEXT("Animation"), this));

	m_pRigidbody = dynamic_cast<CRigidbody*>(Add_Component(LEVEL_STATIC, TEXT("Rigidbody_Default"), TEXT("Rigidbody"), m_pTransformCom));

	CBoxCollider::BOXCOLLISION_DESC pDesc;
	pDesc.vScale = { 0.5f, 1.3f, 0.5f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

	return S_OK;
}

HRESULT CChainsaw_Monster::Add_Textures()
{
	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Headshot"), TEXT("Death_Headshot"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Slash"), TEXT("Slash"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Bodyshot"), TEXT("Death_Bodyshot"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Pushed"), TEXT("Pushed"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Pushed_Recovery"), TEXT("Pushed_Recovery"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_HeadExplode_Backward"), TEXT("HeadExplode_Backward"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Death_Push_Floor"), TEXT("Death_Fly_Floor"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Start_Push_Floor"), TEXT("Death_Fly"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Death_Push_Wall"), TEXT("Death_Fly_Wall"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_GetUp"), TEXT("GetUp"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Groinshot"), TEXT("Death_Groinshot"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Idle_Up"), TEXT("Idle_Up"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Jump"), TEXT("Jump"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Death_Shotgun"), TEXT("Death_Shotgun"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Monster_Walk"), TEXT("Walk"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Air"), TEXT("Air"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Chainsaw_Landing"), TEXT("Landing"))))
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
