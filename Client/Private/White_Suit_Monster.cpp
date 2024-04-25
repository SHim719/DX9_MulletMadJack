#include "White_Suit_Monster.h"
#include "GameInstance.h"
#include "Enemy_Bullet.h"
#include "PlayerManager.h"
#include "CUi_SpecialHit.h"
#include "Enemy_Corpse.h"



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

    _float3 Scale = {1.3f, 1.3f, 1.f };
    m_pTransformCom->Set_Scale(Scale);
    m_pBoxCollider->Set_Scale({ 0.5f, 1.3f, 0.5f });

    m_pRigidbody->Set_Friction(0.f);
    m_pRigidbody->Set_Velocity({ 0.f, 0.f, 0.f });
    m_pRigidbody->Set_UseGravity(false);
    

    m_pAnimationCom->Play_Animation(TEXT("Idle"), 0.1f, true);

    m_strTag = "Monster";
    m_substrTag = "White_Suit_Monster";

    m_fHp = 10.f;

    return S_OK;
}

void CWhite_Suit_Monster::PriorityTick(_float fTimeDelta)
{
    m_bThisFrameHit = false;
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
    if (!m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 2.f))
        return;

    m_pTransformCom->Set_Billboard_Matrix(m_pCamera->Get_Billboard_Matrix());
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CWhite_Suit_Monster::Render()
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
    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Idle"), TEXT("Idle"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Walk"), TEXT("Walk"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Jump"), TEXT("Jump"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Pushed"), TEXT("Pushed"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Pushed_Recovery"), TEXT("Pushed_Recovery"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Aim"), TEXT("Alert"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Shot"), TEXT("Shot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Hit"), TEXT("Hit"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Bodyshot"), TEXT("Death_Bodyshot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Headshot"), TEXT("Death_Headshot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Groinshot"), TEXT("Death_Eggshot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Death_Shotgun"), TEXT("Death_Shotgun")))) 
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_HeadExplode"), TEXT("Head_Explode"))))
        return E_FAIL;
    
    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Fly"), TEXT("Death_Fly"))))
          return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Death_Push_Floor"), TEXT("Death_Fly_Floor"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Death_Push_Wall"), TEXT("Death_Fly_Wall"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Air"), TEXT("Air"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Landing"), TEXT("Landing"))))
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


_bool CWhite_Suit_Monster::On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)
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

    if (CPlayer_Manager::Get_Instance()->Get_WeaponType() != CPlayer::KATANA)
    {
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
    }
    else
    {
        pDesc->eHitType = BODY_SHOT;
        Hit(pDesc);
        return true;
    }
    

    return false;
}

void CWhite_Suit_Monster::OnCollisionEnter(CGameObject* pOther)
{
    if (STATE_FLY == m_eState && "Wall" == pOther->Get_Tag())
    {
        m_bWallColl = true;
    }

    if ("Floor" == pOther->Get_Tag()
        && m_pTransformCom->Get_Pos().y > pOther->Get_Transform()->Get_Pos().y)
    {
        m_pRigidbody->Set_Ground(true);
    }
}

_bool CWhite_Suit_Monster::Check_HeadShot(_float3 vHitLocalPos)
{
    return -0.083f < vHitLocalPos.x && vHitLocalPos.x < 0.0253f && 0.243f <= vHitLocalPos.y && vHitLocalPos.y <= 0.5f;
}

_bool CWhite_Suit_Monster::Check_BodyShot(_float3 vHitLocalPos)
{
    return (-0.164f < vHitLocalPos.x && vHitLocalPos.x < 0.164f) &&
        ((0.015f <= vHitLocalPos.y && vHitLocalPos.y < 0.243f) || (-0.5f < vHitLocalPos.y && vHitLocalPos.y < -0.086f));
}

_bool CWhite_Suit_Monster::Check_EggShot(_float3 vHitLocalPos)
{
    return  (-0.125f < vHitLocalPos.x && vHitLocalPos.x < 0.12f) && (-0.086f <= vHitLocalPos.y && vHitLocalPos.y < 0.015);
}

void CWhite_Suit_Monster::Hit(void* pArg)
{
    ENEMYHIT_DESC* pDesc = (ENEMYHIT_DESC*)pArg;

    CGameObject* pHitBlood = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Effect", L"Prototype_HitBlood");
    pHitBlood->Get_Transform()->Set_Position(pDesc->fHitWorldPos);

    _bool bHitByKatana = CPlayer_Manager::Get_Instance()->Get_Player_WeaponType() == CPlayer::KATANA;
    if (CPlayer_Manager::Get_Instance()->Get_WeaponType() == CPlayer::KATANA) {
        m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Blood"), false);
        m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Blood"), true);
    }

    switch (pDesc->eHitType)
    {
    case CPawn::HEAD_SHOT:
    {
        m_fHp -= 10.f;
        break;
    }    
    case CPawn::BODY_SHOT:
    {
        if (bHitByKatana)
        {
            /*m_pGameInstance->Play(L"White_Suit_Slashed", false);
            m_pGameInstance->SetVolume(L"White_Suit_Slashed", 1.f);
            m_pGameInstance->Play(L"White_Suit_Electrified", false);
            m_pGameInstance->SetVolume(L"White_Suit_Electrified", 1.f);
            m_pGameInstance->Play(L"Katana_Hit_Metal", false);
            m_pGameInstance->SetVolume(L"Katana_Hit_Metal", 1.f);*/

            m_pGameInstance->Play(L"Katana_Cutting_Flesh", false);
            m_pGameInstance->SetVolume(L"Katana_Cutting_Flesh", 0.5f);
            m_pGameInstance->Play(L"Blood_Splatter", false);
            m_pGameInstance->SetVolume(L"Blood_Splatter", 0.5f);

            m_fHp -= 8.f;
        }
        else
            m_fHp -= 5.f;
        break;
    }
    case CPawn::EGG_SHOT:
    {
        m_fHp -= 10.f;
        break;
    }
    }

    if (m_fHp <= 0.f)
    {
        if (!bHitByKatana)
        {
            SetState_Death(pDesc);
        }
            
        else
        {
            m_bDestroyed = true;

            CEnemy_Corpse::ENEMYCORPSE_DESC desc;
            desc.eType = WHITE_SUIT;
            desc.isTop = true;
            CGameObject* pCorpseUp = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Corpse", L"Prototype_Corpse", &desc);
            

            _float3 vOffset = 0.15f * m_pTarget->Get_Transform()->Get_GroundRight();
           
            if (1 == CPlayer_Manager::Get_Instance()->Get_SlashCount())
                pCorpseUp->Get_Transform()->Set_Position(m_pTransformCom->Get_Pos() - vOffset);
            else
                pCorpseUp->Get_Transform()->Set_Position(m_pTransformCom->Get_Pos() + vOffset);
               
            pCorpseUp->Get_Transform()->Add_Pos({ 0.f, 0.3f, 0.f });
            static_cast<CBoxCollider*>(pCorpseUp->Find_Component(L"Collider"))->Set_Scale({ 0.5f, 0.5f, 0.5f });



            //vOffset = 0.12f * m_pTarget->Get_Transform()->Get_GroundRight();
            desc.isTop = false;
            CGameObject* pCorpseDown = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Corpse", L"Prototype_Corpse", &desc);
            if (1 == CPlayer_Manager::Get_Instance()->Get_SlashCount())
                pCorpseDown->Get_Transform()->Set_Position(m_pTransformCom->Get_Pos() - vOffset);
            else
                pCorpseDown->Get_Transform()->Set_Position(m_pTransformCom->Get_Pos() + vOffset);
            static_cast<CBoxCollider*>(pCorpseDown->Find_Component(L"Collider"))->Set_Scale({ 1.3f, 1.3f, 1.f });

            CGameObject* pHitEffect
                = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Effect", L"Prototype_HitBloodKatanaEffect");
            pHitEffect->Get_Transform()->Set_Pos(m_pTransformCom->Get_Pos());
        }
        
    }
    else
    {
        SetState_Hit();
    }

    m_bThisFrameHit = true;
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
    case CWhite_Suit_Monster::STATE_HIT:
        State_Hit();
        break;
    case CWhite_Suit_Monster::STATE_EXECUTION:
        State_Execution();
        break;
    case CWhite_Suit_Monster::STATE_FLY:
        State_Fly(fTimeDelta);
        break;
    case CWhite_Suit_Monster::STATE_FLYDEATH:
        State_FlyDeath(fTimeDelta);
        break;
    case CWhite_Suit_Monster::STATE_DEATH:
        State_Death(fTimeDelta);
        break;
    case CWhite_Suit_Monster::STATE_AIR:
        State_Air();
        break;
    case CWhite_Suit_Monster::STATE_LANDING:
        State_Landing();
        break; 
    }
}

void CWhite_Suit_Monster::State_Idle()
{
    if (D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos())) < m_fPerceptionDist)
    {
        m_bFirstMeet = true;
        SetState_Alert();
    }
    else if (m_bFirstMeet)
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
    if (0.05f >= D3DXVec3Length(&m_pRigidbody->Get_Velocity()) && !m_bPushRecovery)
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
        if (m_bSlopeStand)
        {
            SetState_Shot();
            return;
        }
            
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
        if (D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos())) < 9.f)
        {
            SetState_Shot();
        }
        else
        {
            SetState_Move();
        }
    }
}

void CWhite_Suit_Monster::State_Execution()
{
  
}

void CWhite_Suit_Monster::State_Fly(_float fTimeDelta)
{
    m_fFlyTimeAcc += fTimeDelta;

    if (m_fFlyTimeAcc >= m_fFlyTime)
    {
        m_fFlyTimeAcc = 0.f;
        SetState_FlyDeath();
    }
}

void CWhite_Suit_Monster::State_FlyDeath(_float fTimeDelta)
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

void CWhite_Suit_Monster::State_Hit()
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

void CWhite_Suit_Monster::State_Air()
{
    if (m_pRigidbody->IsGround())
        SetState_Landing();
}

void CWhite_Suit_Monster::State_Landing()
{
    if (m_pAnimationCom->IsEndAnim())
        SetState_Move();
    
}

void CWhite_Suit_Monster::SetState_Idle()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_IDLE;

    m_pAnimationCom->Play_Animation(L"Idle", 0.1f, true);

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
    m_pAnimationCom->Play_Animation(L"Alert", 0.15f, false);

    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CWhite_Suit_Monster::SetState_Pushed(_float3 vLook)
{
    if (STATE_DEATH == m_eState || STATE_EXECUTION == m_eState
        || STATE_FLY == m_eState || STATE_FLYDEATH == m_eState)

    m_eState = STATE_PUSHED;
    
    vLook.y = 0.f;
    D3DXVec3Normalize(&vLook, &vLook);

    m_bPushRecovery = false;

    m_pRigidbody->Set_Velocity(vLook * 15.f);
    m_pRigidbody->Set_Friction(5.f);

    m_pAnimationCom->Play_Animation(L"Pushed", 150200.f, false);
}

void CWhite_Suit_Monster::SetState_Shot()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_SHOT;
    m_pGameInstance->Play(L"White_Suit_Shoot", false);
    m_pGameInstance->SetVolume(L"White_Suit_Shoot", 0.3f);

    m_pAnimationCom->Play_Animation(L"Shot", 0.15f, false);

    _float3 vBulletPos = m_pTransformCom->Get_Pos();
    vBulletPos.y += 0.25f;
    
    _float3 vPlayerPos = CPlayer_Manager::Get_Instance()->Get_Player()->Get_Transform()->Get_Pos();
    vPlayerPos.y -= 0.4f;
    CGameObject* pBullet =  m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Bullet", L"Prototype_Bullet");
    pBullet->Get_Transform()->Set_Position(vBulletPos);
    pBullet->Get_Transform()->Set_Target(m_pTransformCom->Get_Pos(), vPlayerPos);
    static_cast<CBoxCollider*>(pBullet->Find_Component(L"Collider"))->Update_BoxCollider(pBullet->Get_Transform()->Get_WorldMatrix());

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

_bool CWhite_Suit_Monster::SetState_Execution()
{
    if (STATE_DEATH == m_eState || STATE_EXECUTION == m_eState
        || STATE_FLY == m_eState || STATE_FLYDEATH == m_eState)
        return false;

    m_bCanIntersect = false;
    m_pBoxCollider->Set_Active(false);

    m_eState = STATE_EXECUTION;
    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));

    return true;
}

void CWhite_Suit_Monster::SetState_Fly(_float3 vLook)
{
    m_pBoxCollider->Set_Active(true);

    m_eState = STATE_FLY;
    
    m_pAnimationCom->Play_Animation(L"Death_Fly", 0.1f, false);

    vLook.y = 0.f;
    m_pRigidbody->Set_Velocity(vLook * 10.f);

    m_fFlyTimeAcc = 0.f;
}

void CWhite_Suit_Monster::SetState_FlyDeath()
{
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

void CWhite_Suit_Monster::SetState_Hit()
{
    m_eState = STATE_HIT;

    m_pAnimationCom->Play_Animation(L"Hit", 0.1f, false);
}

void CWhite_Suit_Monster::SetState_Death(ENEMYHIT_DESC* pDesc)
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_DEATH;
    m_pGameInstance->Play(L"White_Suit_Death", false);
    m_pGameInstance->SetVolume(L"White_Suit_Death", 0.3f);

    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
    m_bCanIntersect = false;
    m_pBoxCollider->Set_Active(false);

    Call_MonsterDieUi(eMonsterGrade::Middle);

    CPlayer::WEAPON_TYPE eWeaponType =CPlayer_Manager::Get_Instance()->Get_WeaponType();
    CUi_SpecialHit::SpecialHit_Desc Arg;
    switch (pDesc->eHitType)
    {
    case CPawn::HEAD_SHOT: {
        m_pAnimationCom->Play_Animation(L"Death_Headshot", 0.1f, false);
        if(eWeaponType == CPlayer::SHOTGUN) m_pAnimationCom->Play_Animation(L"Head_Explode", 0.1f, false);

        Arg.Hit = eSpecialHit::HEADSHOT;
        Arg.iCount = 4;
        m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_SpecialHit"), eUiRenderType::Render_NonBlend, &Arg);
        break;
    }
    case CPawn::BODY_SHOT: {
        m_pAnimationCom->Play_Animation(L"Death_Bodyshot", 0.1f, false);
        if (eWeaponType == CPlayer::SHOTGUN) m_pAnimationCom->Play_Animation(L"Death_Shotgun", 0.1f, false);
 
        Arg.Hit = eSpecialHit::FINISHED;
        Arg.iCount = 4;
        m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_SpecialHit"), eUiRenderType::Render_NonBlend, &Arg);
        break;
    }
    case CPawn::EGG_SHOT:
        m_pAnimationCom->Play_Animation(L"Death_Eggshot", 0.1f, false);
        break;
    }
}

void CWhite_Suit_Monster::SetState_Air()
{
    m_eState = STATE_AIR;

    m_pAnimationCom->Play_Animation(L"Air", 0.1f, false);

    m_pRigidbody->Set_UseGravity(true);
}

void CWhite_Suit_Monster::SetState_Landing()
{
    m_eState = STATE_LANDING;

    m_pAnimationCom->Play_Animation(L"Landing", 0.1f, false);

    m_pRigidbody->Set_UseGravity(false);
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
