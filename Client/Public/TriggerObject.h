#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CBoxCollider;
END

BEGIN(Client)

class CTriggerObject abstract : public CGameObject
{
protected:
	CTriggerObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTriggerObject(const CTriggerObject& rhs);
	virtual ~CTriggerObject() = default;

public:
	void Tick(_float fTimeDelta)	override;
	void LateTick(_float fTimeDelta) override;
	HRESULT Render()				override;

protected:
	CBoxCollider* m_pBoxCollider = { nullptr };

public:
	CGameObject* Clone(void* pArg) = 0;
	void Free() override;
};

END