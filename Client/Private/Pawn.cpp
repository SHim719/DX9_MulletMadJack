#include "Pawn.h"

#include "GameInstance.h"

CPawn::CPawn(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CPawn::CPawn(const CPawn& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CPawn::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPawn::Initialize(void* pArg)
{
	return S_OK;
}

void CPawn::PriorityTick(_float fTimeDelta)
{
}

void CPawn::Tick(_float fTimeDelta)
{
}

void CPawn::LateTick(_float fTimeDelta)
{
}

HRESULT CPawn::Render()
{
	return S_OK;
}

void CPawn::Set_Motions(_float fTimeDelta)
{
}

void CPawn::Free()
{
	__super::Free();
}
