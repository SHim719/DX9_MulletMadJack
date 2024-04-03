#include "Animation.h"

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent{ pGraphic_Device }
{
}

CAnimation::CAnimation(const CAnimation& rhs)
    : CComponent{ rhs }
    , m_fTimeAcc(0.f)
    , m_iTextureNum(0)
{
}

HRESULT CAnimation::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CAnimation::Initialize(void* pArg)
{
    return S_OK;
}

void CAnimation::Change_TextureNum(_float fTimeDelta, _float fTimeAcc, _uint iMaxTextureNum)
{
    // �ϴ� �ִϸ��̼��� ������ �ݺ��Ǵ� ������ �ص�

    m_fTimeAcc += fTimeDelta;

    if (m_fTimeAcc > fTimeAcc)  // ������ �ð� ���ݸ��� ���� �ؽ�ó�� ������ �� (����: Enemy �ؽ�ó�� 0.1�ʸ��� �ٲ�)
    {
        if (m_iTextureNum < iMaxTextureNum)     // ���� �ؽ�ó ��ȣ�� �ؽ�ó ��ȣ �ִ밪�� ���� �ʴ´ٸ� 
            ++m_iTextureNum;    // 1 �������Ѽ� ���� �ؽ�ó �ҷ��� �ִϸ��̼��� �����
        else    // ���� �ؽ�ó ��ȣ�� �ؽ�ó ��ȣ �ִ밪�� �����ϸ�
            m_iTextureNum = 0;  // 0������ �ٲ㼭 ó������ ���ư��� ����

        m_fTimeAcc = 0.f;
    }
}

CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CAnimation* pInstance = new CAnimation(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to create: CAnimation"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CAnimation::Clone(void* pArg)
{
    CAnimation* pInstance = new CAnimation(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to clone: CAnimation"));

        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAnimation::Free()
{
    __super::Free();
}
