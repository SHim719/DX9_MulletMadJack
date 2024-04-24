#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "CSans_Bone.h"


BEGIN(Client)

using BoneRoopArg = struct BoneRoopInfo
{
	SansBonePos Pos = { SansBonePos::End };
	SansBoneDir Dir = { SansBoneDir::End };
	_float RoopGap = { 0 };
	_uint Floor = { 0 };
	_float Speed = { 0 };
	_float LifeTime = { 0 };
};
class CBoneRoop : public CGameObject
{
private:
	CBoneRoop(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBoneRoop(const CGameObject& rhs);
	virtual ~CBoneRoop() = default;
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
	_float m_fSpeed = { 0 };
	SansBonePos m_ePos = {};
	_bool m_bRoop = { false };
	_bool m_bDead = { false };

public:
	static CBoneRoop* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override; // pArg -> BoneRoopArg
	virtual void Free() override;
};

END