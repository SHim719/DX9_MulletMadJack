#include "Execution_Body.h"
#include "GameInstance.h"
#include "CGame_Manager.h"
CExecution_Body::CExecution_Body(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CExecution_Body::CExecution_Body(const CExecution_Body& rhs)
	: CUi(rhs)
{
}

HRESULT CExecution_Body::Initialize_Prototype()
{

	return S_OK;

}

HRESULT CExecution_Body::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components(NULL))
		return E_FAIL;

	Default_Set_Size();
	Default_Set_Delay(0.15f);
	Initialize_Set_Scale_Pos_Rotation(NULL);
	Set_Texture_Index(0);

	return S_OK;
}

HRESULT CExecution_Body::Initialize_Active()
{
	m_iTexture_Index = 0;
	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);

	return S_OK;
}

void CExecution_Body::PriorityTick(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
}

void CExecution_Body::Tick(_float fTimeDelta)
{
	//m_fDivide -= fTimeDelta * 1.5f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.9f));

	if (AnimationDelay(fTimeDelta) < 0.f && m_iTexture_Index <= Texture_Tag()->Get_MaxTextureNum()) {
		m_iTexture_Index++;
		AnimationDelayReset();
	}

	if (m_iTexture_Index > Texture_Tag()->Get_MaxTextureNum()) {
		m_iTexture_Index = 0;
		AnimationDelayReset();
	}

	m_fScale = { Get_Texture_Info().x / m_fDivide , Get_Texture_Info().y / m_fDivide, 1.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);

}

void CExecution_Body::LateTick(_float fTimeDelta)
{
	//_float2 fLissajousPos = Lissajous_Curve(fTimeDelta, m_fLissajousTime, m_UiDesc.m_fX, m_UiDesc.m_fY, 1.5f, 2, 3, 1, 2, 6);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + CGame_Manager::Get_Instance()->Object_Shake(5).x, m_UiDesc.m_fY + CGame_Manager::Get_Instance()->Object_Shake(5).y, 0.9f));
}

HRESULT CExecution_Body::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 101);
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(Texture_Tag()->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

void CExecution_Body::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	Set_Ui_Pos(0, -250);
	Set_Divide(1.f);

	m_fScale = { m_UiDesc.m_fSizeX / Get_Divide() , m_UiDesc.m_fSizeY / Get_Divide(), 1.f };
	m_fRotation = { 0.f, 0.f, 0.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CExecution_Body::Initialize_Set_Speed()
{
}

void CExecution_Body::Default_Set_LifeTime()
{
}

void CExecution_Body::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 1024;
	m_UiDesc.m_fSizeY = 1024;
}

POINT CExecution_Body::Get_Texture_Info()
{
	if (Texture_Tag() == nullptr)
		return { 0, 0 };

	return { 1024,1024 };
}

_float2 CExecution_Body::Lissajous_Curve(_float _fTimeDelta, _float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta, _float _fLissajousSpeed)
{
	_fLissajousTime += _fTimeDelta * _fLissajousSpeed;

	_fPosX = _fWitth * sin(_fLagrangianX * _fLissajousTime + 3.14f / _fPhaseDelta);
	_fPosY = _fHeight * cos(_fLagrangianY * _fLissajousTime);

	return { _fPosX,_fPosY };
}

CTexture* CExecution_Body::Texture_Tag()
{
	string str = CPlayer_Manager::Get_Instance()->Get_Execution_Target();
	
	if (str == "Chainsaw_Monster") return m_pTexture_Chainsaw;
	else return m_pTextureCom;
}

HRESULT CExecution_Body::Add_Components(void* pArg)
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


HRESULT CExecution_Body::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Execution_Body_Texture")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Execution_Body2_Texture")
		, (CComponent**)&m_pTexture_Chainsaw)))
		return E_FAIL;


	return S_OK;

}

CUi* CExecution_Body::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CExecution_Body* pInstance = new CExecution_Body(pGraphic_Device);

	if (FAILED(pInstance->Initialize(NULL)))
	{
		MSG_BOX(TEXT("Failed to Created : CExecution_Body"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CExecution_Body::Free()
{
	__super::Free();
	Safe_Release(m_pTexture_Chainsaw);
}
