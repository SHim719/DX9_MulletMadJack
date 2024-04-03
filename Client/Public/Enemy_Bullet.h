#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Animation.h"

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
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform*		m_pTransformCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CAnimation*		m_pAnimationCom = { nullptr };

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