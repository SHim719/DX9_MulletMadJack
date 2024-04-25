#include "MathManager.h"
#include "GameInstance.h"


IMPLEMENT_SINGLETON(CMath_Manager)

CMath_Manager::CMath_Manager()
{

}

void CMath_Manager::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(pGraphic_Device);

	m_pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(m_pGameInstance);
}

void CMath_Manager::Tick(_float fTimeDelta)
{

}

void CMath_Manager::Clear()
{

}

void CMath_Manager::Start()
{

}

void CMath_Manager::Render()
{


}

_float2 CMath_Manager::Lissajous_Curve(IN _float _fTimeDelta, IN OUT _float& _fLissajousTime, IN _float _fWitth, IN _float _fHeight, IN _float _fLagrangianX, IN _float _fLagrangianY, IN _float _fPhaseDelta, IN _float _fLissajousSpeed)
{
	_fLissajousTime += _fTimeDelta * _fLissajousSpeed;

	_float _fPosX = _fWitth * sin(_fLagrangianX * _fLissajousTime + 3.14f / _fPhaseDelta);
	_float _fPosY = _fHeight * cos(_fLagrangianY * _fLissajousTime);

	return { _fPosX,_fPosY };
}

_float CMath_Manager::Random_Float(_float _fMin, _float _fMax)
{
	std::random_device RD;
	std::mt19937 gen(RD());

	std::uniform_real_distribution<_float> dis(_fMin, _fMax);
	
	return dis(gen);
}

_float2 CMath_Manager::Random_Float2(_float2 _f2Min, _float2 _f2Max)
{
	std::random_device RD;
	std::mt19937 gen(RD());

	std::uniform_real_distribution<_float> disX(_f2Min.x, _f2Max.x);
	std::uniform_real_distribution<_float> disY(_f2Min.y, _f2Max.y);

	return { disX(gen),disY(gen) };
}

_float3 CMath_Manager::Random_Float3(_float3 _f3Min, _float3 _f3Max)
{
	std::random_device RD;
	std::mt19937 gen(RD());

	std::uniform_real_distribution<_float> disX(_f3Min.x, _f3Max.x);
	std::uniform_real_distribution<_float> disY(_f3Min.y, _f3Max.y);
	std::uniform_real_distribution<_float> disZ(_f3Min.z, _f3Max.z);

	return { disX(gen),disY(gen),disZ(gen) };
}

_int CMath_Manager::Random_Int(_int _iMin, _int _iMax)
{
	std::random_device RD;
	std::mt19937 gen(RD());

	std::uniform_int_distribution<_int> dis(_iMin, _iMax);

	return dis(gen);
}

_int CMath_Manager::Random_Binary()
{
	std::random_device RD;
	std::mt19937 gen(RD());

	std::uniform_int_distribution<_int> dis(0, 1);

	return dis(gen);
}

_bool CMath_Manager::Random_Bool()
{
	std::random_device RD;
	std::mt19937 gen(RD());

	std::uniform_int_distribution<_int> dis(0, 1);
	
	if(dis(gen) == 0)
		return false;
	else
		return true;

}

_float3 CMath_Manager::Vector3x3_BezierCurve(IN _float3 _vStart, IN _float3 _vControl, IN _float3 _vEnd, IN OUT _float& _fTimeControl, IN _float _fTimeDelta, IN _float _fSpeed)
{
	_float3 vResult;
	_float fDetail = _fTimeControl;

	vResult.x = _vEnd.x * BezierB1(fDetail) + _vControl.x * BezierB2(fDetail) + _vStart.x * BezierB3(fDetail);
	vResult.y = _vEnd.y * BezierB1(fDetail) + _vControl.y * BezierB2(fDetail) + _vStart.y * BezierB3(fDetail);
	vResult.z = _vEnd.z * BezierB1(fDetail) + _vControl.z * BezierB2(fDetail) + _vStart.z * BezierB3(fDetail);

	_fTimeControl += _fTimeDelta * _fSpeed;

	return vResult;
}

_float CMath_Manager::BezierB1(_float t)
{
	return t*t;
}

_float CMath_Manager::BezierB2(_float t)
{
	return 2*t*(1-t);
}

_float CMath_Manager::BezierB3(_float t)
{
	return (1-t)*(1-t);
}
void CMath_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
	Destroy_Instance();
}
