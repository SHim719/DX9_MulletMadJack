#include "..\Public\Player_Manager.h"
#include "GameObject.h"

CPlayer_Manager::CPlayer_Manager()
{
}

HRESULT CPlayer_Manager::Initialize()
{
	return S_OK;
}

CPlayer_Manager* CPlayer_Manager::Create()
{
	CPlayer_Manager* pInstance = new CPlayer_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayer_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Manager::Free()
{
	for (auto& Pair : m_mapCamera)
	{
		Safe_Release(Pair.second);
	}
	m_mapCamera.clear();
}

HRESULT CPlayer_Manager::Create_Camera(const wstring& _wstrCameraKey, CCoreCamera* pCamera)
{
	if (pCamera == nullptr)
		return E_FAIL;

	auto iter = m_mapCamera.find(_wstrCameraKey);

	if (iter != m_mapCamera.end())
	{
		MSG_BOX(TEXT("Already Exist Camera"));
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

HRESULT CPlayer_Manager::Change_Camera(const wstring& _wstrCameraKey)
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

HRESULT CPlayer_Manager::Render_CurCamera(const _float& fTimeDelta)
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