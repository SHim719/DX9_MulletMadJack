#pragma once

#include "Base.h"

/*Ŭ���̾�Ʈ�����ڰ� ���������� �����س� ��üŸ��(Player, Monster)�� �θ� �Ǵ� Ŭ�����̴�. */


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
	class CGameInstance* m_pGameInstance = { nullptr };


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
	class CTransform* m_pTransformCom = { nullptr }; // ��� ���� ������Ʈ���� Transform ������Ʈ�� ������ �ִٴ� ����

public:
	class CTransform* Get_Transform() { return m_pTransformCom; }

#pragma endregion  

public:
	virtual _bool On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg = nullptr) { return false; }
	virtual void Hit(void* pArg);

public:
	virtual void OnCollisionEnter(CGameObject* pOther) {}
	virtual void OnCollisionStay(CGameObject* pOther) 
	{
		int x = 10;
	}
	virtual void OnCollisionExit(CGameObject* pOther) {}

	virtual void OnTriggerEnter(CGameObject* pOther) {}
	virtual void OnTriggerStay(CGameObject* pOther) {}
	virtual void OnTriggerExit(CGameObject* pOther) {}

protected:
	_bool m_bDestroyed = { false };
	_bool m_bActive = { true };
	_bool m_bCanIntersect = { true };
	_bool m_bThisFrameHit = { false };

	string m_strTag = "";

public:
	void Set_Destroy(_bool b) { m_bDestroyed = b; }
	_bool Is_Destroyed() { return m_bDestroyed; }

	void Set_Active(_bool b) { m_bActive = b; }
	_bool Is_Active() { return m_bActive; }

	void Set_Intersect(_bool b) { m_bCanIntersect = b; }
	_bool Can_Intersect() { return m_bCanIntersect; }

	const string& Get_Tag() const { return m_strTag; }
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