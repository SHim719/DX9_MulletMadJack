#include "..\Public\GameInstance.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Object_Manager.h"


IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{

}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, _Out_ LPDIRECT3DDEVICE9* ppOut)
{
	/* 엔진을 사용하기위해 필요한 기타 초기화 작업들을 거친다. */

	/* 그래픽 디바이스 초기화. */
	m_pGraphic_Device = CGraphic_Device::Create(GraphicDesc, ppOut);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 인풋 디바이스 초기화. */
	m_pKey_Manager = CKey_Manager::Create();
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 사운드 디바이스 초기화. */

	m_pRenderer = CRenderer::Create(*ppOut);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	/* 레벨 매니져를 준비해놓는다. */
	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;


	/* 오브젝트 매니져 사용할 준비. . */
	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	/* 컴포넌트 매니져 사용할 준비. . */
	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	// Ui 매니져 사용할 준비
	m_pUi_Manager = CUi_Manager::Create(*ppOut);
	if (nullptr == m_pUi_Manager)
		return E_FAIL;
	
	
	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pObject_Manager)
		return;

	m_pKey_Manager->Update();

	m_pObject_Manager->PriorityTick(fTimeDelta);
	m_pObject_Manager->Tick(fTimeDelta);
	m_pObject_Manager->LateTick(fTimeDelta);

	m_pUi_Manager->PriorityTick(fTimeDelta);
	m_pUi_Manager->Tick(fTimeDelta);
	m_pUi_Manager->LateTick(fTimeDelta);

	m_pUi_Manager->Blend_PriorityTick(fTimeDelta);
	m_pUi_Manager->Blend_Tick(fTimeDelta);
	m_pUi_Manager->Blend_LateTick(fTimeDelta);

	m_pLevel_Manager->Tick(fTimeDelta);
}

HRESULT CGameInstance::Draw()
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pRenderer)
		return E_FAIL;

	m_pRenderer->Draw();	

	m_pUi_Manager->Ui_Render_Begin();
	m_pUi_Manager->Ui_Render();
	m_pUi_Manager->Ui_Render_End();

	return m_pLevel_Manager->Render();
}
void CGameInstance::Clear(_uint iLevelIndex)
{
	m_pRenderer->Clear();
	m_pObject_Manager->Clear(iLevelIndex);	
	m_pComponent_Manager->Clear(iLevelIndex);
}

#pragma region GRAPHIC_DEVICE
void CGameInstance::Render_Begin()
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_Begin();
}

void CGameInstance::Render_End(HWND hWnd)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_End(hWnd);
}
#pragma endregion

#pragma region LEVEL_MANAGER
HRESULT CGameInstance::Change_Level(CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Change_Level(pNewLevel);
}

CLevel* CGameInstance::Find_Level()
{
	if (nullptr == m_pLevel_Manager)
		return nullptr;

	return m_pLevel_Manager->Get_Level();
}
#pragma endregion

#pragma region TIMER_MANAGER
_float CGameInstance::Get_TimeDelta(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}
HRESULT CGameInstance::Add_Timer(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimerTag);
}
void CGameInstance::Compute_TimeDelta(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return ;

	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}
#pragma endregion

#pragma region OBJECT_MANAGER
HRESULT CGameInstance::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);
	
}
CGameObject* CGameInstance::Add_Clone(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Add_Clone(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}
#pragma endregion

#pragma region COMPONENT_MANAGER
HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring & strPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}
#pragma endregion


#pragma region RENDERER
HRESULT CGameInstance::Add_RenderObjects(CRenderer::RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderObjects(eRenderGroup, pRenderObject);	
}

#pragma region UIMANAGER
HRESULT CGameInstance::Add_Ui_LifePrototype(const wstring& Ui_LifePrototypeTag, CUi* Ui_LifePrototype)
{
	if (nullptr == m_pUi_Manager)
		return E_FAIL;

	return m_pUi_Manager->Add_Ui_LifePrototype(Ui_LifePrototypeTag, Ui_LifePrototype);
}
HRESULT CGameInstance::Add_Ui_Active(const wstring& Ui_ActiveTag, CUi* Ui_Active)
{
	if (nullptr == m_pUi_Manager)
		return E_FAIL;

	return m_pUi_Manager->Add_Ui_Active(Ui_ActiveTag, Ui_Active);
}
HRESULT CGameInstance::Add_Ui_LifeClone(const wstring& Ui_LifePrototypeTag, eUiRenderType UiRenderType, void* pArg)
{
	if (nullptr == m_pUi_Manager)
		return E_FAIL;

	return m_pUi_Manager->Add_Ui_LifeClone(Ui_LifePrototypeTag, UiRenderType, pArg);
}
CUi* CGameInstance::Add_Ui_PartClone(const wstring& Ui_PartPrototypeTag, void* pArg)
{
	if (nullptr == m_pUi_Manager)
		return nullptr;


	return m_pUi_Manager->Add_Ui_PartClone(Ui_PartPrototypeTag, pArg);
}
void CGameInstance::Set_UiManager_Winsize(_uint iWinSizeX, _uint iWinSizeY)
{
	m_pUi_Manager->Set_WinSize(iWinSizeX, iWinSizeY);
}
#pragma endregion

void CGameInstance::Release_Engine()
{
	CGameInstance::Get_Instance()->Free();

	CGameInstance::Get_Instance()->Destroy_Instance();	
}

void CGameInstance::Free()
{	
	Safe_Release(m_pUi_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pKey_Manager);
}
