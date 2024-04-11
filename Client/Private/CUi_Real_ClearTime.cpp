#include "CUi_Real_ClearTime.h"
#include "CGame_Manager.h"
#include "GameInstance.h"
#include "CUi_ClearTime_BackGround.h"


CUi_Real_ClearTime::CUi_Real_ClearTime(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_Real_ClearTime::CUi_Real_ClearTime(const CUi_Real_ClearTime& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_Real_ClearTime::Initialize_Prototype()
{
	Default_Set_LifeTime();
	Default_Set_Size();

	return S_OK;
}

HRESULT CUi_Real_ClearTime::Initialize(void* pArg)
{
	if (FAILED(Add_Components(pArg)))
		return E_FAIL;

	Initialize_Set_Scale_Pos_Rotation(nullptr);
	Initialize_Set_Speed();
	Initialize_Set_Time();
	Initialize_Set_ClearTime();
	Initialize_Set_BackGround();

	return S_OK;
}

void CUi_Real_ClearTime::PriorityTick(_float fTimeDelta)
{
}

void CUi_Real_ClearTime::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	m_fScaleTime -= fTimeDelta;

	if (m_fScaleTime > 0)
		Scaling(fTimeDelta);

	if (m_fLifeTime < 0)
		m_bDead = true;
}

void CUi_Real_ClearTime::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_Real_ClearTime::Render()
{
	m_pBackGround->Render();
	_float3 Origin = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	for (size_t i = 0; i < m_Number.size(); ++i)
	{
		Pos.x += 60;
		m_pTransformCom->Set_Position(Pos);
		m_pTransformCom->Bind_WorldMatrix();
		m_pTextureCom->Bind_Texture(m_Number[i]);
		m_pVIBufferCom->Render();
	}

	m_pTransformCom->Set_Position(Origin);
	return S_OK;
}

HRESULT CUi_Real_ClearTime::Add_Components(void* pArg)
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

HRESULT CUi_Real_ClearTime::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("CUi_Real_ClearTime_Texture")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_Real_ClearTime::Default_Set_LifeTime()
{
	m_fLifeTime = 1.5f;
	m_fScaleTime = 0.3f;
}

void CUi_Real_ClearTime::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 1400;
	m_UiDesc.m_fSizeY = 1400;
}

void CUi_Real_ClearTime::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	_float3 Scale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = -350.f;
	m_UiDesc.m_fY = 50.f;

	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_State
	(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUi_Real_ClearTime::Initialize_Set_Speed()
{
}

void CUi_Real_ClearTime::Initialize_Set_ClearTime()
{
	Cal_ClearTime();
}

void CUi_Real_ClearTime::Initialize_Set_BackGround()
{
	m_pBackGround = (CUi_ClearTime_BackGround*)m_pGameInstance->Add_Ui_PartClone
	(TEXT("CUi_ClearTime_BackGround"), nullptr);
}

void CUi_Real_ClearTime::Initialize_Set_Time()
{
	m_fLifeTime = 1.5f;
	m_fScaleTime = 0.3f;
}

void CUi_Real_ClearTime::Scaling(_float fTimeDelta)
{
	_float3 Scale = m_pTransformCom->Get_Scale();
	Scale.x -= fTimeDelta * 4700;
	Scale.y -= fTimeDelta * 4700;
	m_pTransformCom->Set_Scale(Scale);
}

void CUi_Real_ClearTime::Cal_ClearTime()
{
	_uint First, Second, Third, Fourth, Fifth, Six;
	_float time = CGame_Manager::Get_Instance()->Get_StageClearTime();

	First = 0;

	Second = (_uint)(time / 60);

	_float Thirdtime = time - (Second*60);
	Third = (_uint)(Thirdtime / 10);

	_float Fourthtime = Thirdtime - (Third*10);
	Fourth = (_uint)Fourthtime;

	_uint FifthTime = _uint(time);
	_float FifthPoint = time - FifthTime;
	Fifth = _uint(FifthPoint * 10);
	
	_float SixPoint = time - FifthTime - (Fifth/10.f);
	Six = _uint(SixPoint * 100);

	m_Number.push_back(First);
	m_Number.push_back(Second);
	m_Number.push_back(Third);
	m_Number.push_back(Fourth);
	m_Number.push_back(Fifth);
	m_Number.push_back(Six);
}

CUi_Real_ClearTime* CUi_Real_ClearTime::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_Real_ClearTime* pInstance = new CUi_Real_ClearTime(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_Real_ClearTime Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CUi_Real_ClearTime::Clone(void* pArg)
{
	CUi_Real_ClearTime* pInstance = new CUi_Real_ClearTime(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CUi_Real_ClearTime Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Real_ClearTime::Free()
{
	m_Number.clear();

	Safe_Release(m_pBackGround);

	__super::Free();
}
