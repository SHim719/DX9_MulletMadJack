#include "Pistol_Gunfire.h"
#include "PlayerManager.h"

CPistol_Gunfire::CPistol_Gunfire(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CPistol_Gunfire::CPistol_Gunfire(const CPistol_Gunfire& rhs)
	: CUi(rhs)
{
}

HRESULT CPistol_Gunfire::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CPistol_Gunfire::Initialize(void* pArg)
{
	m_iGunfireDir = *static_cast<int*>(pArg);

	if (E_FAIL == Add_Components(NULL))
		return E_FAIL;

	Default_Set_Size();
	Default_Set_Delay(0.f);
	Initialize_Set_Scale_Pos_Rotation(NULL);
	Set_Texture_Index(0);
	Default_Set_LifeTime();
	return S_OK;
}

HRESULT CPistol_Gunfire::Initialize_Active()
{
	Set_Texture_Index(0);
	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);

	return S_OK;
}

void CPistol_Gunfire::PriorityTick(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
}

void CPistol_Gunfire::Tick(_float fTimeDelta)
{

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));

	m_fScale = { Get_Texture_Info().x / m_fDivide , Get_Texture_Info().y / m_fDivide, 0.f };

	m_fDivide += 1.f * fTimeDelta;
	m_fLifeTime -= fTimeDelta;

	if(m_fLifeTime < 0.f)
		m_bDead = true;

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);

}

void CPistol_Gunfire::LateTick(_float fTimeDelta)
{
	//_float2 fLissajousPos = Lissajous_Curve(fTimeDelta, m_fLissajousTime, m_UiDesc.m_fX, m_UiDesc.m_fY, 1, 3, 3, 1, 2, 6);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + fLissajousPos.x, m_UiDesc.m_fY + fLissajousPos.y, 0.f));
}

HRESULT CPistol_Gunfire::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, 10);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 123);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	return S_OK;
}

void CPistol_Gunfire::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	Set_Ui_Pos(200, 0);
	Set_Divide(1.5f);

	m_fScale = { m_UiDesc.m_fSizeX / Get_Divide() , m_UiDesc.m_fSizeY / Get_Divide(), 1.f };
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 90);
	
	m_fRotation = { 0.f, 0.f, (float)(dis(gen) * (float)(m_iGunfireDir)) };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CPistol_Gunfire::Initialize_Set_Speed()
{
}

void CPistol_Gunfire::Default_Set_LifeTime()
{
	m_fLifeTime = 0.05f;
}

void CPistol_Gunfire::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 768;
	m_UiDesc.m_fSizeY = 768;
}

POINT CPistol_Gunfire::Get_Texture_Info()
{
	if (m_pTextureCom == nullptr)
		return { 0, 0 };

	return { 768 , 768 };
}

_float2 CPistol_Gunfire::Lissajous_Curve(_float _fTimeDelta, _float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta, _float _fLissajousSpeed)
{
	_fLissajousTime += _fTimeDelta * _fLissajousSpeed;

	_fPosX = _fWitth * sin(_fLagrangianX * _fLissajousTime + 3.14f / _fPhaseDelta);
	_fPosY = _fHeight * cos(_fLagrangianY * _fLissajousTime);

	return { _fPosX,_fPosY };
}

HRESULT CPistol_Gunfire::Add_Components(void* pArg)
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

HRESULT CPistol_Gunfire::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Pistol_Fire_Textures")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;

}

CPistol_Gunfire* CPistol_Gunfire::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPistol_Gunfire* pInstance = new CPistol_Gunfire(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CPistol_Gunfire Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi* CPistol_Gunfire::Clone(void* pArg)
{
	CPistol_Gunfire* pInstance = new CPistol_Gunfire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("CPistol_Gunfire Clone Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CPistol_Gunfire::Free()
{
	__super::Free();
}
