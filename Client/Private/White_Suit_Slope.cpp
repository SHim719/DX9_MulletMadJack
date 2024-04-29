#include "White_Suit_Slope.h"
#include "GameInstance.h"
#include "Enemy_Bullet.h"
#include "PlayerManager.h"
#include "CUi_SpecialHit.h"
#include "Enemy_Corpse.h"

CWhite_Suit_Slope::CWhite_Suit_Slope(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CPawn{ pGraphic_Device }
{
}

CWhite_Suit_Slope::CWhite_Suit_Slope(const CWhite_Suit_Slope& rhs)
    : CPawn{ rhs }
{
}

HRESULT CWhite_Suit_Slope::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CWhite_Suit_Slope::Initialize(void* pArg)
{
    if (FAILED(Add_Components()))
        return E_FAIL;

    if (FAILED(Add_Textures()))
        return E_FAIL;

    m_pCamera = dynamic_cast<CFPS_Camera*>(m_pGameInstance->Get_CurCamera());
    m_pTarget = CPlayer_Manager::Get_Instance()->Get_Player();
    Safe_AddRef(m_pTarget);

    _float3 Scale = { 1.3f, 1.3f, 1.f };
    m_pTransformCom->Set_Scale(Scale);
    m_pBoxCollider->Set_Scale({ 0.5f, 1.3f, 0.5f });

    m_pRigidbody->Set_Friction(0.f);
    m_pRigidbody->Set_Velocity({ 0.f, 0.f, 0.f });
    m_pRigidbody->Set_UseGravity(false);

    m_pAnimationCom->Play_Animation(TEXT("Idle"), 0.1f, true);

    m_strTag = "Monster";
    m_substrTag = "White_Suit_Slope";

    m_fHp = 10.f;
    m_fPerceptionDist = 9.f;

    return S_OK;
}

void CWhite_Suit_Slope::PriorityTick(_float fTimeDelta)
{
    m_bThisFrameHit = false;
}

void CWhite_Suit_Slope::Tick(_float fTimeDelta)
{
    Process_State(fTimeDelta);

    m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
    m_pRigidbody->Update(fTimeDelta);
    m_pAnimationCom->Update(fTimeDelta);
}

void CWhite_Suit_Slope::LateTick(_float fTimeDelta)
{
    if (!m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 2.f))
        return;

    m_pTransformCom->Set_Billboard_Matrix(m_pCamera->Get_Billboard_Matrix());
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CWhite_Suit_Slope::Render()
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

HRESULT CWhite_Suit_Slope::Add_Components()
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

HRESULT CWhite_Suit_Slope::Add_Textures()
{
    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_White_Suit_Monster_Idle"), TEXT("Idle"))))
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

    return S_OK;
}

HRESULT CWhite_Suit_Slope::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CWhite_Suit_Slope::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    return S_OK;
}


_bool CWhite_Suit_Slope::On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)
{
    if (STATE_DEATH == m_eState || STATE_FLY == m_eState)
        return false;

    if (m_bThisFrameHit)
        return true;

    if (CPlayer_Manager::Get_Instance()->Get_WeaponType() == CPlayer::KATANA && CPlayer_Manager::Get_Instance()->Get_PlayerToTarget(m_pTransformCom->Get_Pos()) > 1.5f)
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

void CWhite_Suit_Slope::OnTriggerEnter(CGameObject* pOther)
{
    if (CPlayer::SLOPE_STATE == static_cast<CPlayer*>(pOther)->Get_PlayerState())
    {
        SetState_Fly(_float3(0.f, 1.f, 0.f));
        static_cast<CPlayer*>(pOther)->Camera_Shake_Order(1000000.f, 0.2f);
        m_pGameInstance->Play(L"Player_Kick_Hit", false);
        m_pGameInstance->SetVolume(L"Player_Kick_Hit", 1.f);
    }
}

_bool CWhite_Suit_Slope::Check_HeadShot(_float3 vHitLocalPos)
{
    return -0.083f < vHitLocalPos.x && vHitLocalPos.x < 0.0253f && 0.243f <= vHitLocalPos.y && vHitLocalPos.y <= 0.5f;
}

_bool CWhite_Suit_Slope::Check_BodyShot(_float3 vHitLocalPos)
{
    return (-0.164f < vHitLocalPos.x && vHitLocalPos.x < 0.164f) &&
        ((0.015f <= vHitLocalPos.y && vHitLocalPos.y < 0.243f) || (-0.5f < vHitLocalPos.y && vHitLocalPos.y < -0.086f));
}

_bool CWhite_Suit_Slope::Check_EggShot(_float3 vHitLocalPos)
{
    return  (-0.125f < vHitLocalPos.x && vHitLocalPos.x < 0.12f) && (-0.086f <= vHitLocalPos.y && vHitLocalPos.y < 0.015);
}

void CWhite_Suit_Slope::Hit(void* pArg)
{
    ENEMYHIT_DESC* pDesc = (ENEMYHIT_DESC*)pArg;

    CGameObject* pHitBlood = m_pGameInstance->Add_Clone(LEVEL_STATIC, L"Effect", L"Prototype_HitBlood");
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
            m_fHp -= 8.f;
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

            Call_MonsterDieUi(eMonsterGrade::Middle);
            m_pGameInstance->Play(L"White_Suit_Death", false);
            m_pGameInstance->SetVolume(L"White_Suit_Death", 0.3f);

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
        }

    }
    else
    {
        SetState_Hit();
    }

    m_bThisFrameHit = true;
}

void CWhite_Suit_Slope::Process_State(_float fTimeDelta)
{
    switch (m_eState)
    {
    case CWhite_Suit_Slope::STATE_IDLE:
        State_Idle();
        break;
    case CWhite_Suit_Slope::STATE_ALERT:
        State_Alert();
        break;
    case CWhite_Suit_Slope::STATE_SHOT:
        State_Shot();
        break;
    case CWhite_Suit_Slope::STATE_HIT:
        State_Hit();
        break;
    case CWhite_Suit_Slope::STATE_FLY:
        State_Fly(fTimeDelta);
        break;
    case CWhite_Suit_Slope::STATE_DEATH:
        State_Death(fTimeDelta);
        break;

    }
}

void CWhite_Suit_Slope::State_Idle()
{
    if (D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos())) < m_fPerceptionDist)
    {
        m_bFirstMeet = true;
        SetState_Alert();
    }
}

void CWhite_Suit_Slope::State_Alert()
{
    if (m_pAnimationCom->IsEndAnim())
    {
        SetState_Shot();
    }
}

void CWhite_Suit_Slope::State_Shot()
{
    if (m_pAnimationCom->IsEndAnim())
    {
        _float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));
        if (fTargetDist < 6.f)
        {
            SetState_Shot();
        }
        else if (fTargetDist > 9.f)
        {
            SetState_Idle();
        }
    }
}


void CWhite_Suit_Slope::State_Fly(_float fTimeDelta)
{
    m_fFlyTimeAcc += fTimeDelta;

    if (m_fFlyTimeAcc >= m_fFlyTime)
    {
        m_fFlyTimeAcc = 0.f;
        m_bDestroyed = true;
    }
}


void CWhite_Suit_Slope::State_Hit()
{
    if (m_pAnimationCom->IsEndAnim())
    {
        _float fTargetDist = D3DXVec3Length(&(m_pTarget->Get_Transform()->Get_Pos() - m_pTransformCom->Get_Pos()));
        if (fTargetDist < 6.f)
        {
            SetState_Shot();
        }
        else if (fTargetDist > 9.f)
        {
            SetState_Idle();
        }

    }
}

void CWhite_Suit_Slope::State_Death(_float fTimeDelta)
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

void CWhite_Suit_Slope::SetState_Idle()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_IDLE;

    m_pAnimationCom->Play_Animation(L"Idle", 0.1f, true);

    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}


void CWhite_Suit_Slope::SetState_Alert()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_ALERT;
    m_pAnimationCom->Play_Animation(L"Alert", 0.15f, false);

    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}


void CWhite_Suit_Slope::SetState_Shot()
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
    CGameObject* pBullet = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Bullet", L"Prototype_Bullet");
    pBullet->Get_Transform()->Set_Position(vBulletPos);
    pBullet->Get_Transform()->Set_Target(m_pTransformCom->Get_Pos(), vPlayerPos);
    static_cast<CBoxCollider*>(pBullet->Find_Component(L"Collider"))->Update_BoxCollider(pBullet->Get_Transform()->Get_WorldMatrix());

    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}


void CWhite_Suit_Slope::SetState_Fly(_float3 vLook)
{
    m_pBoxCollider->Set_Active(true);

    m_eState = STATE_FLY;

    m_pGameInstance->Play(L"White_Suit_Death", false);
    m_pGameInstance->SetVolume(L"White_Suit_Death", 0.3f);

    m_pAnimationCom->Play_Animation(L"Death_Fly", 0.1f, false);

    m_pRigidbody->Set_Velocity(vLook * 10.f);

    m_fFlyTimeAcc = 0.f;

    CUi_SpecialHit::SpecialHit_Desc Arg;
    Arg.Hit = eSpecialHit::FINISHED;
    Arg.iCount = 4;
    m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_SpecialHit"), eUiRenderType::Render_NonBlend, &Arg);
}



void CWhite_Suit_Slope::SetState_Hit()
{
    m_eState = STATE_HIT;

    m_pAnimationCom->Play_Animation(L"Hit", 0.1f, false);
}

void CWhite_Suit_Slope::SetState_Death(ENEMYHIT_DESC* pDesc)
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

    CPlayer::WEAPON_TYPE eWeaponType = CPlayer_Manager::Get_Instance()->Get_WeaponType();
    CUi_SpecialHit::SpecialHit_Desc Arg;
    switch (pDesc->eHitType)
    {
    case CPawn::HEAD_SHOT: {
        m_pAnimationCom->Play_Animation(L"Death_Headshot", 0.1f, false);
        if (eWeaponType == CPlayer::SHOTGUN) m_pAnimationCom->Play_Animation(L"Head_Explode", 0.1f, false);

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

CWhite_Suit_Slope* CWhite_Suit_Slope::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CWhite_Suit_Slope* pInstance = new CWhite_Suit_Slope(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to create: CWhite_Suit_Slope"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWhite_Suit_Slope::Clone(void* pArg)
{
    CWhite_Suit_Slope* pInstance = new CWhite_Suit_Slope(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to create: CWhite_Suit_Slope"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWhite_Suit_Slope::Free()
{
    __super::Free();

    Safe_Release(m_pTarget);
    Safe_Release(m_pAnimationCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pBoxCollider);
    Safe_Release(m_pRigidbody);
}
