#include "White_Suit_Monster.h"
#include "GameInstance.h"
#include "Enemy_Bullet.h"
#include "PlayerManager.h"

CWhite_Suit_Monster::CWhite_Suit_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CPawn { pGraphic_Device }
{
}

CWhite_Suit_Monster::CWhite_Suit_Monster(const CWhite_Suit_Monster& rhs)
    : CPawn { rhs }
{
}

HRESULT CWhite_Suit_Monster::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CWhite_Suit_Monster::Initialize(void* pArg)
{
    if (FAILED(Add_Components()))
        return E_FAIL;

    if (FAILED(Add_Textures()))
        return E_FAIL;

    m_pCamera = dynamic_cast<CFPS_Camera*>(m_pGameInstance->Get_CurCamera());
    m_pTarget = CPlayer_Manager::Get_Instance()->Get_Player();
    Safe_AddRef(m_pTarget);

    _float3 Scale = {1.1f, 1.1f, 1.f };
    m_pTransformCom->Set_Scale(Scale);

    m_pRigidbody->Set_Friction(0.f);
    m_pRigidbody->Set_Velocity({ 0.f, 0.f, 0.f });
    m_pRigidbody->Set_Gravity({ 0.f, 0.f, 0.f });

    m_pAnimationCom->Play_Animation(TEXT("Idle"), 0.1f, true);

    m_strTag = "Monster";
    return S_OK;
}

void CWhite_Suit_Monster::PriorityTick(_float fTimeDelta)
{

}

void CWhite_Suit_Monster::Tick(_float fTimeDelta)
{
    Process_State(fTimeDelta);

    m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
    m_pRigidbody->Update(fTimeDelta);
    m_pAnimationCom->Update(fTimeDelta);
}

void CWhite_Suit_Monster::LateTick(_float fTimeDelta)
{
    m_pTransformCom->Set_Billboard_Matrix(m_pCamera->Get_Billboard_Matrix());

    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CWhite_Suit_Monster::Render()
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

HRESULT CWhite_Suit_Monster::Add_Components()
{
    m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

    m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

    m_pAnimationCom = dynamic_cast<CAnimation*>(Add_Component(LEVEL_STATIC, TEXT("Animation_Default"), TEXT("Animation"), this));

    m_pRigidbody = dynamic_cast<CRigidbody*>(Add_Component(LEVEL_STATIC, TEXT("Rigidbody_Default"), TEXT("Rigidbody"), m_pTransformCom));

    CBoxCollider::BOXCOLLISION_DESC pDesc;
    pDesc.vScale = { 0.5f, 1.f, 0.5f };

    m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

    return S_OK;
}

HRESULT CWhite_Suit_Monster::Add_Textures()
{
    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Idle"), TEXT("Idle"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Walk"), TEXT("Walk"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Jump"), TEXT("Jump"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Pushed"), TEXT("Pushed"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Pushed_Recovery"), TEXT("Pushed_Recovery"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Aim"), TEXT("Alert"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Shot"), TEXT("Shot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Hit"), TEXT("Hit"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Bodyshot"), TEXT("Death_Bodyshot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Headshot"), TEXT("Death_Headshot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Groinshot"), TEXT("Death_Eggshot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Death_Shotgun"), TEXT("Death_Shotgun"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_HeadExplode"), TEXT("HeadExplode"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Death_Push_Floor"), TEXT("Death_Push_Floor"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_White_Suit_Monster_Death_Push_Wall"), TEXT("Death_Push_Wall"))))
        return E_FAIL;

    return S_OK;
}

HRESULT CWhite_Suit_Monster::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CWhite_Suit_Monster::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}


void CWhite_Suit_Monster::On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)
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

_bool CWhite_Suit_Monster::Check_HeadShot(_float3 vHitLocalPos)
{
    return -0.1f < vHitLocalPos.x && vHitLocalPos.x < 0.1f && 0.375f <= vHitLocalPos.y && vHitLocalPos.y < 0.5f;
}

_bool CWhite_Suit_Monster::Check_BodyShot(_float3 vHitLocalPos)
{
    return (-0.15f < vHitLocalPos.x && vHitLocalPos.x < 0.15f) &&
        ((0.12f <= vHitLocalPos.y && vHitLocalPos.y < 0.375f) || (-0.5f < vHitLocalPos.y && vHitLocalPos.y < -0.2f));
}

_bool CWhite_Suit_Monster::Check_EggShot(_float3 vHitLocalPos)
{
    return  (-0.1f < vHitLocalPos.x && vHitLocalPos.x < 0.1f) && (-0.2f <= vHitLocalPos.y && vHitLocalPos.y < 0.12f);
}

void CWhite_Suit_Monster::Hit(void* pArg)
{
    ENEMYHIT_DESC* pDesc = (ENEMYHIT_DESC*)pArg;

    switch (pDesc->eHitType)
    {
    case CPawn::HEAD_SHOT:
    {
        m_fHp -= 5.f;
        break;
    }    
    case CPawn::BODY_SHOT:
    {
        m_fHp -= 3.f;
        break;
    }
    case CPawn::EGG_SHOT:
    {
        m_fHp -= 5.f;
        break;
    }
    }

    SetState_Death(pDesc);
}

void CWhite_Suit_Monster::Process_State(_float fTimeDelta)
{
    switch (m_eState)
    {
    case CWhite_Suit_Monster::STATE_IDLE:
        State_Idle();
        break;
    case CWhite_Suit_Monster::STATE_MOVE:
        State_Move();
        break;
    case CWhite_Suit_Monster::STATE_ALERT:
        State_Alert();
        break;
    case CWhite_Suit_Monster::STATE_PUSHED:
        State_Pushed();
        break;
    case CWhite_Suit_Monster::STATE_SHOT:
        State_Shot();
        break;
    case CWhite_Suit_Monster::STATE_JUMP:
        State_Jump();
        break;
    case CWhite_Suit_Monster::STATE_DEATH:
        State_Death(fTimeDelta);
        break;
    }
}

void CWhite_Suit_Monster::State_Idle()
{
    if (D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos())) < m_fPerceptionDist)
    {
        SetState_Alert();
    }
    else
    {
        SetState_Move();
    }
}

void CWhite_Suit_Monster::State_Move()
{
    if (D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos())) < m_fPerceptionDist)
    {
        SetState_Shot();
        return;
    }
        
   _float3 vToTargetDir = (m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos());
   vToTargetDir.y = 0.f;
   D3DXVec3Normalize(&vToTargetDir, &vToTargetDir);
   
   m_pRigidbody->Set_Velocity(vToTargetDir * m_fSpeed);
}

void CWhite_Suit_Monster::State_Alert()
{
    if (m_pAnimationCom->IsEndAnim())
    {
        SetState_Shot();
    }
}

void CWhite_Suit_Monster::State_Pushed()
{
    if (0.025f >= D3DXVec3Length(&m_pRigidbody->Get_Velocity()) && !m_bPushRecovery)
    {
        m_pRigidbody->Set_Friction(0.f);

        m_pAnimationCom->Play_Animation(L"Pushed_Recovery", 0.1f, false);
        m_bPushRecovery = true;
    }

    if (m_pAnimationCom->IsEndAnim())
    {
        SetState_Shot();
        m_bPushRecovery = false;
    }
}

void CWhite_Suit_Monster::State_Shot()
{
    if (m_pAnimationCom->IsEndAnim())
    {
        _float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));
        if (fTargetDist < 6.f)
        {
            int iRandNum = rand() % 5;
            if (iRandNum < 4)
                SetState_Shot();
            else
                SetState_Jump();
        }
        else if (fTargetDist > 9.f)
        {
            SetState_Idle();
        }

        else
        {
            SetState_Move();
        }
        
    }
}

void CWhite_Suit_Monster::State_Jump()
{
    if (m_pAnimationCom->IsEndAnim())
    {
        if (D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos())) < m_fPerceptionDist)
        {
            SetState_Shot();
        }
        else
        {
            SetState_Move();
        }
    }
}

void CWhite_Suit_Monster::State_Death(_float fTimeDelta)
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

void CWhite_Suit_Monster::SetState_Idle()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_IDLE;

    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CWhite_Suit_Monster::SetState_Move()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_MOVE;
    m_pAnimationCom->Play_Animation(L"Walk", 0.12f, true);
}

void CWhite_Suit_Monster::SetState_Alert()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_ALERT;
    m_pAnimationCom->Play_Animation(L"Alert", 0.1f, false);

    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CWhite_Suit_Monster::SetState_Pushed(_float3 vLook)
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_PUSHED;
    
    vLook.y = 0.f;
    D3DXVec3Normalize(&vLook, &vLook);

    m_pRigidbody->Set_Velocity(vLook * 4.f);
    m_pRigidbody->Set_Friction(4.f);

    m_pAnimationCom->Play_Animation(L"Pushed", 150200.f, false);
}

void CWhite_Suit_Monster::SetState_Shot()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_SHOT;
    m_pAnimationCom->Play_Animation(L"Shot", 0.15f, false);
    // 총알 생성

    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CWhite_Suit_Monster::SetState_Jump()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_JUMP;
    m_pAnimationCom->Play_Animation(L"Jump", 0.1f, false);

    _float fRand = _float(rand() % 2);
    if (0.f == fRand)
        fRand = -1.f;

    m_pRigidbody->Set_Velocity(m_pTarget->Get_Transform()->Get_Right() * m_fSpeed * fRand);
}

void CWhite_Suit_Monster::SetState_Death(ENEMYHIT_DESC* pDesc)
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_DEATH;
    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
    // TODO: 무기 타입에 따라서 모션 변경 
    switch (pDesc->eHitType)
    {
    case CPawn::HEAD_SHOT:
        m_pAnimationCom->Play_Animation(L"Death_Headshot", 0.1f, false);
        break;
    case CPawn::BODY_SHOT:
        m_pAnimationCom->Play_Animation(L"Death_Bodyshot", 0.1f, false);
        break;
    case CPawn::EGG_SHOT:
        m_pAnimationCom->Play_Animation(L"Death_Eggshot", 0.1f, false);
        break;
    }
}

CWhite_Suit_Monster* CWhite_Suit_Monster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CWhite_Suit_Monster* pInstance = new CWhite_Suit_Monster(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to create: CWhite_Suit_Monster"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWhite_Suit_Monster::Clone(void* pArg)
{
    CWhite_Suit_Monster* pInstance = new CWhite_Suit_Monster(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to create: CWhite_Suit_Monster"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWhite_Suit_Monster::Free()
{
    __super::Free();

    Safe_Release(m_pTarget);
    Safe_Release(m_pAnimationCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pBoxCollider);
    Safe_Release(m_pRigidbody);
}
