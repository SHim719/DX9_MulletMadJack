#include "CUi_Heart.h"
#include "Ui_Pos.h"


CUi_Heart::CUi_Heart(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Heart::CUi_Heart(const CUi_Heart& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Heart::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUi_Heart::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Heart::PriorityTick(_float fTimeDelta)
{
}

void CUi_Heart::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;
	m_fTextureTime += fTimeDelta;


	if (m_fActiveTime > 0)
	{
		Move(fTimeDelta);
	}
	else if (m_fActiveTime < 0 && m_bEnter)
	{
		_float3 unique = { Ui_Pos::Heart.x - 450, Ui_Pos::Heart.y, Ui_Pos::Heart.z };
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Ui_Pos::Heart);
		m_pUniqueTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &unique);
	}
	else if (m_fActiveTime < 0 && !m_bEnter)
	{
		m_bActive = false;
	}

	if (m_fTextureTime > 0.06)
	{
		TextureSwitching();
		m_fTextureTime = 0.f;
	}
}

void CUi_Heart::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Heart::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;


	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();


	if (FAILED(m_pUniqueTransformCom->Bind_WorldMatrix()))
		return E_FAIL;


	m_pUniqueVIBufferCom->Render();


	return S_OK;
}

void CUi_Heart::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };


	m_UiDesc.m_fX = 315.f;
	m_UiDesc.m_fY = -500.f;


	m_pTransformCom->Set_Scale(Scale);
	m_pUniqueTransformCom->Set_Scale(Scale);


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
	m_pUniqueTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX-450, m_UiDesc.m_fY, 0.f));
}

void CUi_Heart::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(750);
	m_pUniqueTransformCom->Set_Speed(750);
}

void CUi_Heart::Move(_float fTimeDelta)
{
	if (m_bEnter)
	{
		m_pTransformCom->Go_Up(fTimeDelta);
		m_pTransformCom->Go_Left(fTimeDelta);
		m_pUniqueTransformCom->Go_Up(fTimeDelta);
		m_pUniqueTransformCom->Go_Left(fTimeDelta);
	}

	else
	{
		m_pTransformCom->Go_Down(fTimeDelta);
		m_pTransformCom->Go_Right(fTimeDelta);
		m_pUniqueTransformCom->Go_Down(fTimeDelta);
		m_pUniqueTransformCom->Go_Right(fTimeDelta);
	}
}

void CUi_Heart::TextureSwitching()
{
	++m_iTexture_Index;
	if (m_iTexture_Index > m_pTextureCom->Get_MaxTextureNum())
	{
		m_iTexture_Index = 0;
	}
}

void CUi_Heart::Enter(bool _Enter)
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

HRESULT CUi_Heart::Initialize_Active()
{
	
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(nullptr);
	Initialize_Set_Speed();


	return S_OK;
}

void CUi_Heart::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 0.3f;
}

void CUi_Heart::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 70;
	m_UiDesc.m_fSizeY = 100;
}

HRESULT CUi_Heart::Add_Components(void* pArg)
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

HRESULT CUi_Heart::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Heart_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

CUi_Heart* CUi_Heart::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Heart* pInstance = new CUi_Heart(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Heart Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Heart::Free()
{
	Safe_Release(m_pUniqueTransformCom);
	Safe_Release(m_pUniqueVIBufferCom);
	__super::Free();
}
