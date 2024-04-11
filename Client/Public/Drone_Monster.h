#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Animation.h"
#include "FPS_Camera.h"
#include "Pawn.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CDrone_Monster final : public CPawn
{
	enum STATE { STATE_IDLE, STATE_REVEAL, STATE_AIM, STATE_ATTACK, STATE_FLYBACK, STATE_END };

private:
	CDrone_Monster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDrone_Monster(const CDrone_Monster& rhs);
	virtual ~CDrone_Monster() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CAnimation* m_pAnimationCom = { nullptr };

	CFPS_Camera* m_pFPS_Camera = { nullptr };

private:
	PAWN_DESC		m_Drone_Monster_Desc{};

	_float			m_fAttack_TimeGap;

	STATE			m_eState;

	bool			IsPlaying;

	bool			m_bAttack;
	bool			m_bReveal;
	bool			m_bDead;
	bool			m_bWalking;

private:
	HRESULT			Add_Components();
	HRESULT			Add_Textures();
	HRESULT			Begin_RenderState();
	HRESULT			End_RenderState();

	virtual void Set_Motions(_float fTimeDelta) override;

	void	Decide_Pawn_Motions(_float fTimeDelta);
	void	Pawn_Reveal_Motion(_float fTimeDelta);
	void	Pawn_Attack_Motion(_float fTimeDelta);

public:
	static CDrone_Monster* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END