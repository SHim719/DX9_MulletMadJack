#include "..\Public\Player.h"

#include "GameInstance.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	
 	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_CrossHair"), true);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol_Right_Hand"), true);
	m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Pistol"), true);
	return S_OK;

}

void CPlayer::PriorityTick(_float fTimeDelta)
{
}

void CPlayer::Tick(_float fTimeDelta)
{
}

void CPlayer::LateTick(_float fTimeDelta)
{

}

HRESULT CPlayer::Render()
{

	return S_OK;
}

CPlayer * CPlayer::Create()
{
	CPlayer*	pInstance = new CPlayer(NULL);

	if (FAILED(pInstance->Initialize(NULL)))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
}
