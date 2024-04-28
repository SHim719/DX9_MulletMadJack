#pragma once
#include "Base.h"
#include "Component_Manager.h"


BEGIN(Engine)

class ENGINE_DLL CUi abstract : public CBase
{
public:
	typedef struct UiPositionSize
	{
		_float m_fX = { 0 };
		_float m_fY = { 0 };
		_float m_fSizeX = { 0 };
		_float m_fSizeY = { 0 };
		_float3 m_Rotation = { 0, 0, 0 };
	}Ui_Pos_Size_Rotation;


protected:
	CUi(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi(const CUi& rhs);
	virtual ~CUi() = default;


	
public:
	virtual HRESULT Initialize_Prototype() PURE;
	virtual HRESULT Initialize(void* pArg) PURE;
	virtual HRESULT Initialize_Active() { return S_OK; };
	virtual void PriorityTick(_float fTimeDelta) PURE;
	virtual void Tick(_float fTimeDelta) PURE;
	virtual void LateTick(_float fTimeDelta) PURE;
	virtual HRESULT Render() PURE;


protected:
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) PURE;
	virtual void Initialize_Set_Speed() PURE;


protected:
	virtual HRESULT Add_Components(void* pArg) PURE;
	virtual HRESULT Add_Texture(void* pArg) PURE;


//for UiLife
protected:
	virtual void Default_Set_LifeTime();
	virtual void Default_Set_Size();


//for UiActive
protected:
	virtual void Initialize_Set_ActiveTime();
	virtual void Initialize_Set_Size();


public:
//for UiClear
	virtual void Enter(bool _Enter);

protected:
	HRESULT Add_Component(_uint iLevelIndex, const wstring& strPrototypeTag, CComponent** ppOut, void* pArg = nullptr);

public:
	bool Is_Dead() { return m_bDead; }
	void Set_Dead() { m_bDead = true; }

 	bool Get_Active() { return m_bActive; }
	void Set_Active(bool _isActive) 
	{
		if (_isActive == true && m_bActive == true)
		{	
			return;
		}
		
		if (_isActive == true)
		{
			Initialize_Active();
		}

		m_bActive = _isActive; 
	}

	void Set_Texture_Index(_uint iIndex) { m_iTexture_Index = iIndex; }
	_uint Get_Texture_Index() { return m_iTexture_Index; }
	void Add_Texture_Index(_uint iNum) { m_iTexture_Index += iNum; }

protected:
	LPDIRECT3DDEVICE9 m_pGraphic_Device;
	class CGameInstance* m_pGameInstance = { nullptr };

	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

protected:
	void Set_Lifetime(_float Lifetime) { m_fLifeTime = Lifetime; }
	void Cal_Life_Blink(_float fTimeDelta);
	bool Cal_BlinkRender(_float BlinkGap);


protected:
	_float m_fLifeTime = { 0.f };
	_float m_fBlink = { 0.f };
	bool m_bBlink = { false };


protected:
	bool m_bDead = { false };
	Ui_Pos_Size_Rotation m_UiDesc;
	bool m_bActive = { false };
	_float m_fActiveTime = { 0.f };
	_uint m_iTexture_Index = {0};
	

protected:
	bool m_bEnter = { false };
	

public:
	virtual void Free() override;
	virtual CUi* Clone(void* pArg);
};

END
