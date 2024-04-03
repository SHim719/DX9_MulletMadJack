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
    // 일단 애니메이션이 무한히 반복되는 식으로 해둠

    m_fTimeAcc += fTimeDelta;

    if (m_fTimeAcc > fTimeAcc)  // 설정한 시간 간격마다 다음 텍스처가 나오게 함 (예시: Enemy 텍스처는 0.1초마다 바뀜)
    {
        if (m_iTextureNum < iMaxTextureNum)     // 현재 텍스처 번호가 텍스처 번호 최대값을 넘지 않는다면 
            ++m_iTextureNum;    // 1 증가시켜서 다음 텍스처 불러서 애니메이션이 진행됨
        else    // 현재 텍스처 번호가 텍스처 번호 최대값에 도달하면
            m_iTextureNum = 0;  // 0번으로 바꿔서 처음부터 돌아가게 만듬

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
