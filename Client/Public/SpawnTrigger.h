#pragma once


#include "Client_Defines.h"
#include "TriggerObject.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

typedef struct tagSpawnDesc
{
	_float3 vPosition;
	MONSTERTYPE eType;
}SPAWN_DESC;

class CSpawnTrigger final : public CTriggerObject
{
private:
	CSpawnTrigger(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpawnTrigger(const CSpawnTrigger& rhs);
	virtual ~CSpawnTrigger() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	HRESULT Add_Components();

private:
	void OnTriggerEnter(CGameObject* pOther) override;

private:
	static vector<SPAWN_DESC> m_vecSpawnInfos;
	
	_int	m_iMinIdx = { 0 };
	_int	m_iMaxIdx = { 0 };

public:
	static void Add_SpawnInfo(SPAWN_DESC desc) { m_vecSpawnInfos.push_back(desc); }
	static void reserve(_uint iSize) { m_vecSpawnInfos.reserve(iSize); }
	static void Clear() { m_vecSpawnInfos.clear(); }

	void Set_MinIdx(_int iMin) { m_iMinIdx = iMin; }
	void Set_MaxIdx(_int iMax) { m_iMaxIdx = iMax; }
	_int Get_MinIdx() { return m_iMinIdx; }
	_int Get_MaxIdx() { return m_iMaxIdx; }

public:
	static CSpawnTrigger* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg)	override;
 	void Free() override;
};

END

