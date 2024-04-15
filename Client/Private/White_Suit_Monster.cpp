#include "White_Suit_Monster.h"
#include "GameInstance.h"
#include "Enemy_Bullet.h"

CWhite_Suit_Monster::CWhite_Suit_Monster(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CPawn{ pGraphic_Device }
{
}

CWhite_Suit_Monster::CWhite_Suit_Monster(const CWhite_Suit_Monster& rhs)
    : CPawn{ rhs }
    , m_fBullet_TimeGap(0.f)
    , m_fWalking_TimeGap(0.f)
    , m_fShooting_TimeGap(0.f)
    , m_fBlocking_TimeGap(0.f)
    , IsPlaying(false)
    , m_bIdle(true)
    , m_bAimed(false)
    , m_bDead(false)
    , m_bWalking(false)
    , m_bShooting(false)
    , m_bBlock(false)
{
}

HRESULT CWhite_Suit_Monster::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CWhite_Suit_Monster::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    memcpy(&m_White_Suit_Monster_Desc, pArg, sizeof m_White_Suit_Monster_Desc);

    if (FAILED(Add_Components()))
        return E_FAIL;

    if (FAILED(Add_Textures()))
        return E_FAIL;

    m_pFPS_Camera = dynamic_cast<CFPS_Camera*>(m_pGameInstance->Get_CurCamera());

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_White_Suit_Monster_Desc.vPosition);
 

    _float3 Scale = { (_float)1.1f, (_float)1.1f, (_float)1.1f };
    m_pTransformCom->Set_Scale(Scale);

    m_pTransformCom->Set_Target(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION));

    m_pAnimationCom->Play_Animation(TEXT("White_Suit_Monster_Idle"), 0.1f, true);

    return S_OK;
}

void CWhite_Suit_Monster::PriorityTick(_float fTimeDelta)
{

}

void CWhite_Suit_Monster::Tick(_float fTimeDelta)
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

void CWhite_Suit_Monster::LateTick(_float fTimeDelta)
{
    m_pTransformCom->Set_Billboard_Matrix(m_pFPS_Camera->Get_Billboard_Matrix());

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

    if (FAILED(End_RenderState()))
        return E_FAIL;

    return S_OK;
}

HRESULT CWhite_Suit_Monster::Add_Components()
{
    m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

    m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), &m_White_Suit_Monster_Desc));

    m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animation"), TEXT("Animation"), this));

    return S_OK;
}

HRESULT CWhite_Suit_Monster::Add_Textures()
{
    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_2nd_Hit"), TEXT("White_Suit_Monster_2nd_Hit"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Aim"), TEXT("White_Suit_Monster_Aim"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Blocking"), TEXT("White_Suit_Monster_Blocking"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Breathe"), TEXT("White_Suit_Monster_Breathe"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Death_Shotgun"), TEXT("White_Suit_Monster_Death_Shotgun"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_BodyShot"), TEXT("White_Suit_Monster_BodyShot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Headshot"), TEXT("White_Suit_Monster_Headshot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Death_Shot_Floor"), TEXT("White_Suit_Monster_Death_Shot_Floor"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Death_Shot_Wall"), TEXT("White_Suit_Monster_Death_Shot_Wall"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_GroinShot"), TEXT("White_Suit_Monster_GroinShot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_HeadExplode"), TEXT("White_Suit_Monster_HeadExplode"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_1st_Hit"), TEXT("White_Suit_Monster_1st_Hit"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Idle"), TEXT("White_Suit_Monster_Idle"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Death_Backwards_Machinegun"), TEXT("White_Suit_Monster_Death_Backwards_Machinegun"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Shoot"), TEXT("White_Suit_Monster_Shoot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Walk"), TEXT("White_Suit_Monster_Walk"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Death_Push_Floor"), TEXT("White_Suit_Monster_Death_Push_Floor"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_White_Suit_Monster_Death_Push_Wall"), TEXT("White_Suit_Monster_Death_Push_Wall"))))
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

void CWhite_Suit_Monster::Set_Motions(_float fTimeDelta)
{
    switch (m_eState)
    {
    case STATE_IDLE:
        break;

    case STATE_AIM:
        break;

    case STATE_WALK:
        m_pTransformCom->Go_Floor_Backward(fTimeDelta);
        break;

    case STATE_SHOOT:
        break;

    case STATE_BLOCK:
        break;

    case STATE_HEADSHOT:
        break;

    case STATE_BODYSHOT:
        break;

    case STATE_GROINSHOT:
        break;

    default:
        break;
    }

}

void CWhite_Suit_Monster::On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)
{
    srand(unsigned(time(nullptr)));

    _float4x4   WorldMatrixInverse = m_pTransformCom->Get_WorldMatrix_Inverse();
    _float3     vHitLocalPos = *D3DXVec3TransformCoord(&_float3(), &fHitWorldPos, &WorldMatrixInverse);

    if (-0.1f < vHitLocalPos.x && vHitLocalPos.x < 0.1f && 0.375f <= vHitLocalPos.y && vHitLocalPos.y < 0.5f)
        m_eState = STATE_HEADSHOT;

    else if (-0.1f < vHitLocalPos.x && vHitLocalPos.x < 0.1f && 0.1f <= vHitLocalPos.y && vHitLocalPos.y < 0.375f && rand() % 2 == 0)
        m_eState = STATE_BLOCK;

    else if (-0.1f < vHitLocalPos.x && vHitLocalPos.x < 0.1f && 0.1f <= vHitLocalPos.y && vHitLocalPos.y < 0.375f && rand() % 2 == 1)
        m_eState = STATE_BODYSHOT;

    else if (-0.1f < vHitLocalPos.x && vHitLocalPos.x < 0.1f && -0.1f <= vHitLocalPos.y && vHitLocalPos.y < 0.1f)
        m_eState = STATE_GROINSHOT;

    else if (-0.1f < vHitLocalPos.x && vHitLocalPos.x < 0.1f && -0.5f < vHitLocalPos.y && vHitLocalPos.y < -0.1f)
        m_eState = STATE_BODYSHOT;
}

void CWhite_Suit_Monster::Decide_Pawn_Motions(_float fTimeDelta)
{
    Pawn_Aiming_Motion(fTimeDelta);
    Pawn_Shooting_Motion(fTimeDelta);
    Pawn_Walking_Motion(fTimeDelta);
    Pawn_Blocking_Motion(fTimeDelta);
    Pawn_Dying_Motion(fTimeDelta);
}

void CWhite_Suit_Monster::Pawn_Aiming_Motion(_float fTimeDelta)
{
    if (D3DXVec3Length(&(m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION) -
        m_pTransformCom->Get_State(CTransform::STATE_POSITION))) < 3.f && !m_bDead && !m_bAimed && !m_bShooting && !IsPlaying)
    {
        m_eState = STATE_AIM;

        m_bAimed = true;
        IsPlaying = m_bAimed;

        m_bIdle = false;

        // �ִϸ��̼��� �� �� ������ ����� ������ if���� �ٽ� ������ �� ��
        m_pAnimationCom->Play_Animation(TEXT("White_Suit_Monster_Aim"), 0.1f, false);
    }
}

void CWhite_Suit_Monster::Pawn_Shooting_Motion(_float fTimeDelta)
{
    m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION);

    m_fShooting_TimeGap += fTimeDelta;

    srand(unsigned(time(nullptr)));

    if (rand() % 2 == 0)  // �ӽ÷� ������ Ȯ���� ��
    {
        m_bWalking = true;
        m_bShooting = false;
    }
    else
    {
        m_bWalking = false;
        m_bShooting = true;
    }

    if (!m_bIdle && !m_bDead && !m_bWalking && !IsPlaying)
    {
        m_eState = STATE_SHOOT;

        m_bShooting = true;
        IsPlaying = m_bShooting;

        m_pAnimationCom->Play_Animation(TEXT("White_Suit_Monster_Shoot"), 0.1f, false);

        if (m_fShooting_TimeGap > 1.f)
        {
            CEnemy_Bullet::ENEMY_BULLET_DESC    Enemy_BulletDesc{};
            Enemy_BulletDesc.vPosition = _float3(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
            Enemy_BulletDesc.vAt = _float3(m_pFPS_Camera->Get_CAMERA_DESC().vEye);
            Enemy_BulletDesc.fSpeedPerSec = 5.f;
            Enemy_BulletDesc.fRotationPerSec = D3DXToRadian(90.f);

            if (nullptr == m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Enemy_Bullet"), TEXT("Prototype_GameObject_Enemy_Bullet"), &Enemy_BulletDesc))
                return;

            m_bShooting = false;

            m_fShooting_TimeGap = 0.f;
        }
    }

    if (D3DXVec3Length(&(m_pFPS_Camera->Get_Camera_TransformCom()->Get_State(CTransform::STATE_POSITION) -
        m_pTransformCom->Get_State(CTransform::STATE_POSITION))) >= 3.f)
    {
        m_bShooting = false;
    }
}

void CWhite_Suit_Monster::Pawn_Walking_Motion(_float fTimeDelta)
{
	if (m_bWalking && !m_bIdle && !m_bDead && !m_bShooting && !IsPlaying)
	{
        m_eState = STATE_WALK;

        IsPlaying = m_bWalking;

		m_fWalking_TimeGap += fTimeDelta;

        m_pAnimationCom->Play_Animation(TEXT("White_Suit_Monster_Walk"), 0.1f, true);

		if (m_fWalking_TimeGap > 1.f)
		{
			m_bWalking = false;

            m_eState = STATE_END;

			m_fWalking_TimeGap = 0.f;
		}
	}
}

void CWhite_Suit_Monster::Pawn_Blocking_Motion(_float fTimeDelta)
{
    if (m_eState == STATE_BLOCK && !m_bDead)
    {
        m_pAnimationCom->Play_Animation(TEXT("White_Suit_Monster_Blocking"), 0.1f, false);

        m_eState = STATE_END;
    }
}

void CWhite_Suit_Monster::Pawn_Dying_Motion(_float fTimeDelta)
{
    // �Ѿ��� ��� �¾Ҵ����� ���� ���� ����� ������ ��
    if (m_eState == STATE_HEADSHOT && !m_bDead)
    {
        m_bDead = true;

        m_pAnimationCom->Play_Animation(TEXT("White_Suit_Monster_Headshot"), 0.1f, false);
    }

    if (m_eState == STATE_BODYSHOT && !m_bDead)
    {
        m_bDead = true;

        m_pAnimationCom->Play_Animation(TEXT("White_Suit_Monster_BodyShot"), 0.1f, false);
    }

    if (m_eState == STATE_GROINSHOT && !m_bDead)
    {
        m_bDead = true;

        m_pAnimationCom->Play_Animation(TEXT("White_Suit_Monster_GroinShot"), 0.1f, false);
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

    Safe_Release(m_pAnimationCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
}
