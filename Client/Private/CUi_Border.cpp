#include "CUi_Border.h"

CUi_Border::CUi_Border(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Border::CUi_Border(const CUi_Border& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Border::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Border::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Border::PriorityTick(_float fTimeDelta)
{
}

void CUi_Border::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;

	if (m_fActiveTime > 0)
	{
		Move(fTimeDelta);
	}
}

void CUi_Border::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Border::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;


	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();


	if (FAILED(m_pUniqueTransformCom->Bind_WorldMatrix()))
		return E_FAIL;


	//m_pUniqueVIBufferCom->Render();


	return S_OK;
}

HRESULT CUi_Border::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);


	return S_OK;
}

void CUi_Border::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 0.15f;
}

void CUi_Border::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 260;
	m_UiDesc.m_fSizeY = 300;
}

void CUi_Border::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(1500);

}

void CUi_Border::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };


	m_UiDesc.m_fX = 690.f;
	m_UiDesc.m_fY = 200.f;


	m_pTransformCom->Set_Scale(Scale);
	m_pUniqueTransformCom->Set_Scale(Scale);


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
	m_pUniqueTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX - 450, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_Border::Add_Components(void* pArg)
{
	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pTransformCom)))
		return E_FAIL;


	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pUniqueVIBufferCom)))
		return E_FAIL;


	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pUniqueTransformCom)))
		return E_FAIL;


	if (FAILED(Add_Texture(nullptr)))
		return E_FAIL;


	return S_OK;

}

HRESULT CUi_Border::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Border_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}
void CUi_Border::Enter(bool _Enter)
{
	m_bEnter = _Enter;
	m_bActive = true;
	if (m_bEnter)
	{
		Initialize_Set_ActiveTime();
		Initialize_Set_Size();
		Initialize_Set_Speed();
		Initialize_Set_Scale_Pos_Rotation(nullptr);
	}
	else
	{
		Initialize_Set_ActiveTime();
	}
}

void CUi_Border::Move(_float fTimeDelta)
{
	if (m_bEnter)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	else
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}
}

void CUi_Border::Scaling(_float fTimeDelta)
{

}

CUi_Border* CUi_Border::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Border* pInstance = new CUi_Border(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Border Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Border::Free()
{
	Safe_Release(m_pUniqueTransformCom);
	Safe_Release(m_pUniqueVIBufferCom);
	__super::Free();
}
