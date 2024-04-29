#pragma once

#include "Client_Defines.h"
#include "Effect.h"


BEGIN(Client)

class CStarLight final : public CEffect
{
protected:
    CStarLight(LPDIRECT3DDEVICE9 pGraphic_Device);
    CStarLight(const CStarLight& rhs);
    virtual ~CStarLight() = default;

public:
    HRESULT Initialize(void* pArg) override;

private:
    HRESULT Add_Components();

    HRESULT Begin_RenderState() override;
    HRESULT End_RenderState() override;

public:
    static CStarLight* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    CStarLight* Clone(void* pArg)   override;
    void Free() override;
};

END