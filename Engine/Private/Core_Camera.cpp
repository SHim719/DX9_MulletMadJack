#include "Core_Camera.h"
#include "GameInstance.h"

CCoreCamera::CCoreCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CCoreCamera::CCoreCamera(const CCoreCamera& rhs)
	: CGameObject(rhs)
{

}


void CCoreCamera::Free()
{
	__super::Free();
}

HRESULT CCoreCamera::Initialize()
{
	return S_OK;
}