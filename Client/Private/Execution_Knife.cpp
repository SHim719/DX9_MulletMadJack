#include "Execution_Knife.h"
#include "GameInstance.h"

CExecution_Knife::CExecution_Knife(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CExecution_Knife::CExecution_Knife(const CExecution_Knife& rhs)
	: CUi(rhs)
{
}

HRESULT CExecution_Knife::Initialize_Prototype()
{

	return S_OK;

}

HRESULT CExecution_Knife::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components(NULL))
		return E_FAIL;

	Default_Set_Size();
	Default_Set_Delay(0.11f);
	//Default_Set_Delay(1.f);
	Initialize_Set_Scale_Pos_Rotation(NULL);
	Set_Texture_Index(0);

	return S_OK;
}

HRESULT CExecution_Knife::Initialize_Active()
{
	m_iTexture_Index = 0;
	m_fLissajousTime = 0;
	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);

	return S_OK;
}

void CExecution_Knife::PriorityTick(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
}

void CExecution_Knife::Tick(_float fTimeDelta)
{
	//m_fDivide -= fTimeDelta * 1.5f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));

	if (AnimationDelay(fTimeDelta) < 0.f && m_iTexture_Index <= m_pTextureCom->Get_MaxTextureNum()) {
		m_iTexture_Index++;
		AnimationDelayReset();
	}

	if (m_iTexture_Index > m_pTextureCom->Get_MaxTextureNum()) {
		m_iTexture_Index = 2;
		//CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("Execution_Knife"), false);
		AnimationDelayReset();
	}

	m_fScale = { Get_Texture_Info().x / m_fDivide , Get_Texture_Info().y / m_fDivide, 0.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);

}

void CExecution_Knife::LateTick(_float fTimeDelta)
{
	_float2 fLissajousPos = Lissajous_Curve(fTimeDelta, m_fLissajousTime, m_UiDesc.m_fX, m_UiDesc.m_fY,	400.f, 400.f, 1, 1, 1, 12);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + fLissajousPos.x, m_UiDesc.m_fY + fLissajousPos.y, 0.f));
	if (m_iTexture_Index == 2) {
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX - 600, m_UiDesc.m_fY + 600, 0.f));
	}
}

HRESULT CExecution_Knife::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();
	return S_OK;
}

void CExecution_Knife::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	Set_Ui_Pos(780, -680);
	Set_Divide(1.f);

	m_fScale = { m_UiDesc.m_fSizeX / Get_Divide() , m_UiDesc.m_fSizeY / Get_Divide(), 1.f };
	m_fRotation = { 0.f, 0.f, 45.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 1.f));
}

void CExecution_Knife::Initialize_Set_Speed()
{
}

void CExecution_Knife::Default_Set_LifeTime()
{
}

void CExecution_Knife::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 1024;
	m_UiDesc.m_fSizeY = 1024;
}

POINT CExecution_Knife::Get_Texture_Info()
{
	if (m_pTextureCom == nullptr)
		return { 0, 0 };

	return { 1024,1024 };
}

_float2 CExecution_Knife::Lissajous_Curve(_float _fTimeDelta, _float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta, _float _fLissajousSpeed)
{
	_fLissajousTime += _fTimeDelta * _fLissajousSpeed;

	float fPosX = _fWitth * sin((_fLagrangianX * _fLissajousTime) + (3.14f / _fPhaseDelta));
	float fPosY = _fHeight * sin(_fLagrangianY * _fLissajousTime);

	return { fPosX,fPosY };
}

HRESULT CExecution_Knife::Add_Components(void* pArg)
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


HRESULT CExecution_Knife::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Execution_Knife_Texture")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;

}

CUi* CExecution_Knife::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CExecution_Knife* pInstance = new CExecution_Knife(pGraphic_Device);

	if (FAILED(pInstance->Initialize(NULL)))
	{
		MSG_BOX(TEXT("Failed to Created : CExecution_Knife"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CExecution_Knife::Free()
{
	__super::Free();
}
