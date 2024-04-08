#include "CUi_Fine.h"
#include "Ui_Pos.h"


CUi_Fine::CUi_Fine(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUi(pGraphic_Device)
{
}

CUi_Fine::CUi_Fine(const CUi_Fine& rhs)
    :CUi(rhs)
{
}

HRESULT CUi_Fine::Initialize_Prototype()
{
    if (FAILED(Add_Components(nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUi_Fine::Initialize(void* pArg)
{
    return S_OK;
}

void CUi_Fine::PriorityTick(_float fTimeDelta)
{
}

void CUi_Fine::Tick(_float fTimeDelta)
{
    m_fActiveTime -= fTimeDelta;

    if (m_fActiveTime > 0)
    {
        Move(fTimeDelta);
    }
    else if (m_fActiveTime < 0 && m_bEnter)
    {
        m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Ui_Pos::Fine);
    }
    else if (m_fActiveTime < 0 && !m_bEnter)
    {
        m_bActive = false;
    }
}

void CUi_Fine::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Fine::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;


    m_pTextureCom->Bind_Texture(m_iTexture_Index);
    m_pVIBufferCom->Render();


    return S_OK;
}

HRESULT CUi_Fine::Initialize_Active()
{
    Initialize_Set_ActiveTime();
    Initialize_Set_Size();
    Initialize_Set_Speed();
    Initialize_Set_Scale_Pos_Rotation(nullptr);


    return S_OK;
}

void CUi_Fine::Initialize_Set_ActiveTime()
{
    m_fActiveTime = 0.3f;
}

void CUi_Fine::Initialize_Set_Size()
{
    m_UiDesc.m_fSizeX = 256;
    m_UiDesc.m_fSizeY = 128;
}

void CUi_Fine::Initialize_Set_Speed()
{
    m_pTransformCom->Set_Speed(750);
}

void CUi_Fine::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
    _float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
    
    m_UiDesc.m_fX = -540.f;
    m_UiDesc.m_fY = -500.f;

    m_pTransformCom->Set_Scale(Scale);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_Fine::Add_Components(void* pArg)
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

HRESULT CUi_Fine::Add_Texture(void* pArg)
{
    if (FAILED(Add_Component(LEVEL_STATIC,
        TEXT("CUi_Fine_Texture"),
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;


    return S_OK;
}

void CUi_Fine::Enter(bool _Enter)
{
    m_bEnter = _Enter;
    m_bActive = true;
    if (m_bEnter)
    {
        Initialize_Set_ActiveTime();
        Initialize_Set_Size();
        Initialize_Set_Speed();
        Initialize_Set_Scale_Pos_Rotation(nullptr);
    }
    else
    {
        Initialize_Set_ActiveTime();
    }
}

void CUi_Fine::Move(_float fTimeDelta)
{
    if (m_bEnter)
    {
        m_pTransformCom->Go_Up(fTimeDelta);
    }
    else
    {
        m_pTransformCom->Go_Down(fTimeDelta);
    }
}

CUi_Fine* CUi_Fine::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUi_Fine* pInstance = new CUi_Fine(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("CUi_PEACE Create Failed"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUi_Fine::Free()
{
    __super::Free();
}
