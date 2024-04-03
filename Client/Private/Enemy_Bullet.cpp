#include "Enemy_Bullet.h"

#include "GameInstance.h"

CEnemy_Bullet::CEnemy_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CEnemy_Bullet::CEnemy_Bullet(const CEnemy_Bullet& rhs)
	: CGameObject{ rhs }
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

	return S_OK;
}

void CEnemy_Bullet::PriorityTick(_float fTimeDelta)
{
}

void CEnemy_Bullet::Tick(_float fTimeDelta)
{
}

void CEnemy_Bullet::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CEnemy_Bullet::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_pAnimationCom->Get_TextureNum())))
		return E_FAIL;

	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnemy_Bullet::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(__super::Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("Com_VIBuffer")));

	CTransform::TRANSFORM_DESC	TransformDesc{};

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	m_pTransformCom = dynamic_cast<CTransform*>(__super::Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Com_Transform")));

	m_pTextureCom = dynamic_cast<CTexture*>(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Enemy_Bullet"), TEXT("Com_Texture")));
	
	m_pAnimationCom = dynamic_cast<CAnimation*>(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animation"), TEXT("Com_Animation")));

	return S_OK;
}

HRESULT CEnemy_Bullet::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CEnemy_Bullet::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

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

	Safe_Release(m_pAnimationCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
}
