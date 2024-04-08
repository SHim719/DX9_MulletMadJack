#include "Enemy.h"

#include "GameInstance.h"
#include "Enemy_Bullet.h"

CEnemy::CEnemy(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CEnemy::CEnemy(const CEnemy& rhs)
    : CGameObject{ rhs }
    , m_fTimeAcc(0.f)
    , m_bChange_Direction(false)
{
}

HRESULT CEnemy::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEnemy::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    memcpy(&m_EnemyDesc, pArg, sizeof m_EnemyDesc);

    if (FAILED(Add_Components()))
        return E_FAIL;

    // Level_GamePlay 객체에 접근
    if (FAILED(Add_Textures()))
        return E_FAIL;

    m_pFPS_Camera = dynamic_cast<CFPS_Camera*>(m_pGameInstance->Get_Instance()->Get_CurCamera());

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, &m_EnemyDesc.vPosition);

    m_pAnimationCom->Play_Animation(TEXT("Enemy_Shoot"), 0.1f, true);

    return S_OK;
}

void CEnemy::PriorityTick(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;

    if (m_fTimeAcc > 0.f && m_fTimeAcc <= 1.f)  // 1초마다 좌우로 움직이게 만듬
    {
        if (m_bChange_Direction)
            m_pTransformCom->Go_Right(fTimeDelta);
        else
            m_pTransformCom->Go_Left(fTimeDelta);
    }

    if (m_fTimeAcc > 1.f)
    {
        if (m_bChange_Direction)
            m_bChange_Direction = false;
        else
            m_bChange_Direction = true;

        CEnemy_Bullet::ENEMY_BULLET_DESC    Enemy_BulletDesc{};
        Enemy_BulletDesc.vPosition = _float3(m_pTransformCom->Get_State(m_pTransformCom->STATE_POSITION));
        Enemy_BulletDesc.vAt = _float3(m_pFPS_Camera->Get_CAMERA_DESC().vEye);
        Enemy_BulletDesc.fSpeedPerSec = 10.f;
        Enemy_BulletDesc.fRotationPerSec = D3DXToRadian(90.f);

        if (nullptr == m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, TEXT("Layer_Enemy_Bullet"), TEXT("Prototype_GameObject_Enemy_Bullet"), &Enemy_BulletDesc))
            return;

        m_fTimeAcc = 0.f;
    }
}

void CEnemy::Tick(_float fTimeDelta)
{
    Key_Input();

    m_pAnimationCom->Update(fTimeDelta);
}

void CEnemy::LateTick(_float fTimeDelta)
{
    // 몬스터 출력할 때의 변환 행렬을 FPS 카메라 객체에서 만든 빌보드 행렬로 세팅함
    m_pTransformCom->Set_Billboard_Matrix(m_pFPS_Camera->Get_Billboard_Matrix());

    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CEnemy::Render()
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

HRESULT CEnemy::Add_Components()
{
    m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("Com_VIBuffer")));

    m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Com_Transform"), &m_EnemyDesc));

    m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animation"), TEXT("Com_Animation"), this));

    return S_OK;
}

HRESULT CEnemy::Add_Textures()
{
    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_2nd_Hit"), TEXT("Enemy_2nd_Hit"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Aim"), TEXT("Enemy_Aim"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Blocking"), TEXT("Enemy_Blocking"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Breathe"), TEXT("Enemy_Breathe"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Shotgun"), TEXT("Enemy_Death_Shotgun"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Machinegun"), TEXT("Enemy_Death_Machinegun"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Headshot"), TEXT("Enemy_Headshot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Shot_Floor"), TEXT("Enemy_Death_Shot_Floor"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Shot_Wall"), TEXT("Enemy_Death_Shot_Wall"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_GroinShot"), TEXT("Enemy_GroinShot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_HeadExplode"), TEXT("Enemy_HeadExplode"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_1st_Hit"), TEXT("Enemy_1st_Hit"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Idle"), TEXT("Enemy_Idle"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Backwards_Machinegun"), TEXT("Enemy_Death_Backwards_Machinegun"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Shoot"), TEXT("Enemy_Shoot"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Walk"), TEXT("Enemy_Walk"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Push_Floor"), TEXT("Enemy_Death_Push_Floor"))))
        return E_FAIL;

    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Death_Push_Wall"), TEXT("Enemy_Death_Push_Wall"))))
        return E_FAIL;

    return S_OK;
}

HRESULT CEnemy::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CEnemy::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

void CEnemy::Key_Input()
{
    if (GetAsyncKeyState('1') & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_2nd_Hit"), 0.1f, false);

    if (GetAsyncKeyState('2') & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Aim"), 0.1f, false);

    if (GetAsyncKeyState('3') & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Blocking"), 0.1f, false);

    if (GetAsyncKeyState('4') & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Breathe"), 0.1f, true);

    if (GetAsyncKeyState('5') & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Death_Shotgun"), 0.1f, false);

    if (GetAsyncKeyState('6') & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Death_Machinegun"), 0.1f, false);

    if (GetAsyncKeyState('7') & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Headshot"), 0.1f, false);

    if (GetAsyncKeyState('8') & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Death_Shot_Floor"), 0.1f, false);

    if (GetAsyncKeyState('9') & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Death_Shot_Wall"), 0.1f, false);

    if (GetAsyncKeyState('0') & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_GroinShot"), 0.1f, false);

    if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_HeadExplode"), 0.1f, false);

    if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_1st_Hit"), 0.1f, false);

    if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Idle"), 0.1f, true);

    if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Death_Backwards_Machinegun"), 0.1f, false);

    if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Shoot"), 0.1f, true);

    if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Walk"), 0.1f, true);

    if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Death_Push_Floor"), 0.1f, false);

    if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
        m_pAnimationCom->Play_Animation(TEXT("Enemy_Death_Push_Wall"), 0.1f, false);
}

CEnemy* CEnemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CEnemy* pInstance = new CEnemy(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to create: CEnemy"));

        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CEnemy::Clone(void* pArg)
{
    CEnemy* pInstance = new CEnemy(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to create: CEnemy"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEnemy::Free()
{
    __super::Free();

    Safe_Release(m_pAnimationCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
}
