#include "CUi_SpecialHit.h"
#include "CUi_Background.h"
#include "GameInstance.h"


CUi_SpecialHit::CUi_SpecialHit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi_Life(pGraphic_Device)
{
}

CUi_SpecialHit::CUi_SpecialHit(const CUi_SpecialHit& rhs)
	:CUi_Life(rhs)
{
}

HRESULT CUi_SpecialHit::Initialize_Prototype()
{
	Default_Set_LifeTime();
	Default_Set_Size();
	return S_OK;
}

HRESULT CUi_SpecialHit::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;


	Initialize_Set_Scale_Pos_Rotation(pArg);
	Initialize_Set_Speed();
	Initalize_Set_Background();
	

	return S_OK;
}

void CUi_SpecialHit::PriorityTick(_float fTimeDelta)
{
}

void CUi_SpecialHit::Tick(_float fTimeDelta)
{
	//Cal_Life_Blink(fTimeDelta);
}

void CUi_SpecialHit::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_SpecialHit::Render()
{
	if (Cal_BlinkRender(0.1f))
	{
		m_pBackGround->Render();

		if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
			return E_FAIL;

		m_pTextureCom->Bind_Texture(m_iTexture_Index);

		m_pVIBufferCom->Render();
	}
	return S_OK;
}

HRESULT CUi_SpecialHit::Add_Components(void* pArg)
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


	if (FAILED(Add_Texture(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUi_SpecialHit::Add_Texture(void* pArg)
{
	eSpecialHit* type = (eSpecialHit*)pArg;
	if (*type == eSpecialHit::HEADSHOT)
	{
		if (FAILED(Add_Component(LEVEL_STATIC, 
			TEXT("CUi_SpecialHit_Texture"),
			(CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if(*type == eSpecialHit::FINISHED)
	{

	}


	return S_OK;
}


void CUi_SpecialHit::Default_Set_LifeTime()
{
	m_fLifeTime = 3.f;
}

void CUi_SpecialHit::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 100;
	m_UiDesc.m_fSizeY = 40;

}

void CUi_SpecialHit::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	m_UiDesc.m_fX = -500;
	m_UiDesc.m_fY = -50;
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };
	_float3 Rotation = { 0.f, 0.f, 15.f };
	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Rotation_XYZ(Rotation);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
	
}

void CUi_SpecialHit::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(50.f);
}

void CUi_SpecialHit::Initalize_Set_Background()
{
	Ui_Pos_Size temp;
	temp.m_fSizeX = { 200 };
	temp.m_fSizeY = { 40 };
	temp.m_fX = m_UiDesc.m_fX;
	temp.m_fY = m_UiDesc.m_fY;

	m_pBackGround = (CUi_Background*)m_pGameInstance->
		Add_Ui_BackgroundClone(&temp);
}

CUi_SpecialHit* CUi_SpecialHit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_SpecialHit* pInstance = new CUi_SpecialHit(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_SpecialHit Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi_Life* CUi_SpecialHit::Clone(void* pArg)
{
	CUi_Life* pInstance = new CUi_SpecialHit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_SpecialHit_Clone Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CUi_SpecialHit::Free()
{
	Safe_Release(m_pBackGround);
	__super::Free();
}
