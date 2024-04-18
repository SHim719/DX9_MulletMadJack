#include "CUi_SpecialHit.h"
#include "CUi_Background.h"
#include "GameInstance.h"
#include "CUi_MonsterDie.h"
#include "CUi_SpecialHit_Part.h"


CUi_SpecialHit::CUi_SpecialHit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CUi_SpecialHit::CUi_SpecialHit(const CUi_SpecialHit& rhs)
	:CUi(rhs)
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
	Initialize_Set_Scale_Limit();
	Initialize_Set_Background();
	Initialize_Set_SpecialHit_Part();


	return S_OK;
}

void CUi_SpecialHit::PriorityTick(_float fTimeDelta)
{
}

void CUi_SpecialHit::Tick(_float fTimeDelta)
{
	Move(fTimeDelta);
	Scaling(fTimeDelta);
	Cal_Life_Blink(fTimeDelta);
}

void CUi_SpecialHit::LateTick(_float fTimeDelta)
{
	_float3 BackPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	BackPos.x -= 50.f;
	BackPos.y -= 8;
	m_pBackGround->Set_Pos(BackPos);


	_float3 PartPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	PartPos.x += 190.f;
	PartPos.y += 30.f;
	m_pSpecialHit_Part->Set_Pos(PartPos);
}

HRESULT CUi_SpecialHit::Render()
{
	if (Cal_BlinkRender(0.1f))
	{
		m_pBackGround->Render();
		m_pSpecialHit_Part->Render();
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
	SpecialHit_Desc* type = (SpecialHit_Desc*)pArg;
	
	if (type->Hit == eSpecialHit::HEADSHOT)
	{
		if (FAILED(Add_Component(LEVEL_STATIC, 
			TEXT("CUi_SpecialHit_HEADSHOT_Texture"),
			(CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if(type->Hit == eSpecialHit::FINISHED)
	{
		if (FAILED(Add_Component(LEVEL_STATIC,
			TEXT("CUi_SpecialHit_FINISHED_Texture"),
			(CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}


	return S_OK;
}


void CUi_SpecialHit::Default_Set_LifeTime()
{
	m_fLifeTime = 3.f;
}

void CUi_SpecialHit::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 256;
	m_UiDesc.m_fSizeY = 50;
}

void CUi_SpecialHit::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	SpecialHit_Desc* temp = (SpecialHit_Desc*)pArg;
	_uint i = (temp->iCount) % 5;

	m_UiDesc.m_fX = -550;
	m_UiDesc.m_fY += + (_float)(i * 25);

	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 0 };
	m_UiDesc.m_Rotation = { 0.f, 0.f, 10.f };


	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Rotation_XYZ(m_UiDesc.m_Rotation);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
	
}

void CUi_SpecialHit::Initialize_Set_Speed()
{
	m_pTransformCom->Set_Speed(50.f);
}

void CUi_SpecialHit::Initialize_Set_Background()
{
	Ui_Pos_Size_Rotation temp;
	temp.m_fSizeX = { 350 };
	temp.m_fSizeY = { 40 };
	temp.m_fX = m_UiDesc.m_fX - 50;
	temp.m_fY = m_UiDesc.m_fY - 8;
	temp.m_Rotation = m_UiDesc.m_Rotation;

	m_pBackGround = (CUi_Background*)m_pGameInstance->
		Add_Ui_PartClone(TEXT("CUi_BackGround"), &temp);
}

void CUi_SpecialHit::Initialize_Set_Scale_Limit()
{
	m_fScaleUpperLimit = { 800, 157, 0 };
	m_fScaleDownLimit = { 246, 46, 0 };
}

void CUi_SpecialHit::Initialize_Set_SpecialHit_Part()
{
	CUi_SpecialHit_Part::PartDesc Set;
	Set.Desc.m_fX = m_UiDesc.m_fX + 200;
	Set.Desc.m_fY = m_UiDesc.m_fY + 100;
	Set.Desc.m_fSizeX = m_UiDesc.m_fSizeX * 0.6f;
	Set.Desc.m_fSizeY = m_UiDesc.m_fSizeY * 0.6f;
	Set.Desc.m_Rotation = m_UiDesc.m_Rotation;
	Set.Second = PlusSecond::Two;

	m_pSpecialHit_Part = (CUi_SpecialHit_Part*)
		m_pGameInstance->Add_Ui_PartClone(TEXT("CUi_SpecialHit_Part"), &Set);
}

void CUi_SpecialHit::Move(_float fTimeDelta)
{
	m_fMoveTime += fTimeDelta;
	if (m_fMoveTime < 1.5f)
	{
		m_pTransformCom->Go_Up(fTimeDelta);
	}
	else
	{
		m_pTransformCom->Go_Down(fTimeDelta);
	}
}

void CUi_SpecialHit::Scaling(_float fTimeDelta)
{	
	m_fScaleTime -= fTimeDelta;
	if (m_fScaleTime > 0.7f)
	{
		_float3 ScaleUp = { 1.07f, 1.07f, 1.f };
		m_pTransformCom->Multiply_Scale(ScaleUp);
		if (m_pTransformCom->Get_Scale() > m_fScaleUpperLimit)
		{
			m_pTransformCom->Set_Scale(m_fScaleUpperLimit);
		}
	}
	else if (m_fScaleTime > 0.3f)
	{
		_float3 ScaleDown = { 0.95f, 0.95f, 0.f };
		_float3 b = m_pTransformCom->Get_Scale();
		m_pTransformCom->Multiply_Scale(ScaleDown);	
	}
	else
	{
		m_pTransformCom->Set_Scale(m_fScaleDownLimit);
	}
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

CUi* CUi_SpecialHit::Clone(void* pArg)
{
	CUi_SpecialHit* pInstance = new CUi_SpecialHit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_SpecialHit_Clone Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CUi_SpecialHit::Free()
{
	Safe_Release(m_pSpecialHit_Part);
	Safe_Release(m_pBackGround);
	__super::Free();
}
