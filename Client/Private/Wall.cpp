#include "Wall.h"

#include "GameInstance.h"


CWall::CWall(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CWall::CWall(const CWall& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CWall::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWall::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	if (nullptr != pArg)
		m_iTexture_Index = *(_uint*)pArg;

	m_strTag = "Wall";

	return S_OK;
}

void CWall::PriorityTick(_float fTimeDelta)
{
}

void CWall::Tick(_float fTimeDelta)
{
	//m_pBoxCollider->Update_BoxCollider(m_pTransformCom->Get_WorldMatrix());
}

void CWall::LateTick(_float fTimeDelta)
{
	if (m_pGameInstance->In_WorldFrustum(m_pTransformCom->Get_Pos(), 5.f))
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CWall::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	m_pBoxCollider->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	return S_OK;
}

HRESULT CWall::Add_Components()
{
	m_pTransformCom = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_RectX*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_RectX_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Wall_Textures"), TEXT("Wall_Textures"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	CBoxCollider::BOXCOLLISION_DESC desc;
	desc.vScale = { 1.f, 1.f, 1.f };
	desc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &desc));
	if (nullptr == m_pBoxCollider)
		return E_FAIL;


	return S_OK;
}

_bool CWall::On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist, void* pArg)
{
	std::cout << "Ray_Intersect : Wall " << '\n';
	std::cout << "Hit World Pos: " << "X : " << fHitWorldPos.x << " Y : " << fHitWorldPos.y << " Z : " << fHitWorldPos.z << '\n';
	int x = 10;
	return true;
}

CWall* CWall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWall* pInstance = new CWall(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWall::Clone(void* pArg)
{
	CWall* pInstance = new CWall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWall::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBoxCollider);
}
