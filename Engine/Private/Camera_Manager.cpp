#include "..\Public\Camera_Manager.h"
#include "GameObject.h"
#include "Core_Camera.h"

CCamera_Manager::CCamera_Manager()
{
}

HRESULT CCamera_Manager::Initialize()
{
	return S_OK;
}

CCamera_Manager* CCamera_Manager::Create()
{
	CCamera_Manager* pInstance = new CCamera_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Manager::Free()
{
	for (auto& Pair : m_mapCamera)
	{
		Safe_Release(Pair.second);
	}
	m_mapCamera.clear();
}

HRESULT CCamera_Manager::Create_Camera(const wstring& _wstrCameraKey, CCoreCamera* pCamera)
{
	if (pCamera == nullptr)
		return E_FAIL;

	auto iter = m_mapCamera.find(_wstrCameraKey);

	if (iter != m_mapCamera.end())
	{
		Safe_Release(pCamera);
		//MSG_BOX(TEXT("Already Exist Camera"));
		return S_OK;
	}

	if (nullptr == pCamera)
	{
		MSG_BOX(TEXT("Failed to Create Camera"));
		return E_FAIL;
	}

	m_pCurCamera = pCamera;
	m_mapCamera.emplace(_wstrCameraKey, pCamera);

	return S_OK;
}

HRESULT CCamera_Manager::Change_Camera(const wstring& _wstrCameraKey)
{
	auto iter = m_mapCamera.find(_wstrCameraKey);

	if (iter == m_mapCamera.end())
	{
		MSG_BOX(TEXT("Not Found Camera"));
		return E_FAIL;
	}

	m_pCurCamera = iter->second;

	return S_OK;
}

HRESULT CCamera_Manager::Render_CurCamera(const _float& fTimeDelta)
{
	if (nullptr == m_pCurCamera) {
		return E_FAIL;
	}

	//if (nullptr == m_pCurCamera || nullptr != m_pTarget)
	//m_pCurCamera->Set_Target(m_pTarget);

	m_pCurCamera->PriorityTick(fTimeDelta);
	m_pCurCamera->Tick(fTimeDelta);
	m_pCurCamera->LateTick(fTimeDelta);
	m_pCurCamera->Render();

	return S_OK;
}