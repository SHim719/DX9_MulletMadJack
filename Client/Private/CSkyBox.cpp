#include "CSkyBox.h"

#include "GameInstance.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CSkyBox::CSkyBox(const CSkyBox& rhs)
    : CGameObject{ rhs }
{
}

HRESULT CSkyBox::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSkyBox::Initialize(void* pArg)
{
    if (FAILED(Add_Components()))
        return E_FAIL;

    m_iTexture_Index = 1;
    return S_OK;
}

void CSkyBox::PriorityTick(_float fTimeDelta)
{
}

void CSkyBox::Tick(_float fTimeDelta)
{
}

void CSkyBox::LateTick(_float fTimeDelta)
{
    _float4x4   ViewMatrix;

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

    D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, (_float3*)&ViewMatrix.m[3][0]);

    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CSkyBox::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
        return E_FAIL;

    if (FAILED(Begin_RenderState()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    if (FAILED(End_RenderState()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkyBox::Add_Components()
{
    m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
    if (nullptr == m_pTransformCom)
        return E_FAIL;

    m_pVIBufferCom = dynamic_cast<CVIBuffer_Cube*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Cube_Default"), TEXT("VIBuffer"), nullptr));
    if (nullptr == m_pVIBufferCom)
        return E_FAIL;

    m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("SkyBox_Texture"), TEXT("SkyBox_Texture"), nullptr));
    if (nullptr == m_pTextureCom)
        return E_FAIL;

    return S_OK;
}

HRESULT CSkyBox::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);

    m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    return S_OK;
}

HRESULT CSkyBox::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

    m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

    return S_OK;
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSkyBox* pInstance = new CSkyBox(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to create: CSkyBox"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSkyBox::Clone(void* pArg)
{
    CSkyBox* pInstance = new CSkyBox(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to clone: CSkyBox"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkyBox::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
}
