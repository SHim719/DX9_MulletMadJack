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
	else 
	{
		if (m_Ui_ActiveBlend.end() != m_Ui_ActiveBlend.find(Ui_ActiveTag))
			return E_FAIL;

		m_Ui_ActiveBlend.emplace(Ui_ActiveTag, Ui_Active);
	}
	return S_OK;
}

HRESULT CUi_Manager::Add_Ui_ShopActive(const wstring& Ui_ShopTag, CUi* Ui_Shop)
{
	if (m_Ui_ShopActive.end() != m_Ui_ShopActive.find(Ui_ShopTag))
		return E_FAIL;

	m_Ui_ShopActive.emplace(Ui_ShopTag, Ui_Shop);

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

class CUi* CUi_Manager::Add_Ui_PartCloneRender(const wstring& Ui_LifePrototypeTag, eUiRenderType UiRenderType, void* pArg)
{
	auto pLife = m_Ui_LifePrototypes.find(Ui_LifePrototypeTag);
	if (m_Ui_LifePrototypes.end() == pLife)
		return nullptr;

	CUi* pUi_Life = (pLife->second)->Clone(pArg);
	if (nullptr == pUi_Life)
		return nullptr;

	if (UiRenderType == eUiRenderType::Render_NonBlend)
	{
		m_Ui_Partlist.emplace_back(pUi_Life);
	}

	return pUi_Life;
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

	for (auto& iter : m_Ui_ShopActive) {
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

	auto Partiter = m_Ui_Partlist.begin();
	for (; Partiter != m_Ui_Partlist.end(); )
	{
		if ((*Partiter)->Is_Dead())
		{
			Safe_Release(*Partiter);
			Partiter = m_Ui_Partlist.erase(Partiter);
		}
		else
		{
			(*Partiter)->Tick(fTimeDelta);
			++Partiter;
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

	for (auto& iter : m_Ui_ShopActive) {
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

	for (auto& iter : m_Ui_ShopActive) {
		if (iter.second->Get_Active())
			iter.second->LateTick(fTimeDelta);
	}
}

void CUi_Manager::Ui_Render_Begin()
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixIdentity(&m_ProjMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, (_float)m_iWinSizeX, (_float)m_iWinSizeY, 0.0f, 100.f);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
}

void CUi_Manager::Ui_Shop_Render_Begin()
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixIdentity(&m_ProjMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, (_float)m_iWinSizeX, (_float)m_iWinSizeY, 0.0f, 100.f);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
}

HRESULT CUi_Manager::Ui_Shop_Render()
{
	Ui_Shop_Render_Begin();
	for (auto& iter : m_Ui_ShopActive) {
		if (iter.second->Get_Active() == true)
			iter.second->Render();
	}
	Ui_Shop_Render_End();
	return S_OK;
}

void CUi_Manager::Ui_Shop_Render_End()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CUi_Manager::Ui_Render()
{
	for (auto& Clone : m_Ui_LifeClonelist)
		Clone->Render();

	for (auto& PartClone : m_Ui_Partlist)
		PartClone->Render();
	
	for (auto& iter : m_Ui_Active) {
		if (iter.second->Get_Active() == true)
			iter.second->Render();
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND,
		D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND,
		D3DBLEND_INVSRCALPHA);

	for (auto& BlendClone : m_Ui_LifeBlendClonelist)
		BlendClone->Render();

	for (auto& iter : m_Ui_ActiveBlend) {
		if(iter.second->Get_Active() == true)
			iter.second->Render();
	}
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	return S_OK;
}

void CUi_Manager::Ui_Render_End()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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

void CUi_Manager::Set_Enter(bool _Enter)
{
	for (auto& Pair : m_Ui_Active)
		Pair.second->Enter(_Enter);

	for (auto& Pair : m_Ui_ActiveBlend)
		Pair.second->Enter(_Enter);
}

CUi* CUi_Manager::Get_ActiveBlendUI(const wstring& strKey)
{
	auto it = m_Ui_ActiveBlend.find(strKey);
	if (it == m_Ui_ActiveBlend.end())
		return nullptr;

	return it->second;
}

CUi* CUi_Manager::Get_ActiveNonBlendUI(const wstring& strKey)
{
	auto it = m_Ui_Active.find(strKey);
	if (it == m_Ui_Active.end())
		return nullptr;

	return it->second;
}

bool CUi_Manager::Get_Ui_ActiveState(const wstring& Ui_ActiveTag)
{
	auto iter = m_Ui_Active.find(Ui_ActiveTag);
	if (m_Ui_Active.end() != iter)
	{
		return iter->second->Get_Active();
	}

	auto Blenditer = m_Ui_ActiveBlend.find(Ui_ActiveTag);
	if (m_Ui_ActiveBlend.end() != Blenditer)
	{
		return Blenditer->second->Get_Active();
	}

	return false;
}

void CUi_Manager::Set_Ui_ShopActiveState(const wstring& Ui_ShopTag, bool _isActive)
{
	auto iter = m_Ui_ShopActive.find(Ui_ShopTag);
	if (m_Ui_ShopActive.end() != iter)
	{
		iter->second->Set_Active(_isActive);
	}
}

void CUi_Manager::Set_Ui_ActiveTextureIndex(const wstring& Ui_ActiveTag, int _iTextureIndex)
{
	auto iter = m_Ui_Active.find(Ui_ActiveTag);
	if (m_Ui_Active.end() != iter)
	{
		iter->second->Set_Texture_Index(_iTextureIndex);
	}

	auto Blenditer = m_Ui_ActiveBlend.find(Ui_ActiveTag);
	if (m_Ui_ActiveBlend.end() != Blenditer)
	{
		Blenditer->second->Set_Texture_Index(_iTextureIndex);
	}
}

int CUi_Manager::Get_Ui_ActiveTextureIndex(const wstring& Ui_ActiveTag)
{

	auto iter = m_Ui_Active.find(Ui_ActiveTag);
	if (m_Ui_Active.end() != iter)
	{
		return iter->second->Get_Texture_Index();
	}

	auto Blenditer = m_Ui_ActiveBlend.find(Ui_ActiveTag);
	if (m_Ui_ActiveBlend.end() != Blenditer)
	{
		return Blenditer->second->Get_Texture_Index();
	}

	return 0;
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

	for (auto& PartClone : m_Ui_Partlist)
		Safe_Release(PartClone);
	m_Ui_Partlist.clear();

	for (auto& BlendClone : m_Ui_LifeBlendClonelist)
		Safe_Release(BlendClone);
	m_Ui_LifeBlendClonelist.clear();


	for (auto& Pair : m_Ui_Active)
		Safe_Release(Pair.second);
	m_Ui_Active.clear();


	for (auto& Pair : m_Ui_ActiveBlend)
		Safe_Release(Pair.second);
	m_Ui_ActiveBlend.clear();

	for (auto& Pair : m_Ui_ShopActive)
		Safe_Release(Pair.second);
	m_Ui_ShopActive.clear();

	for (auto& Pair : m_Ui_LifePrototypes)
		Safe_Release(Pair.second);
	m_Ui_LifePrototypes.clear();

	Safe_Release(m_pGraphic_Device);
}
