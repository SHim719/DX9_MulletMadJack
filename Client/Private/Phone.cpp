#include "Phone.h"
#include "PlayerManager.h"

CPhone::CPhone(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUi(pGraphic_Device)
{
}

CPhone::CPhone(const CPhone& rhs)
	: CUi(rhs)
{
}

HRESULT CPhone::Initialize_Prototype()
{

	return S_OK;

}

HRESULT CPhone::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components(NULL))
		return E_FAIL;

	Default_Set_Size();
	Default_Set_Delay(0.05f);
	Initialize_Set_Scale_Pos_Rotation(NULL);
	Set_Texture_Index(0);

	Initialize_BackGround();
	Intialize_PhoneNumber();
	Initialize_Face();
	
	return S_OK;
}

HRESULT CPhone::Initialize_Active()
{
	Set_Texture_Index(0);
	Default_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(NULL);

	return S_OK;
}

void CPhone::PriorityTick(_float fTimeDelta)
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
}

void CPhone::Tick(_float fTimeDelta)
{



	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));

	if (AnimationDelay(fTimeDelta) < 0.f && m_iTexture_Index <= m_pTextureCom->Get_MaxTextureNum()) {
		m_iTexture_Index++;
		AnimationDelayReset();
	}

	//Animation End
	if (m_iTexture_Index > m_pTextureCom->Get_MaxTextureNum()) {
		m_iTexture_Index = 0;
		AnimationDelayReset();
	}

	m_fScale = { Get_Texture_Info().x / m_fDivide , Get_Texture_Info().y / m_fDivide, 1.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);
	

	// kimminheok delete this!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// delete membervariable m_fTestDelta too and 
	// Synchronize_PlayerLife_Number fuction qualification change
	m_fTestDelta += fTimeDelta;
}

void CPhone::LateTick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->GetKey(eKeyCode::LShift)) {
		//Temp Rotate
		m_pTransformCom->Rotation_XYZ({ 0.f, 0.f, -40.f });
		_float2 fOffSet = { -100, 150 };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + fOffSet.x, m_UiDesc.m_fY + fOffSet.y, 0.f));
	}
	else {
	_float2 fLissajousPos = Lissajous_Curve(fTimeDelta, m_fLissajousTime, m_UiDesc.m_fX, m_UiDesc.m_fY, 1, 3, 3, 1, 2, 6);
	_float2 fLissajousRun = CPlayer_Manager::Get_Instance()->Get_Lissajous_Run_Phone(m_UiDesc.m_fX, m_UiDesc.m_fY);

	if (CPlayer_Manager::Get_Instance()->Get_Player_MoveState() == CPlayer::MOVE_STATE::MOVE)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + fLissajousPos.x + fLissajousRun.x, m_UiDesc.m_fY + fLissajousPos.y + fLissajousRun.y, 0.f));
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX + fLissajousPos.x, m_UiDesc.m_fY + fLissajousPos.y, 0.f));
	}
	}
		Set_BackGround();
		Set_PhoneNumber();
		Set_Face(fTimeDelta);
}

HRESULT CPhone::Render()
{

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Render_BackGround()))
		return E_FAIL;

	if (FAILED(Render_Number()))
		return E_FAIL;

	if (FAILED(Render_Face()))
		return E_FAIL;

	return S_OK;
}

void CPhone::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	Set_Ui_Pos(-450, -360);
	Set_Divide(2.3f);

	m_fScale = { m_UiDesc.m_fSizeX / Get_Divide() , m_UiDesc.m_fSizeY / Get_Divide(), 1.f };
	m_fRotation = { 0.f, 0.f, -20.f };

	m_pTransformCom->Set_Scale(m_fScale);
	m_pTransformCom->Rotation_XYZ(m_fRotation);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CPhone::Initialize_Set_Speed()
{
}

void CPhone::Default_Set_LifeTime()
{
}

void CPhone::Default_Set_Size()
{
	m_UiDesc.m_fSizeX = 2048;
	m_UiDesc.m_fSizeY = 2048;
}

POINT CPhone::Get_Texture_Info()
{
	if (m_pTextureCom == nullptr)
		return { 0, 0 };

	return { 2048, 2048 };
}

_float2 CPhone::Lissajous_Curve(_float _fTimeDelta, _float& _fLissajousTime, _float _fPosX, _float _fPosY, _float _fWitth, _float _fHeight, _float _fLagrangianX, _float _fLagrangianY, _float _fPhaseDelta, _float _fLissajousSpeed)
{
	_fLissajousTime += _fTimeDelta * _fLissajousSpeed;

	_fPosX = _fWitth * sin(_fLagrangianX * _fLissajousTime + 3.14f / _fPhaseDelta);
	_fPosY = _fHeight * cos(_fLagrangianY * _fLissajousTime);

	return { _fPosX,_fPosY };
}

HRESULT CPhone::Add_Components(void* pArg)
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

	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pFirstNumVIBufferCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pFirstNumTransformCom)))
		return E_FAIL;

	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pSecondNumVIBufferCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pSecondNumTransformCom)))
		return E_FAIL;

	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pFaceVIBufferCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pFaceTransformCom)))
		return E_FAIL;

	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pBackGroundVIBufferCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pBackGroundTransformCom)))
		return E_FAIL;

	if (FAILED(Add_Texture(pArg)))
		return E_FAIL;

	return S_OK;
}


HRESULT CPhone::Add_Texture(void* pArg)
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Phone_Textures")
		, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Phone_Number_Textures")
		, (CComponent**)&m_pFirstNumTextureCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Phone_Number_Textures")
		, (CComponent**)&m_pSecondNumTextureCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Phone_Face_Textures")
		, (CComponent**)&m_pFaceTextureCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Phone_BackGround_Textures")
		, (CComponent**)&m_pBackGroundTextureCom)))
		return E_FAIL;

	return S_OK;

}

CUi* CPhone::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPhone* pInstance = new CPhone(pGraphic_Device);

	if (FAILED(pInstance->Initialize(NULL)))
	{
		MSG_BOX(TEXT("Failed to Created : CPhone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CPhone::Intialize_PhoneNumber()
{
	Default_Set_PhoneSize();
	Set_PhonePos();
	Set_PhoneRotation();
	_float3 FirstScale = { m_FirstNumDesc.m_fSizeX, m_FirstNumDesc.m_fSizeY, 1 };
	m_pFirstNumTransformCom->Set_Scale(FirstScale);
	_float3 SecondScale = { m_SecondNumDesc.m_fSizeX, m_SecondNumDesc.m_fSizeY, 1 };
	m_pSecondNumTransformCom->Set_Scale(SecondScale);
}

void CPhone::Default_Set_PhoneSize()
{
	m_FirstNumDesc.m_fSizeX = 50;
	m_FirstNumDesc.m_fSizeY = 100;
	m_SecondNumDesc.m_fSizeX = 50;
	m_SecondNumDesc.m_fSizeY = 100;
}

void CPhone::Set_PhoneNumber()
{
	Set_PhonePos();
	Set_PhoneRotation();
	Synchronize_PlayerLife_Number();
}

void CPhone::Set_PhonePos()
{
	_float3 FirstNumpos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	FirstNumpos.x += 140;
	FirstNumpos.y += 125;
	m_pFirstNumTransformCom->Set_Position(FirstNumpos);

	_float3 SecondNumpos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	SecondNumpos.x += 185;
	SecondNumpos.y += 137;
	m_pSecondNumTransformCom->Set_Position(SecondNumpos);
}

void CPhone::Set_PhoneRotation()
{
	_float3 Rotation = { m_fRotation.x, m_fRotation.y, m_fRotation.z + 35 };
	
	m_pFirstNumTransformCom->Rotation_XYZ(Rotation);
	m_pSecondNumTransformCom->Rotation_XYZ(Rotation);
}

void CPhone::Synchronize_PlayerLife_Number()
{

	m_fLifeTime = (_uint)m_fTestDelta;
	//m_fLifeTime = Get_Player_Life and change > -> <
	if (m_fLifeTime > 15)
	{
		m_fLifeTime = 15;
	}

	m_iFirstNumTextureNum = m_fLifeTime / 10;
	m_iSecondNumTextureNum = m_fLifeTime - (m_iFirstNumTextureNum * 10);
}

HRESULT CPhone::Render_Number()
{

	if (FAILED(m_pFirstNumTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pFirstNumTextureCom->Bind_Texture(m_iFirstNumTextureNum)))
		return E_FAIL;

	m_pFirstNumVIBufferCom->Render();

	if (FAILED(m_pSecondNumTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pSecondNumTextureCom->Bind_Texture(m_iSecondNumTextureNum)))
		return E_FAIL;

	m_pSecondNumVIBufferCom->Render();
	
	return S_OK;
}


void CPhone::Initialize_Face()
{
	Default_Set_FaceSize();
	Set_FacePos();
	Set_FaceRotation();
	_float3 FaceScale = { m_Face.m_fSizeX, m_Face.m_fSizeY, 1 };
	m_pFaceTransformCom->Set_Scale(FaceScale);
}

void CPhone::Default_Set_FaceSize()
{
	m_Face.m_fSizeX = 105;
	m_Face.m_fSizeY = 105;
}

void CPhone::Set_Face(_float fTimeDelta)
{
	m_fFaceChangeTime += fTimeDelta;
	Set_FacePos();
	Set_FaceRotation();
	if (m_fFaceChangeTime > 3.f)
	{
		m_fFaceChangeTime = _float(rand()%3 + 1);
		Set_FaceTexture();
	}
}

void CPhone::Set_FacePos()
{
	_float3 Facepos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Facepos.x += 130;
	Facepos.y += 235;
	m_pFaceTransformCom->Set_Position(Facepos);
}

void CPhone::Set_FaceRotation()
{
	_float3 FaceRotation = { m_fRotation.x, m_fRotation.y, m_fRotation.z + 35 };

	m_pFaceTransformCom->Rotation_XYZ(FaceRotation);
}

void CPhone::Set_FaceTexture()
{
	if (m_fLifeTime > 7)
	{
		m_iFaceTexture = rand() % 3;
	}
	else
	{
		m_iFaceTexture = rand() % 3 + 3;
	}
}

HRESULT CPhone::Render_Face()
{
	if (FAILED(m_pFaceTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pFaceTextureCom->Bind_Texture(m_iFaceTexture)))
			return E_FAIL;

	m_pFaceVIBufferCom->Render();

	return S_OK;
}

void CPhone::Initialize_BackGround()
{
	Default_Set_BackGroundSize();
	Set_BackGroundPos();
	Set_BackGroundRotation();
	_float3 BackGroundScale = { m_BackGround.m_fSizeX, m_BackGround.m_fSizeY, 1 };
	m_pBackGroundTransformCom->Set_Scale(BackGroundScale);
}

void CPhone::Default_Set_BackGroundSize()
{
	m_BackGround.m_fSizeX = 120;
	m_BackGround.m_fSizeY = 111;
}

void CPhone::Set_BackGround()
{
	Set_BackGroundPos();
	Set_BackGroundRotation();
	Set_BackGroundTexture();
}

void CPhone::Set_BackGroundPos()
{
	_float3 BackGroundpos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	BackGroundpos.x += 164;
	BackGroundpos.y += 132;
	m_pBackGroundTransformCom->Set_Position(BackGroundpos);
}

void CPhone::Set_BackGroundRotation()
{
	_float3 Rotation = { m_fRotation.x, m_fRotation.y, m_fRotation.z + 35 };

	m_pBackGroundTransformCom->Rotation_XYZ(Rotation);
}

void CPhone::Set_BackGroundTexture()
{
	if (m_fLifeTime <= 5)
	{
		m_iBackGroundTexture = 2;
	}
	else if (m_fLifeTime <= 10)
	{
		m_iBackGroundTexture = 1;
	}
	else
	{
		m_iBackGroundTexture = 0;
	}
}

HRESULT CPhone::Render_BackGround()
{
	if (FAILED(m_pBackGroundTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pBackGroundTextureCom->Bind_Texture(m_iBackGroundTexture)))
		return E_FAIL;

	m_pBackGroundVIBufferCom->Render();

	return S_OK;
}

void CPhone::Free()
{
	__super::Free();

	Safe_Release(m_pFirstNumTextureCom);
	Safe_Release(m_pFirstNumTransformCom);
	Safe_Release(m_pFirstNumVIBufferCom);

	Safe_Release(m_pSecondNumTextureCom);
	Safe_Release(m_pSecondNumTransformCom);
	Safe_Release(m_pSecondNumVIBufferCom);

	Safe_Release(m_pFaceTextureCom);
	Safe_Release(m_pFaceTransformCom);
	Safe_Release(m_pFaceVIBufferCom);

	Safe_Release(m_pBackGroundTextureCom);
	Safe_Release(m_pBackGroundTransformCom);
	Safe_Release(m_pBackGroundVIBufferCom);
}