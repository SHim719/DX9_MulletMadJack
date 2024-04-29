#include "CUi_Ultimate_Finish.h"

CUi_Ultimate_Finish::CUi_Ultimate_Finish(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUi(pGraphic_Device)
{
}

CUi_Ultimate_Finish::CUi_Ultimate_Finish(const CUi_Ultimate_Finish& rhs)
    :CUi(rhs)
{
}

HRESULT CUi_Ultimate_Finish::Initialize_Prototype()
{
    if (FAILED(Add_Components(nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUi_Ultimate_Finish::Initialize(void* pArg)
{
    return S_OK;
}

void CUi_Ultimate_Finish::PriorityTick(_float fTimeDelta)
{
}

void CUi_Ultimate_Finish::Tick(_float fTimeDelta)
{
    m_fActiveTime -= fTimeDelta;
    if (m_fActiveTime > 0)
    {
        Scaling(fTimeDelta);
    }
    else
    {
        CPlayer_Manager::Get_Instance()->Set_BossCutscene(false);
        m_bActive = false;
    }
    ChangeColor();
}

void CUi_Ultimate_Finish::LateTick(_float fTimeDelta)
{
}

void CUi_Ultimate_Finish::Render_Begin()
{
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(m_Red, m_Green, m_Blue, 255));
}

HRESULT CUi_Ultimate_Finish::Render()
{
    Render_Begin();

    if (FAILED(m_pBackGroundTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pBackGroundTextureCom->Bind_Texture(0);
    m_pBackGroundVIBufferCom->Render();

    Render_End();

    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pTextureCom->Bind_Texture(m_iTexture_Index);
    m_pVIBufferCom->Render();

    return S_OK;
}
void CUi_Ultimate_Finish::Render_End()
{
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
}

HRESULT CUi_Ultimate_Finish::Initialize_Active()
{
    Initialize_Set_ActiveTime();
    Initialize_Set_Size();
    Initialize_Set_Speed();
    Initialize_Set_Scale_Pos_Rotation(nullptr);

    m_eRGBstate = RGBState::Red;
    m_Red = 255;
    m_Green = 0;
    m_Blue = 0;

    return S_OK;
}

void CUi_Ultimate_Finish::Initialize_Set_ActiveTime()
{
    m_fActiveTime = 3.f;
    m_fScalingTime = 0;
}

void CUi_Ultimate_Finish::Initialize_Set_Size()
{
    m_UiDesc.m_fSizeX = 5600;
    m_UiDesc.m_fSizeY = 700;

    m_BackGround.m_fSizeX = 450;
    m_BackGround.m_fSizeY = 200;
}

void CUi_Ultimate_Finish::Initialize_Set_Speed()
{
}

void CUi_Ultimate_Finish::Initialize_Set_Scale_Pos_Rotation(void* pArg)
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
void CUi_Ultimate_Finish::ChangeColor()
{
    switch (m_eRGBstate)
    {
    case Client::RGBState::Red:
        if (m_Green >= 245)
        {
            m_Green = 255;
            m_Red = 255;
            m_eRGBstate = RGBState::Yellow;
        }
        else
            m_Green += 10;
        break;
    case Client::RGBState::Yellow:
        if (m_Red >= 10)
        {
            m_Green = 255;
            m_Red = 0;
            m_eRGBstate = RGBState::Green;
        }
        else
            m_Red -= 10;
        break;
    case Client::RGBState::Green:
        if (m_Blue >= 245)
        {
            m_Blue = 255;
            m_Green = 0;
            m_eRGBstate = RGBState::Blue;
        }
        else
        {
            m_Green -= 10;
            m_Blue += 10;
        }
        break;
    case Client::RGBState::Blue:
        if (m_Red >= 245)
        {
            m_Red = 255;
            m_Blue = 255;
            m_eRGBstate = RGBState::Purple;
        }
        else
        {
            m_Red += 10;
        }
        break;
    case Client::RGBState::Purple:
        if (m_Blue <= 10)
        {
            m_Red = 255;
            m_Blue = 0;
            m_eRGBstate = RGBState::Red;
        }
        else
        {
            m_Blue -= 10;
        }
        break;
    case Client::RGBState::End:
        break;
    default:
        break;
    }
}
HRESULT CUi_Ultimate_Finish::Add_Components(void* pArg)
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

HRESULT CUi_Ultimate_Finish::Add_Texture(void* pArg)
{
    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("CUi_Ultimate_Finish_Texture"),
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("CUi_Finish_BackGround_Texture"),
        (CComponent**)&m_pBackGroundTextureCom)))
        return E_FAIL;

    return S_OK;
}

void CUi_Ultimate_Finish::Scaling(_float fTimeDelta)
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

CUi_Ultimate_Finish* CUi_Ultimate_Finish::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUi_Ultimate_Finish* pInstance = new CUi_Ultimate_Finish(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("CUi_Ultimate_Finish Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUi_Ultimate_Finish::Free()
{
    __super::Free();

    Safe_Release(m_pBackGroundTextureCom);
    Safe_Release(m_pBackGroundTransformCom);
    Safe_Release(m_pBackGroundVIBufferCom);
}
