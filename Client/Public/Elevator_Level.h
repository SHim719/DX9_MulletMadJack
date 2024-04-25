#include "Client_Defines.h"
#include "Level.h"


BEGIN(Client)

class CElevator_Level final : public CLevel
{
private:
	CElevator_Level(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CElevator_Level() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CElevator_Level* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

private:
	HRESULT Ready_Layer_Camera(const wstring& strLayerTag);
	HRESULT Ready_Layer_Player();

	class CPlayer* m_pPlayer = { nullptr };
	class CFPS_Camera* m_pFPS_Camera = { nullptr };
};

END