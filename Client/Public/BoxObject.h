#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)

class CBoxObject final : public CGameObject
{
private:
	CBoxObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBoxObject(const CBoxObject& rhs);
	virtual ~CBoxObject() = default;

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

public:
	static CBoxObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END


