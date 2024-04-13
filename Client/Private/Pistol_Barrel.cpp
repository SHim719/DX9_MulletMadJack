#include "Pistol_Barrel.h"
#include "GameInstance.h"

CPistol_Barrel::CPistol_Barrel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CPistol_Barrel::CPistol_Barrel(const CPistol_Barrel& rhs)
	: CUi(rhs)
{
}

HRESULT CPistol_Barrel::Initialize_Prototype()
{

	return S_OK;

}

HRESULT CPistol_Barrel::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components(NULL))
		return E_FAIL;

	Default_Set_Size();
	Default_Set_Delay(0.1f);
	Initialize_Set_Scale_Pos_Rotation(NULL);
	Set_Texture_Index(0);

	return S_OK;
}

HRESULT CPistol_Barrel::Initialize_Active()
{
	m_iTexture_Index = 0;
	m_fLissajousTime = 0.f;
	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);

	return S_OK;
}

void CPistol_Barrel::PriorityTick(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
}

void CPistol_Barrel::Tick(_float fTimeDelta)
{

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));

	if (AnimationDelay(fTimeDelta) < 0.f && m_iTexture_Index <= m_pTextureCom->Get_MaxTextureNum()) {
		m_iTexture_Index++;
		AnimationDelayReset();
	}

	if (m_iTexture_Index > m_pTextureCom->Get_MaxTextureNum()) {
		m_iTexture_Index = 0;
		CGameInstance::Get_Instance()->Set_Ui_ActiveState(TEXT("Ui_Pistol_Barrel"), false);
		AnimationDelayReset();
	}

	m_fScale = { Get_Texture_Info().x / m_fDivide , Get_Texture_Info().y / m_fDivide, 1.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);

}

void CPistol_Barrel::LateTick(_float fTimeDelta)
{
	_float2 fLissajousPos = Lissajous_Curve(fTimeDelta, m_fLissajousTime, m_UiDesc.m_fX, m_UiDesc.m_fY, 400, 300, 2, 1, 2, 8);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + fLissajousPos.x, m_UiDesc.m_fY + fLissajousPos.y, 0.f));
}

HRESULT CPistol_Barrel::Render()
{

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

void CPistol_Barrel::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	Set_Ui_Pos(-100, -400);
	Set_Divide(1.f);

	m_fScale = { Get_Texture_Info().x / m_fDivide , Get_Texture_Info().y / m_fDivide, 1.f };
	m_fRotation = { 0.f, 0.f, 0.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CPistol_Barrel::Initialize_Set_Speed()
{
}

void CPistol_Barrel::Default_Set_LifeTime()
{
}

void CPistol_Barrel::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 840;
	m_UiDesc.m_fSizeY = 1514;
}

POINT CPistol_Barrel::Get_Texture_Info()
{
	if (m_pTextureCom == nullptr)
		return { 0, 0 };

	return { 256, 256 };
}

_float2 CPistol_Barrel::Lissajous_Curve(_float _fTimeDelta, _float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta, _float _fLissajousSpeed)
{
	_fLissajousTime += _fTimeDelta * _fLissajousSpeed;

	_fPosX = _fWitth * sin(_fLagrangianX * _fLissajousTime + 3.14f / _fPhaseDelta);
	_fPosY = _fHeight * cos(_fLagrangianY * _fLissajousTime);

	return { _fPosX,_fPosY };
}

HRESULT CPistol_Barrel::Add_Components(void* pArg)
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


HRESULT CPistol_Barrel::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Pistol_Barrel_Textures")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;

}

CUi* CPistol_Barrel::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPistol_Barrel* pInstance = new CPistol_Barrel(pGraphic_Device);

	if (FAILED(pInstance->Initialize(NULL)))
	{
		MSG_BOX(TEXT("Failed to Created : CPistol_Barrel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CPistol_Barrel::Free()
{
	__super::Free();
}
