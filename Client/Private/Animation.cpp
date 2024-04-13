#include "Animation.h"

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent{ pGraphic_Device }
    , m_pGameInstance{ CGameInstance::Get_Instance() }
{
}

CAnimation::CAnimation(const CAnimation& rhs)
    : CComponent{ rhs }
    , m_pGameInstance{ rhs.m_pGameInstance }
    , m_fTimeAcc(0.f)
    , m_iTextureNum(0)
    , m_iMaxTextureNum(0)
    , m_fFrameGap(0.f)
    , m_bLoop(true)
{
}

HRESULT CAnimation::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CAnimation::Initialize(void* pArg)
{
    CGameObject* pInstance = static_cast<CGameObject*>(pArg);

    m_pCurTexture = dynamic_cast<CTexture*>(pInstance);

    return S_OK;
}

HRESULT CAnimation::Insert_Textures(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strAnimeTag)
{
    CTexture* pInstance = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(iLevelIndex, strPrototypeTag));

    if (nullptr != Find_Texture(strPrototypeTag))
        return E_FAIL;

    m_Textures.emplace(strAnimeTag, pInstance);

    return S_OK;
}

CTexture* CAnimation::Find_Texture(const wstring& strPrototypeTag)
{
    auto iter = m_Textures.find(strPrototypeTag);

    if (iter == m_Textures.end())
        return nullptr;

    return iter->second;
}

HRESULT CAnimation::Bind_Texture(_uint iTextureIndex)
{
    if (iTextureIndex >= (_uint)m_pCurTexture->Get_Textures().size())
        return E_FAIL;

    return m_pGraphic_Device->SetTexture(0, m_pCurTexture->Get_Textures()[iTextureIndex]);
}

void CAnimation::Play_Animation(const wstring& strAnimeTag, _float fTimeGap, bool bLoop)
{
    m_pCurTexture = Find_Texture(strAnimeTag);

    m_iMaxTextureNum = m_pCurTexture->Get_MaxTextureNum();

    m_fFrameGap = fTimeGap;

    m_bLoop = bLoop;

    m_iTextureNum = 0;
}

void CAnimation::Update(_float fTimeDelta, bool& IsPlaying, bool& IsDead)
{
    m_fTimeAcc += fTimeDelta;

    if (m_fTimeAcc > m_fFrameGap)  // ������ �ð� ���ݸ��� ���� �ؽ�ó�� ������ �� (����: Pawn �ؽ�ó�� 0.1�ʸ��� �ٲ�)
    {
        if (m_iTextureNum < m_iMaxTextureNum)   // ���� �ؽ�ó ��ȣ�� �ؽ�ó ��ȣ �ִ밪�� ���� �ʴ´ٸ�
            ++m_iTextureNum;    // 1 �������Ѽ� ���� �ؽ�ó �ҷ��� �ִϸ��̼��� �����
        else if (m_bLoop)   // �ݺ��Ǵ� �ִϸ��̼��̶��
            m_iTextureNum = 0;  // 0������ �ٲ㼭 ó������ ���ư��� ����

        if (m_iTextureNum == m_iMaxTextureNum && true == IsPlaying)
            IsPlaying = false;

        m_fTimeAcc = 0.f;
    }
}

void CAnimation::Render()
{
    m_pCurTexture->Bind_Texture(m_iTextureNum);
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
    for (auto& iter : m_Textures)
        Safe_Release(iter.second);

    m_Textures.clear();

    __super::Free();
}
