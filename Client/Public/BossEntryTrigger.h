#pragma once
#include "Client_Defines.h"
#include "TriggerObject.h"


BEGIN(Client)
class CBossEntryTrigger : public CTriggerObject
{
private:
	CBossEntryTrigger(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBossEntryTrigger(const CBossEntryTrigger& rhs);
	virtual ~CBossEntryTrigger() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	HRESULT Add_Components();

private:
	void OnTriggerEnter(CGameObject* pOther) override;

private:
	class CLevel_Boss* m_pBoss_Level = { nullptr };

public:
	void Set_Level(CLevel_Boss* pLevel) { m_pBoss_Level = pLevel; }

public:
	static CBossEntryTrigger* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg)	override;
	void Free() override;
};
END

