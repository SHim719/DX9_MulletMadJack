#pragma once
#include "Client_Defines.h"
#include "Component.h"
#include "GameInstance.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)

class CAnimation final : public CComponent
{
private:
    CAnimation(LPDIRECT3DDEVICE9 pGraphic_Device);
    CAnimation(const CAnimation& rhs);
    virtual ~CAnimation() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);

public:
    HRESULT         Insert_Textures(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strAnimeTag);
    void            Play_Animation(const wstring& strAnimeTag, _float fTimeAcc, bool bLoop);

private:
    CTexture*       Find_Texture(const wstring& strPrototypeTag);
    HRESULT         Bind_Texture(_uint iTextureIndex);

public:
    _uint           Get_TextureNum() { return m_iTextureNum; }
    CTexture*       Get_Texture() { return m_pCurTexture; }

    _bool           IsEndAnim() { return m_bAnimEnd; }

public:
    void            Update(_float fTimeDelta);
    void            Render();

private:
    _float			m_fTimeAcc = { 0.f };         // timedelta�� ������Ű�� ����
    _uint			m_iTextureNum;      // ���� �ؽ�ó ��ȣ
    _uint			m_iMaxTextureNum;      // ���� �ؽ�ó ��ȣ
    _float          m_fFrameGap = { 0.f };
    _bool           m_bLoop = { false };
    _bool           m_bAnimEnd = { false };

    CTexture* m_pCurTexture = { nullptr };
    map<const wstring, CTexture*>   m_Textures;

private:
    LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };
    CGameInstance*      m_pGameInstance = { nullptr };

public:
    static CAnimation* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END