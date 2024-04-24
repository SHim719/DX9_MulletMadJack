#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "CSans_Gaster.h"


BEGIN(Client)

using GasterRoopArg = struct GasterRoopInfo
{
	SansGasterFirePos FirePos = { SansGasterFirePos::End };
	SansGasterPos Pos = { SansGasterPos::End };
	_float RoopGap = { 0 };
	_uint Floor = { 0 };
	_float LifeTime = { 0 };
};
class CGasterRoop : public CGameObject
{
private:
	CGasterRoop(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGasterRoop(const CGameObject& rhs);
	virtual ~CGasterRoop() = default;
	HRESULT Initialize(void* pArg);


public:
	void Tick(_float fTimeDelta);


private:
	void Roop(_float fTimeDelta);


private:
	_float m_fRoopTime = { 0 };
	_float m_fRoopGap = { 0 };
	_float m_fLifeTime = { 0 };
	_uint m_iFloor = { 0 };
	SansGasterFirePos m_eFirePos = { SansGasterFirePos::End };
	SansGasterPos m_ePos = { SansGasterPos::End };
	_bool m_bRoop = { false };
	_bool m_bDead = { false };


public:
	static CGasterRoop* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override; // pArg -> GasterRoopArg
	virtual void Free() override;
};

END