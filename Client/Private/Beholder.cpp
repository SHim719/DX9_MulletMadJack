#include "Beholder.h"
#include "GameInstance.h"
#include "Enemy_Bullet.h"
#include "PlayerManager.h"
#include "CUi_SpecialHit.h"
#include "Enemy_Corpse.h"
#include "FPS_Camera.h"
#include <math.h>
#define PI 3.14159265
double rad2deg(double radian);
double deg2rad(double degree);
double rad2deg(double radian)
{
    return radian * 180 / PI;
}
double deg2rad(double degree)
{
    return degree * PI / 180;
}

CBeholder::CBeholder(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CPawn{ pGraphic_Device }
{
}

CBeholder::CBeholder(const CBeholder& rhs)
    : CPawn{ rhs }
{
}

HRESULT CBeholder::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBeholder::Initialize(void* pArg)
{
    if (FAILED(Add_Components()))
        return E_FAIL;

    if (FAILED(Add_Textures()))
        return E_FAIL;

    m_pCamera = dynamic_cast<CFPS_Camera*>(m_pGameInstance->Get_CurCamera());
    m_pTarget = CPlayer_Manager::Get_Instance()->Get_Player();
    Safe_AddRef(m_pTarget);

    _float3 Scale = { 3.f, 3.f, 1.f };
    m_pTransformCom->Set_Scale(Scale);
    m_pTransformCom->Set_Position(_float3(0.f, 5.f, 10.f));
    m_pBoxCollider->Set_Scale({ 3.f, 3.f, 3.f });

    m_pRigidbody->Set_Friction(0.f);
    m_pRigidbody->Set_Velocity({ 0.f, 0.f, 0.f });
    m_pRigidbody->Set_UseGravity(false);

    m_pAnimationCom->Play_Animation(TEXT("Idle"), 0.1f, true);

    m_strTag = "Monster";
    m_substrTag = "Beholder";

    //m_fHp = 1000.f;
    //jeongtest
    m_fHp = 100.f;
    
    return S_OK;
}

void CBeholder::PriorityTick(_float fTimeDelta)
{
    m_bThisFrameHit = false;
}

void CBeholder::Tick(_float fTimeDelta)
{
    Process_State(fTimeDelta);
    Set_PatternEndCheck();

    m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
    m_pRigidbody->Update(fTimeDelta);
    m_pAnimationCom->Update(fTimeDelta);

    if (m_pGameInstance->GetKeyDown(eKeyCode::Enter)){
        
        //Pattern
        //Player_Tracking_Laser();
    	//All_Round_Laser();
        //All_Round_Laser_LandMine();
        //Shoot();
        //AirStrike();
        
        //m_ePattern = PATTERN_ROUND_AIRSTRIKE;
        //Set_PatternStart();
        //CPlayer_Manager::Get_Instance()->Set_RoundPattern(true);

        /*m_ePattern = PATTERN_ROUND_AIRSTRIKE_BOOM;
        Set_PatternStart();
        CPlayer_Manager::Get_Instance()->Set_RoundPattern(true);*/
    }
    ActivePattern(fTimeDelta);
    PatternState(fTimeDelta);

    m_fRecentHitTime += fTimeDelta;
}

void CBeholder::LateTick(_float fTimeDelta)
{
    if (!m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 2.f))
        return;

    m_pTransformCom->Set_Billboard_Matrix(m_pCamera->Get_Billboard_Matrix());
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBeholder::Render()
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

HRESULT CBeholder::Add_Components()
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

HRESULT CBeholder::Add_Textures()
{
    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_BOSS, TEXT("Beholder_Idle_Texture"), TEXT("Idle"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_BOSS, TEXT("Beholder_Damaged_Texture"), TEXT("Hit"))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBeholder::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CBeholder::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    return S_OK;
}


_bool CBeholder::On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)
{
    if (STATE_DEATH == m_eState || STATE_FLY == m_eState || STATE_EXECUTION == m_eState
        || STATE_FLYDEATH == m_eState)
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
        pDesc->eHitType = BODY_SHOT;
        Hit(pDesc);
        return true;
    }
    else
    {
        pDesc->eHitType = BODY_SHOT;
        Hit(pDesc);
        return true;
    }


    return false;
}

void CBeholder::OnCollisionEnter(CGameObject* pOther)
{
    if (STATE_FLY == m_eState && "Wall" == pOther->Get_Tag())
    {
        m_bWallColl = true;
    }
}

_bool CBeholder::Check_HeadShot(_float3 vHitLocalPos)
{
    return -0.083f < vHitLocalPos.x && vHitLocalPos.x < 0.0253f && 0.243f <= vHitLocalPos.y && vHitLocalPos.y <= 0.5f;
}

_bool CBeholder::Check_BodyShot(_float3 vHitLocalPos)
{
    return (-0.164f < vHitLocalPos.x && vHitLocalPos.x < 0.164f) &&
        ((0.015f <= vHitLocalPos.y && vHitLocalPos.y < 0.243f) || (-0.5f < vHitLocalPos.y && vHitLocalPos.y < -0.086f));
}

_bool CBeholder::Check_EggShot(_float3 vHitLocalPos)
{
    return  (-0.125f < vHitLocalPos.x && vHitLocalPos.x < 0.12f) && (-0.086f <= vHitLocalPos.y && vHitLocalPos.y < 0.015);
}

void CBeholder::PatternState(_float _fTimeDelta)
{
    switch (m_ePattern)
    {
    case PATTERN_IDLE:
        break;
    case PATTERN_WAITING:
        break;
    case PATTERN_PLAYERTRACKING:
        Player_Tracking_Laser();
        m_ePattern = PATTERN_WAITING;
        break;
    case PATTERN_ALLROUNDLASER:
        All_Round_Laser();
        m_ePattern = PATTERN_WAITING;
        break;
    case PATTERN_ALLROUNDLASERLANDMINE:
        All_Round_Laser_LandMine();
        m_ePattern = PATTERN_WAITING;
        break;
    case PATTERN_AIRSTRIKE:
        AirStrike();
        m_ePattern = PATTERN_WAITING;
        break;
    case PATTERN_SHOOT:
        if (m_iShootCount < m_iShootCountMax) {
            if (m_fShootDelay <= 0.f) {
                m_iShootCount++;
                m_fShootDelay = m_fShootDelayMax;
                Shoot();
            }
            else
                m_fShootDelay -= _fTimeDelta;
        }
        else {
            m_iShootCount = 0;
            Set_PatternEnd();
        }
        break;
    case PATTERN_ROUND_AIRSTRIKE:
        if (m_fRoundStrikeRadius < m_fRoundStrikeRadiusMax){
            if (m_fRoundStrikeDelay <= 0.f) {
                m_fRoundStrikeRadius += 1.f;
                m_fRoundStrikeDelay = m_fRoundStrikeDelayMax;
                RoundAirStrike();
            }
            else
                m_fRoundStrikeDelay -= _fTimeDelta;
        }
        else{
            m_fRoundStrikeRadius = 6.f;
            CPlayer_Manager::Get_Instance()->Set_RoundPattern(false);
            Set_PatternEnd();
        }
		break;
    case PATTERN_ROUND_AIRSTRIKE_BOOM:
        if (m_fRoundStrikeRadius < m_fRoundStrikeRadiusMax) {
            if (m_fRoundStrikeBoomDelay <= 0.f) {
                m_fRoundStrikeBoomDelay = m_fRoundStrikeBoomDelayMax;
                RoundAirStrikeBoom(_fTimeDelta);
            }
            else
                m_fRoundStrikeBoomDelay -= _fTimeDelta;
        }
        else {
            m_fRoundStrikeRadius = 6.f;
            CPlayer_Manager::Get_Instance()->Set_RoundPattern(false);
            Set_PatternEnd();
        }
        break;
    case PATTERN_ROUND_AIRSTRIKE_LANDMINE:
        if (m_fRoundStrikeRadius < m_fRoundStrikeRadiusMax) {
            if (m_fRoundStrikeBoomDelay <= 0.f) {
                m_fRoundStrikeBoomDelay = m_fRoundStrikeBoomDelayMax;
                RoundAirStrikeLandMine(_fTimeDelta);
            }
            else
                m_fRoundStrikeBoomDelay -= _fTimeDelta;
        }
        else {
            m_fRoundStrikeRadius = 6.f;
            CPlayer_Manager::Get_Instance()->Set_RoundPattern(false);
            Set_PatternEnd();
        }
        break;
	default:
		break;
    }
}

void CBeholder::ActivePattern(_float fTimeDelta)
{
    if (m_ePattern == PATTERN_IDLE) {

        if (m_fPatternTimeDelay <= 0.f) {

            if (m_iPatternCount > m_iPatternCountMax) m_iPatternCount = 0;
            switch (m_iPatternCount)
            {
            case 0:
                m_ePattern = PATTERN_PLAYERTRACKING;
                Set_PatternStart();
                break;
            case 1:
                m_ePattern = PATTERN_ALLROUNDLASER;
                Set_PatternStart();
                break;
            case 2:
                m_ePattern = PATTERN_ALLROUNDLASERLANDMINE;
                Set_PatternStart();
                break;
            case 3:
                m_ePattern = PATTERN_AIRSTRIKE;
                Set_PatternStart();
                break;
            case 4:
                m_ePattern = PATTERN_SHOOT;
                Set_PatternStart();
                break;
            case 5:
                m_ePattern = PATTERN_ROUND_AIRSTRIKE;
                Set_PatternStart();
                CPlayer_Manager::Get_Instance()->Set_RoundPattern(true);
                break;
            case 6:
                m_ePattern = PATTERN_ROUND_AIRSTRIKE_LANDMINE;
                Set_PatternStart();
                CPlayer_Manager::Get_Instance()->Set_RoundPattern(true);
                break;
            case 7:
                m_ePattern = PATTERN_ROUND_AIRSTRIKE_BOOM;
                Set_PatternStart();
                CPlayer_Manager::Get_Instance()->Set_RoundPattern(true);
                break;
            default:
                Shoot();
                break;
            }
            m_iPatternCount++;
            m_fPatternTimeDelay = m_fPatternTimeDelayMax;


        }
        else
            m_fPatternTimeDelay -= fTimeDelta;

    }


}

void CBeholder::Player_Tracking_Laser()
{
    m_pGameInstance->Play(L"Beholder_Laser0", false);
    m_pGameInstance->SetVolume(L"Beholder_Laser0", 1.f);

    BeholderAttackOrder LaserOrder;
    LaserOrder.eOrder = PLAYERTRACKING;
    LaserOrder.vMasterPos = m_pTransformCom->Get_Pos();
    CGameObject* pLaser = m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"Laser", L"Prototype_TrackingLaser", &LaserOrder);
 
}

void CBeholder::All_Round_Laser()
{
    m_pGameInstance->Play(L"Beholder_Laser1", false);
    m_pGameInstance->SetVolume(L"Beholder_Laser1", 1.f);

    for(int i = 0; i < 30; ++i){
    _float fRandomX = CMath_Manager::Get_Instance()->Random_Float(-15,15);
    _float fRandomZ = CMath_Manager::Get_Instance()->Random_Float(-5,25);

    BeholderAttackOrder LaserOrder;
    LaserOrder.eOrder = FREETRACKING;
    LaserOrder.vMasterPos = m_pTransformCom->Get_Pos();
    LaserOrder.vLook = { fRandomX, 0.f, fRandomZ };

    CGameObject* pLaser = m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"Laser", L"Prototype_TrackingLaser", &LaserOrder);
    }

}

void CBeholder::All_Round_Laser_LandMine()
{
    m_pGameInstance->Play(L"Beholder_Laser2", false);
    m_pGameInstance->SetVolume(L"Beholder_Laser2", 1.f);

    for (int i = 0; i < 5; ++i) {
		_float fRandomX = CMath_Manager::Get_Instance()->Random_Float(-15, 15);
		_float fRandomZ = CMath_Manager::Get_Instance()->Random_Float(-5, 25);

		BeholderAttackOrder LaserOrder;
		LaserOrder.eOrder = LANDMINESET;
		LaserOrder.vMasterPos = m_pTransformCom->Get_Pos();
		LaserOrder.vLook = { fRandomX, 0.f, fRandomZ };

		CGameObject* pLaser = m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"Laser", L"Prototype_TrackingLaser", &LaserOrder);
	}
}

void CBeholder::AirStrike()
{
        _float3 vTargetPos = CPlayer_Manager::Get_Instance()->Get_Player()->Get_Transform()->Get_Pos();
        AirBoom(vTargetPos);
}

void CBeholder::RoundAirStrike()
{
    float f_radius = m_fRoundStrikeRadius;
    int f_start_theta = 0;
    int f_end_theta = 360;

    for (int i = f_start_theta; i < f_end_theta; i = i+12)
    {
        _float2 p;
        float f_x = (float)(cos(deg2rad(i)) * f_radius);
        float f_y = (float)(sin(deg2rad(i)) * f_radius);
        p.x = f_x;
        p.y = f_y;
        
        _bool bExplode = true;
        CGameObject* pObj = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"LandMine", L"Prototype_LandMine", &bExplode);
        //pObj->Get_Transform()->Set_Position(_float3(0.f, 0.f, 10.f));
        pObj->Get_Transform()->Add_Pos(_float3(p.x, -0.3f, p.y + 10.f));
    }


}

void CBeholder::RoundAirStrikeBoom(float _fTimeDelta)
{
    _float2 fLissajous = CMath_Manager::Get_Instance()->Lissajous_Curve(_fTimeDelta, m_fLissajousTime, m_fRoundStrikeRadius, m_fRoundStrikeRadius,1,1, 1, 40);
    m_fRoundStrikeRadius += 6 * _fTimeDelta;

    AirBoom({ fLissajous.x, 0.f, fLissajous.y + 10.f });
}

void CBeholder::RoundAirStrikeLandMine(float _fTimeDelta)
{
    _float2 fLissajous = CMath_Manager::Get_Instance()->Lissajous_Curve(_fTimeDelta, m_fLissajousTime, m_fRoundStrikeRadius, m_fRoundStrikeRadius, 1, 1, 1, 20);
    m_fRoundStrikeRadius += 4 * _fTimeDelta;

    _bool bExplode = true;
    CGameObject* pObj = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"LandMine", L"Prototype_LandMine", &bExplode);
    pObj->Get_Transform()->Add_Pos(_float3(fLissajous.x, -0.3f, fLissajous.y + 10.f));
}

void CBeholder::AirBoom(_float3 vPos)
{
    for (int i = 0; i < 15; ++i) {
        _float3 vTargetPos = vPos;
        _float3 vLookPos = { 0.f, 0.f, 0.f };
        _float fRandomX = CMath_Manager::Get_Instance()->Random_Float(-1.f, 1.f);
        _float fRandomY = CMath_Manager::Get_Instance()->Random_Float(1, 20);
        _float fRandomZ = CMath_Manager::Get_Instance()->Random_Float(-1.f, 1.f);

        vTargetPos += { fRandomX, 10.f + fRandomY, fRandomZ };
        vLookPos = { vTargetPos.x, 5.f + fRandomY, vTargetPos.z };

        BeholderAttackOrder LaserOrder;
        LaserOrder.eOrder = AIRSTRIKE;
        LaserOrder.vMasterPos = vTargetPos;
        LaserOrder.vLook = vLookPos;

        CGameObject* pLaser = m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"Laser", L"Prototype_TrackingLaser", &LaserOrder);
        m_pGameInstance->Play(L"Beholder_AirBoom", false);
        m_pGameInstance->SetVolume(L"Beholder_AirBoom", 0.3f);
    }
}

void CBeholder::Shoot()
{
    m_pGameInstance->Play(L"Beholder_Bullet", false);
    m_pGameInstance->SetVolume(L"Beholder_Bullet", 0.5f);

    _float3 vBulletPos = m_pTransformCom->Get_Pos();
    vBulletPos.y += 0.25f;

    _float3 vPlayerPos = CPlayer_Manager::Get_Instance()->Get_Player()->Get_Transform()->Get_Pos();
    vPlayerPos.y -= 0.4f;
    CGameObject* pBullet = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Bullet", L"Prototype_Bullet");
    pBullet->Get_Transform()->Set_Position(vBulletPos);
    pBullet->Get_Transform()->Set_Scale({ 2.f, 1.f, 2.f });
    pBullet->Get_Transform()->Set_Target(m_pTransformCom->Get_Pos(), vPlayerPos);
    static_cast<CBoxCollider*>(pBullet->Find_Component(L"Collider"))->Update_BoxCollider(pBullet->Get_Transform()->Get_WorldMatrix());
}

void CBeholder::Hit(void* pArg)
{
    ENEMYHIT_DESC* pDesc = (ENEMYHIT_DESC*)pArg;

    CGameObject* pHitBlood = m_pGameInstance->Add_Clone(LEVEL_STATIC, L"Effect", L"Prototype_HitBlood");
    pHitBlood->Get_Transform()->Set_Position(pDesc->fHitWorldPos);
    pHitBlood->Get_Transform()->Set_Scale({ 3.f, 3.f, 0.5f });

    //zzz
    CPlayer_Manager::Get_Instance()->Set_PlayerHP(CPlayer_Manager::Get_Instance()->Get_PlayerHP() + 1);

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

    m_fRecentHitTime = 0;
    m_bThisFrameHit = true;
}

void CBeholder::Process_State(_float fTimeDelta)
{
    switch (m_eState)
    {
    case CBeholder::STATE_IDLE:
        State_Idle();
        break;
    case CBeholder::STATE_MOVE:
        State_Move();
        break;
    case CBeholder::STATE_ALERT:
        State_Alert();
        break;
    case CBeholder::STATE_PUSHED:
        State_Pushed();
        break;
    case CBeholder::STATE_SHOT:
        State_Shot();
        break;
    case CBeholder::STATE_JUMP:
        State_Jump();
        break;
    case CBeholder::STATE_HIT:
        State_Hit();
        break;
    case CBeholder::STATE_EXECUTION:
        State_Execution();
        break;
    case CBeholder::STATE_FLY:
        State_Fly(fTimeDelta);
        break;
    case CBeholder::STATE_FLYDEATH:
        State_FlyDeath(fTimeDelta);
        break;
    case CBeholder::STATE_DEATH:
        State_Death(fTimeDelta);
        break;

    }
}

void CBeholder::State_Idle()
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

void CBeholder::State_Move()
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

void CBeholder::State_Alert()
{
    if (m_pAnimationCom->IsEndAnim())
    {
        SetState_Shot();
    }
}

void CBeholder::State_Pushed()
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

void CBeholder::State_Shot()
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

void CBeholder::State_Jump()
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

void CBeholder::State_Execution()
{

}

void CBeholder::State_Fly(_float fTimeDelta)
{
    m_fFlyTimeAcc += fTimeDelta;

    if (m_fFlyTimeAcc >= m_fFlyTime)
    {
        m_fFlyTimeAcc = 0.f;
        SetState_FlyDeath();
    }
}

void CBeholder::State_FlyDeath(_float fTimeDelta)
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

void CBeholder::State_Hit()
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

void CBeholder::State_Death(_float fTimeDelta)
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

void CBeholder::SetState_Idle()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_IDLE;

    m_pAnimationCom->Play_Animation(L"Idle", 0.1f, true);

    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CBeholder::SetState_Move()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_MOVE;
    //m_pAnimationCom->Play_Animation(L"Walk", 0.12f, true);
}

void CBeholder::SetState_Alert()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_ALERT;
    m_pAnimationCom->Play_Animation(L"Idle", 0.15f, false);

    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
}

void CBeholder::SetState_Pushed(_float3 vLook)
{
    if (STATE_DEATH == m_eState || STATE_EXECUTION == m_eState
        || STATE_FLY == m_eState || STATE_FLYDEATH == m_eState)

        m_eState = STATE_PUSHED;

    vLook.y = 0.f;
    D3DXVec3Normalize(&vLook, &vLook);

    m_bPushRecovery = false;

    m_pRigidbody->Set_Velocity(vLook * 15.f);
    m_pRigidbody->Set_Friction(5.f);

    //m_pAnimationCom->Play_Animation(L"Pushed", 150200.f, false);
}

void CBeholder::SetState_Shot()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_SHOT;
    m_pGameInstance->Play(L"White_Suit_Shoot", false);
    m_pGameInstance->SetVolume(L"White_Suit_Shoot", 0.3f);

    //m_pAnimationCom->Play_Animation(L"Shot", 0.15f, false);

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

void CBeholder::SetState_Jump()
{
    if (STATE_DEATH == m_eState)
        return;
    m_eState = STATE_JUMP;
   // m_pAnimationCom->Play_Animation(L"Jump", 0.1f, false);

    _float fRand = _float(rand() % 2);
    if (0.f == fRand)
        fRand = -1.f;

    m_pRigidbody->Set_Velocity(m_pTarget->Get_Transform()->Get_Right() * m_fSpeed * fRand);
}

_bool CBeholder::SetState_Execution()
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

void CBeholder::SetState_Fly(_float3 vLook)
{
    m_pBoxCollider->Set_Active(true);

    m_eState = STATE_FLY;

    //m_pAnimationCom->Play_Animation(L"Death_Fly", 0.1f, false);

    vLook.y = 0.f;
    m_pRigidbody->Set_Velocity(vLook * 10.f);

    m_fFlyTimeAcc = 0.f;
}

void CBeholder::SetState_FlyDeath()
{
    m_eState = STATE_FLYDEATH;

    m_pBoxCollider->Set_Active(false);

    Call_MonsterDieUi(eMonsterGrade::Middle);

    m_pRigidbody->Set_Velocity(_float3(0.f, 0.f, 0.f));
    m_pRigidbody->Set_UseGravity(true);
}

void CBeholder::SetState_Hit()
{
    m_eState = STATE_HIT;

    m_pAnimationCom->Play_Animation(L"Hit", 0.1f, false);
}

void CBeholder::SetState_Death(ENEMYHIT_DESC* pDesc)
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
        //m_pAnimationCom->Play_Animation(L"Death_Headshot", 0.1f, false);
        if (eWeaponType == CPlayer::SHOTGUN) m_pAnimationCom->Play_Animation(L"Head_Explode", 0.1f, false);

        Arg.Hit = eSpecialHit::HEADSHOT;
        Arg.iCount = 4;
        m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_SpecialHit"), eUiRenderType::Render_NonBlend, &Arg);
        break;
    }
    case CPawn::BODY_SHOT: {
        //m_pAnimationCom->Play_Animation(L"Death_Bodyshot", 0.1f, false);
        if (eWeaponType == CPlayer::SHOTGUN) m_pAnimationCom->Play_Animation(L"Death_Shotgun", 0.1f, false);

        Arg.Hit = eSpecialHit::FINISHED;
        Arg.iCount = 4;
        m_pGameInstance->Add_Ui_LifeClone(TEXT("CUi_SpecialHit"), eUiRenderType::Render_NonBlend, &Arg);
        break;
    }
    case CPawn::EGG_SHOT:
        //m_pAnimationCom->Play_Animation(L"Death_Eggshot", 0.1f, false);
        break;
    }
}

void CBeholder::Set_PatternEndCheck()
{
    if(CPlayer_Manager::Get_Instance()->Get_IsPattern() == false)
		m_ePattern = PATTERN_IDLE;
}

void CBeholder::Set_PatternStart()
{
    CPlayer_Manager::Get_Instance()->Set_Pattern(true);
}

void CBeholder::Set_PatternEnd()
{
    CPlayer_Manager::Get_Instance()->Set_Pattern(false);
}

CBeholder* CBeholder::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBeholder* pInstance = new CBeholder(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to create: CBeholder"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBeholder::Clone(void* pArg)
{
    CBeholder* pInstance = new CBeholder(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to create: CBeholder"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBeholder::Free()
{
    __super::Free();

    Safe_Release(m_pTarget);
    Safe_Release(m_pAnimationCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pBoxCollider);
    Safe_Release(m_pRigidbody);
}
