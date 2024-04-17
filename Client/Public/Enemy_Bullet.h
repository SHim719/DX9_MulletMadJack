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
private:
	CEnemy_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnemy_Bullet(const CEnemy_Bullet& rhs);
	virtual ~CEnemy_Bullet() = default;

public:
	 HRESULT Initialize_Prototype();
	 HRESULT Initialize(void* pArg) override;
	 void PriorityTick(_float fTimeDelta) override;
	 void Tick(_float fTimeDelta) override;
	 void LateTick(_float fTimeDelta) override;
	 HRESULT Render() override;

private:
	CVIBuffer_Bullet* m_pVIBufferCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CBoxCollider*	m_pBoxCollider = { nullptr };

private:
	void OnTriggerEnter(CGameObject* pOther) override;

private:
	_float				m_fBulletDuration = 3.f;
	//D3DMATERIAL9		m_MaterialDesc{};

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