#include "Artemis.h"
#include "GameInstance.h"
#include "PlayerManager.h"
#include "CUi_SpecialHit.h"
#include "MathManager.h"
#include "FPS_Camera.h"

CArtemis::CArtemis(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPawn{ pGraphic_Device }
{
}

CArtemis::CArtemis(const CArtemis& rhs)
	: CPawn{ rhs }
{
}

HRESULT CArtemis::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CArtemis::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Add_Textures()))
		return E_FAIL;

	m_pCamera = dynamic_cast<CFPS_Camera*>(m_pGameInstance->Get_Instance()->Get_CurCamera());
	m_pTarget = CPlayer_Manager::Get_Instance()->Get_Player();
	Safe_AddRef(m_pTarget);

	_float3 Scale = { 8.f, 8.f, 1.f };
	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_Pos(_float3(0.f, 0.5f, 10.f));

	m_pAnimationCom->Play_Animation(TEXT("IDLE"), 0.1f, true);
	m_pRigidbody->Set_UseGravity(false);

	m_strTag = "Monster";

	m_fHp = 200.f;
	m_fFlyTime = 1.5f;
	m_fSpeed = 2.f;
	return S_OK;
}

void CArtemis::PriorityTick(_float fTimeDelta)
{
	m_bThisFrameHit = false;
}

void CArtemis::Tick(_float fTimeDelta)
{
	Process_State(fTimeDelta);

	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
	m_pRigidbody->Update(fTimeDelta);
	m_pAnimationCom->Update(fTimeDelta);
}

void CArtemis::LateTick(_float fTimeDelta)
{
	if (!m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 2.f))
		return;

	m_pTransformCom->Set_Billboard_Matrix(m_pCamera->Get_Billboard_Matrix());

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CArtemis::Render()
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

_bool CArtemis::On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)
{
	if (STATE_DEATH == m_eState)
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

void CArtemis::OnCollisionEnter(CGameObject* pOther)
{
	if ("Player" == pOther->Get_Tag())
	{
		_float fDamage = 10.f;
		pOther->Hit(&fDamage);
	}
}

void CArtemis::Hit(void* pArg)
{
	ENEMYHIT_DESC* pDesc = (ENEMYHIT_DESC*)pArg;

	m_bThisFrameHit = true;

	CGameObject* pHitBlood = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Effect", L"Prototype_HitBlood");
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

void CArtemis::Process_State(_float fTimeDelta)
{
	switch (m_eState)
	{
	case CArtemis::STATE_IDLE:
		State_Idle(fTimeDelta);
		break;
	case CArtemis::STATE_MOVE:
		State_Move(fTimeDelta);
		break;
	case CArtemis::STATE_DEATH:
		State_Death();
		break;
	}
}

void CArtemis::State_Idle(float _fTimeDelta)
{
	_float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));

	if (m_fMoveDelay <= 0.f)
	{
		_float vToTargetDirX = CMath_Manager::Get_Instance()->Random_Float(-15, 15);
		_float vToTargetDirY = CMath_Manager::Get_Instance()->Random_Float(15, 25);
		_float vToTargetDirZ = CMath_Manager::Get_Instance()->Random_Float(-5, 25);

		_float3 vToTargetDir = { vToTargetDirX, vToTargetDirY, vToTargetDirZ };

		m_pTransformCom->Set_Target(m_pTransformCom->Get_Pos(), vToTargetDir);
		m_pTransformCom->Set_Speed(m_fMinSpeed);
		SetState_Move();
		m_fMoveDelay = m_fMoveDelayMax;
	}
	else {
		m_fMoveDelay -= _fTimeDelta;
	}
}

void CArtemis::State_Move(float _fTimeDelta)
{
	_float fTargetDist = D3DXVec3Length(&(m_vTargetPosition - m_pTransformCom->Get_Pos()));

	
		if (fTargetDist < 1.f)
		{
			m_fMoveDelay = 1.f;
			SetState_Idle();
			return;
		}
		else {
			D3DXVec3Normalize(&m_vTargetDirection, &(m_vTargetPosition - m_pTransformCom->Get_Pos()));
			m_pTransformCom->Set_Pos(m_pTransformCom->Get_Pos() + m_vTargetDirection * m_pTransformCom->Get_Speed() * _fTimeDelta);
			m_pTransformCom->Set_Speed(m_pTransformCom->Get_Speed() + 5.f * _fTimeDelta);
			if (m_pTransformCom->Get_Speed() >= m_fMaxSpeed) m_pTransformCom->Set_Speed(m_fMaxSpeed);
		}


}


void CArtemis::State_Death()
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

void CArtemis::SetState_Idle()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_IDLE;

	m_pAnimationCom->Play_Animation(L"Move", 0.1f, true);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CArtemis::SetState_Move()
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_MOVE;

	m_pAnimationCom->Play_Animation(L"IDLE", 0.1f, true);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CArtemis::SetState_Death(ENEMYHIT_DESC* pDesc)
{
	if (STATE_DEATH == m_eState)
		return;

	m_eState = STATE_DEATH;
	m_pGameInstance->Play(L"Drone_Death", false);
	m_pGameInstance->SetVolume(L"Drone_Death", 0.5f);

	m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
	m_pAnimationCom->Play_Animation(L"Death", 0.1f, false);
}

HRESULT CArtemis::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

	m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component(LEVEL_STATIC, TEXT("Animation_Default"), TEXT("Animation"), this));

	m_pRigidbody = dynamic_cast<CRigidbody*>(Add_Component(LEVEL_STATIC, TEXT("Rigidbody_Default"), TEXT("Rigidbody"), m_pTransformCom));

	CBoxCollider::BOXCOLLISION_DESC pDesc;
	pDesc.vScale = { 3.f, 3.f, 3.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

	return S_OK;
}

HRESULT CArtemis::Add_Textures()
{
	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Artemis_Texture"), TEXT("IDLE"))))
		return E_FAIL;

	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Artemis_Attack_Texture"), TEXT("Attack"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CArtemis::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CArtemis::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CArtemis* CArtemis::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CArtemis* pInstance = new CArtemis(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to create: CArtemis"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CArtemis::Clone(void* pArg)
{
	CArtemis* pInstance = new CArtemis(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to create: CArtemis"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArtemis::Free()
{
	__super::Free();

	Safe_Release(m_pTarget);
	Safe_Release(m_pAnimationCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRigidbody);
}
