#include "CUi_LiveStream.h"
#include "Ui_Pos.h"


CUi_LiveStream::CUi_LiveStream(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_LiveStream::CUi_LiveStream(const CUi_LiveStream& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_LiveStream::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_LiveStream::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_LiveStream::PriorityTick(_float fTimeDelta)
{
}

void CUi_LiveStream::Tick(_float fTimeDelta)
{
	m_fActiveTime -= fTimeDelta;

	if (m_fActiveTime > 0)
	{
		Scaling(fTimeDelta);
	}
	else if (m_fActiveTime < 0 && m_bEnter)
	{
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Ui_Pos::LiveStream);
	}
	else if (m_fActiveTime < 0 && !m_bEnter)
	{
		m_bActive = false;
	}
}

void CUi_LiveStream::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_LiveStream::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;


	m_pTextureCom->Bind_Texture(m_iTexture_Index);
	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CUi_LiveStream::Initialize_Active()
{
	Initialize_Set_ActiveTime();
	Initialize_Set_Size();
	Initialize_Set_Speed();
	Initialize_Set_Scale_Pos_Rotation(nullptr);


	return S_OK;
}

void CUi_LiveStream::Initialize_Set_ActiveTime()
{
	m_fActiveTime = 0.3f;
}

void CUi_LiveStream::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 320;
	m_UiDesc.m_fSizeY = 150;
}

void CUi_LiveStream::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(50);
}

void CUi_LiveStream::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };


	m_UiDesc.m_fX = -520.f;
	m_UiDesc.m_fY = 300.f;


	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

HRESULT CUi_LiveStream::Add_Components(void* pArg)
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

HRESULT CUi_LiveStream::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_LiveStream_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

void CUi_LiveStream::Enter(bool _Enter)
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

void CUi_LiveStream::Scaling(_float fTimeDelta)
{
	m_fScaleTime += fTimeDelta;
	if (m_bEnter)
	{
		if (m_fScaleTime > 0.05)
		{
			m_UiDesc.m_fSizeX -= 5;
			m_UiDesc.m_fSizeY -= 5;
		}
		_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
		m_pTransformCom->Set_Scale(Scale);
	}
	else
	{
		if (m_fScaleTime > 0.05)
		{
			m_UiDesc.m_fSizeX += 4;
			m_UiDesc.m_fSizeY += 4;
		}
		_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
		m_pTransformCom->Set_Scale(Scale);
	}
	Move(fTimeDelta);
}

void CUi_LiveStream::Move(_float fTimeDelta)
{
	m_pTransformCom->Go_Right(fTimeDelta);
}


CUi_LiveStream* CUi_LiveStream::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_LiveStream* pInstance = new CUi_LiveStream(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_LiveStream Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_LiveStream::Free()
{
	__super::Free();
}
