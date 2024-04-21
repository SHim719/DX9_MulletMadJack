#include "CSans.h"
#include "FPS_Camera.h"
#include "CBone_Spawner.h"


CSans::CSans(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPawn(pGraphic_Device)
{
}

CSans::CSans(const CSans& rhs)
	:CPawn(rhs)
{
}

HRESULT CSans::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSans::Initialize(void* pArg)
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
    m_pBoxCollider->Set_Scale({ 1.f, 1.f, 1.f });

    m_pRigidbody->Set_Friction(0.f);
    m_pRigidbody->Set_Velocity({ 0.f, 0.f, 0.f });
    m_pRigidbody->Set_UseGravity(false);

    m_pAnimationCom->Play_Animation(TEXT("Idle"), 0.1f, true);

    m_pBoneSpawner = CBone_Spawner::Create();
    
    return S_OK;
}

void CSans::PriorityTick(_float fTimeDelta)
{
}

void CSans::Tick(_float fTimeDelta)
{
    SubPatternTime(fTimeDelta);
    State_Pattern();
}

void CSans::LateTick(_float fTimeDelta)
{
}

HRESULT CSans::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pAnimationCom->Render();

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;


	return S_OK;
}

HRESULT CSans::Add_Components()
{
    m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

    m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

    m_pAnimationCom = dynamic_cast<CAnimation*>(Add_Component(LEVEL_STATIC, TEXT("Animation_Default"), TEXT("Animation"), this));

    CBoxCollider::BOXCOLLISION_DESC pDesc;
    pDesc.vScale = { 1.f, 1.f, 1.f };

    m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

    return S_OK;
}

HRESULT CSans::Add_Textures()
{
    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_GAMEPLAY, TEXT("Texture_Sans_Idle"), TEXT("Idle"))))
        return E_FAIL;

    return S_OK;
}

void CSans::SubPatternTime(_float fTimeDelta)
{
    m_fPatternTime -= fTimeDelta;
}

void CSans::Set_PatternState(SANSPatternSTATE State)
{
    m_ePatternState = State;
}

void CSans::Add_SansState()
{
    switch (m_eState)
    {
    case SANSSTATE::STATE_IDLE:
        m_eState = SANSSTATE::STATE_PATTERN1;
        break;
    case SANSSTATE::STATE_PATTERN1:
        m_eState = SANSSTATE::STATE_PATTERN2;
        break;
    case SANSSTATE::STATE_PATTERN2:
        m_eState = SANSSTATE::STATE_PATTERN3;
        break;
    case SANSSTATE::STATE_PATTERN3:
        m_eState = SANSSTATE::STATE_PATTERN4;
        break;
    case SANSSTATE::STATE_PATTERN4:
        m_eState = SANSSTATE::STATE_DEATH;
        break;
    case SANSSTATE::STATE_DEATH:
        m_bDestroyed = true;
        break;
    case SANSSTATE::STATE_END:
        break;
    default:
        break;
    }
}

void CSans::State_Pattern()
{
    if (m_fPatternTime < 0)
    {
        switch (m_ePatternState)
        {
        case SANSPatternSTATE::READY:
            Set_PatternState(SANSPatternSTATE::FIRST);        
            break;

        case SANSPatternSTATE::FIRST:
            Set_PatternState(SANSPatternSTATE::SECOND);
            break;

        case SANSPatternSTATE::SECOND:
            Set_PatternState(SANSPatternSTATE::THIRD);
            break;

        case SANSPatternSTATE::THIRD:
            Set_PatternState(SANSPatternSTATE::FOURTH);
            break;

        case SANSPatternSTATE::FOURTH:
            Set_PatternState(SANSPatternSTATE::End);
            break;

        case SANSPatternSTATE::End:
            Add_SansState();
            break;
        default:
            break;
        }

        m_pBoneSpawner->Spawn(m_eState, m_ePatternState);
        Initialize_PatternTime();
    }
}

void CSans::Initialize_PatternTime()
{
    switch (m_ePatternTimeGap)
    {
    case CSans::PatternTimeGAP::SMALL:
        m_fPatternTime = 0.3f;
        break;
    case CSans::PatternTimeGAP::DEFAULT:
        m_fPatternTime = 0.6f;
        break;
    case CSans::PatternTimeGAP::BIG:
        m_fPatternTime = 1.f;
        break;
    default:
        break;
    }
}

CSans* CSans::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSans* pInstance = new CSans(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to create: CSans"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSans::Clone(void* pArg)
{
    CSans* pInstance = new CSans(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to create: CSansClone"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSans::Free()
{
	__super::Free();

    Safe_Release(m_pBoneSpawner);
    Safe_Release(m_pTarget);
    Safe_Release(m_pAnimationCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pBoxCollider);
}
