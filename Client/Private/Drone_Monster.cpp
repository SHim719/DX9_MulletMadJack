#include "Drone_Monster.h"
#include "GameInstance.h"
#include "PlayerManager.h"

CDrone_Monster::CDrone_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CPawn{ pGraphic_Device }
{
}

CDrone_Monster::CDrone_Monster(const CDrone_Monster& rhs)
    : CPawn { rhs }
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

    m_pCamera = dynamic_cast<CFPS_Camera*>(m_pGameInstance->Get_CurCamera());
    m_pTarget = CPlayer_Manager::Get_Instance()->Get_Player();
    Safe_AddRef(m_pTarget);

    _float3 Scale = { 1.f, 1.f, 1.f };
    m_pTransformCom->Set_Scale(Scale);

    _float3 vPos = { 1.f, 1.6f, 2.7f };
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, &vPos);

    m_pRigidbody->Set_Friction(0.f);
    m_pRigidbody->Set_Velocity({ 0.f, 0.f, 0.f });
    m_pRigidbody->Set_Gravity({ 0.f, 0.f, 0.f });

    m_pAnimationCom->Play_Animation(TEXT("Idle"), 0.1f, true);

    m_strTag = "Monster";

    return S_OK;
}

void CDrone_Monster::PriorityTick(_float fTimeDelta)
{

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
    if (STATE_DEATH == m_eState)
        return;

    _float4x4   WorldMatrixInverse = m_pTransformCom->Get_WorldMatrix_Inverse();
    _float3     vHitLocalPos = *D3DXVec3TransformCoord(&_float3(), &fHitWorldPos, &WorldMatrixInverse);

    ENEMYHIT_DESC* pDesc = (ENEMYHIT_DESC*)pArg;
    pDesc->fHitWorldPos = fHitWorldPos;
    pDesc->fDist = fDist;

    if (Check_Hit(vHitLocalPos))
    {
        pDesc->eHitType = HEAD_SHOT;
        Hit(pDesc);
    }
}

void CDrone_Monster::OnCollisionEnter(CGameObject* pOther)
{
    // �÷��̾���� �浹 ����

}

_bool CDrone_Monster::Check_Hit(_float3 vHitLocalPos)
{
    return -0.13f < vHitLocalPos.x && vHitLocalPos.x < 0.13f && -0.5f <= vHitLocalPos.y && vHitLocalPos.y < -0.17f;
}

void CDrone_Monster::Hit(void* pArg)
{
    ENEMYHIT_DESC* pDesc = (ENEMYHIT_DESC*)pArg;

    switch (pDesc->eHitType)
    {
    case CPawn::HEAD_SHOT:
        m_fHp -= 1.f;
        break;
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

    case CDrone_Monster::STATE_AIM:
        State_Aim();
        break;

    case CDrone_Monster::STATE_ALERT:
        State_Alert();
        break;

    case CDrone_Monster::STATE_ATTACK:
        State_Attack(fTimeDelta);
        break;

    case CDrone_Monster::STATE_DEATH:
        State_Death(fTimeDelta);
        break;
    }
}

void CDrone_Monster::State_Idle()
{
    _float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));

    if (fTargetDist < m_fPerceptionDist)
    {
        SetState_Alert();
    }
    else
    {
        if (m_pAnimationCom->IsEndAnim())
        {
            SetState_Idle();
        }
    }
}

void CDrone_Monster::State_Aim()
{
    // �÷��̾�� �浹���� �� ƨ���� ������ ���� �ʿ�

    if (m_pAnimationCom->IsEndAnim())
    {
        m_fSpeed = 0.f;

        SetState_Attack();
    }
}

void CDrone_Monster::State_Alert()
{
    if (m_pAnimationCom->IsEndAnim())
    {
        SetState_Attack();
    }
}

void CDrone_Monster::State_Attack(_float fTimeDelta)
{
    if (m_pAnimationCom->IsEndAnim())
    {
        _float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));

        if (fTargetDist < 9.f)
        {
            SetState_Attack();
        }
        else if (fTargetDist >= 9.f)
        {
            SetState_Idle();
        }
    }

    _float3 vToTargetDir = (m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos());
    vToTargetDir.y = 0.f;
    D3DXVec3Normalize(&vToTargetDir, &vToTargetDir);

    m_fSpeed += fTimeDelta;
    m_pRigidbody->Set_Velocity(vToTargetDir * m_fSpeed);

    // �÷��̾ ���� �����ϴ� ���� �ʿ�
}

void CDrone_Monster::State_Death(_float fTimeDelta)
{
    if (m_pAnimationCom->IsEndAnim())
    {
        m_fDeathTime -= fTimeDelta;

        if (m_fDeathTime <= 0.f)
            m_bDestroyed = true;
    }

    // �ڷ� ���ư��鼭 ������ �������� ���� �ʿ�
}

void CDrone_Monster::SetState_Idle()
{
    if (STATE_DEATH == m_eState)
        return;

    m_eState = STATE_IDLE;
    m_bPerceivedPlayer = false;

    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));

    m_pAnimationCom->Play_Animation(L"Idle", 0.15f, true);
}

void CDrone_Monster::SetState_Aim()
{
    if (STATE_DEATH == m_eState)
        return;

    m_eState = STATE_AIM;

    _float3 vLook = m_pTarget->Get_Transform()->Get_Look();
    vLook.y = 0.f;
    D3DXVec3Normalize(&vLook, &vLook);

    m_pRigidbody->Set_Velocity(vLook * m_fSpeed);
    m_pRigidbody->Set_Friction(3.f);

    m_pAnimationCom->Play_Animation(L"Aim", 0.15f, true);
}

void CDrone_Monster::SetState_Alert()
{
    if (STATE_DEATH == m_eState)
        return;

    m_eState = STATE_ALERT;
    m_bPerceivedPlayer = true;

    m_pAnimationCom->Play_Animation(L"Reveal", 0.1f, false);

    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CDrone_Monster::SetState_Attack()
{
    if (STATE_DEATH == m_eState)
        return;

    m_eState = STATE_ATTACK;

    m_pAnimationCom->Play_Animation(L"Attack", 0.1f, false);
}

void CDrone_Monster::SetState_Death(ENEMYHIT_DESC* pDesc)
{
    if (STATE_DEATH == m_eState)
        return;

    m_eState = STATE_DEATH;

    m_pAnimationCom->Play_Animation(L"Flyback", 0.1f, false);
}

HRESULT CDrone_Monster::Add_Components()
{
    m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

    m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

    m_pAnimationCom = dynamic_cast<CAnimation*>(Add_Component(LEVEL_STATIC, TEXT("Animation_Default"), TEXT("Animation"), this));

    m_pRigidbody = dynamic_cast<CRigidbody*>(Add_Component(LEVEL_STATIC, TEXT("Rigidbody_Default"), TEXT("Rigidbody"), m_pTransformCom));

    CBoxCollider::BOXCOLLISION_DESC pDesc;
    pDesc.vScale = { 0.5f, 0.5f, 0.5f };

    m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

    return S_OK;
}

HRESULT CDrone_Monster::Add_Textures()
{
    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Monster_Aim"), TEXT("Aim"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Monster_Attack"), TEXT("Attack"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Monster_Idle"), TEXT("Idle"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Monster_Reveal"), TEXT("Reveal"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Drone_Monster_Flyback"), TEXT("Flyback"))))
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

void CDrone_Monster::Set_Motions(_float fTimeDelta)
{
    switch (m_eState)
    {
    case STATE_IDLE:
        break;

    case STATE_REVEAL:
        break;

    case STATE_FLYING:
        m_pTransformCom->Go_Floor_Backward(fTimeDelta);
        break;

    case STATE_ATTACK:
        break;

    case STATE_FLYBACK:
        break;

    default:
        break;
    }
}

//_bool CDrone_Monster::On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)
//{
//    _float4x4   WorldMatrixInverse = m_pTransformCom->Get_WorldMatrix_Inverse();
//    _float3     vHitLocalPos = *D3DXVec3TransformCoord(&_float3(), &fHitWorldPos, &WorldMatrixInverse);
//
//    if (-0.2f < vHitLocalPos.x && vHitLocalPos.x < 0.2f && -0.5f < vHitLocalPos.y && vHitLocalPos.y < -0.2f)
//        m_eState = STATE_FLYBACK;
//}

void CDrone_Monster::Decide_Pawn_Motions(_float fTimeDelta)
{
    Pawn_Reveal_Motion(fTimeDelta);
    Pawn_Moving_Motion(fTimeDelta);
    Pawn_Attack_Motion(fTimeDelta);
    Pawn_Flyback_Motion(fTimeDelta);
}

void CDrone_Monster::Pawn_Reveal_Motion(_float fTimeDelta)
{
    if (D3DXVec3Length(&(m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION) - m_Drone_Monster_Desc.vPosition)) < 5.f
        && !m_bDead && !m_bReveal && !m_bAttack && !IsPlaying)
    {
        m_eState = STATE_REVEAL;

        m_bReveal = true;
        IsPlaying = m_bReveal;

        // �ִϸ��̼��� �� �� ������ ����� ������ if���� �ٽ� ������ �� ��
        m_pAnimationCom->Play_Animation(TEXT("Drone_Monster_Reveal"), 0.1f, false);
    }
}

void CDrone_Monster::Pawn_Moving_Motion(_float fTimeDelta)
{
    m_fFlying_TimeGap += fTimeDelta;

    if (D3DXVec3Length(&(m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION) -
        m_pTransformCom->Get_State(CTransform::STATE_POSITION))) >= 1.f && !m_bIdle)
    {
        m_bFlying = true;
    }

    if (m_bFlying && !m_bIdle && !m_bDead && !m_bAttack && !IsPlaying)
    {
        m_eState = STATE_FLYING;

        IsPlaying = m_bFlying;

        m_fFlying_TimeGap += fTimeDelta;

        m_pAnimationCom->Play_Animation(TEXT("Chainsaw_Monster_Walk"), 0.1f, false);

        if (m_fFlying_TimeGap > 1.f)
        {
            m_bFlying = false;

            m_eState = STATE_END;

            m_fFlying_TimeGap = 0.f;
        }
    }
}

void CDrone_Monster::Pawn_Attack_Motion(_float fTimeDelta)
{
    m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION);

    m_fAttack_TimeGap += fTimeDelta;

    if (D3DXVec3Length(&(m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION) - m_Drone_Monster_Desc.vPosition)) >= 5.f)
        return;

    if (D3DXVec3Length(&(m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION) - m_Drone_Monster_Desc.vPosition)) < 5.f
        && !m_bDead && !m_bFlying && !IsPlaying)
    {
        m_eState = STATE_ATTACK;

        m_bAttack = true;
        IsPlaying = m_bAttack;

        m_pAnimationCom->Play_Animation(TEXT("Drone_Monster_Attack"), 0.1f, false);
    }
}

void CDrone_Monster::Pawn_Flyback_Motion(_float fTimeDelta)
{
    if (m_eState == STATE_FLYBACK && !m_bDead)
    {
        m_bDead = true;

        m_pAnimationCom->Play_Animation(TEXT("Drone_Monster_Flyback"), 0.1f, false);
    }
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
