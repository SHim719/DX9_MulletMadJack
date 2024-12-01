#pragma once
#include "Base.h"

BEGIN(Engine)

enum class eUiRenderType
{
	Render_NonBlend,
	Render_Blend,
	End
};
class CUi_Manager final : public CBase
{

private:
	CUi_Manager(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CUi_Manager() = default;


public:
	HRESULT Add_Ui_LifePrototype(const wstring& Ui_LifePrototypeTag, class CUi* Ui_LifePrototype);
	HRESULT Add_Ui_LifeClone(const wstring& Ui_LifePrototypeTag, eUiRenderType UiRenderType, void* pArg);
	class CUi* Add_Ui_PartCloneRender(const wstring& Ui_LifePrototypeTag, eUiRenderType UiRenderType, void* pArg);
	class CUi* Add_Ui_PartClone(const wstring& Ui_PartPrototypeTag, void* pArg);
	HRESULT Add_Ui_Active(const wstring& Ui_ActiveTag, eUiRenderType UiRenderType, class CUi* Ui_Active);

	HRESULT Add_Ui_ShopActive(const wstring& Ui_ShopTag, class CUi* Ui_Shop);

public:
	void PriorityTick(_float fTimeDelta);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	void Ui_Render_Begin();
	void Ui_Shop_Render_Begin();
	HRESULT Ui_Shop_Render();
	void Ui_Shop_Render_End();
	HRESULT Ui_Render();
	void Ui_Render_End();
	void Set_WinSize(_uint iWinSizeX, _uint iWinSizeY);

//for UiActive
	void Set_Ui_ActiveState(const wstring& Ui_ActiveTag, bool _isActive);
	bool Get_Ui_ActiveState(const wstring& Ui_ActiveTag);
	void Set_Ui_ShopActiveState(const wstring& Ui_ShopTag, bool _isActive = true);

	void Set_Ui_ActiveTextureIndex(const wstring& Ui_ActiveTag, int _iTextureIndex);
	int  Get_Ui_ActiveTextureIndex(const wstring& Ui_ActiveTag);


//for UiClear
	void Set_Enter(bool _Enter);

	CUi* Get_ActiveBlendUI(const wstring& strKey);
	CUi* Get_ActiveNonBlendUI(const wstring& strKey);
private:
	map<const wstring, class CUi*> m_Ui_LifePrototypes;
	list<class CUi*> m_Ui_LifeClonelist;
	list<class CUi*> m_Ui_LifeBlendClonelist;
	list<class CUi*> m_Ui_Partlist;
	map<const wstring, class CUi*> m_Ui_Active;
	map<const wstring, class CUi*> m_Ui_ActiveBlend;

	map<const wstring, class CUi*> m_Ui_ShopActive;

	LPDIRECT3DDEVICE9 m_pGraphic_Device;

private:
	_float4x4 m_ViewMatrix;
	_float4x4 m_ProjMatrix;
	_uint m_iWinSizeX = { 0 };
	_uint m_iWinSizeY = { 0 };


public:
	static CUi_Manager* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
