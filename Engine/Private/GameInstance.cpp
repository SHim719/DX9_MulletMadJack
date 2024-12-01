#include "..\Public\GameInstance.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Object_Manager.h"
#include "Collision_Manager.h"
#include "Camera_Manager.h"
#include "Sound_Manager.h"
#include "Frustum.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{

}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, _Out_ LPDIRECT3DDEVICE9* ppOut)
{
	m_pGraphic_Device = CGraphic_Device::Create(GraphicDesc, ppOut);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pKey_Manager = CKey_Manager::Create();
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppOut);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	m_pUi_Manager = CUi_Manager::Create(*ppOut);
	if (nullptr == m_pUi_Manager)
		return E_FAIL;

	m_pCollision_Manager = CCollision_Manager::Create();
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;
	
	m_pCamera_Manager = CCamera_Manager::Create();
	if(nullptr == m_pCamera_Manager)
		return E_FAIL;

	m_pText = CText::Create(*ppOut, m_pGraphic_Device->Get_Font(), 
		m_pGraphic_Device->Get_BigFont());
	if (nullptr == m_pText)
		return E_FAIL;

	m_pSound_Manager = CSound_Manager::Create();
	if (nullptr == m_pSound_Manager)
		return E_FAIL;

	m_pFrustum = CFrustum::Create(*ppOut);
	if (nullptr == m_pFrustum)
		return E_FAIL;


	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pObject_Manager)
		return;
	
	m_pObject_Manager->Destroy_Objects();

	m_pKey_Manager->Update();

	m_pObject_Manager->PriorityTick(fTimeDelta * m_fTimeScale);
	m_pObject_Manager->Tick(fTimeDelta * m_fTimeScale);
	
	m_pUi_Manager->PriorityTick(fTimeDelta);
	m_pUi_Manager->Tick(fTimeDelta);
	m_pUi_Manager->LateTick(fTimeDelta);

	m_pCollision_Manager->Tick();

	m_pCamera_Manager->Render_CurCamera(fTimeDelta);

	m_pFrustum->Tick();

	m_pObject_Manager->LateTick(fTimeDelta * m_fTimeScale);

	m_pRenderer->Sort_AlphaBlendObj();

	m_pLevel_Manager->Tick(fTimeDelta);
}

HRESULT CGameInstance::Draw()
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pRenderer)
		return E_FAIL;

	m_pRenderer->Draw();

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
LPD3DXFONT CGameInstance::Get_Font()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;

	return m_pGraphic_Device->Get_Font();
}
#pragma endregion

#pragma region LEVEL_MANAGER
HRESULT CGameInstance::Change_Level(CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Change_Level(pNewLevel);
}

_uint CGameInstance::Get_CurrentLevelID()
{
	return m_pLevel_Manager->Get_CurrentLevelID();
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
void CGameInstance::Set_TimeDivide(_float fTimeDivide)
{
	if (nullptr == m_pTimer_Manager)
		return;

	m_pTimer_Manager->Set_TimeDivide(fTimeDivide);
}
void CGameInstance::Set_TimeScale(_float fTimeScale)
{
	m_fTimeScale = fTimeScale;
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

CComponent* CGameInstance::Find_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);
}

CGameObject* CGameInstance::Find_GameObject(_uint iLevelIndex, const wstring& strLayerTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_GameObject(iLevelIndex, strLayerTag, iIndex);
}

CLayer* CGameInstance::Find_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_Layer(iLevelIndex, strLayerTag);
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

HRESULT CGameInstance::Add_Ui_Active(const wstring& Ui_ActiveTag, eUiRenderType Ui_RenderType, CUi* Ui_Active)
{
	if (nullptr == m_pUi_Manager)
		return E_FAIL;

	return m_pUi_Manager->Add_Ui_Active(Ui_ActiveTag, Ui_RenderType, Ui_Active);
}

class CUi* CGameInstance::Add_Ui_PartCloneRender(const wstring& Ui_LifePrototypeTag, eUiRenderType UiRenderType, void* pArg)
{
	if (nullptr == m_pUi_Manager)
		return nullptr;

	return m_pUi_Manager->Add_Ui_PartCloneRender(Ui_LifePrototypeTag, UiRenderType, pArg);
}

HRESULT CGameInstance::Add_Ui_LifeClone(const wstring& Ui_LifePrototypeTag, eUiRenderType UiRenderType, void* pArg)
{
	if (nullptr == m_pUi_Manager)
		return E_FAIL;

	return m_pUi_Manager->Add_Ui_LifeClone(Ui_LifePrototypeTag, UiRenderType, pArg);
}

HRESULT CGameInstance::Add_Ui_ShopActive(const wstring& Ui_ShopTag, CUi* Ui_Shop)
{
	if (nullptr == m_pUi_Manager)
		return E_FAIL;

	return m_pUi_Manager->Add_Ui_ShopActive(Ui_ShopTag, Ui_Shop);
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

void CGameInstance::Set_Ui_ActiveState(const wstring& Ui_ActiveTag, bool _isActive)
{
	m_pUi_Manager->Set_Ui_ActiveState(Ui_ActiveTag, _isActive);
}
void CGameInstance::Set_Ui_ShopActiveState(const wstring& Ui_ShopTag, bool _isActive)
{
	m_pUi_Manager->Set_Ui_ShopActiveState(Ui_ShopTag, _isActive);
}
void CGameInstance::Set_Enter(bool _Enter)
{
	m_pUi_Manager->Set_Enter(_Enter);
}
void CGameInstance::Ui_Render()
{
	m_pUi_Manager->Ui_Render_Begin();
	m_pUi_Manager->Ui_Render();
	m_pUi_Manager->Ui_Render_End();
}

void CGameInstance::Ui_Shop_Render()
{
	m_pUi_Manager->Ui_Shop_Render_Begin();
	m_pUi_Manager->Ui_Shop_Render();
	m_pUi_Manager->Ui_Shop_Render_End();
}

bool CGameInstance::Get_Ui_ActiveState(const wstring& Ui_ActiveTag)
{
	return m_pUi_Manager->Get_Ui_ActiveState(Ui_ActiveTag);
}

void CGameInstance::Set_Ui_ActiveTextureIndex(const wstring& Ui_ActiveTag, int _iTextureIndex)
{
	m_pUi_Manager->Set_Ui_ActiveTextureIndex(Ui_ActiveTag, _iTextureIndex);
}

int CGameInstance::Get_Ui_ActiveTextureIndex(const wstring& Ui_ActiveTag)
{
	return m_pUi_Manager->Get_Ui_ActiveTextureIndex(Ui_ActiveTag);
}

CUi* CGameInstance::Get_ActiveBlendUI(const wstring& strKey)
{
	return m_pUi_Manager->Get_ActiveBlendUI(strKey);
}

CUi* CGameInstance::Get_ActiveNonBlendUI(const wstring& strKey)
{
	return m_pUi_Manager->Get_ActiveNonBlendUI(strKey);
}

#pragma endregion

#pragma region COLLISION_MANAGER
void CGameInstance::Add_RayDesc(const RAY_DESC& RayDesc)
{
	m_pCollision_Manager->Add_RayDesc(RayDesc);
}
_bool CGameInstance::Ray_Cast(const RAY_DESC& RayDesc, OUT CGameObject*& pOutHit, OUT _float3& fHitWorldPos, OUT _float& fDist) 
{
	if (nullptr == m_pCollision_Manager)
		return false;

	return m_pCollision_Manager->Ray_Cast(RayDesc, pOutHit, fHitWorldPos, fDist);
}
#pragma endregion

#pragma region CAMERA_MANAGER
HRESULT CGameInstance::Create_Camera(const wstring& _wstrCameraKey, CCoreCamera* pCamera)
{
	if (FAILED(m_pCamera_Manager->Create_Camera(_wstrCameraKey, pCamera)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Change_Camera(const wstring& _wstrCameraKey)
{
	if (FAILED(m_pCamera_Manager->Change_Camera(_wstrCameraKey)))
		return E_FAIL;

	return S_OK;
}

CCoreCamera* CGameInstance::Get_CurCamera()
{
	if (nullptr == m_pCamera_Manager)
		return nullptr;

	return m_pCamera_Manager->Get_CurCamera();
}

#pragma endregion

#pragma region TEXT
void CGameInstance::Print_Text(CText::Text_Info Text)
{
	m_pText->Print_Text(Text);
}

void CGameInstance::Print_Big_Text(CText::Text_Info BigText)
{
	m_pText->Print_Big_Text(BigText);
}
#pragma endregion

#pragma region SOUND_MANAGER
HRESULT CGameInstance::Create_Sound(const string& strPath, const wstring& strSoundTag)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;

	return m_pSound_Manager->Create_Sound(strPath, strSoundTag);
}

HRESULT CGameInstance::Play(const wstring& strSoundTag, _bool bLoop)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;

	return m_pSound_Manager->Play(strSoundTag, bLoop);

	return S_OK;
}

HRESULT CGameInstance::Stop(const wstring& strSoundTag)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;

	return m_pSound_Manager->Stop(strSoundTag);
}


HRESULT CGameInstance::SetVolume(const wstring& strSoundTag, const _float& fVolume)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;

	return m_pSound_Manager->SetVolume(strSoundTag, fVolume);
}

HRESULT CGameInstance::SetPosition(const wstring& strSoundTag, _float fPosition)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;
	
	return m_pSound_Manager->SetPosition(strSoundTag, fPosition);
}




_bool CGameInstance::Is_Playing(const wstring& strSoundTag)
{
	if (nullptr == m_pSound_Manager)
		return false;

	return m_pSound_Manager->Is_Playing(strSoundTag);
}
_bool CGameInstance::In_WorldFrustum(_float3 vWorldPos, _float fRadius)
{
	return m_pFrustum->In_WorldFrustum(vWorldPos, fRadius);
}
#pragma endregion

void CGameInstance::Release_Engine()
{
	CGameInstance::Get_Instance()->Free();

	CGameInstance::Get_Instance()->Destroy_Instance();	
}

void CGameInstance::Free()
{	
	Safe_Release(m_pText);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pUi_Manager);
	Safe_Release(m_pCollision_Manager);
	Safe_Release(m_pCamera_Manager);
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pGraphic_Device);
}
