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
	}Ui_Pos_Size;


protected:
	CUi(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUi(const CUi& rhs);
	virtual ~CUi() = default;


public:
	virtual HRESULT Initialize_Prototype() PURE;
	virtual HRESULT Initialize(void* pArg) PURE;
	virtual void PriorityTick(_float fTimeDelta) PURE;
	virtual void Tick(_float fTimeDelta) PURE;
	virtual void LateTick(_float fTimeDelta) PURE;
	virtual HRESULT Render() PURE;


protected:
	virtual void Initialize_Set_Scale_Pos_Rotation(void* pArg) PURE;
	virtual void Initialize_Set_Speed() PURE;


	virtual void Default_Set_LifeTime() PURE;
	virtual void Default_Set_Size() PURE;


protected:
	virtual HRESULT Add_Components(void* pArg) PURE;
	virtual HRESULT Add_Texture(void* pArg) PURE;


protected:
	HRESULT Add_Component(_uint iLevelIndex, const wstring& strPrototypeTag, CComponent** ppOut, void* pArg = nullptr);


public:
	bool Is_Dead() { return m_bDead; }
	void Set_Dead() { m_bDead = true; }


protected:
	LPDIRECT3DDEVICE9 m_pGraphic_Device;
	class CGameInstance* m_pGameInstance = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };


protected:
	bool m_bDead = { false };
	Ui_Pos_Size m_UiDesc;
	_uint m_iTexture_Index = {0};


public:
	virtual void Free() override;
	virtual CUi* Clone(void* pArg);
};

END
