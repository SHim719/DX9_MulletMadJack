#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Animation.h"
#include "Pawn.h"
#include "FPS_Camera.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEnemy_Bullet final : public CGameObject
{
public:
	typedef struct tagEnemyBullet : public CTransform::TRANSFORM_DESC
	{
		_float3 vPosition;
		_float3 vAt;
	}ENEMY_BULLET_DESC;

private:
	CEnemy_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnemy_Bullet(const CEnemy_Bullet& rhs);
	virtual ~CEnemy_Bullet() = default;

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
	CTexture* m_pTextureCom = { nullptr };
	CAnimation* m_pAnimationCom = { nullptr };
	CFPS_Camera* m_pFPS_Camera = { nullptr };

private:
	ENEMY_BULLET_DESC	m_Enemy_BulletDesc{};
	_float				m_fTimeAcc;

private:
	HRESULT Add_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	static CEnemy_Bullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END