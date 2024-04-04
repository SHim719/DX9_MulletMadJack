#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
protected:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	class CGameObject* m_pOwner = { nullptr };
	class CTransform*	m_pTransform = { nullptr };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	void Free()	override;
};

END

