#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)

class CSodaMachine : public CGameObject
{
private:
	CSodaMachine(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSodaMachine(const CSodaMachine& rhs);
	virtual ~CSodaMachine() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg)			override;
	void PriorityTick(_float fTimeDelta)	override;
	void Tick(_float fTimeDelta)			override;
	void LateTick(_float fTimeDelta)		override;
	HRESULT Render()						override;

private:
	_bool m_bBroken = { false };
	CGameObject* pBannerObj = { nullptr };
public:
	void Set_Broken(_bool _b) { m_bBroken = _b; }
	void Set_Banner(CGameObject* pBanner) { pBannerObj = pBanner; }

	void OnCollisionEnter(CGameObject* pOther)	override;
private:
	HRESULT Add_Components();

private:
	CVIBuffer*		m_pVIBuffer_Machine = { nullptr };	
	CTexture*		m_pTextureCom = { nullptr };
	CBoxCollider*	m_pBoxCollider = { nullptr };
public:
	static CSodaMachine* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END
