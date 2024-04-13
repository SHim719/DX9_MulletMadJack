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
public:
	enum State
	{
		IDLE,
		POURING,
		BROKEN,
		STATE_END,
	};

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
	HRESULT Add_Components();
private:
	CVIBuffer* m_pVIBuffer_Machine = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CBoxCollider* m_pBoxCollider = { nullptr };

public:
	void OnCollisionEnter(CGameObject* pOther)	override;

	//void On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist) override;
private:
	State m_eState = { IDLE };


private:
	void Pouring_Soda(_float fTimeDelta);

private:
	_float3 m_vPourPos = { 0.f, 0.f, 1.f };
	_uint m_iPourCount = 6;
	float m_fPourTime = 0.f;

	CGameObject* m_pBanner = { nullptr };

public:
	void Set_Banner(CGameObject* pBanner) { m_pBanner = pBanner; }

public:
	void Set_PourPos(const _float3& vPourPos) { m_vPourPos = vPourPos; }
	const _float3& Get_PourPos() const { return m_vPourPos; }

public:
	static CSodaMachine* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END
