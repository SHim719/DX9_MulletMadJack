#include "PlayerManager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPlayer_Manager)

CPlayer_Manager::CPlayer_Manager()
{

}

void CPlayer_Manager::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(pGraphic_Device);

	m_pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(m_pGameInstance);
}

void CPlayer_Manager::Tick(_float fTimeDelta)
{

	switch (m_eActionType)
	{
		case ACTION_TYPE::ACTION_NONE: {
			if (m_bActionIDLE == false) {
				m_bTempDisable = false;
				Set_DisableEnd(false);
				m_bActionIDLE = true;
			}
				
			break;
		}

		case ACTION_TYPE::ACTION_EXECUTION: {
			m_bTempDisable = true;
			if (m_bTempDisableEnd == true && m_bActionIDLE == true) {
				Set_DisableEnd(false);
				//Camera_Shake_Order(600000.f, 0.4f);
				/*m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Kick"), true);*/
				m_pGameInstance->Set_Ui_ActiveState(TEXT("Execution_Neck"), true);
				m_pGameInstance->Set_Ui_ActiveState(TEXT("Execution_Head"), true);
				m_pGameInstance->Set_Ui_ActiveState(TEXT("Execution_Body"), true);
				m_pGameInstance->Set_Ui_ActiveState(TEXT("Execution_Hand"), true);
				m_bActionIDLE = false;
			}

			break;
		}

		case ACTION_TYPE::ACTION_DRINKCAN: {
			m_bTempDisable = true;
			if (m_bTempDisableEnd == true && m_bActionIDLE == true) {
				Set_DisableEnd(false);
				m_pGameInstance->Set_Ui_ActiveState(TEXT("Ui_Drink"), true);
				m_bActionIDLE = false;
			}

			break;
		}
	}

#pragma region TempDisable Position Decide
	if (m_bTempDisable == true) {
		_float TempDisablePosition = Get_TempDisablePosition();

		if (TempDisablePosition <= m_fTempDisablePositionLimit && Get_DisableEnd() == false)
		{
			TempDisablePosition += fTimeDelta * 4000.f;
			Set_TempDisablePosition(TempDisablePosition);
			Set_DisableEnd(false);
		}

		if (TempDisablePosition >= Get_TempDisablePositionLimit() )
		{
			Set_TempDisablePosition(Get_TempDisablePositionLimit());
			Set_DisableEnd(true);
		}
	}

	if (m_bTempDisable == false) {
		_float TempDisablePosition = Get_TempDisablePosition();

		if (TempDisablePosition >= 0.f && Get_DisableEnd() == false)
		{
			TempDisablePosition -= fTimeDelta * 4000.f;
			Set_TempDisablePosition(TempDisablePosition);
			Set_DisableEnd(false);
		}

		if (TempDisablePosition <= 0.f)
		{
			Set_TempDisablePosition(0.f);
			Set_DisableEnd(true);
		}
	}
#pragma endregion

}

_float CPlayer_Manager::Get_PlayerToTargetX(_float _TargetX)
{
	return _TargetX - m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION).x;
}

_float CPlayer_Manager::Get_PlayerToTargetY(_float _TargetY)
{
	return _TargetY - m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION).y;
}

_float CPlayer_Manager::Get_PlayerToTargetZ(_float _TargetZ)
{
	return _TargetZ - m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION).z;
}

_float CPlayer_Manager::Get_PlayerToTarget(_float _TargetX, _float _TargetY, _float _TargetZ)
{
	_float3 Pos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION);

	return (_float)sqrt(pow(fabs(Pos.x - _TargetX), 2) + pow(fabs(Pos.y - _TargetY), 2) + pow(fabs(Pos.z - _TargetZ), 2));
}

_float CPlayer_Manager::Get_PlayerToTarget(_float3 _Target)
{
	_float3 Pos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION);

	return (_float)sqrt(pow(fabs(Pos.x - _Target.x), 2) + pow(fabs(Pos.y - _Target.y), 2) + pow(fabs(Pos.z - _Target.z), 2));
}

_float2 CPlayer_Manager::Lissajous_Adjust(_float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta)
{
	//_fLissajousTime += _fTimeDelta * _fLissajousSpeed;

	_fPosX = _fWitth * sin(_fLagrangianX * _fLissajousTime + 3.14f / _fPhaseDelta);
	_fPosY = _fHeight * cos(_fLagrangianY * _fLissajousTime);

	return { _fPosX , _fPosY };
}

_float2 CPlayer_Manager::Get_TempDisablePosition_BothHand()
{
	switch (Get_Action_Type()) {
		case ACTION_EXECUTION:
			return { m_fTempDisablePosition,m_fTempDisablePosition };
		case ACTION_DRINKCAN:
			return { m_fTempDisablePosition, 0 };
		default:
			return { m_fTempDisablePosition,m_fTempDisablePosition };
	}

	return { m_fTempDisablePosition,m_fTempDisablePosition };
}

void CPlayer_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
	Destroy_Instance();
}

