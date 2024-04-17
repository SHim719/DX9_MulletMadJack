#include "CUi_LoadingBackGround.h"
#include "GameInstance.h"

CUi_LoadingBackGround::CUi_LoadingBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUi(pGraphic_Device)
{
}

CUi_LoadingBackGround::CUi_LoadingBackGround(const CUi_LoadingBackGround& rhs)
    :CUi(rhs)
{
}

HRESULT CUi_LoadingBackGround::Initialize_Prototype()
{
    Default_Set_Size();

    return S_OK;
}

HRESULT CUi_LoadingBackGround::Initialize(void* pArg)
{
    if (FAILED(Add_Components(nullptr)))
        return E_FAIL;

    Initialize_Bar();
    Initialize_Set_Scale_Pos_Rotation(nullptr);
    return S_OK;
}

void CUi_LoadingBackGround::PriorityTick(_float fTimeDelta)
{
}

void CUi_LoadingBackGround::Tick(_float fTimeDelta)
{
    m_fTextureSwitchingTime += fTimeDelta;
    if (m_fTextureSwitchingTime > 2)
    {
        m_fTextureSwitchingTime = 0;
        ++m_iTexture_Index;
        if (m_iTexture_Index > m_pTextureCom->Get_MaxTextureNum())
        {
            m_iTexture_Index = 0;
        }
    }
    Adjust_LoadingBar(fTimeDelta);
}

void CUi_LoadingBackGround::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_LoadingBackGround::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    m_pTextureCom->Bind_Texture(m_iTexture_Index);
    m_pVIBufferCom->Render();

    Render_Bar();

    return S_OK;
}
void CUi_LoadingBackGround::Initialize_Set_Speed()
{
}

void CUi_LoadingBackGround::Set_BarRatio(_float Ratio)
{
    m_fNew_Ratio = Ratio;
}

void CUi_LoadingBackGround::Render_Bar()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);

    m_pBarTransformCom->Bind_WorldMatrix();
    m_pBarTextureCom->Bind_Texture(0);
    m_pBarVIBufferCom->Render();

    Render_Text();

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CUi_LoadingBackGround::Render_Text()
{

    _uint Percent = _uint(m_fOld_Ratio * 100);
    wstring temp = to_wstring(Percent);
    temp += L"%";
    m_PrintPercent.Text = temp.c_str();
    m_PrintPercent.Length = (int)wcslen(m_PrintPercent.Text);
    m_PrintPercent.Rect = { 980, 540, 1200, 600 };
    m_PrintPercent.RGBA = D3DCOLOR_RGBA(255, 0, 0, 255);
    m_pGameInstance->Print_Big_Text(m_PrintPercent);
}

void CUi_LoadingBackGround::Initialize_Bar()
{
    Add_Component(
        LEVEL_STATIC,
        TEXT("VIBuffer_Rect_Default"),
        (CComponent**)&m_pBarVIBufferCom);

    Add_Component(LEVEL_STATIC,
        TEXT("Transform_Default"),
        (CComponent**)&m_pBarTransformCom);

    Add_Component(LEVEL_STATIC
        , TEXT("CUi_Loading_BackGroundBar_Texture")
        , (CComponent**)&m_pBarTextureCom);

    _float Xpos = 300;
    _float Ypos = -250;

    m_pBarTransformCom->Set_Scale(m_Origin_BarScale);
    m_pBarTransformCom->Set_State(CTransform::STATE_POSITION,
        &_float3(Xpos, Ypos, 0.f));
}

void CUi_LoadingBackGround::Adjust_LoadingBar(_float fTimeDelta)
{
    m_fLoadingTime += fTimeDelta;
    if (m_fNew_Ratio == 1)
    {
        m_fOld_Ratio = 1;
        m_pBarTransformCom->Set_Scale(m_Origin_BarScale);
    }
    else if (m_fLoadingTime > 0.05 && m_fNew_Ratio > m_fOld_Ratio)
    {
        m_fLoadingTime = 0;
        m_fOld_Ratio += 0.03f;
        m_Render_BarScale.x = m_Origin_BarScale.x * m_fOld_Ratio;
        m_pBarTransformCom->Set_Scale(m_Render_BarScale);
    }
    
}

void CUi_LoadingBackGround::Default_Set_Size()
{
    m_UiDesc.m_fSizeX = 1500;
    m_UiDesc.m_fSizeY = 700;
}

void CUi_LoadingBackGround::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
    _float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

    m_UiDesc.m_fX = 0;
    m_UiDesc.m_fY = 0;

    m_pTransformCom->Set_Scale(Scale);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, 
        &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_LoadingBackGround::Add_Components(void* pArg)
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

HRESULT CUi_LoadingBackGround::Add_Texture(void* pArg)
{
    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("CUi_Loading_BackGround_Texture"),
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

CUi_LoadingBackGround* CUi_LoadingBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUi_LoadingBackGround* pInstance = new CUi_LoadingBackGround(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("CUi_LoadingBackGround Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CUi* CUi_LoadingBackGround::Clone(void* pArg)
{
    CUi_LoadingBackGround* pInstance = new CUi_LoadingBackGround(*this);
    if (FAILED(pInstance->Initialize(nullptr)))
    {
        MSG_BOX(TEXT("CUi_LoadingBackGround clone Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUi_LoadingBackGround::Free()
{
    __super::Free();
    Safe_Release(m_pBarTextureCom);
    Safe_Release(m_pBarTransformCom);
    Safe_Release(m_pBarVIBufferCom);
}
