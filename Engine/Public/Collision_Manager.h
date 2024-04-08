#pragma once

#include "Base.h"
#include "Collider.h"


BEGIN(Engine)
class CCollision_Manager : public CBase
{
private:
	union CollisionID
	{
		struct
		{
			UINT32 left;
			UINT32 right;
		};

		UINT64 id;
	};
private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

	HRESULT Initialize();

public:
	void Tick();

private:
	list<RAY_DESC>	m_RayDescs; //플레이어가 레이를 쏘면 GameInstance의 Add_RayDesc를 호출해서 레이정보를 여기에 넣어줌
private:
	void Intersect_Ray();		// 매프레임마다 호출돼서 만약 레이와 피킹됐으면 Collider 컴포넌트에 On_Ray_Intersect호출
	// Collider의 On_Ray_Intersect는 GameObject의 On_Ray_Intersect를 호출
	// On_Ray_Intersect는 가상함수이므로 상속받아서 사용하면 됨.
public:
	void Add_RayDesc(const RAY_DESC& RayDesc) { m_RayDescs.push_back(RayDesc); }

private:
	unordered_map<UINT64, bool> m_CollisionInfo;

private:
	void Collision_Box(_uint iLevel, const wstring& strDstLayer, const wstring& strSrcLayer);

	bool Check_Intersect_AABB(class CBoxCollider* pDstCollider, CBoxCollider* pSrcCollider, OUT _float3& _fDist);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CCollision_Manager* Create();
	void Free() override;

};

END

