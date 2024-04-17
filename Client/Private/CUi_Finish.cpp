#include "CUi_Finish.h"

CUi_Finish::CUi_Finish(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUi(pGraphic_Device)
{
}

CUi_Finish::CUi_Finish(const CUi_Finish& rhs)
    :CUi(rhs)
{
}

HRESULT CUi_Finish::Initialize_Prototype()
{
    if (FAILED(Add_Components(nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUi_Finish::Initialize(void* pArg)
{
    return S_OK;
}

void CUi_Finish::PriorityTick(_float fTimeDelta)
{
}

void CUi_Finish::Tick(_float fTimeDelta)
{
    m_fActiveTime -= fTimeDelta;
    if (m_fActiveTime > 0)
    {
        Scaling(fTimeDelta);
    }
    else
    {
        m_bActive = false;
    }
}

void CUi_Finish::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Finish::Render()
{
    if (FAILED(m_pBackGroundTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pBackGroundTextureCom->Bind_Texture(0);
    m_pBackGroundVIBufferCom->Render();

    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pTextureCom->Bind_Texture(m_iTexture_Index);
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUi_Finish::Initialize_Active()
{
    Initialize_Set_ActiveTime();
    Initialize_Set_Size();
    Initialize_Set_Speed();
    Initialize_Set_Scale_Pos_Rotation(nullptr);

    return S_OK;
}

void CUi_Finish::Initialize_Set_ActiveTime()
{
    m_fActiveTime = 3.f;
    m_fScalingTime = 0;
}

void CUi_Finish::Initialize_Set_Size()
{
    m_UiDesc.m_fSizeX = 5600;
    m_UiDesc.m_fSizeY = 700;

    m_BackGround.m_fSizeX = 450;
    m_BackGround.m_fSizeY = 200;
}

void CUi_Finish::Initialize_Set_Speed()
{
}

void CUi_Finish::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
    _float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
    m_UiDesc.m_fX = 0;
    m_UiDesc.m_fY = 250;

    m_pTransformCom->Set_Scale(Scale);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, 
        &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));

    _float3 BackScale = { m_BackGround.m_fSizeX, m_BackGround.m_fSizeY, 1 };
    m_BackGround.m_fX = 0;
    m_BackGround.m_fY = 250;

    m_pBackGroundTransformCom->Set_Scale(BackScale);
    m_pBackGroundTransformCom->Set_State(CTransform::STATE_POSITION,
        &_float3(m_BackGround.m_fX, m_BackGround.m_fY, 0.f));
}

HRESULT CUi_Finish::Add_Components(void* pArg)
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

    if (FAILED(Add_Component(
        LEVEL_STATIC,
        TEXT("VIBuffer_Rect_Default"),
        (CComponent**)&m_pBackGroundVIBufferCom)))
        return E_FAIL;

    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("Transform_Default"),
        (CComponent**)&m_pBackGroundTransformCom)))
        return E_FAIL;

    if (FAILED(Add_Texture(nullptr)))
        return E_FAIL;


    return S_OK;
}

HRESULT CUi_Finish::Add_Texture(void* pArg)
{
    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("CUi_Finish_Texture"),
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;
 
    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("CUi_Finish_BackGround_Texture"),
        (CComponent**)&m_pBackGroundTextureCom)))
        return E_FAIL;

    return S_OK;
}

void CUi_Finish::Scaling(_float fTimeDelta)
{
    m_fScalingTime += fTimeDelta;
    if (m_fScalingTime < 0.25f)
    {
        _float3 Scale = m_pTransformCom->Get_Scale();
        Scale.x -= fTimeDelta * 12000;
        Scale.y -= fTimeDelta * 1500;
        m_pTransformCom->Set_Scale(Scale);
    }
    else
    {
        _float3 TestScale = m_pTransformCom->Get_Scale();
        m_pTransformCom->Set_Scale(m_OriginScale);
    }
}

CUi_Finish* CUi_Finish::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUi_Finish* pInstance = new CUi_Finish(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("CUi_Finish Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUi_Finish::Free()
{
    __super::Free();

    Safe_Release(m_pBackGroundTextureCom);
    Safe_Release(m_pBackGroundTransformCom);
    Safe_Release(m_pBackGroundVIBufferCom);
}
