#pragma once

#include "Base.h"

/*클라이언트개발자가 실질적으로 생성해낸 객체타입(Player, Monster)의 부모가 되는 클래스이다. */


BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void PriorityTick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };


protected:
	virtual HRESULT Begin_RenderState();
	virtual HRESULT End_RenderState();

#pragma region Component_Related 
protected:
	map<const wstring, class CComponent*>	m_Components;

public:
	class CComponent* Add_Component(_uint iPrototoypeLevelIndex, const wstring& strPrototypeTag, const wstring& strComponentTag, void* pArg = nullptr);
	class CComponent* Find_Component(const wstring& strComponentTag);

protected:
	_uint m_iTexture_Index = 0;

public:
	void Set_Texture_Index(_uint iIndex) { m_iTexture_Index = iIndex; }
	_uint Get_Texture_Index() { return m_iTexture_Index; }

protected:
	class CTransform* m_pTransformCom = { nullptr }; // 모든 게임 오브젝트들은 Transform 컴포넌트를 가지고 있다는 가정

public:
	class CTransform* Get_Transform() { return m_pTransformCom; };

#pragma endregion  

public:
	virtual void On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg = nullptr) {} // Ray Picking했을 때 호출되는 함수

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END

/* Mouse Picking!!!!*/
//void CToolCamera::Mouse_Ray()
//{
//	_float3 fMouseNDC_Near = _float3(m_tCurrentMousePos.x * 2.0f / g_iWinSizeX - 1, -m_tCurrentMousePos.y * 2.0f / g_iWinSizeY + 1, 0.f);
//	_float3 fMouseNDC_Far = _float3(m_tCurrentMousePos.x * 2.0f / g_iWinSizeX - 1, -m_tCurrentMousePos.y * 2.0f / g_iWinSizeY + 1, 1.f);
//
//	_float4x4 inverseProjView;
//	D3DXMatrixInverse(&inverseProjView, nullptr, &(m_ViewMatrix * m_ProjMatrix));
//
//	_float3 fMouseWorld_Near = *D3DXVec3TransformCoord(&fMouseWorld_Near, &fMouseNDC_Near, &inverseProjView);
//	_float3 fMouseWorld_Far = *D3DXVec3TransformCoord(&fMouseWorld_Far, &fMouseNDC_Far, &inverseProjView);
//
//	_float3 vRayDir = *D3DXVec3Normalize(&vRayDir, &(fMouseWorld_Far - fMouseWorld_Near));
//
//	RAY_DESC rayDesc{};
//	rayDesc.iLevel = LEVEL_TOOL;
//	rayDesc.strDstLayer = L"Wall";
//	rayDesc.vRayDir = vRayDir;
//	rayDesc.vRayWorldPos = fMouseWorld_Near;
//
//	m_pGameInstance->Add_RayDesc(rayDesc);
//}