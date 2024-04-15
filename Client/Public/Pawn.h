#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CPawn abstract: public CGameObject
{
public:
	typedef struct tagPawn : public CTransform::TRANSFORM_DESC
	{
		_float3 vPosition;
		_float3 vAt;
	}PAWN_DESC;

protected:
	CPawn(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPawn(const CPawn& rhs);
	virtual ~CPawn() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual void	Set_Motions(_float fTimeDelta);

protected:
	_float	m_fCorpseDuration = 5.f;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END