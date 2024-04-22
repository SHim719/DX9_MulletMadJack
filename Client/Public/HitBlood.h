#include "Client_Defines.h"
#include "Effect.h"


BEGIN(Client)

class CHitBlood final : public CEffect
{
protected:
    CHitBlood(LPDIRECT3DDEVICE9 pGraphic_Device);
    CHitBlood(const CHitBlood& rhs);
    virtual ~CHitBlood() = default;

public:
    HRESULT Initialize(void* pArg) override;

private:
    HRESULT Add_Components();

public:
    static CHitBlood* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    CHitBlood* Clone(void* pArg)   override;
    void Free() override;
};

END