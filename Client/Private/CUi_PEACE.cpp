#include "CUi_PEACE.h"
#include "GameInstance.h"


CUi_PEACE::CUi_PEACE(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi_Active(pGraphic_Device)
{
}

CUi_PEACE::CUi_PEACE(const CUi_PEACE& rhs)
	: CUi_Active(rhs)
{
}

HRESULT CUi_PEACE::Initialize_Prototype()
{
	Add_Texture(nullptr);

	return S_OK;
}

HRESULT CUi_PEACE::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_PEACE::PriorityTick(_float fTimeDelta)
{
}

void CUi_PEACE::Tick(_float fTimeDelta)
{
}

void CUi_PEACE::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_PEACE::Render()
{
	return S_OK;
}

void CUi_PEACE::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
}

void CUi_PEACE::Initialize_Set_Speed()
{
}

void CUi_PEACE::Initialize_Set_ActiveTime()
{
}

void CUi_PEACE::Initialize_Set_Size()
{
}

HRESULT CUi_PEACE::Add_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUi_PEACE::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Peace_Texture"),
		(CComponent**)&m_pTextureCom)))	
		return E_FAIL;


	return S_OK;
}

CUi_PEACE* CUi_PEACE::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return nullptr;
}

void CUi_PEACE::Free()
{
	__super::Free();
}
