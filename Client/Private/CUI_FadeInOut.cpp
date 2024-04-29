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
    switch (m_eFadeState)
    {
    case FADEIN:
        m_fAlpha -= m_fSpeed * fTimeDelta;
        break;
    case FADEOUT:
        m_fAlpha += m_fSpeed * fTimeDelta;
        break;
    case FADEOUTIN:
    {
        if (!m_bFadeOutInLoop)
        {
            m_fAlpha += m_fSpeed * fTimeDelta;
            if (m_fAlpha >= m_fTargetFadeOutAlpha)
                m_bFadeOutInLoop = true;
        }
        else
            m_fAlpha -= m_fSpeed * fTimeDelta;
    }
        break;
    case FADESTATE_END:
        break;
    }
        
    m_fAlpha = clamp(m_fAlpha, m_fTargetFadeInAlpha, m_fTargetFadeOutAlpha);

    if (false == m_bManualOff)
    {
        switch (m_eFadeState)
        {
        case CUI_FadeInOut::FADEIN:
        case CUI_FadeInOut::FADEOUT:
            if (m_fAlpha == m_fTargetFadeInAlpha || m_fAlpha == m_fTargetFadeOutAlpha)
                Set_Active(false);
            break;
        case CUI_FadeInOut::FADEOUTIN:
            if (m_bFadeOutInLoop && (m_fAlpha == m_fTargetFadeInAlpha))
                Set_Active(false);
            break;
        case CUI_FadeInOut::FADESTATE_END:
            break;
        default:
            break;
        }
    }
        
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

    m_arrTextures[m_eFadeColor]->Bind_Texture(0);

    Begin_RenderState();

    m_pVIBufferCom->Render();

    End_RenderState();

    return S_OK;
}
void CUI_FadeInOut::End_RenderState()
{
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
}
void CUI_FadeInOut::Set_FadeIn(_float fSpeed, FADECOLOR eColor, _float fTargetAlpha, _bool ManualOff)
{
    m_eFadeState = FADEIN;
    m_eFadeColor = eColor;
    m_fSpeed = fSpeed;
    m_fAlpha = 255.f;
    m_fTargetFadeOutAlpha = 255.f;
    m_fTargetFadeInAlpha = fTargetAlpha;
    m_bManualOff = ManualOff;
}
void CUI_FadeInOut::Set_FadeOut(_float fSpeed, FADECOLOR eColor, _float fTargetAlpha, _bool ManualOff)
{
    m_eFadeState = FADEOUT;
    m_eFadeColor = eColor;
    m_fSpeed = fSpeed;
    m_fAlpha = 0.f;
    m_fTargetFadeOutAlpha = fTargetAlpha;
    m_fTargetFadeInAlpha = 0.f;
    m_bManualOff = ManualOff;
}

void CUI_FadeInOut::Set_FadeOutIn(_float fSpeed, FADECOLOR eColor, _float fTargetFadeOutAlpha, _float fTargetFadeInAlpha, _bool ManualOff)
{
    m_eFadeState = FADEOUTIN;
    m_eFadeColor = eColor;
    m_fSpeed = fSpeed;
    m_fAlpha = 0.f;
    m_fTargetFadeOutAlpha = fTargetFadeOutAlpha;
    m_fTargetFadeInAlpha = fTargetFadeInAlpha;
    m_bManualOff = ManualOff;
    m_bFadeOutInLoop = false;
}


_bool CUI_FadeInOut::IsFinished()
{
    if ((m_eFadeState == FADEIN || m_eFadeState == FADEOUTIN) && m_fAlpha == m_fTargetFadeInAlpha)
        return true;

    if (m_eFadeState == FADEOUT && m_fAlpha == m_fTargetFadeOutAlpha)
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
        (CComponent**)&m_arrTextures[BLACK])))
        return E_FAIL;

    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("CUi_Dead_FadeOut_Texture"),
        (CComponent**)&m_arrTextures[RED])))
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

    for (auto& pTexture : m_arrTextures)
    {
        if (pTexture)
         Safe_Release(pTexture);
    }
        
}
