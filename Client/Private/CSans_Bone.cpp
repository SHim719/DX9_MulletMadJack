#include "CSans_Bone.h"


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

    Initialize_Arg(pArg);

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
}

void CSans_Bone::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CSans_Bone::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Texture(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    if (m_pBoxCollider)
        m_pBoxCollider->Render();

    return S_OK;
}

void CSans_Bone::OnTriggerEnter(CGameObject* pOther)
{
}

HRESULT CSans_Bone::Add_Components()
{
    m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component
    (LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer")));

    m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component
    (LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

    m_pTextureCom = dynamic_cast<CTexture*>(__super::Add_Component
    (LEVEL_GAMEPLAY, TEXT("CSans_Bone_Texture"), TEXT("CSans_Bone_Texture")));

    CBoxCollider::BOXCOLLISION_DESC pDesc;
    pDesc.vScale = { 0.25f, 0.25f, 0.5f };
    pDesc.vOffset = { 0.f, 0.f, 0.f };

    m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component
    (LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

    return S_OK;
}

void CSans_Bone::Initialize_Arg(void* pArg)
{
    //jeongrae incomplete
    SansBoneArg* Arg = (SansBoneArg*)pArg;
    SansBoneSize Size = Arg->Size;
    _uint floor = Arg->floor;
    m_pTransformCom->Set_Speed(Arg->fSpeed);
    
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

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pBoxCollider);
}
