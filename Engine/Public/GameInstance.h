#pragma once

#include "Renderer.h"
#include "Component_Manager.h"
#include "Key_Manager.h"
#include "CUi_Manager.h"
#include "Layer.h"
#include "Core_Camera.h"
/* 클라이언트와 엔진의 중계자의 역활을 수행한다. */
/* 클라이언트에서 엔진의 기능을 쓰고하자 한다라면 무조건 게임인스턴스를 떠올리면 되도록. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(_uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, _Out_ LPDIRECT3DDEVICE9* ppOut);
	void Tick_Engine(_float fTimeDelta);
	HRESULT Draw();
	void Clear(_uint iLevelIndex);

#pragma region GRAPHIC_DEVICE
public: /* For.Graphic_Device */
	void Render_Begin();
	void Render_End(HWND hWnd = 0);
	LPD3DXFONT Get_Font();
#pragma endregion

#pragma region LEVEL_MANAGER
public: /* For.Level_Manager */
	HRESULT Change_Level(class CLevel* pNewLevel);
	_uint Get_CurrentLevelID();
#pragma endregion

#pragma region TIMER_MANAGER
public:
	_float	Get_TimeDelta(const wstring& strTimerTag);
	HRESULT Add_Timer(const wstring& strTimerTag);
	void Compute_TimeDelta(const wstring& strTimerTag);
#pragma endregion

#pragma region OBJECT_MANAGER
public:
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	class CGameObject* Add_Clone(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	class CComponent* Find_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComponentTag, _uint iIndex = 0);
	class CGameObject* Find_GameObject(_uint iLevelIndex, const wstring& strLayerTag, _uint iIndex = 0);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);
#pragma endregion

#pragma region COMPONENT_MANAGER
public:
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion


#pragma region RENDERER
public:
	HRESULT Add_RenderObjects(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
#pragma endregion

#pragma region KEY_MANAGER
	bool GetKeyDown(eKeyCode _keyCode) { return	 m_pKey_Manager->GetKeyDown(_keyCode); }
	bool GetKeyUp(eKeyCode _keyCode)	{ return m_pKey_Manager->GetKeyUp(_keyCode); }
	bool GetKey(eKeyCode _keyCode)	{ return m_pKey_Manager->GetKey(_keyCode); }
	bool GetKeyNone(eKeyCode _keyCode) { return m_pKey_Manager->GetKeyNone(_keyCode); }
#pragma endregion


#pragma region UI_MANAGER
	HRESULT Add_Ui_LifePrototype(const wstring& Ui_LifePrototypeTag, class CUi* Ui_LifePrototype);
	HRESULT Add_Ui_Active(const wstring& Ui_ActiveTag, eUiRenderType Ui_RenderType, class CUi* Ui_Active);
	HRESULT Add_Ui_LifeClone(const wstring& Ui_LifePrototypeTag, eUiRenderType UiRenderType, void* pArg);
	HRESULT Add_Ui_ShopActive(const wstring& Ui_ShopTag, class CUi* Ui_Shop);
	class CUi* Add_Ui_PartClone(const wstring& Ui_PartPrototypeTag, void* pArg);
	void Set_UiManager_Winsize(_uint iWinSizeX, _uint iWinSizeY);
	void Set_Ui_ActiveState(const wstring& Ui_ActiveTag, bool _isActive = true);
	void Set_Ui_ShopActiveState(const wstring& Ui_ShopTag, bool _isActive = true);
	// for UiClear
	void Set_Enter(bool _Enter);
	void Ui_Render();
	void Ui_Shop_Render();
	bool Get_Ui_ActiveState(const wstring& Ui_ActiveTag);
	void Set_Ui_ActiveTextureIndex(const wstring& Ui_ActiveTag, int _iTextureIndex);
	int  Get_Ui_ActiveTextureIndex(const wstring& Ui_ActiveTag);
#pragma endregion


#pragma region COLLISION_MANAGER
	void Add_RayDesc(const RAY_DESC& RayDesc);
	_bool Ray_Cast(const RAY_DESC& RayDesc, OUT class CGameObject*& pOutHit, OUT _float3& fHitWorldPos, OUT _float& fDist);
#pragma endregion


#pragma region CAMERA_MANAGER
	HRESULT Create_Camera(const wstring& _wstrCameraKey, CCoreCamera* pCamera);
	HRESULT Change_Camera(const wstring& _wstrCameraKey);
	class CCoreCamera* Get_CurCamera();
#pragma endregion


private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CRenderer*				m_pRenderer = { nullptr };
	class CKey_Manager*				m_pKey_Manager = { nullptr };
	class CUi_Manager*				m_pUi_Manager = { nullptr };
	class CCollision_Manager*		m_pCollision_Manager = { nullptr };
	class CCamera_Manager*			m_pCamera_Manager = { nullptr };


public:
	static void Release_Engine();
	virtual void Free() override;
	
};

END