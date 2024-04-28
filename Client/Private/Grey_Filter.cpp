#include "Grey_Filter.h"

CGrey_Filter::CGrey_Filter(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUi(pGraphic_Device)
{
}

CGrey_Filter::CGrey_Filter(const CGrey_Filter& rhs)
    :CUi(rhs)
{
}

HRESULT CGrey_Filter::Initialize_Prototype()
{
    if (FAILED(Add_Components(nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CGrey_Filter::Initialize(void* pArg)
{
    return S_OK;
}

void CGrey_Filter::PriorityTick(_float fTimeDelta)
{
}

void CGrey_Filter::Tick(_float fTimeDelta)
{
}

void CGrey_Filter::LateTick(_float fTimeDelta)
{
}

void CGrey_Filter::Begin_RenderState()
{
    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
    //m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255, 255, 255, (_uint)m_fAlpha));
}

HRESULT CGrey_Filter::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pTextureCom->Bind_Texture(0);

    Begin_RenderState();

    m_pVIBufferCom->Render();

    End_RenderState();

    return S_OK;
}
void CGrey_Filter::End_RenderState()
{
   // m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
}



HRESULT CGrey_Filter::Initialize_Active()
{
    Initialize_Set_ActiveTime();
    Initialize_Set_Size();
    Initialize_Set_Speed();
    Initialize_Set_Scale_Pos_Rotation(nullptr);
    return S_OK;
}

void CGrey_Filter::Initialize_Set_ActiveTime()
{
    m_fActiveTime = 1.5f;
}

void CGrey_Filter::Initialize_Set_Size()
{
    m_UiDesc.m_fSizeX = g_iWinSizeX;
    m_UiDesc.m_fSizeY = g_iWinSizeY;
}

void CGrey_Filter::Initialize_Set_Speed()
{
}

void CGrey_Filter::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
    _float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
    m_UiDesc.m_fX = 0;
    m_UiDesc.m_fY = 0;

    m_pTransformCom->Set_Scale(Scale);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}


HRESULT CGrey_Filter::Add_Components(void* pArg)
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

HRESULT CGrey_Filter::Add_Texture(void* pArg)
{
    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("Grey_Filter_Texture"),
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

CGrey_Filter* CGrey_Filter::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CGrey_Filter* pInstance = new CGrey_Filter(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("CGrey_Filter Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGrey_Filter::Free()
{
    __super::Free();
}
