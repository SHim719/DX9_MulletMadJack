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
	return _TargetZ - m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION).y;
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

void CPlayer_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
	Destroy_Instance();
}

