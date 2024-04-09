#pragma once

#include "Base.h"
#include "Core_Camera.h"

BEGIN(Engine)

class CCamera_Manager final : public CBase
{
private:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	HRESULT Initialize();

public:
	virtual void Free() override;

	static CCamera_Manager* Create();

public:
	HRESULT Create_Camera(const wstring& _wstrCameraKey, CCoreCamera* pCamera);
	//CCore_Camera* Create_Free_Camera(const wstring& _wstrCameraKey, const _float& _fFovY, const _float& _fAspect, const _float& _fNear, const _float& _fFar);
	HRESULT Change_Camera(const wstring& _wstrCameraKey);
	void Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }

	HRESULT Render_CurCamera(const _float& fTimeDelta);

	CCoreCamera* Get_CurCamera() { return m_pCurCamera; }


private:
	map<wstring, CCoreCamera*> m_mapCamera;
	wstring m_wstrCurCameraKey;
	LPDIRECT3DDEVICE9 pGraphicDevice;
	CCoreCamera* m_pCurCamera = nullptr;
	CGameObject* m_pTarget = nullptr;

};

END