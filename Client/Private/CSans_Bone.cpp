#include "CSans_Bone.h"
#include "PlayerManager.h"


CSans_Bone::CSans_Bone(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{
}

CSans_Bone::CSans_Bone(const CSans_Bone& rhs)
    :CGameObject(rhs)
{
}

HRESULT CSans_Bone::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSans_Bone::Initialize(void* pArg)
{
    if (FAILED(Add_Components()))
        return E_FAIL;

    if (FAILED(Add_Texture()))
        return E_FAIL;

    Initialize_Arg(pArg);

    m_pAnimationCom->Play_Animation(TEXT("Bone"), 0.1f, true);

    CBoxCollider::BOXCOLLISION_DESC pDesc;

    _float3 Scale = m_pTransformCom->Get_Scale();
    Scale.z -= 0.9f;
    Scale.x -= 0.05f;
    pDesc.vScale = Scale;
    pDesc.vOffset = { 0.f, 0.f, 0.f };
    m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component
    (LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));


    m_pBoxCollider->Set_Active(true);

    m_strTag = "SansBone";

    return S_OK;
}

void CSans_Bone::PriorityTick(_float fTimeDelta)
{
}

void CSans_Bone::Tick(_float fTimeDelta)
{
    m_fLife -= fTimeDelta;
    if (m_fLife < 0)
    {
        m_bDestroyed = true;
    }

    Move(fTimeDelta);
    m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
}

void CSans_Bone::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

void CSans_Bone::BeginRenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
    m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR,
        D3DCOLOR_RGBA(255, 255, 255, 150));
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

HRESULT CSans_Bone::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pAnimationCom->Render();

    BeginRenderState();

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    if (m_pBoxCollider)
        m_pBoxCollider->Render();

    EndRenderState();


    return S_OK;
}

void CSans_Bone::EndRenderState()
{
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CSans_Bone::OnTriggerEnter(CGameObject* pOther)
{
    if ("Player" == pOther->Get_Tag())
    {
        CPlayer_Manager::Get_Instance()->Get_Player()->SansHit();
    }
}

HRESULT CSans_Bone::Add_Components()
{
    m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component
    (LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

    m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component
    (LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

    m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component
    (LEVEL_STATIC, TEXT("Animation_Default"), TEXT("Animation"), this));

    return S_OK;
}

HRESULT CSans_Bone::Add_Texture()
{
    if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_STATIC, TEXT("Texture_Sans_Bone"), TEXT("Bone"))))
        return E_FAIL;

    return S_OK;
}

void CSans_Bone::Initialize_Arg(void* pArg)
{
    SansBoneArg* Arg = (SansBoneArg*)pArg;
    SansBonePos BonePos = Arg->Pos;
    _uint floor = Arg->floor;
    m_pTransformCom->Set_Speed(Arg->fSpeed);
    _float3 Pos = {};
    _float3 Scale = {};
    if (BonePos == SansBonePos::Left)
    {
        Pos = { -1.5f, 0.7f, 4.f };
        Scale = { 1.5f, 0.5f, 1.f };
        m_pTransformCom->Set_Position(Pos);
        m_pTransformCom->Set_Scale(Scale);
    }
    else if (BonePos == SansBonePos::Middle)
    {
        Pos = { 0.f, 0.7f, 4.f };
        Scale = { 1.5f, 0.5f, 1.f };
        m_pTransformCom->Set_Position(Pos);
        m_pTransformCom->Set_Scale(Scale);
    }
    else if (BonePos == SansBonePos::Right)
    {
        Pos = { 1.5f, 0.7f, 4.f };
        Scale = { 1.5f, 0.5f, 1.f };
        m_pTransformCom->Set_Position(Pos);
        m_pTransformCom->Set_Scale(Scale);
    }

    _float3 AdjustFloorPos = m_pTransformCom->Get_Pos();
    switch (floor)
    {
    case 1:
        break;
    case 2:
        AdjustFloorPos.y += 1.5f;
        m_pTransformCom->Set_Position(AdjustFloorPos);
        break;
    case 3:
        AdjustFloorPos.y += 3.f;
        m_pTransformCom->Set_Position(AdjustFloorPos);
        break;
    default:
        break;
    }
}

void CSans_Bone::Move(_float fTimeDelta)
{
    m_pTransformCom->Go_Floor_Backward(fTimeDelta);
}

CSans_Bone* CSans_Bone::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSans_Bone* pInstance = new CSans_Bone(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to create: CSans_Bone"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSans_Bone::Clone(void* pArg)
{
    CSans_Bone* pInstance = new CSans_Bone(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to create clone: CSans_Bone"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSans_Bone::Free()
{
    __super::Free();

    Safe_Release(m_pAnimationCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pBoxCollider);
}
