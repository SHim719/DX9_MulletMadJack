#pragma once
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)

class CElevator_L final : public CGameObject
{
private:
	CElevator_L(LPDIRECT3DDEVICE9 pGraphic_Device);
	CElevator_L(const CElevator_L& rhs);
	virtual ~CElevator_L() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg)			override;
	void PriorityTick(_float fTimeDelta)	override;
	void Tick(_float fTimeDelta)			override;
	void LateTick(_float fTimeDelta)		override;
	HRESULT Render()						override;

private:
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CBoxCollider* m_pBoxCollider = { nullptr };
private:
	HRESULT Add_Components();

private:
	_bool m_bOpening = false;
	_float m_fDestroyTime = 3.f;

private:
	void OnTriggerEnter(CGameObject* pOther) override;

public:
	static CElevator_L* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END

