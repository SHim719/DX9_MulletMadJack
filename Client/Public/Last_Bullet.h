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

class CLast_Bullet final : public CGameObject
{
private:
	CLast_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLast_Bullet(const CLast_Bullet& rhs);
	virtual ~CLast_Bullet() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg) override;
	void PriorityTick(_float fTimeDelta) override;
	void Tick(_float fTimeDelta) override;
	void LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

private:
	CVIBuffer_Bullet* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CBoxCollider* m_pBoxCollider = { nullptr };

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
	static CLast_Bullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END