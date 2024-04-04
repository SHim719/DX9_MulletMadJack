#include "Cui_Special3Sec.h"
#include "GameInstance.h"
#include "CUi_SpecialHit_Part.h"


CUi_Special3Sec::CUi_Special3Sec(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi_Life(pGraphic_Device)
{
}

CUi_Special3Sec::CUi_Special3Sec(const CUi_Special3Sec& rhs)
	: CUi_Life(rhs)
{
}

HRESULT CUi_Special3Sec::Initialize_Prototype()
{
	Default_Set_LifeTime();
	Default_Set_Size();
	return S_OK;
}

HRESULT CUi_Special3Sec::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;


	Initialize_Set_Scale_Pos_Rotation(pArg);
	Initialize_Set_Background();
	Initialize_Set_Speed();

	return S_OK;
}

void CUi_Special3Sec::PriorityTick(_float fTimeDelta)
{
}

void CUi_Special3Sec::Tick(_float fTimeDelta)
{
	m_fMoveTime += fTimeDelta;
	Move(fTimeDelta);
	Cal_Life_Blink(fTimeDelta);
}

void CUi_Special3Sec::LateTick(_float fTimeDelta)
{
	m_pSpecialHit_Part->Set_Pos(m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION));
}

HRESULT CUi_Special3Sec::Render()
{
	if (Cal_BlinkRender(0.1f))
	{
		m_pSpecialHit_Part->Render();

		if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
			return E_FAIL;

		m_pTextureCom->Bind_Texture(m_iTexture_Index);

		m_pVIBufferCom->Render();
	}
	return S_OK;
}

void CUi_Special3Sec::Move(_float fTimeDelta)
{
	if (m_fMoveTime < 0.5)
	{
		m_pTransformCom->Go_Up(fTimeDelta);
		m_pTransformCom->Go_Right(fTimeDelta);
	}
	else
	{
		m_pTransformCom->Go_Up(fTimeDelta);
	}
}

void CUi_Special3Sec::Initialize_Set_Background()
{
	m_pSpecialHit_Part = (CUi_SpecialHit_Part*)m_pGameInstance->
		Add_Ui_PartClone(TEXT("CUi_BackGround"), &m_UiDesc);
}

HRESULT CUi_Special3Sec::Add_Components(void* pArg)
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

	if(FAILED(Add_Texture(pArg)))
		return E_FAIL;


	return S_OK;
}


HRESULT CUi_Special3Sec::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_Special3Sec_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Special3Sec::Default_Set_LifeTime()
{
	m_fLifeTime = 1.5f;
}

void CUi_Special3Sec::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 128;
	m_UiDesc.m_fSizeY = 32;
}

void CUi_Special3Sec::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	m_UiDesc.m_fX = -250;
	m_UiDesc.m_fY = -150;

	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 0.f };
	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUi_Special3Sec::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(200);
}

CUi_Special3Sec* CUi_Special3Sec::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Special3Sec* pInstance = new CUi_Special3Sec(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Special3Sec Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CUi_Special3Sec::Clone(void* pArg)
{
	CUi_Special3Sec* pInstance = new CUi_Special3Sec(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_Special3Sec_Clone Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Special3Sec::Free()
{
	Safe_Release(m_pSpecialHit_Part);
	__super::Free();
}
