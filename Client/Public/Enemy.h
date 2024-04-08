#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Animation.h"
#include "FPS_Camera.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEnemy final : public CGameObject
{
public:
	typedef struct tagEnemy : public CTransform::TRANSFORM_DESC
	{
		_float3 vPosition;
		_float3 vAt;
	}ENEMY_DESC;

private:
	CEnemy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnemy(const CEnemy& rhs);
	virtual ~CEnemy() = default;

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
	CFPS_Camera* m_pFPS_Camera = { nullptr };
	CAnimation* m_pAnimationCom = { nullptr };
	CLevel* m_pLevel = { nullptr };

private:
	ENEMY_DESC		m_EnemyDesc{};
	_float			m_fTimeAcc;
	bool			m_bChange_Direction;

private:
	HRESULT Add_Components();
	HRESULT	Add_Textures();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();
	void	Key_Input();

public:
	static CEnemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END