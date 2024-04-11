#include "CUi_UpGrade_Select.h"


CUi_UpGrade_Select::CUi_UpGrade_Select(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUi(pGraphic_Device)
{
}

CUi_UpGrade_Select::CUi_UpGrade_Select(const CUi_UpGrade_Select& rhs)
    :CUi(rhs)
{
}

HRESULT CUi_UpGrade_Select::Initialize_Prototype()
{
    Default_Set_LifeTime();
    Default_Set_Size();

    return S_OK;
}

HRESULT CUi_UpGrade_Select::Initialize(void* pArg)
{
    if (FAILED(Add_Components(pArg)))
        return E_FAIL;

    Initialize_Set_Scale_Pos_Rotation(pArg);
    Initialize_Set_Speed();

    return S_OK;
}

void CUi_UpGrade_Select::PriorityTick(_float fTimeDelta)
{
}

void CUi_UpGrade_Select::Tick(_float fTimeDelta)
{
    m_fSwitchingTime += fTimeDelta;

    if(m_fSwitchingTime > 0.2)
        ++m_iTexture_Index;

    if (m_iTexture_Index > m_pTextureCom->Get_MaxTextureNum())
    {
        m_iTexture_Index = 0;
    }
}

void CUi_UpGrade_Select::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_UpGrade_Select::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pTextureCom->Bind_Texture(m_iTexture_Index);
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUi_UpGrade_Select::Add_Components(void* pArg)
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

HRESULT CUi_UpGrade_Select::Add_Texture(void* pArg)
{
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("CUi_UpGrade_Select_Texture")
        ,(CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

void CUi_UpGrade_Select::Default_Set_LifeTime()
{
    m_fLifeTime = 0;
}

void CUi_UpGrade_Select::Default_Set_Size()
{
    m_UiDesc.m_fSizeX = 260;
    m_UiDesc.m_fSizeY = 420;
}

void CUi_UpGrade_Select::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
    _float3* Arg = (_float3*)pArg;
    m_UiDesc.m_fX = Arg->x;
    m_UiDesc.m_fY = Arg->y;

    _float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

    m_pTransformCom->Set_Scale(Scale);
    m_pTransformCom->Set_State
    (CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUi_UpGrade_Select::Initialize_Set_Speed()
{
}

void CUi_UpGrade_Select::Set_Pos(_float3 Position)
{
    m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Position);
}

CUi_UpGrade_Select* CUi_UpGrade_Select::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUi_UpGrade_Select* pInstance = new CUi_UpGrade_Select(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("CUi_UpGrade_Select Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CUi* CUi_UpGrade_Select::Clone(void* pArg)
{
    CUi_UpGrade_Select* pInstance = new CUi_UpGrade_Select(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("CUi_UpGrade_Select Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUi_UpGrade_Select::Free()
{
    __super::Free();
}
