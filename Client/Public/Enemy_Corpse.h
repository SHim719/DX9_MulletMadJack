#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)

class CEnemy_Corpse final : public CGameObject
{
public:
	typedef struct
	{
		MONSTERTYPE eType;
		_bool		isTop;
	}ENEMYCORPSE_DESC;

private:
	CEnemy_Corpse(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnemy_Corpse(const CEnemy_Corpse& rhs);
	virtual ~CEnemy_Corpse() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg)			override;
	void PriorityTick(_float fTimeDelta)	override;
	void Tick(_float fTimeDelta)			override;
	void LateTick(_float fTimeDelta)		override;
	HRESULT Render()						override;

private:
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CAnimation* m_pAnimation = { nullptr };
	CBoxCollider* m_pBoxCollider = { nullptr };
	CRigidbody* m_pRigidbody = { nullptr };
private:
	HRESULT Add_Components();
	HRESULT Init_Animations();

private:
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

private:
	_bool	m_bFlip = false;

public:
	static CEnemy_Corpse* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END


