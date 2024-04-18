#include "Drone_Monster.h"

#include "GameInstance.h"

CDrone_Monster::CDrone_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CPawn{ pGraphic_Device }
{
}

CDrone_Monster::CDrone_Monster(const CDrone_Monster& rhs)
    : CPawn { rhs }
    , IsPlaying(false)
    , m_bIdle(true)
    , m_bAttack(false)
    , m_bReveal(false)
    , m_bDead(false)
    , m_bFlying(false)
    , m_fAttack_TimeGap(0.f)
    , m_fFlying_TimeGap(0.f)
{
}

HRESULT CDrone_Monster::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDrone_Monster::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    //memcpy(&m_Drone_Monster_Desc, pArg, sizeof m_Drone_Monster_Desc);

    if (FAILED(Add_Components()))
        return E_FAIL;

    if (FAILED(Add_Textures()))
        return E_FAIL;

    m_pFPS_Camera = dynamic_cast<CFPS_Camera*>(m_pGameInstance->Get_Instance()->Get_CurCamera());

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_Drone_Monster_Desc.vPosition);

    m_pAnimationCom->Play_Animation(TEXT("Drone_Monster_Idle"), 0.1f, true);

    return S_OK;
}

void CDrone_Monster::PriorityTick(_float fTimeDelta)
{

}

void CDrone_Monster::Tick(_float fTimeDelta)
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

void CDrone_Monster::LateTick(_float fTimeDelta)
{
    m_pTransformCom->Set_Billboard_Matrix(m_pFPS_Camera->Get_Billboard_Matrix());

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

    if (FAILED(End_RenderState()))
        return E_FAIL;

    return S_OK;
}

HRESULT CDrone_Monster::Add_Components()
{
    m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

    m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), &m_Drone_Monster_Desc));

    m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animation"), TEXT("Animation"), this));

    return S_OK;
}

HRESULT CDrone_Monster::Add_Textures()
{
    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Drone_Monster_Aim"), TEXT("Drone_Monster_Aim"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Drone_Monster_Attack"), TEXT("Drone_Monster_Attack"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Drone_Monster_Idle"), TEXT("Drone_Monster_Idle"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Drone_Monster_Reveal"), TEXT("Drone_Monster_Reveal"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Drone_Monster_Flyback"), TEXT("Drone_Monster_Flyback"))))
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

_bool CDrone_Monster::On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)
{
    _float4x4   WorldMatrixInverse = m_pTransformCom->Get_WorldMatrix_Inverse();
    _float3     vHitLocalPos = *D3DXVec3TransformCoord(&_float3(), &fHitWorldPos, &WorldMatrixInverse);

    if (-0.2f < vHitLocalPos.x && vHitLocalPos.x < 0.2f && -0.5f < vHitLocalPos.y && vHitLocalPos.y < -0.2f)
        m_eState = STATE_FLYBACK;
}

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

        // 애니메이션을 한 번 완전히 재생할 때까지 if문에 다시 들어오면 안 됨
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

    Safe_Release(m_pAnimationCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
}
