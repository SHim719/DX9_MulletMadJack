#pragma once

#include "Base.h"
#include "Core_Camera.h"

class CCoreCamera;
class CGameObject;
BEGIN(Engine)

class CCamera_Manager final : public CBase
{
private:
	CCamera_Manager(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCamera_Manager() = default;

public:
	HRESULT Initialize();

public:
	static CCamera_Manager* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

public:
	HRESULT Create_Third_Camera(const wstring& _wstrCameraKey, CCoreCamera* pCamera);
	//CCore_Camera* Create_Free_Camera(const wstring& _wstrCameraKey, const _float& _fFovY, const _float& _fAspect, const _float& _fNear, const _float& _fFar);
	HRESULT Change_Camera(const wstring& _wstrCameraKey);
	void Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }

	HRESULT Update_CurCamera(const _float& fTimeDelta);




private:
	map<wstring, CCoreCamera*> m_mapCamera;
	wstring m_wstrCurCameraKey;
	LPDIRECT3DDEVICE9 pGraphicDevice;
	CCoreCamera* m_pCurCamera = nullptr;
	CGameObject* m_pTarget = nullptr;

};

END