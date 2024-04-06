#include "CUi_Heart_BackGround.h"

CUi_Heart_BackGround::CUi_Heart_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Heart_BackGround::CUi_Heart_BackGround(const CUi_Heart_BackGround& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Heart_BackGround::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Heart_BackGround::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Heart_BackGround::PriorityTick(_float fTimeDelta)
{
}

void CUi_Heart_BackGround::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;

	if (m_fActiveTime > 0)
	{
		Move(fTimeDelta);
	}
}

void CUi_Heart_BackGround::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Heart_BackGround::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUi_Heart_BackGround::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);


	return S_OK;
}

void CUi_Heart_BackGround::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 0.15f;
}

void CUi_Heart_BackGround::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 1024;
	m_UiDesc.m_fSizeY = 128;
}

void CUi_Heart_BackGround::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(1500);
}

void CUi_Heart_BackGround::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = 100.f;
	m_UiDesc.m_fY = -500.f;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.9f));
}
HRESULT CUi_Heart_BackGround::Add_Components(void* pArg)
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


	if (FAILED(Add_Texture(nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUi_Heart_BackGround::Add_Texture(void* pArg)
{
	if(FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Heart_BackGround_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

void CUi_Heart_BackGround::Move(_float fTimeDelta)
{
	if (m_bEnter)
	{
		m_pTransformCom->Go_Up(fTimeDelta);
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	else
	{
		m_pTransformCom->Go_Down(fTimeDelta);
		m_pTransformCom->Go_Right(fTimeDelta);
	}
}

void CUi_Heart_BackGround::Enter(bool _Enter)
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

CUi_Heart_BackGround* CUi_Heart_BackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Heart_BackGround* pInstance = new CUi_Heart_BackGround(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Heart_BackGround Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Heart_BackGround::Free()
{
	__super::Free();
}
