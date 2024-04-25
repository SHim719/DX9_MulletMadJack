#pragma once

#include "TriggerObject.h"

BEGIN(Engine)
class CLevel;
END

class CStageEndTrigger : public CTriggerObject
{
private:
	CStageEndTrigger(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStageEndTrigger(const CStageEndTrigger& rhs);
	virtual ~CStageEndTrigger() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	HRESULT Add_Components();

private:
	LEVEL m_eNextLevel;

private:
	void OnTriggerEnter(CGameObject* pOther) override;

public:
	void Set_NextLevel(LEVEL eNextLevel) { m_eNextLevel = eNextLevel; }

public:
	static CStageEndTrigger* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg)	override;
	void Free() override;

};

