#include "Enemy.h"

#include "GameInstance.h"

CEnemy::CEnemy(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CEnemy::CEnemy(const CEnemy& rhs)
    : CGameObject{ rhs }
{
}

HRESULT CEnemy::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEnemy::Initialize(void* pArg)
{
    if (FAILED(Add_Components()))
        return E_FAIL;

    // Level_GamePlay ��ü�� ����

    // Level_GamePlay ��ü�� �ִ� FPS ī�޶� ��ü�� ���� ��
    m_pFPS_Camera = dynamic_cast<CLevel_GamePlay*>(m_pLevel)->Get_Camera();

    return S_OK;
}

void CEnemy::PriorityTick(_float fTimeDelta)
{
}

void CEnemy::Tick(_float fTimeDelta)
{
}

void CEnemy::LateTick(_float fTimeDelta)
{
    // 0.1�ʸ��� �ؽ�ó�� �ٲ㼭 �ִϸ��̼��� ����ǰ� ��
    m_pAnimationCom->Change_TextureNum(fTimeDelta, 0.1f, m_pTextureCom->Get_MaxTextureNum());

    // ���� ����� ���� ��ȯ ����� FPS ī�޶� ��ü���� ���� ������ ��ķ� ������
    m_pTransformCom->Set_Billboard_Matrix(m_pFPS_Camera->Get_Billboard_Matrix());

    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CEnemy::Render()
{
    if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Texture(m_pAnimationCom->Get_TextureNum()))) // ���� ��ȣ�� �ؽ�ó ���
        return E_FAIL;

    if (FAILED(Begin_RenderState()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    if (FAILED(End_RenderState()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEnemy::Add_Components()
{
    m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("Com_VIBuffer")));

    CTransform::TRANSFORM_DESC	TransformDesc{};

    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Com_Transform")));

    m_pTextureCom = dynamic_cast<CTexture*>(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy"), TEXT("Com_Texture")));
    
    m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animation"), TEXT("Com_Animation")));

    return S_OK;
}

HRESULT CEnemy::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CEnemy::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

CEnemy* CEnemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CEnemy* pInstance = new CEnemy(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to create: CEnemy"));

        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CEnemy::Clone(void* pArg)
{
    CEnemy* pInstance = new CEnemy(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to create: CEnemy"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEnemy::Free()
{
    __super::Free();

    Safe_Release(m_pAnimationCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
}
