#include "CTextManager.h"
#include "GameInstance.h"
#include "CText_BackGround.h"
#include "CText.h"


CTextManager::CTextManager()
	:m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

void CTextManager::Initialize()
{
	m_Text[_uint(TextType::TutorialMidSpot)].reserve(7);
	m_Text[_uint(TextType::TutorialClear)].reserve(6);
	m_Text[_uint(TextType::ShopEnd)].reserve(2);

	Initialize_TutorialClearText();
	Initialize_TutorialMidSpotText();
	Initialize_ShopEndText();
}

void CTextManager::Initialize_TutorialClearText()
{
	CText::Text_Info TutorialEndText0;
	TutorialEndText0.Text = L"�׽�Ʈ�� ����߳׿�!!! \n"
		"�� �׾��ݾƿ�! ������";
	TutorialEndText0.Length = (_uint)wcslen(TutorialEndText0.Text);
	TutorialEndText0.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialClear].emplace_back(TutorialEndText0);

	CText::Text_Info TutorialEndText1;
	TutorialEndText1.Text = L"����! ���.. ���ִ��� ������ �־��! \n"
		"... �����ϰ� �־��!!!";
	TutorialEndText1.Length = (_uint)wcslen(TutorialEndText1.Text);
	TutorialEndText1.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialClear].emplace_back(TutorialEndText1);

	CText::Text_Info TutorialEndText2;
	TutorialEndText2.Text = L"���ִ��� �ִ� ������ 5�� ���ҽ��ϴ�!";
	TutorialEndText2.Length = (_uint)wcslen(TutorialEndText2.Text);
	TutorialEndText2.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialClear].emplace_back(TutorialEndText2);


	CText::Text_Info TutorialEndText3;
	TutorialEndText3.Text = L"������ ������ ��� ���� �ٽ� ���� �ؿ�! \n"
		"�� ���� ����� �о�����?? ������";
	TutorialEndText3.Length = (_uint)wcslen(TutorialEndText3.Text);
	TutorialEndText3.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialClear].emplace_back(TutorialEndText3);

	CText::Text_Info TutorialEndText4;
	TutorialEndText4.Text = L"1�� �����մϴ�! ����� �����!";
	TutorialEndText4.Length = (_uint)wcslen(TutorialEndText4.Text);
	TutorialEndText4.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialClear].emplace_back(TutorialEndText4);

	CText::Text_Info TutorialEndText5;
	TutorialEndText5.Text = L"15�� ���ҽ��ϴ�. ������� �κ��� �ı��ϼ���!!";
	TutorialEndText5.Length = (_uint)wcslen(TutorialEndText5.Text);
	TutorialEndText5.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialClear].emplace_back(TutorialEndText5);
}

void CTextManager::Initialize_TutorialMidSpotText()
{
	CText::Text_Info TutorialText0;
	TutorialText0.Text = L"�ȳ��ϼ���, ���������! ���� ����̿���. ���� ���̺� ��Ʈ���� ���Դϴ�!";
	TutorialText0.Length = (_uint)wcslen(TutorialText0.Text);
	TutorialText0.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText0);

	CText::Text_Info TutorialText1;
	TutorialText1.Text = L"����� ���ִ��� �����ϴ� ����� ���鸸 ���� ���Ѻ��� �ִٰ��!";
	TutorialText1.Length = (_uint)wcslen(TutorialText1.Text);
	TutorialText1.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText1);

	CText::Text_Info TutorialText2;
	TutorialText2.Text = L"�� ��� ����� ����� ������ �����ϰ� �־��.";
	TutorialText2.Length = (_uint)wcslen(TutorialText2.Text);
	TutorialText2.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText2);

	CText::Text_Info TutorialText3;
	TutorialText3.Text = L"'���ƿ�' ���п� �ǰ� ���� �޴��ϰ���!";
	TutorialText3.Length = (_uint)wcslen(TutorialText3.Text);
	TutorialText3.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText3);

	CText::Text_Info TutorialText4;
	TutorialText4.Text = L"������ 15�� ���ҽ��ϴ�! \n"
		"���� ���϶����� ��û�ڰ� �� ��� �� �̴ٰϴ�!";
	TutorialText4.Length = (_uint)wcslen(TutorialText4.Text);
	TutorialText4.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText4);

	CText::Text_Info TutorialText5;
	TutorialText5.Text = L"������ �ƹ��͵� �� �׿��� ��û�ڸ� �����ϰ� ����ٸ�, 15�� �ڿ� �װ� �� �ſ���.";
	TutorialText5.Length = (_uint)wcslen(TutorialText5.Text);
	TutorialText5.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText5);

	CText::Text_Info TutorialText6;
	TutorialText6.Text = L"�غ�Ƴ���? ���� �κ����� �����ϼ���!";
	TutorialText6.Length = (_uint)wcslen(TutorialText6.Text);
	TutorialText6.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText6);
}

void CTextManager::Initialize_ShopEndText()
{
	CText::Text_Info ShopEndText1;
	ShopEndText1.Text = L"������. �����մϴ�!";
	ShopEndText1.Length = (_uint)wcslen(ShopEndText1.Text);
	ShopEndText1.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::ShopEnd].emplace_back(ShopEndText1);

	CText::Text_Info ShopEndText2;
	ShopEndText2.Text = L"��ȭ ���۷��̼��Դϴ�. �ູ�� �Ϸ� �Ǽ���.";
	ShopEndText2.Length = (_uint)wcslen(ShopEndText2.Text);
	ShopEndText2.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::ShopEnd].emplace_back(ShopEndText2);
}

void CTextManager::Print_Text(TextType type, _uint Number)
{
	if(m_Text[_uint(type)].size() > Number)
		m_pGameInstance->Print_Text(m_Text[_uint(type)][Number]);
}

void CTextManager::Set_Pos_Text(TextType type, _uint Number, RECT Rect)
{
	m_Text[_uint(type)][Number].Rect = Rect;
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
	for (size_t i = 0; i < (size_t)TextType::End; i++)
	{
		m_Text[i].clear();
	}
}
