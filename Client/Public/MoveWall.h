#pragma once

#include "Client_Defines.h"
#include "GameObject.h"



BEGIN(Engine)
class CTransform;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)

class CMoveWall final : public CGameObject
{
private:
	CMoveWall(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMoveWall(const CMoveWall& rhs);
	virtual ~CMoveWall() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg)			override;
	void PriorityTick(_float fTimeDelta)	override;
	void Tick(_float fTimeDelta)			override;
	void LateTick(_float fTimeDelta)		override;
	HRESULT Render()						override;

private:
	CVIBuffer_MoveWall* m_pVIBuffer = { nullptr };
	CTexture* m_pTexture = { nullptr };
	CBoxCollider* m_pBoxCollider = { nullptr };

private:
	HRESULT Add_Components();

public:
	static CMoveWall* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END


