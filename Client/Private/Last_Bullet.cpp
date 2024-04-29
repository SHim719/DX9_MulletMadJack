#include "Last_Bullet.h"

#include "GameInstance.h"
#include "PlayerManager.h"
#include "Light_Manager.h"

/*
 LightDesc.Diffuse * Texture.Pixel * MaterialDesc.Diffuse
	 1, 1, 1, 1		   0.5, 0.2, 0.6, 1		1, 0, 0, 1
		 0, 0, 1, 1
*/


CLast_Bullet::CLast_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CLast_Bullet::CLast_Bullet(const CLast_Bullet& rhs)
	: CGameObject{ rhs }
	, m_fBulletDuration(3.f)
{
}

HRESULT CLast_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLast_Bullet::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	m_strTag = "Bullet";
	return S_OK;
}

void CLast_Bullet::PriorityTick(_float fTimeDelta)
{
}

void CLast_Bullet::Tick(_float fTimeDelta)
{
	if (m_pTransformCom->Get_Pos().y < 0.f) {
		CGameObject* pEffect = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Effect", L"Prototype_Explosion");
		pEffect->Get_Transform()->Set_Position(m_pTransformCom->Get_Pos());
		pEffect->Get_Transform()->Set_Scale({ 25.f, 25.f, 1.f });

		m_pGameInstance->Play(L"Drone_Explosion", false);
		m_pGameInstance->SetVolume(L"Drone_Explosion", 0.7f);
		m_bDestroyed = true;
	}

	if (CPlayer_Manager::Get_Instance()->Get_BossCutscene() == false && m_bDeath != true) {
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	else {
		if (m_bDeath == false) {

			m_fDeathDelay = CMath_Manager::Get_Instance()->Random_Float(4.f, 8.f);
			m_fDeathDelayMax = m_fDeathDelay;

			m_fDeathExplodeDelay = CMath_Manager::Get_Instance()->Random_Float(0.15f, 0.3f);
			m_fDeathExplodeDelayMax = m_fDeathExplodeDelay;
			m_bDeath = true;
		}
	}

	if (m_bDeath == true && CPlayer_Manager::Get_Instance()->Get_BossCutscene() == false)
		DeathTrigger(fTimeDelta);

	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());

	m_fBulletDuration -= fTimeDelta;

	//if (m_fBulletDuration <= 0)
		//m_bDestroyed = true;
}

void CLast_Bullet::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

HRESULT CLast_Bullet::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (m_pBoxCollider)
		m_pBoxCollider->Render();

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CLast_Bullet::DeathTrigger(_float fTimeDelta)
{
	if (m_fDeathDelay <= 0.f) {

			CGameObject* pEffect = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Effect", L"Prototype_Explosion");
			pEffect->Get_Transform()->Set_Position(m_pTransformCom->Get_Pos());
			pEffect->Get_Transform()->Set_Scale({ 25.f, 25.f, 1.f });

			m_pGameInstance->Play(L"Drone_Explosion", false);
			m_pGameInstance->SetVolume(L"Drone_Explosion", 0.7f);

			m_bDestroyed = true;

	}
	else
	{
		if (m_fDeathExplodeDelay <= 0.f)
		{
			CGameObject* pEffect = m_pGameInstance->Add_Clone(m_pGameInstance->Get_CurrentLevelID(), L"Effect", L"Prototype_Explosion");

			_float3 vPos = m_pTransformCom->Get_Pos();

			vPos.x += CMath_Manager::Get_Instance()->Random_Float(-2.f, 2.f);
			vPos.y += CMath_Manager::Get_Instance()->Random_Float(-2.f, 2.f);
			vPos.z += CMath_Manager::Get_Instance()->Random_Float(-2.f, 2.f);

			pEffect->Get_Transform()->Set_Position(vPos);
			pEffect->Get_Transform()->Set_Scale({ 6.f, 6.f, 1.f });

			m_pGameInstance->Play(L"Drone_Explosion", false);
			m_pGameInstance->SetVolume(L"Drone_Explosion", 0.2f);
			m_fDeathExplodeDelay = m_fDeathExplodeDelayMax;
		}
		else
		{
			m_fDeathExplodeDelay -= fTimeDelta;
		}

		m_pTransformCom->Set_Speed(1.f);
		m_pTransformCom->Go_Down(fTimeDelta);

		m_fDeathDelay -= fTimeDelta;
	}
}

void CLast_Bullet::OnTriggerEnter(CGameObject* pOther)
{
	if ("Player" == pOther->Get_Tag())
	{
		
		CPlayer_Manager::Get_Instance()->Get_Player()->Get_Rigidbody()->Set_Ground(true);
		CPlayer_Manager::Get_Instance()->Get_Player()->Get_Rigidbody()->Set_VelocityY(10.f);
		m_pGameInstance->Set_Ui_ActiveState(TEXT("Camera_Dash"), true);
	}

	//m_bDestroyed = true;
}

HRESULT CLast_Bullet::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Bullet*>(__super::Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Bullet_Default"), TEXT("VIBuffer")));

	CTransform::TRANSFORM_DESC	TransformDesc{};
	TransformDesc.fSpeedPerSec = 12.f;
	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), &TransformDesc));

	m_pTextureCom = dynamic_cast<CTexture*>(__super::Add_Component(LEVEL_STATIC, TEXT("Enemy_Missile_Texture"), TEXT("Enemy_Bullet_Texture")));

	CBoxCollider::BOXCOLLISION_DESC pDesc;
	pDesc.vScale = { 1.5f, 2.5f, 1.f };
	pDesc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

	return S_OK;
}

HRESULT CLast_Bullet::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	return S_OK;
}

HRESULT CLast_Bullet::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

CLast_Bullet* CLast_Bullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLast_Bullet* pInstance = new CLast_Bullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to create: CLast_Bullet"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLast_Bullet::Clone(void* pArg)
{
	CLast_Bullet* pInstance = new CLast_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to create: CLast_Bullet"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLast_Bullet::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBoxCollider);
}
