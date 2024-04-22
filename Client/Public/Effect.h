#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer;
END

BEGIN(Client)

class CEffect abstract : public CGameObject
{
protected:
    CEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
    CEffect(const CEffect& rhs);
    virtual ~CEffect() = default;

public:
    virtual void PriorityTick(_float fTimeDelta) override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    HRESULT Begin_RenderState() override;
    HRESULT End_RenderState()   override;

protected:
    CAnimation* m_pAnimation = { nullptr };
    CVIBuffer*  m_pVIBuffer = { nullptr };

    wstring     m_strSoundEffect;

public:
    virtual CEffect* Clone(void* pArg) = 0;
    virtual void Free() override;
};

END