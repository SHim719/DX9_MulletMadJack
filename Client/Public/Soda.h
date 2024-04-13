#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)

class CSoda : public CGameObject
{
private:
	CSoda(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSoda(const CSoda& rhs);
	virtual ~CSoda() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg)			override;
	void	PriorityTick(_float fTimeDelta)	override;
	void	Tick(_float fTimeDelta)			override;
	void	LateTick(_float fTimeDelta)		override;
	HRESULT Render()						override;

private:
	HRESULT Add_Components();

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CRigidbody*		m_pRigidbody = { nullptr };
	CBoxCollider*	m_pBoxCollider = { nullptr };

private:
	void OnCollisionEnter(CGameObject* pOther) override {}

private:
	_float m_fLifeTime = 5.f;

public:
	static CSoda* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END
