#pragma once
#include "Client_Defines.h"
#include "Component.h"

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
    // fTimeAcc���� �ؽ�ó�� �ٲ� �ִϸ��̼��� ����Ǵ� �Լ�
    void    Change_TextureNum(_float fTimeDelta, _float fTimeAcc, _uint iMaxTextureNum);

public:
    _uint   Get_TextureNum() { return m_iTextureNum; }

private:
    _float			m_fTimeAcc;         // timedelta�� ������Ű�� ����
    _uint			m_iTextureNum;      // ���� �ؽ�ó ��ȣ

private:
    LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };

public:
    static CAnimation* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END