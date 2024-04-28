#pragma once
#include "Client_Defines.h"
#include "Effect.h"


BEGIN(Client)

class CSansMiss final : public CEffect
{
protected:
    CSansMiss(LPDIRECT3DDEVICE9 pGraphic_Device);
    CSansMiss(const CSansMiss& rhs);
    virtual ~CSansMiss() = default;

public:
    HRESULT Initialize(void* pArg) override;

private:
    HRESULT Add_Components();

public:
    static CSansMiss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    CSansMiss* Clone(void* pArg)   override;
    void Free() override;
};

END