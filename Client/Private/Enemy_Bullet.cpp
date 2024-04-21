#include "Enemy_Bullet.h"

#include "GameInstance.h"
#include "PlayerManager.h"
#include "Light_Manager.h"

/*
 LightDesc.Diffuse * Texture.Pixel * MaterialDesc.Diffuse 
	 1, 1, 1, 1		   0.5, 0.2, 0.6, 1		1, 0, 0, 1
		 0, 0, 1, 1 
*/


CEnemy_Bullet::CEnemy_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CEnemy_Bullet::CEnemy_Bullet(const CEnemy_Bullet& rhs)
	: CGameObject{ rhs }
	, m_fBulletDuration(3.f)
{
}

HRESULT CEnemy_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Bullet::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	m_strTag = "Bullet";
	return S_OK;
}

void CEnemy_Bullet::PriorityTick(_float fTimeDelta)
{
}

void CEnemy_Bullet::Tick(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);
	m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());

	m_fBulletDuration -= fTimeDelta;

	if (m_fBulletDuration <= 0)
		m_bDestroyed = true;
}

void CEnemy_Bullet::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

HRESULT CEnemy_Bullet::Render()
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

void CEnemy_Bullet::OnTriggerEnter(CGameObject* pOther)
{
	if ("Player" == pOther->Get_Tag())
	{
		float fDamage = 3.f;
		//static_cast<CPlayer*>(pOther)->Hit(&fDamage);
	}

	m_bDestroyed = true;
}

HRESULT CEnemy_Bullet::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Bullet*>(__super::Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Bullet_Default"), TEXT("VIBuffer")));

	CTransform::TRANSFORM_DESC	TransformDesc{};
	TransformDesc.fSpeedPerSec = 12.f;
	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), &TransformDesc));

	m_pTextureCom = dynamic_cast<CTexture*>(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Enemy_Bullet_Texture"), TEXT("Enemy_Bullet_Texture")));

	CBoxCollider::BOXCOLLISION_DESC pDesc;
	pDesc.vScale = { 0.25f, 0.25f, 0.5f };
	pDesc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &pDesc));

	return S_OK;
}

HRESULT CEnemy_Bullet::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	return S_OK;
}

HRESULT CEnemy_Bullet::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

CEnemy_Bullet* CEnemy_Bullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnemy_Bullet* pInstance = new CEnemy_Bullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to create: CEnemy_Bullet"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Bullet::Clone(void* pArg)
{
	CEnemy_Bullet* pInstance = new CEnemy_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to create: CEnemy_Bullet"));

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Bullet::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBoxCollider);
}
