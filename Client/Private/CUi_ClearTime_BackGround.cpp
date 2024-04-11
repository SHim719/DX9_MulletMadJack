#include "CUi_ClearTime_BackGround.h"
#include "GameInstance.h"
#include "CUi_Time_Division.h"


CUi_ClearTime_BackGround::CUi_ClearTime_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_ClearTime_BackGround::CUi_ClearTime_BackGround(const CUi_ClearTime_BackGround& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_ClearTime_BackGround::Initialize_Prototype()
{
	Default_Set_LifeTime();
	Default_Set_Size();

	return S_OK;
}

HRESULT CUi_ClearTime_BackGround::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;

	Initialize_Set_Scale_Pos_Rotation(pArg);
	Initialize_Set_Speed();
	Initialize_Set_Division();

	return S_OK;
}

void CUi_ClearTime_BackGround::PriorityTick(_float fTimeDelta)
{
}

void CUi_ClearTime_BackGround::Tick(_float fTimeDelta)
{
}

void CUi_ClearTime_BackGround::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_ClearTime_BackGround::Render()
{


	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pTextureCom->Bind_Texture(m_iTexture_Index);

	m_pVIBufferCom->Render();

	for (auto& iter : m_pDivision)
		iter->Render();

	return S_OK;
}

HRESULT CUi_ClearTime_BackGround::Add_Components(void* pArg)
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

	Add_Texture(pArg);

	return S_OK;
}

HRESULT CUi_ClearTime_BackGround::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("CUi_ClearTime_BackGround_Texture")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}
void CUi_ClearTime_BackGround::Default_Set_LifeTime()
{
	m_fLifeTime = 0.f;
}

void CUi_ClearTime_BackGround::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 955;
	m_UiDesc.m_fSizeY = 100;
}

void CUi_ClearTime_BackGround::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = -155;
	m_UiDesc.m_fY = 50;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State
	(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUi_ClearTime_BackGround::Initialize_Set_Speed()
{
}

void CUi_ClearTime_BackGround::Initialize_Set_Division()
{
	m_pDivision.clear();
	m_pDivision.reserve(4);
	m_pDivision.push_back((CUi_Time_Division*)m_pGameInstance->Add_Ui_PartClone
	(TEXT("CUi_Time_Division"), nullptr));
	m_pDivision.push_back((CUi_Time_Division*)m_pGameInstance->Add_Ui_PartClone
	(TEXT("CUi_Time_Division"), nullptr));
	m_pDivision.push_back((CUi_Time_Division*)m_pGameInstance->Add_Ui_PartClone
	(TEXT("CUi_Time_Division"), nullptr));
	m_pDivision.push_back((CUi_Time_Division*)m_pGameInstance->Add_Ui_PartClone
	(TEXT("CUi_Time_Division"), nullptr));

	_float3 Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Pos.x += -45;
	Pos.y += 15;
	m_pDivision[0]->Set_Pos(Pos);
	Pos.y -= 30;
	m_pDivision[1]->Set_Pos(Pos);
	Pos.x += 120;
	m_pDivision[2]->Set_Pos(Pos);
	Pos.y += 30;
	m_pDivision[3]->Set_Pos(Pos);
}

void CUi_ClearTime_BackGround::Set_Pos(_float3 Position)
{
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, &Position);
}

void CUi_ClearTime_BackGround::Scaling(_float fTimeDelta)
{
}

CUi_ClearTime_BackGround* CUi_ClearTime_BackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_ClearTime_BackGround* pInstance = new CUi_ClearTime_BackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_ClearTime_BackGround Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CUi_ClearTime_BackGround::Clone(void* pArg)
{
	CUi_ClearTime_BackGround* pInstance = new CUi_ClearTime_BackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_ClearTime_BackGround Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_ClearTime_BackGround::Free()
{
	for(auto iter : m_pDivision)
		Safe_Release(iter);

	__super::Free();
}
