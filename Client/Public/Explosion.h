#pragma once

#include "Client_Defines.h"
#include "Effect.h"


BEGIN(Client)

class CExplosion final : public CEffect
{
protected:
    CExplosion(LPDIRECT3DDEVICE9 pGraphic_Device);
    CExplosion(const CExplosion& rhs);
    virtual ~CExplosion() = default;

public:
    HRESULT Initialize(void* pArg) override;

private:
    HRESULT Add_Components();

    HRESULT Begin_RenderState() override;
    HRESULT End_RenderState() override;

public:
    static CExplosion* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    CExplosion* Clone(void* pArg)   override;
    void Free() override;
};

END