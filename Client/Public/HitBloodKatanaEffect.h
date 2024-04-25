#include "Client_Defines.h"
#include "Effect.h"


BEGIN(Client)

class CHitBloodKatanaEffect final : public CEffect
{
protected:
    CHitBloodKatanaEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
    CHitBloodKatanaEffect(const CHitBloodKatanaEffect& rhs);
    virtual ~CHitBloodKatanaEffect() = default;

public:
    HRESULT Initialize(void* pArg) override;

private:
    HRESULT Add_Components();

public:
    static CHitBloodKatanaEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    CHitBloodKatanaEffect* Clone(void* pArg)   override;
    void Free() override;
};

END