#include "CGasterRoop.h"

CGasterRoop::CGasterRoop(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CGasterRoop::CGasterRoop(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

HRESULT CGasterRoop::Initialize(void* pArg)
{
	GasterRoopArg* Arg = (GasterRoopArg*)pArg;
	m_ePos = Arg->Pos;
	m_eFirePos = Arg->FirePos;
	m_fRoopGap = Arg->RoopGap;
	m_iFloor = Arg->Floor;
	m_fLifeTime = Arg->LifeTime;

	return S_OK;
}

void CGasterRoop::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	if (m_fLifeTime < 0)
	{
		m_bDestroyed = true;
	}
	Roop(fTimeDelta);
}

void CGasterRoop::Roop(_float fTimeDelta)
{
	m_fRoopTime += fTimeDelta;
	if (m_fRoopTime > m_fRoopGap)
	{
		m_fRoopTime = 0;
		GasterArg pArg;
		pArg._floor = m_iFloor;
		pArg.FirePos = m_eFirePos;
		pArg.Pos = m_ePos;
		m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, L"GasterLaser", L"Prototype_CSans_Gaster", &pArg);
	}
}

CGasterRoop* CGasterRoop::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGasterRoop* m_pInstance = new CGasterRoop(pGraphic_Device);

	return m_pInstance;
}

CGameObject* CGasterRoop::Clone(void* pArg)
{
	CGasterRoop* m_pInstance = new CGasterRoop(*this);

	if (FAILED(m_pInstance->Initialize(pArg)))
	{

	}

	return m_pInstance;
}

void CGasterRoop::Free()
{
	__super::Free();
}
