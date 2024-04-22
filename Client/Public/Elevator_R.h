#pragma once
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CBoxCollider;
END

BEGIN(Client)

class CElevator_R final : public CGameObject
{
private:
	CElevator_R(LPDIRECT3DDEVICE9 pGraphic_Device);
	CElevator_R(const CElevator_R& rhs);
	virtual ~CElevator_R() = default;

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
	static CElevator_R* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END

