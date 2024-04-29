#pragma once

#include "Client_Defines.h"
#include "Effect.h"


BEGIN(Client)

class CBlink final : public CEffect
{
protected:
    CBlink(LPDIRECT3DDEVICE9 pGraphic_Device);
    CBlink(const CBlink& rhs);
    virtual ~CBlink() = default;

public:
    HRESULT Initialize(void* pArg) override;

private:
    HRESULT Add_Components();

    HRESULT Begin_RenderState() override;
    HRESULT End_RenderState() override;

public:
    static CBlink* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    CBlink* Clone(void* pArg)   override;
    void Free() override;
};

END