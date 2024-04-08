#include "CUi_Floor_F.h"

CUi_Floor_F::CUi_Floor_F(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Floor_F::CUi_Floor_F(const CUi_Floor_F& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Floor_F::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Floor_F::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_Floor_F::PriorityTick(_float fTimeDelta)
{
}

void CUi_Floor_F::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;

	if (m_fActiveTime > 0)
	{
		Sub_Speed(fTimeDelta);
		Move(fTimeDelta);
		Rotation(fTimeDelta);
	}
	else
	{
	}
}

void CUi_Floor_F::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Floor_F::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUi_Floor_F::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);

	return S_OK;
}

void CUi_Floor_F::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 3.f;
}

void CUi_Floor_F::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 80;
	m_UiDesc.m_fSizeY = 80;
}

void CUi_Floor_F::Initialize_Set_Speed()
{
	//m_fSpeed = 400;
	m_fSpeed = 0;
	m_pTransformCom->Set_Speed(m_fSpeed);
}

void CUi_Floor_F::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 0.f };

	m_UiDesc.m_fX = -220;
	m_UiDesc.m_fY = 270;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_Floor_F::Add_Components(void* pArg)
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

HRESULT CUi_Floor_F::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Floor_F_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}


void CUi_Floor_F::Sub_Speed(_float fTimeDelta)
{
	m_fSpeed -= fTimeDelta * 1000;
	if (m_fSpeed < 0)
	{
		m_fSpeed = 0;
	}
	m_pTransformCom->Set_Speed(m_fSpeed);
}

void CUi_Floor_F::Move(_float fTimeDelta)
{
	m_pTransformCom->Go_Down(fTimeDelta);
	m_pTransformCom->Go_Right(fTimeDelta);
}

void CUi_Floor_F::Rotation(_float fTimeDelta)
{
	m_fRotationTime += fTimeDelta;
	if (m_fRotationTime > 0.1)
	{
		_float4x4 world = m_pTransformCom->Get_WorldMatrix();
		_float4x4 rotation;
		D3DXMatrixIdentity(&rotation);
		world = world * *D3DXMatrixRotationY(&rotation, To_Radian(1));
		m_pTransformCom->Set_WorldMatrix(world);
	

		m_fRotationTime = 0;
	}
}

CUi_Floor_F* CUi_Floor_F::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Floor_F* pInstance = new CUi_Floor_F(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Floor_F Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Floor_F::Free()
{
	__super::Free();
}
