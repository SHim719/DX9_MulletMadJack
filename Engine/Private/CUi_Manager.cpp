#include "CUi_Manager.h"
#include "CUi.h"


CUi_Manager::CUi_Manager(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CUi_Manager::Add_Ui_LifePrototype(const wstring& Ui_LifePrototypeTag, CUi* Ui_LifePrototype)
{
	if (m_Ui_LifePrototypes.end() != m_Ui_LifePrototypes.find(Ui_LifePrototypeTag))
		return E_FAIL;

	m_Ui_LifePrototypes.emplace(Ui_LifePrototypeTag, Ui_LifePrototype);

	return S_OK;
}

HRESULT CUi_Manager::Add_Ui_Active(const wstring& Ui_ActiveTag, eUiRenderType UiRenderType, CUi* Ui_Active)
{
	if (UiRenderType == eUiRenderType::Render_NonBlend)
	{
		if (m_Ui_Active.end() != m_Ui_Active.find(Ui_ActiveTag))
			return E_FAIL;

		m_Ui_Active.emplace(Ui_ActiveTag, Ui_Active);
	}
	else {
		if (m_Ui_ActiveBlend.end() != m_Ui_ActiveBlend.find(Ui_ActiveTag))
			return E_FAIL;

		m_Ui_ActiveBlend.emplace(Ui_ActiveTag, Ui_Active);
	}
	return S_OK;
}

HRESULT CUi_Manager::Add_Ui_LifeClone(const wstring& Ui_LifePrototypeTag, eUiRenderType UiRenderType, void* pArg)
{
	auto pLife = m_Ui_LifePrototypes.find(Ui_LifePrototypeTag);
	if (m_Ui_LifePrototypes.end() == pLife)
		return E_FAIL;


	CUi* pUi_Life = (pLife->second)->Clone(pArg);
	if (nullptr == pUi_Life)
		return E_FAIL;


	if (UiRenderType == eUiRenderType::Render_NonBlend)
	{
		m_Ui_LifeClonelist.emplace_back(pUi_Life);
	}


	else if (UiRenderType == eUiRenderType::Render_Blend)
	{
		m_Ui_LifeBlendClonelist.emplace_back(pUi_Life);
	}


	return S_OK;
}

CUi* CUi_Manager::Add_Ui_PartClone(const wstring& Ui_PartPrototypeTag, void* pArg)
{
	auto pLife = m_Ui_LifePrototypes.find(Ui_PartPrototypeTag);
	if(m_Ui_LifePrototypes.end() == pLife)
		return nullptr;

	CUi* pUi_Life = (pLife->second)->Clone(pArg);
	if (nullptr == pUi_Life)
		return nullptr;

	return pUi_Life;
}

void CUi_Manager::PriorityTick(_float fTimeDelta)
{
	auto iter = m_Ui_LifeClonelist.begin();
	for (; iter != m_Ui_LifeClonelist.end(); )
	{
		if ((*iter)->Is_Dead())
		{
			Safe_Release(*iter);
			iter = m_Ui_LifeClonelist.erase(iter);
		}
		else
		{
			(*iter)->PriorityTick(fTimeDelta);
			++iter;
		}
	}

	auto Blenditer = m_Ui_LifeBlendClonelist.begin();
	for (; Blenditer != m_Ui_LifeBlendClonelist.end(); )
	{
		if ((*Blenditer)->Is_Dead())
		{
			Safe_Release(*Blenditer);
			Blenditer = m_Ui_LifeBlendClonelist.erase(Blenditer);
		}
		else
		{
			(*Blenditer)->PriorityTick(fTimeDelta);
			++Blenditer;
		}
	}

	for (auto& iter : m_Ui_Active) {
		if (iter.second->Get_Active())
			iter.second->PriorityTick(fTimeDelta);
	}

	for (auto& iter : m_Ui_ActiveBlend) {
		if (iter.second->Get_Active())
			iter.second->PriorityTick(fTimeDelta);
	}
}

void CUi_Manager::Tick(_float fTimeDelta)
{
	auto iter = m_Ui_LifeClonelist.begin();
	for (; iter != m_Ui_LifeClonelist.end(); )
	{
		if ((*iter)->Is_Dead())
		{
			Safe_Release(*iter);
			iter = m_Ui_LifeClonelist.erase(iter);
		}
		else
		{
			(*iter)->Tick(fTimeDelta);
			++iter;
		}
	}

	auto Blenditer = m_Ui_LifeBlendClonelist.begin();
	for (; Blenditer != m_Ui_LifeBlendClonelist.end(); )
	{
		if ((*Blenditer)->Is_Dead())
		{
			Safe_Release(*Blenditer);
			Blenditer = m_Ui_LifeBlendClonelist.erase(Blenditer);
		}
		else
		{
			(*Blenditer)->Tick(fTimeDelta);
			++Blenditer;
		}
	}

	for (auto& iter : m_Ui_Active) {
		if (iter.second->Get_Active())
			iter.second->Tick(fTimeDelta);
	}

	for (auto& iter : m_Ui_ActiveBlend) {
		if (iter.second->Get_Active())
			iter.second->Tick(fTimeDelta);
	}
}

void CUi_Manager::LateTick(_float fTimeDelta)
{
	auto iter = m_Ui_LifeClonelist.begin();
	for (; iter != m_Ui_LifeClonelist.end(); )
	{
		if ((*iter)->Is_Dead())
		{
			Safe_Release(*iter);
			iter = m_Ui_LifeClonelist.erase(iter);
		}
		else
		{
			(*iter)->LateTick(fTimeDelta);
			++iter;
		}
	}

	auto Blenditer = m_Ui_LifeBlendClonelist.begin();
	for (; Blenditer != m_Ui_LifeBlendClonelist.end(); )
	{
		if ((*Blenditer)->Is_Dead())
		{
			Safe_Release(*Blenditer);
			Blenditer = m_Ui_LifeBlendClonelist.erase(Blenditer);
		}
		else
		{
			(*Blenditer)->LateTick(fTimeDelta);
			++Blenditer;
		}
	}

	for (auto& iter : m_Ui_Active) {
		if (iter.second->Get_Active())
			iter.second->LateTick(fTimeDelta);
	}

	for (auto& iter : m_Ui_ActiveBlend) {
		if (iter.second->Get_Active())
			iter.second->LateTick(fTimeDelta);
	}
}

void CUi_Manager::Ui_Render_Begin()
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixIdentity(&m_ProjMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, (_float)m_iWinSizeX, (_float)m_iWinSizeY, 0.0f, 100.f);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);


}
// 
HRESULT CUi_Manager::Ui_Render()
{
	for (auto& Clone : m_Ui_LifeClonelist)
		Clone->Render();

	for (auto& BlendClone : m_Ui_LifeBlendClonelist)
		BlendClone->Render();

	for (auto& iter : m_Ui_Active) {
		if (iter.second->Get_Active() == true)
			iter.second->Render();
	}

	for (auto& iter : m_Ui_ActiveBlend) {
		if(iter.second->Get_Active() == true)
			iter.second->Render();
	}


	return S_OK;
}

void CUi_Manager::Ui_Render_End()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CUi_Manager::Set_WinSize(_uint iWinSizeX, _uint iWinSizeY)
{
	m_iWinSizeX = iWinSizeX;
	m_iWinSizeY = iWinSizeY;
}

void CUi_Manager::Set_Ui_ActiveState(const wstring& Ui_ActiveTag, bool _isActive)
{

	auto iter = m_Ui_Active.find(Ui_ActiveTag);
	if (m_Ui_Active.end() != iter)
	{
		iter->second->Set_Active(_isActive);
	}

	auto Blenditer = m_Ui_ActiveBlend.find(Ui_ActiveTag);
	if (m_Ui_ActiveBlend.end() != Blenditer)
	{
		Blenditer->second->Set_Active(_isActive);
	}

}

CUi_Manager* CUi_Manager::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return new CUi_Manager(pGraphic_Device);
}

void CUi_Manager::Free()
{
	for (auto& Clone : m_Ui_LifeClonelist)
		Safe_Release(Clone);
	m_Ui_LifeClonelist.clear();


	for (auto& BlendClone : m_Ui_LifeBlendClonelist)
		Safe_Release(BlendClone);
	m_Ui_LifeBlendClonelist.clear();


	for (auto& Pair : m_Ui_LifePrototypes)
		Safe_Release(Pair.second);
	m_Ui_LifePrototypes.clear();


	for (auto& Pair : m_Ui_Active)
		Safe_Release(Pair.second);
	m_Ui_Active.clear();

	for (auto& Pair : m_Ui_ActiveBlend)
		Safe_Release(Pair.second);
	m_Ui_ActiveBlend.clear();

	Safe_Release(m_pGraphic_Device);
}
