#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)

class CLandMine : public CGameObject
{
private:
	CLandMine(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLandMine(const CLandMine& rhs);
	virtual ~CLandMine() = default;

private:
	enum class LandMineState
	{
		Idle,
		Explode
	};

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
	CTexture* m_pTextureCom = { nullptr };
	CRigidbody* m_pRigidbody = { nullptr };
	CBoxCollider* m_pBoxCollider = { nullptr };

private:
	void OnTriggerStay(CGameObject* pOther) override;

private:
	_float m_fLifeTime = 5.f;
	_float m_fExplodeTime = 0.5f;

	_bool m_bExplode = false;
	LandMineState m_eCurState = LandMineState::Explode;

public:
	static CLandMine* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END
