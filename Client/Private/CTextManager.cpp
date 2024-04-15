#include "CTextManager.h"
#include "GameInstance.h"


CTextManager::CTextManager()
	:m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

void CTextManager::Initialize()
{
	Initialize_ShopText();
	Initialize_TutorialText();
}

void CTextManager::Initialize_ShopText()
{
	m_ShopText.reserve(20);

}

void CTextManager::Initialize_TutorialText()
{
	m_TutorialText.reserve(20);
	CText::Text_Info Text1;
	Text1.Text = L"튜토리얼텍스트입니다. 윽엑윽";
	Text1.Length = (_uint)wcslen(Text1.Text);
	Text1.Rect = { 100, 100, 800, 200 };
	m_TutorialText.emplace_back(Text1);

	CText::Text_Info ShopText1;
	ShopText1.Text = L"하하하. 감사합니다!";
	ShopText1.Length = (_uint)wcslen(ShopText1.Text);
	ShopText1.Rect = { 200, 50, 800, 200 };
	m_TutorialText.emplace_back(ShopText1);
}

void CTextManager::Print_Text(TextType type, _uint Number)
{
	if (type == TextType::Tutorial)
	{
		if (m_TutorialText.size() <= (size_t)Number)
		{
			MSG_BOX(TEXT("벡터 최대 사이즈 넘음"));
			assert("벡터 최대 사이즈 넘음");
		}
		m_pGameInstance->Print_Text(m_TutorialText[Number]);
	}
	else if (type == TextType::Shop)
	{
		if (m_ShopText.size() < (size_t)Number)
		{
			assert("벡터 최대 사이즈 넘음");
		}
		m_pGameInstance->Print_Text(m_ShopText[Number]);
	}
}

CTextManager* CTextManager::Create()
{
	CTextManager* m_pInstance = new CTextManager();
	m_pInstance->Initialize();

	return m_pInstance;
}

void CTextManager::Free()
{
	Safe_Release(m_pGameInstance);
	m_TutorialText.clear();
	m_ShopText.clear();
}
