#include "TrackingLaser.h"


CTrackingLaser::CTrackingLaser(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CTrackingLaser::CTrackingLaser(const CTrackingLaser& rhs)
	:CGameObject(rhs)
{
}

HRESULT CTrackingLaser::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrackingLaser::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Add_Texture()))
		return E_FAIL;

	Initialize_Arg(pArg);

	Initialize_Order();

	m_pAnimationCom->Play_Animation(TEXT("TrackingLaser"), 0.1f, true);

	m_eState = TrackingLaserState::Warning;

	m_strTag = "TrackingLaser";

	return S_OK;
}

void CTrackingLaser::PriorityTick(_float fTimeDelta)
{
	Prepair_Order(fTimeDelta);
}

void CTrackingLaser::Tick(_float fTimeDelta)
{
	Execute_Order();

	m_fLife -= fTimeDelta;

	if (m_fLife < 0 || m_pTransformCom->Get_Pos().y < 0.f)
	{
		_bool bExplode = true;
		CGameObject* pObj = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"LandMine", L"Prototype_LandMine",&bExplode);

		switch (m_eAttackOrder)
		{
			case CBeholder::PLAYERTRACKING:
				pObj->Get_Transform()->Set_State(CTransform::STATE_POSITION, &m_vTargetPos);
				break;
			case CBeholder::FREETRACKING:
				pObj->Get_Transform()->Set_State(CTransform::STATE_POSITION, &m_vLissajousPos);
				break;
			case CBeholder::LANDMINESET:
				pObj->Get_Transform()->Set_State(CTransform::STATE_POSITION, &m_vLissajousPos);
				break;
			case CBeholder::AIRSTRIKE:
				pObj->Get_Transform()->Set_State(CTransform::STATE_POSITION, &m_pTransformCom->Get_Pos());
				break;
			case CBeholder::AIRSTRIKETRACKING:
				AirStrike();
				break;
			case CBeholder::MONSTERSPAWN:
				MonsterSpawn();
				break;
			default:
				break;
		}

		m_bDestroyed = true;
	}
	AdjustAlpha(fTimeDelta);

}

void CTrackingLaser::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

void CTrackingLaser::BeginRenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (m_eState == TrackingLaserState::Warning)
	{
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);


		_int fAlpha = CMath_Manager::Get_Instance()->Random_Int(100, 200);
		switch (m_eAttackOrder)
		{
			case CBeholder::PLAYERTRACKING:
				m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255, 0, 0, fAlpha));
				break;
			case CBeholder::FREETRACKING: {
				_int fRed = CMath_Manager::Get_Instance()->Random_Int(0, 255);
				_int fGreen = CMath_Manager::Get_Instance()->Random_Int(0, 255);
				_int fBlue = CMath_Manager::Get_Instance()->Random_Int(0, 255);
				m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(fRed, fGreen, fBlue, fAlpha));
				break;
			}
			case CBeholder::LANDMINESET: {
				_int fRed = CMath_Manager::Get_Instance()->Random_Int(120, 240);
				m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(fRed, 0, 0, fAlpha));
				break;
			}
			case CBeholder::AIRSTRIKE: {
				_int fRed = CMath_Manager::Get_Instance()->Random_Int(0, 255);
				_int fGreen = CMath_Manager::Get_Instance()->Random_Int(0, 255);
				_int fBlue = CMath_Manager::Get_Instance()->Random_Int(0, 255);
				m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(fRed, fGreen, fBlue, fAlpha));
				break;
			}
			case CBeholder::AIRSTRIKETRACKING: {
				_int fRed = CMath_Manager::Get_Instance()->Random_Int(100, 255);\
				m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(fRed, 0, 0, fAlpha));
				break;
			}
			case CBeholder::MONSTERSPAWN: {
				_int fRed = CMath_Manager::Get_Instance()->Random_Int(0, 255);
				_int fGreen = CMath_Manager::Get_Instance()->Random_Int(0, 255);
				_int fBlue = CMath_Manager::Get_Instance()->Random_Int(0, 255);
				m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(fRed, fGreen, fBlue, fAlpha));
				break;
			}
			default:
				m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255, 255, 255, 255));
				break;

		}
	}
	else
	{
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR,
			D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}

HRESULT CTrackingLaser::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pAnimationCom->Render();

	BeginRenderState();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (m_pBoxCollider)
		m_pBoxCollider->Render();

	EndRenderState();

	return S_OK;
}

void CTrackingLaser::EndRenderState()
{
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CTrackingLaser::OnTriggerEnter(CGameObject* pOther)
{

}

void CTrackingLaser::Prepair_Order(float _fTimeDelta)
{
	switch (m_eAttackOrder)
	{
	case CBeholder::ENDORDER:
		return;
	case CBeholder::PLAYERTRACKING:
		PlayerTracking_Prepair();
		return;
	case CBeholder::FREETRACKING:
		FreeTracking_Prepair(_fTimeDelta);
		return;
	case CBeholder::LANDMINESET:
		LandMineSet_Prepair(_fTimeDelta);
		return;
	case CBeholder::AIRSTRIKE:
		AirStrike_Prepair(_fTimeDelta);
		return;
	case CBeholder::AIRSTRIKETRACKING:
		TrackingAirStrike_Prepair(_fTimeDelta);
		return;
	case CBeholder::MONSTERSPAWN:
		MonsterSpawn_Prepair(_fTimeDelta);
		return;
	default:
		return;
	}

}

void CTrackingLaser::Execute_Order()
{
	switch (m_eAttackOrder)
	{
		case CBeholder::ENDORDER:
			return;
		case CBeholder::PLAYERTRACKING:
			PlayerTracking_Order();
			return;
		case CBeholder::FREETRACKING:
			FreeTracking_Order();
			return;
		case CBeholder::LANDMINESET:
			LandMineSet_Order();
			return;
		case CBeholder::AIRSTRIKE:
			AirStrike_Order();
			return;
		case CBeholder::AIRSTRIKETRACKING:
			TrackingAirStrike_Order();
			return;
		case CBeholder::MONSTERSPAWN:
			MonsterSpawn_Order();
			return;
		default:
			return;
	}
}

void CTrackingLaser::PlayerTracking_Order()
{
	_float3 vDir = m_vTargetPos + m_vMasterPos;
	
	m_pTransformCom->Set_Position(vDir/2);


	//x¶û z Ãà acos
	
	_float3 vAngleATAN = m_vMasterPos - m_vTargetPos;
	_float3 vAngleATANfabs = { fabs(vAngleATAN.x), fabs(vAngleATAN.y), fabs(vAngleATAN.z) };
	

	_float fSide = (_float)sqrt(pow(vAngleATANfabs.x,2) + pow(vAngleATANfabs.z,2));

	_float fAngleY = atan2(vAngleATAN.x, vAngleATAN.z);
	_float fAngleX = atan2(fSide, vAngleATANfabs.y);

	fAngleY = D3DXToDegree(fAngleY);
	fAngleX = D3DXToDegree(fAngleX);

	m_pTransformCom->Rotation_XYZ({ fAngleX, fAngleY, 0.f });
	//m_pTransformCom->Set_Scale({ 1.f, 10.f, 1.f});

	_float fLength = (_float)sqrt(pow(fabs(m_vTargetPos.x - m_vMasterPos.x), 2) + pow(fabs(m_vTargetPos.y - m_vMasterPos.y), 2) + pow(fabs(m_vTargetPos.z - m_vMasterPos.z), 2));

	m_pTransformCom->Set_Scale({ 0.1f,fLength, 0.1f });
}

void CTrackingLaser::PlayerTracking_Prepair()
{
	if (m_eState == TrackingLaserState::Warning) {
		m_vTargetPos = m_pTarget->Get_Transform()->Get_Pos();
		m_vTargetPos.y -= 0.5f;
	}
}

void CTrackingLaser::FreeTracking_Order()
{
	_float3 vDir = m_vLissajousPos + m_vMasterPos;

	m_pTransformCom->Set_Position(vDir / 2);

	//x¶û z Ãà acos

	_float3 vAngleATAN = m_vMasterPos - m_vLissajousPos;
	_float3 vAngleATANfabs = { fabs(vAngleATAN.x), fabs(vAngleATAN.y), fabs(vAngleATAN.z) };


	_float fSide = (_float)sqrt(pow(vAngleATANfabs.x, 2) + pow(vAngleATANfabs.z, 2));

	_float fAngleY = atan2(vAngleATAN.x, vAngleATAN.z);
	_float fAngleX = atan2(fSide, vAngleATANfabs.y);

	fAngleY = D3DXToDegree(fAngleY);
	fAngleX = D3DXToDegree(fAngleX);

	m_pTransformCom->Rotation_XYZ({ fAngleX, fAngleY, 0.f });
	//m_pTransformCom->Set_Scale({ 1.f, 10.f, 1.f});

	_float fLength = (_float)sqrt(pow(fabs(m_vLissajousPos.x - m_vMasterPos.x), 2) + pow(fabs(m_vLissajousPos.y - m_vMasterPos.y), 2) + pow(fabs(m_vLissajousPos.z - m_vMasterPos.z), 2));

	m_pTransformCom->Set_Scale({ 0.1f,fLength , 0.1f });
}

void CTrackingLaser::FreeTracking_Prepair(float _fTimeDelta)
{
	if (m_eState == TrackingLaserState::Warning) {
		_float2 fLissajousFloat = CMath_Manager::Get_Instance()->Lissajous_Curve(_fTimeDelta, m_fLissajousTime, 10, 10, m_fLissajousLagrangianX, m_fLissajousLagrangianY, m_fLissajousPhaseDelta, m_fLissajousSpeed);

		m_vLissajousPos = { m_vTargetPos.x + fLissajousFloat.x, 0.f, m_vTargetPos.z + fLissajousFloat.y };
	}
}

void CTrackingLaser::LandMineSet_Order()
{
	_float3 vDir = m_vLissajousPos + m_vMasterPos;

	m_pTransformCom->Set_Position(vDir / 2);

	//x¶û z Ãà acos

	_float3 vAngleATAN = m_vMasterPos - m_vLissajousPos;
	_float3 vAngleATANfabs = { fabs(vAngleATAN.x), fabs(vAngleATAN.y), fabs(vAngleATAN.z) };


	_float fSide = (_float)sqrt(pow(vAngleATANfabs.x, 2) + pow(vAngleATANfabs.z, 2));

	_float fAngleY = atan2(vAngleATAN.x, vAngleATAN.z);
	_float fAngleX = atan2(fSide, vAngleATANfabs.y);

	fAngleY = D3DXToDegree(fAngleY);
	fAngleX = D3DXToDegree(fAngleX);

	m_pTransformCom->Rotation_XYZ({ fAngleX, fAngleY, 0.f });
	//m_pTransformCom->Set_Scale({ 1.f, 10.f, 1.f});

	_float fLength = (_float)sqrt(pow(fabs(m_vLissajousPos.x - m_vMasterPos.x), 2) + pow(fabs(m_vLissajousPos.y - m_vMasterPos.y), 2) + pow(fabs(m_vLissajousPos.z - m_vMasterPos.z), 2));

	m_pTransformCom->Set_Scale({ 0.1f,fLength , 0.1f });

	if (m_fLandMineCoolTime <= 0.f)
	{
		CGameObject* pObj = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"LandMine", L"Prototype_LandMine");
		pObj->Get_Transform()->Set_State(CTransform::STATE_POSITION, &m_vLissajousPos);
		m_fLandMineCoolTime = m_fLandMineCoolTimeMax;
	}
}

void CTrackingLaser::LandMineSet_Prepair(float _fTimeDelta)
{
	if (m_eState == TrackingLaserState::Warning) {
		_float2 fLissajousFloat = CMath_Manager::Get_Instance()->Lissajous_Curve(_fTimeDelta, m_fLissajousTime, 10, 10, m_fLissajousLagrangianX, m_fLissajousLagrangianY, m_fLissajousPhaseDelta, m_fLissajousSpeed);

		m_vLissajousPos = { m_vTargetPos.x + fLissajousFloat.x, 0.f, m_vTargetPos.z + fLissajousFloat.y };
	}

	m_fLandMineCoolTime -= _fTimeDelta;
}

void CTrackingLaser::AirStrike_Order()
{

	_float fLength = (_float)sqrt(pow(fabs(m_vTargetPos.x - m_vMasterPos.x), 2) + pow(fabs(m_vTargetPos.y - m_vMasterPos.y), 2) + pow(fabs(m_vTargetPos.z - m_vMasterPos.z), 2));

	m_pTransformCom->Set_Scale({ 0.3f,fLength, 0.3f });
}

void CTrackingLaser::AirStrike_Prepair(float _fTimeDelta)
{
	/*m_pTransformCom->Set_Speed(2.f);
	m_pTransformCom->Go_Down(_fTimeDelta);*/

	_float3 vPos = m_pTransformCom->Get_Pos();
	m_pTransformCom->Set_PosY(vPos.y - 30.f* _fTimeDelta);

}

void CTrackingLaser::TrackingAirStrike_Order()
{
	_float3 vDir = m_vTargetPos + m_vMasterPos;

	m_pTransformCom->Set_Position(vDir / 2);


	//x¶û z Ãà acos

	//_float3 vAngleATAN = m_vMasterPos - m_vTargetPos;
	//_float3 vAngleATANfabs = { fabs(vAngleATAN.x), fabs(vAngleATAN.y), fabs(vAngleATAN.z) };


	//_float fSide = (_float)sqrt(pow(vAngleATANfabs.x, 2) + pow(vAngleATANfabs.z, 2));

	//_float fAngleY = atan2(vAngleATAN.x, vAngleATAN.z);
	//_float fAngleX = atan2(fSide, vAngleATANfabs.y);

	//fAngleY = D3DXToDegree(fAngleY);
	//fAngleX = D3DXToDegree(fAngleX);

	//m_pTransformCom->Rotation_XYZ({ fAngleX, fAngleY, 0.f });
	//m_pTransformCom->Set_Scale({ 1.f, 10.f, 1.f});

	_float fLength = (_float)sqrt(pow(fabs(m_vTargetPos.x - m_vMasterPos.x), 2) + pow(fabs(m_vTargetPos.y - m_vMasterPos.y), 2) + pow(fabs(m_vTargetPos.z - m_vMasterPos.z), 2));
	m_pTransformCom->Set_Scale({ 1.5f,fLength, 1.5f });
}

void CTrackingLaser::TrackingAirStrike_Prepair(float _fTimeDelta)
{
	_float3 vTargetPos = m_pTarget->Get_Transform()->Get_Pos();
	vTargetPos.y = 15.f;
	
	if (m_eState == TrackingLaserState::Warning) 
		m_vMasterPos = vTargetPos;

	vTargetPos.y = -5.f;
	if (m_eState == TrackingLaserState::Warning)
		m_vTargetPos = vTargetPos;
	
	m_pTransformCom->Rotation_XYZ({ 0.f, m_fRotate, 0.f });
	m_fRotate += _fTimeDelta * 100.f;
}

void CTrackingLaser::MonsterSpawn_Order()
{
	_float3 vDir = m_vTargetPos + m_vMasterPos;

	m_pTransformCom->Set_Position(vDir / 2);

	_float fLength = (_float)sqrt(pow(fabs(m_vTargetPos.x - m_vMasterPos.x), 2) + pow(fabs(m_vTargetPos.y - m_vMasterPos.y), 2) + pow(fabs(m_vTargetPos.z - m_vMasterPos.z), 2));
	m_pTransformCom->Set_Scale({ 1.5f,fLength, 1.5f });
}

void CTrackingLaser::MonsterSpawn_Prepair(float _fTimeDelta)
{

	m_pTransformCom->Rotation_XYZ({ 0.f, m_fRotate, 0.f });
	m_fRotate += _fTimeDelta * 100.f;
}

void CTrackingLaser::AirStrike()
{
	_float3 vTargetPos = CPlayer_Manager::Get_Instance()->Get_Player()->Get_Transform()->Get_Pos();
	AirStrikeBoom(vTargetPos);
}

void CTrackingLaser::AirStrikeBoom(_float3 vPos)
{

	for (int i = 0; i < 15; ++i) {
		_float3 vTargetPos = vPos;
		_float3 vLookPos = { 0.f, 0.f, 0.f };
		_float fRandomX = CMath_Manager::Get_Instance()->Random_Float(-1.f, 1.f);
		_float fRandomY = CMath_Manager::Get_Instance()->Random_Float(1, 20);
		_float fRandomZ = CMath_Manager::Get_Instance()->Random_Float(-1.f, 1.f);

		vTargetPos += { fRandomX, 10.f + fRandomY, fRandomZ };
		vLookPos = { vTargetPos.x, 5.f + fRandomY, vTargetPos.z };

		CBeholder::BeholderAttackOrder LaserOrder;
		LaserOrder.eOrder = CBeholder::AIRSTRIKE;
		LaserOrder.vMasterPos = vTargetPos;
		LaserOrder.vLook = vLookPos;

		CGameObject* pLaser = m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"Laser", L"Prototype_TrackingLaser", &LaserOrder);
	}
}

void CTrackingLaser::MonsterSpawn()
{
	for (int i = 0; i < 5; ++i)
	{
		int iRandMonster = CMath_Manager::Get_Instance()->Random_Int(0,2);
		wstring wstrMonsterName = L"";
		switch (iRandMonster) {
			case 0:
				wstrMonsterName = L"Prototype_White_Suit";
				break;
			case 1:
				wstrMonsterName = L"Prototype_Chainsaw";
				break;
			case 2:
				wstrMonsterName = L"Prototype_Drone";
				break;
		}


		CPawn* pPawn = static_cast<CPawn*>(m_pGameInstance->Add_Clone(LEVEL_BOSS, L"Monster", wstrMonsterName));
		pPawn->SetState_Air();

		_float3 vPos = m_vTargetPos;
		vPos.y += 5.f;
		vPos.x += CMath_Manager::Get_Instance()->Random_Float(-2.f, 2.f);
		vPos.z += CMath_Manager::Get_Instance()->Random_Float(-2.f, 2.f);

		pPawn->Get_Transform()->Set_Pos(vPos);
		static_cast<CBoxCollider*>(pPawn->Find_Component(L"Collider"))->Update_BoxCollider(pPawn->Get_Transform()->Get_WorldMatrix());
	}
}

HRESULT CTrackingLaser::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Box*>(__super::Add_Component
	(LEVEL_STATIC, TEXT("VIBuffer_Box_Default"), TEXT("VIBuffer")));

	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component
	(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform")));

	m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component
	(LEVEL_STATIC, TEXT("Animation_Default"), TEXT("Animation"), this));

	Set_Collider();
	return S_OK;
}

HRESULT CTrackingLaser::Add_Texture()
{
	if (FAILED(m_pAnimationCom->Insert_Textures(LEVEL_BOSS, TEXT("Texture_TrackingLaser"), TEXT("TrackingLaser"))))
		return E_FAIL;

	return S_OK;
}

void CTrackingLaser::Initialize_Arg(void* pArg)
{
	CBeholder::BeholderAttackOrder* pOrder = (CBeholder::BeholderAttackOrder*)pArg;

	m_eAttackOrder = pOrder->eOrder;

	Arg_InitializeSetPosScale();
	switch (m_eAttackOrder)
	{
		case CBeholder::PLAYERTRACKING:
			m_vMasterPos = pOrder->vMasterPos;
			m_vMasterPos.y -= 0.2f;
			break;
		case CBeholder::FREETRACKING:
			m_vMasterPos = pOrder->vMasterPos;
			m_vTargetPos = pOrder->vLook;

			m_fLissajousLagrangianX = (float)CMath_Manager::Get_Instance()->Random_Int(1, 5);
			m_fLissajousLagrangianY = (float)CMath_Manager::Get_Instance()->Random_Int(1, 5);
			m_fLissajousPhaseDelta =  (float)CMath_Manager::Get_Instance()->Random_Int(1, 4);
			m_fLissajousSpeed =				 CMath_Manager::Get_Instance()->Random_Float(0.5, 1);
			break;
		case CBeholder::LANDMINESET:
			m_vMasterPos = pOrder->vMasterPos;
			m_vTargetPos = pOrder->vLook;

			m_fLissajousLagrangianX = (float)CMath_Manager::Get_Instance()->Random_Int(1, 5);
			m_fLissajousLagrangianY = (float)CMath_Manager::Get_Instance()->Random_Int(1, 5);
			m_fLissajousPhaseDelta =  (float)CMath_Manager::Get_Instance()->Random_Int(1, 4);
			m_fLissajousSpeed =				 CMath_Manager::Get_Instance()->Random_Float(0.5, 1);
			break;
		case CBeholder::AIRSTRIKE: {
			m_vMasterPos = pOrder->vMasterPos;
			m_vTargetPos = pOrder->vLook;

			_float3 vDir = m_vTargetPos + m_vMasterPos;
			m_pTransformCom->Set_Position(vDir / 2);
			break;
		}
		case CBeholder::AIRSTRIKETRACKING:
			break;
		case CBeholder::MONSTERSPAWN: {
			m_vMasterPos = pOrder->vMasterPos;
			m_vTargetPos = pOrder->vLook;

			_float3 vDir = m_vTargetPos + m_vMasterPos;
			m_pTransformCom->Set_Position(vDir / 2);
			break;
		}
		default:
			break;
	}
}

void CTrackingLaser::Initialize_Order()
{
	switch(m_eAttackOrder)
	{
		case CBeholder::ENDORDER:
			return;
		case CBeholder::PLAYERTRACKING:
			m_pTarget = CPlayer_Manager::Get_Instance()->Get_Player();
			if(m_eState == TrackingLaserState::Warning)
				m_vTargetPos = m_pTarget->Get_Transform()->Get_Pos();
			return;
		case CBeholder::FREETRACKING:
			return;
		case CBeholder::AIRSTRIKETRACKING:
			m_pTarget = CPlayer_Manager::Get_Instance()->Get_Player();
			return;
		default:
			return;
	}
	return;
}

void CTrackingLaser::Arg_InitializeSetPosScale()
{
	_float3 Scale{};
	_float3 Pos{};
	Scale = { 0.1f, 10.f, 0.1f };
	Pos = { 0.f, 0.f, 10.f};
	m_pTransformCom->Set_Scale(Scale);
	m_pTransformCom->Set_Position(Pos);
}

void CTrackingLaser::Set_Collider()
{
	CBoxCollider::BOXCOLLISION_DESC pDesc;
	pDesc.vScale = { 0.f, 0.f, 0.f };
	pDesc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component
	(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));
}

void CTrackingLaser::AdjustAlpha(_float fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;
	if (m_fLife > 0.5)
	{
		m_eState = TrackingLaserState::Warning;
		if (m_fAlphaTime > 0.05)
		{
			m_fAlphaTime = 0;
			if (m_iAlpha >= 235)
			{
				m_iAlpha = 255;
			}
			else
				m_iAlpha += 5;
		}
	}
	else if (m_fLife > 0.2)
	{
		m_eState = TrackingLaserState::Fire;
		m_iAlpha = 255;
	}
	else if (m_fLife <= 0.1)
	{
		m_eState = TrackingLaserState::End;
		if (m_fAlphaTime > 0.08)
		{
			m_fAlphaTime = 0;
			if (m_iAlpha <= 50)
			{
				m_iAlpha = 0;
			}
			else
				m_iAlpha -= 50;
		}
	}
}

void CTrackingLaser::AdjustScale(_float fTimeDelta)
{
	//if (m_eState == TrackingLaserState::Warning)
	//{
	//	m_fScaleTime += fTimeDelta;

	//	//if (m_fScaleTime > 0.1)
	//	//{
	//	//	m_fScaleTime = 0.f;
	//	//	m_fScale.x += 1;
	//	//	m_pTransformCom->Set_Scale(m_fScale);
	//	//	//_float3 Pos = m_pTransformCom->Get_Pos();
	//	//	//Pos.x -= 0.5;
	//	//	//m_pTransformCom->Set_PosX(Pos.x);
	//	//}

	//	if (m_fScaleTime > 0.1)
	//	{
	//		m_fScaleTime = 0.f;
	//		m_fScale.y += 1;
	//		m_pTransformCom->Set_Scale(m_fScale);
	//		//_float3 Pos = m_pTransformCom->Get_Pos();
	//		//Pos.y += 0.5;
	//		//m_pTransformCom->Set_PosY(Pos.y);
	//	}

	//	//if (m_fScaleTime > 0.1)
	//	//{
	//	//	m_fScaleTime = 0.f;
	//	//	m_fScale.z += 1;
	//	//	m_pTransformCom->Set_Scale(m_fScale);
	//	//	_float3 Pos = m_pTransformCom->Get_Pos();
	//	//	Pos.z -= 0.5;
	//	//	m_pTransformCom->Set_PosZ(Pos.z);
	//	//}
	//}
}

CTrackingLaser* CTrackingLaser::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrackingLaser* pInstance = new CTrackingLaser(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to create: CTrackingLaser"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrackingLaser::Clone(void* pArg)
{
	CTrackingLaser* pInstance = new CTrackingLaser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to create clone: CTrackingLaser"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrackingLaser::Free()
{
	__super::Free();

	Safe_Release(m_pAnimationCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBoxCollider);
}
