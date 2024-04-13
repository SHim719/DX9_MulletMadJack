#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)

class CDoor : public CGameObject
{
public:
	enum DIRECTION
	{
		LEFT = 1,
		RIGHT = -1,
	};
private:
	CDoor(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDoor(const CDoor& rhs);
	virtual ~CDoor() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg)			override;
	void PriorityTick(_float fTimeDelta)	override;
	void Tick(_float fTimeDelta)			override;
	void LateTick(_float fTimeDelta)		override;
	HRESULT Render()						override;

private:
	HRESULT Add_Components();

	void OnCollisionEnter(CGameObject* pOther)	override;

private:
	CVIBuffer*		m_pVIBuffer = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CBoxCollider*	m_pBoxCollider = { nullptr };

private:
	DIRECTION		m_eDir = LEFT;

	_bool			m_bOpening = { false };
	_bool			m_bOpened	= { false };

	_float			m_fStartTheta = 0.f;
	_float			m_fTheta = 0.f;

	_float4x4		m_OriginMatrix;
	_float3			m_vPivot;
public:
	void Set_Direction(DIRECTION eDir) { m_eDir = eDir; }
	void Open_Door() { m_bOpening = true; }

	_bool isDirection_Right();
public:
	static CDoor* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END
