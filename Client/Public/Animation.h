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
    CTexture*       Find_Texture(const wstring& strPrototypeTag);
    HRESULT         Bind_Texture(_uint iTextureIndex);
    void            Play_Animation(const wstring& strAnimeTag, _float fTimeAcc, bool bLoop);

public:
    _uint           Get_TextureNum() { return m_iTextureNum; }
    CTexture*       Get_Texture() { return m_pCurTexture; }

public:
    void            Update(_float fTimeDelta, bool& IsPlaying);
    void            Render();

private:
    _float			m_fTimeAcc;         // timedelta값 누적시키는 변수
    _uint			m_iTextureNum;      // 현재 텍스처 번호
    _uint			m_iMaxTextureNum;      // 현재 텍스처 번호
    _float          m_fFrameGap;
    _bool           m_bLoop;

    CTexture* m_pCurTexture = { nullptr };
    map<const wstring, CTexture*>   m_Textures;

private:
    LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };
    CGameInstance* m_pGameInstance = { nullptr };

public:
    static CAnimation* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END