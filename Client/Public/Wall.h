#pragma once

#include "Client_Defines.h"
#include "GameObject.h"



BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)

class CWall final : public CGameObject
{
private:
	CWall(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWall(const CWall& rhs);
	virtual ~CWall() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg)			override;
	void PriorityTick(_float fTimeDelta)	override;
	void Tick(_float fTimeDelta)			override;
	void LateTick(_float fTimeDelta)		override;
	HRESULT Render()						override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };

private:
	HRESULT Add_Components();

public:
	void On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg = nullptr) override;

public:
	static CWall* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END


