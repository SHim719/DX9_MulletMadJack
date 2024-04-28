#include "CUi_LobbyLogo.h"

CUi_LobbyLogo::CUi_LobbyLogo(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUi(pGraphic_Device)
{
}

CUi_LobbyLogo::CUi_LobbyLogo(const CUi_LobbyLogo& rhs)
    :CUi(rhs)
{
}

HRESULT CUi_LobbyLogo::Initialize_Prototype()
{
    if (FAILED(Add_Components(nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUi_LobbyLogo::Initialize(void* pArg)
{
    return S_OK;
}

void CUi_LobbyLogo::PriorityTick(_float fTimeDelta)
{
}

void CUi_LobbyLogo::Tick(_float fTimeDelta)
{
    TextureSwitching(fTimeDelta);
}

void CUi_LobbyLogo::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_LobbyLogo::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pTextureCom->Bind_Texture(m_iTexture_Index);

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUi_LobbyLogo::Initialize_Active()
{
    Initialize_Set_Size();
    Initialize_Set_Scale_Pos_Rotation(nullptr);

    m_fSwitchingGap = 99999;
    m_fSwitchingTime = 0;

    return S_OK;
}

void CUi_LobbyLogo::Initialize_Set_ActiveTime()
{
}

void CUi_LobbyLogo::Initialize_Set_Size()
{
    m_UiDesc.m_fSizeX = 512;
    m_UiDesc.m_fSizeY = 128;
}

void CUi_LobbyLogo::Initialize_Set_Speed()
{
}

void CUi_LobbyLogo::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
    _float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

    m_UiDesc.m_fX = 0;
    m_UiDesc.m_fY = 0;

    m_pTransformCom->Set_Scale(Scale);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_LobbyLogo::Add_Components(void* pArg)
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

    if (FAILED(Add_Texture(nullptr)))
        return E_FAIL;


    return S_OK;
}

HRESULT CUi_LobbyLogo::Add_Texture(void* pArg)
{
    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("CUi_LobbyLogo_Texture"),
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

void CUi_LobbyLogo::TextureSwitching(_float fTimeDelta)
{
    m_fSwitchingTime += fTimeDelta;
    m_fSwitchingGap += fTimeDelta;

    if (m_fSwitchingGap > 4.f)
    {
        if (m_fSwitchingTime > 0.15)
        {
            ++m_iTexture_Index;
            if (m_iTexture_Index > m_pTextureCom->Get_MaxTextureNum())
            {
                m_iTexture_Index = 0;
                m_fSwitchingGap = 0;
            }
        }
    }
}

CUi_LobbyLogo* CUi_LobbyLogo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUi_LobbyLogo* pInstance = new CUi_LobbyLogo(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("CUi_LobbyLogo Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUi_LobbyLogo::Free()
{
    __super::Free();
}
