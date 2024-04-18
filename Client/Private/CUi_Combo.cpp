#include "CUi_Combo.h"
#include "GameInstance.h"
#include "CUi_Background.h"
#include "CUi_SpecialHit_Part.h"

CUi_Combo::CUi_Combo(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUi(pGraphic_Device)
{
}

CUi_Combo::CUi_Combo(const CUi_Combo& rhs)
    :CUi(rhs)
{
}

HRESULT CUi_Combo::Initialize_Prototype()
{
    Default_Set_LifeTime();
    Default_Set_Pos();
    Default_Set_Size();

    return S_OK;
}

HRESULT CUi_Combo::Initialize(void* pArg)
{
    if (FAILED(Add_Components(nullptr)))
        return E_FAIL;

    Initialize_Set_Speed();
    Initialize_Set_Scale_Pos_Rotation(nullptr);
    Initialize_SetComboDesc(pArg);

    m_pBackGround = (CUi_Background*)m_pGameInstance->
        Add_Ui_PartClone(TEXT("CUi_BackGround"), &m_UiDesc);


    CUi_SpecialHit_Part::PartDesc Set;
    Set.Desc.m_fX = m_UiDesc.m_fX + 30;
    Set.Desc.m_fY = m_UiDesc.m_fY - 10;
    Set.Desc.m_fSizeX = m_UiDesc.m_fSizeX * 0.6f;
    Set.Desc.m_fSizeY = m_UiDesc.m_fSizeY * 0.6f;
    Set.Desc.m_Rotation = m_UiDesc.m_Rotation;
    Set.Second = (PlusSecond)m_iTexture_Index;
    if ((_uint)Set.Second >= 7)
    {
        Set.Second = PlusSecond::Eight;
    }
    m_pSpecialHit_Part = (CUi_SpecialHit_Part*)
        m_pGameInstance->Add_Ui_PartClone(TEXT("CUi_SpecialHit_Part"), &Set);

    return S_OK;
}

void CUi_Combo::PriorityTick(_float fTimeDelta)
{
}

void CUi_Combo::Tick(_float fTimeDelta)
{
    m_fLifeTime -= fTimeDelta;
    m_fBlink += fTimeDelta;
    if (m_bFirstCall)
    {
        Scaling(fTimeDelta);
    }
    else
    {
        m_pTransformCom->Set_Scale(m_OriginComboScale);
    }
    Cal_Life_Blink(fTimeDelta);
}

void CUi_Combo::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Combo::Render()
{
    if(Cal_BlinkRender(0.35f))
    {
        m_pBackGround->Render();
        m_pSpecialHit_Part->Render();

        if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
            return E_FAIL;

        m_pTextureCom->Bind_Texture(m_iTexture_Index);

        m_pVIBufferCom->Render();
    }

    return S_OK;
}

HRESULT CUi_Combo::Add_Components(void* pArg)
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

HRESULT CUi_Combo::Add_Texture(void* pArg)
{
    if (FAILED(Add_Component(LEVEL_STATIC, TEXT("CUi_Combo_Texture")
        ,(CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

void CUi_Combo::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
    _float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

    m_pTransformCom->Set_Scale(Scale);
    m_pTransformCom->Set_State
    (CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
    m_UiDesc.m_Rotation = { 0.f, 0.f, 10.f };
    m_pTransformCom->Rotation_XYZ(m_UiDesc.m_Rotation);
}

void CUi_Combo::Initialize_SetComboDesc(void* pArg)
{
    ComboDesc* Desc = (ComboDesc*)pArg; 
    m_bFirstCall = Desc->bFirstCall;
    m_iTexture_Index = Desc->iKillCount;
    if (m_iTexture_Index >= 7)
    {
        m_iTexture_Index = 7;
    }
}

void CUi_Combo::Default_Set_LifeTime()
{
    m_fLifeTime = 3.5f;
}

void CUi_Combo::Default_Set_Size()
{
    m_UiDesc.m_fSizeX = 150;
    m_UiDesc.m_fSizeY = 50;
}

void CUi_Combo::Default_Set_Pos()
{
    m_UiDesc.m_fX = -400;
    m_UiDesc.m_fY = 50;
}

void CUi_Combo::Initialize_Set_Speed()
{
}

void CUi_Combo::Scaling(_float fTimeDelta)
{
    m_fScalingTime += fTimeDelta;
    if (m_fScalingTime < 0.3f)
    {
        _float3 ScaleUp = { 1.04f, 1.04f, 1.f };
        m_pTransformCom->Multiply_Scale(ScaleUp);
    }
    else if (m_fScalingTime < 0.6)
    {
       _float3 ScaleDown = { 0.97f, 0.97f, 1.f };
       m_pTransformCom->Multiply_Scale(ScaleDown);
    }
    else
    {
        m_fScalingTime = 0;
        m_bFirstCall = false;
        m_pTransformCom->Set_Scale(m_OriginComboScale);
    }
}

CUi_Combo* CUi_Combo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUi_Combo* pInstance = new CUi_Combo(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("CUi_Combo Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CUi* CUi_Combo::Clone(void* pArg)
{
    CUi_Combo* pInstance = new CUi_Combo(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("CUi_Combo Clone Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUi_Combo::Free()
{
    __super::Free();
    Safe_Release(m_pSpecialHit_Part);
    Safe_Release(m_pBackGround);
}
