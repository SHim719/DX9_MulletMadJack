#include "CUI_FadeInOut.h"

CUI_FadeInOut::CUI_FadeInOut(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUi(pGraphic_Device)
{
}

CUI_FadeInOut::CUI_FadeInOut(const CUI_FadeInOut& rhs)
    :CUi(rhs)
{
}

HRESULT CUI_FadeInOut::Initialize_Prototype()
{
    if (FAILED(Add_Components(nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_FadeInOut::Initialize(void* pArg)
{
    return S_OK;
}

void CUI_FadeInOut::PriorityTick(_float fTimeDelta)
{
}

void CUI_FadeInOut::Tick(_float fTimeDelta)
{
    if (FADEIN == m_eFadeState)
    {
        m_fAlpha -= m_fSpeed * fTimeDelta;
    }
    else
    {
        m_fAlpha += m_fSpeed * fTimeDelta;
    }

    m_fAlpha = clamp(m_fAlpha, 0.f, 255.f);
}

void CUI_FadeInOut::LateTick(_float fTimeDelta)
{
}

void CUI_FadeInOut::Begin_RenderState()
{
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
    m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255, 255, 255, (_uint)m_fAlpha));
}

HRESULT CUI_FadeInOut::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pTextureCom->Bind_Texture(m_iTexture_Index);

    Begin_RenderState();

    m_pVIBufferCom->Render();

    End_RenderState();

    return S_OK;
}
void CUI_FadeInOut::End_RenderState()
{
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
}
void CUI_FadeInOut::Set_FadeIn(_float fSpeed)
{
    m_eFadeState = FADEIN;
    m_fSpeed = fSpeed;
    m_fAlpha = 255.f;
}
void CUI_FadeInOut::Set_FadeOut(_float fSpeed)
{
    m_eFadeState = FADEOUT;
    m_fSpeed = fSpeed;
    m_fAlpha = 0.f;
}
_bool CUI_FadeInOut::IsFinished()
{
    if (m_eFadeState == FADEOUT && m_fAlpha == 255.f)
        return true;
    
    if (m_eFadeState == FADEIN && m_fAlpha == 0.f)
        return true;
    return false;
}
HRESULT CUI_FadeInOut::Initialize_Active()
{
    Initialize_Set_ActiveTime();
    Initialize_Set_Size();
    Initialize_Set_Speed();
    Initialize_Set_Scale_Pos_Rotation(nullptr);
    return S_OK;
}

void CUI_FadeInOut::Initialize_Set_ActiveTime()
{
    m_fActiveTime = 1.5f;
}

void CUI_FadeInOut::Initialize_Set_Size()
{
    m_UiDesc.m_fSizeX = g_iWinSizeX;
    m_UiDesc.m_fSizeY = g_iWinSizeY;
}

void CUI_FadeInOut::Initialize_Set_Speed()
{
}

void CUI_FadeInOut::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
    _float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
    m_UiDesc.m_fX = 0;
    m_UiDesc.m_fY = 0;

    m_pTransformCom->Set_Scale(Scale);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}


HRESULT CUI_FadeInOut::Add_Components(void* pArg)
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

HRESULT CUI_FadeInOut::Add_Texture(void* pArg)
{
    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("CUi_FadeInOut_Texture"),
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

CUI_FadeInOut* CUI_FadeInOut::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUI_FadeInOut* pInstance = new CUI_FadeInOut(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("CUI_FadeInOut Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_FadeInOut::Free()
{
    __super::Free();
}
