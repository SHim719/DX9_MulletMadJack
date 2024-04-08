#include "TriggerObject.h"

CTriggerObject::CTriggerObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CTriggerObject::CTriggerObject(const CTriggerObject& rhs)
	: CGameObject{ rhs }
{
}

void CTriggerObject::Free()
{
	__super::Free();

	Safe_Release(m_pBoxCollider);
}
