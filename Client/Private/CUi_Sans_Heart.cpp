#include "CUi_Sans_Heart.h"

CUi_Sans_Heart::CUi_Sans_Heart(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUi(pGraphic_Device)
{
}

CUi_Sans_Heart::CUi_Sans_Heart(const CUi_Sans_Heart& rhs)
    :CUi(rhs)
{
}

HRESULT CUi_Sans_Heart::Initialize_Prototype()
{
    Default_Set_Size();

    return S_OK;
}

HRESULT CUi_Sans_Heart::Initialize(void* pArg)
{
    if (FAILED(Add_Components(pArg)))
        return E_FAIL;

    Initialize_Set_Scale_Pos_Rotation(pArg);
    Initialize_Set_Speed();

    return S_OK;
}

void CUi_Sans_Heart::PriorityTick(_float fTimeDelta)
{
}

void CUi_Sans_Heart::Tick(_float fTimeDelta)
{
}

void CUi_Sans_Heart::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Sans_Heart::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pTextureCom->Bind_Texture(m_iTexture_Index);

    m_pVIBufferCom->Render();

    return S_OK;
}

void CUi_Sans_Heart::Default_Set_Size()
{
    m_UiDesc.m_fSizeX = 30;
    m_UiDesc.m_fSizeY = 30;
}

HRESULT CUi_Sans_Heart::Add_Components(void* pArg)
{
    if (FAILED(Add_Component(
        LEVEL_STATIC,
        TEXT("VIBuffer_Rect_Default"),
        (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("Transform_Default"),
        (CComponent**)&m_pTransformCom)))
        return E_FAIL;

    Add_Texture(pArg);

    return S_OK;
}

HRESULT CUi_Sans_Heart::Add_Texture(void* pArg)
{
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("CUi_Sans_Heart_Texture")
        , (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

void CUi_Sans_Heart::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
    _float3* Arg = (_float3*)pArg;
    m_UiDesc.m_fX = Arg->x;
    m_UiDesc.m_fY = Arg->y;
  
    _float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
    _float3 Pos = { m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f };
    m_pTransformCom->Set_Scale(Scale);
    m_pTransformCom->Set_Position(Pos);
}

void CUi_Sans_Heart::Initialize_Set_Speed()
{
}

CUi_Sans_Heart* CUi_Sans_Heart::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUi_Sans_Heart* pInstance = new CUi_Sans_Heart(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("CUi_Sans_Heart Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CUi* CUi_Sans_Heart::Clone(void* pArg)
{
    CUi_Sans_Heart* pInstance = new CUi_Sans_Heart(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("CUi_Sans_Heart clone Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUi_Sans_Heart::Free()
{
    __super::Free();
}
