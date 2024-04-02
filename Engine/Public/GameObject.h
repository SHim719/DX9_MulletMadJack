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
	class CTransform* m_pTransformCom = { nullptr }; // ��� ���� ������Ʈ���� Transform ������Ʈ�� ������ �ִٴ� ����

public:
	class CTransform* Get_Transform() { return m_pTransformCom; };

#pragma endregion  

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END