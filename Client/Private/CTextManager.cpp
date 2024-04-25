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
	m_Text[_uint(TextType::Sans)].reserve(100);

	Initialize_TutorialClearText();
	Initialize_TutorialMidSpotText();
	Initialize_ShopEndText();
	Initialize_SansText();
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

void CTextManager::Initialize_SansText()
{
	CText::Text_Info SansText0;
	SansText0.Text = L"�ȳ�, �� �ٻ�����?";
	SansText0.Length = (_uint)wcslen(SansText0.Text);
	SansText0.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText0);

	CText::Text_Info SansText1;
	SansText1.Text = L"���� �Ƹ��ٿ� ���̾�.";
	SansText1.Length = (_uint)wcslen(SansText1.Text);
	SansText1.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText1);

	CText::Text_Info SansText2;
	SansText2.Text = L"������ �����Ͱ�, �ɵ��� �Ǿ��";
	SansText2.Length = (_uint)wcslen(SansText2.Text);
	SansText2.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText2);

	CText::Text_Info SansText3;
	SansText3.Text = L"�̷����� �ʰ��� ��������";
	SansText3.Length = (_uint)wcslen(SansText3.Text);
	SansText3.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText3);

	CText::Text_Info SansText4;
	SansText4.Text = L"�������� ��Ÿ�� ��";
	SansText4.Length = (_uint)wcslen(SansText4.Text);
	SansText4.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText4);

	CText::Text_Info SansText5;
	SansText5.Text = L"��, �� �ٵ� ó������ ���� ������ ���� �ʴ��� �𸣰ڴٴϱ�";
	SansText5.Length = (_uint)wcslen(SansText5.Text);
	SansText5.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText5);

	CText::Text_Info SansText6;
	SansText6.Text = L"��, ���� ������ ���� �¾��� �Ŷ� �����߾�?";
	SansText6.Length = (_uint)wcslen(SansText6.Text);
	SansText6.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText6);

	CText::Text_Info SansText7;
	SansText7.Text = L"�츰 �ð����� ���Ӽ��� ������ ��Ģ�� �ִٴ� �� �߰��߾�.";
	SansText7.Length = (_uint)wcslen(SansText7.Text);
	SansText7.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText7);

	CText::Text_Info SansText8;
	SansText8.Text = L"�ð����� ����쵹 �����̰�, ���߰� �ٽ� �����ϰ�...";
	SansText8.Length = (_uint)wcslen(SansText8.Text);
	SansText8.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText8);

	CText::Text_Info SansText9;
	SansText9.Text = L"�׷��� ���ڱ�, ��� ���� ��������.";
	SansText9.Length = (_uint)wcslen(SansText9.Text);
	SansText9.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText9);

	CText::Text_Info SansText10;
	SansText10.Text = L"������... �װ� �� ������, ��?";
	SansText10.Length = (_uint)wcslen(SansText10.Text);
	SansText10.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText10);

	CText::Text_Info SansText11;
	SansText11.Text = L"�� � �������� ���� ���Ұž�.";
	SansText11.Length = (_uint)wcslen(SansText11.Text);
	SansText11.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText11);

	CText::Text_Info SansText12;
	SansText12.Text = L"��� �� ���ڱ�, �ƹ��� ������ �� �ǵ������ٴ� ����� �ȴٴ� �� ���̾�.";
	SansText12.Length = (_uint)wcslen(SansText12.Text);
	SansText12.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText12);

	CText::Text_Info SansText13;
	SansText13.Text = L"�̺�, �� �̹� �ǵ��ư��°� �����߾�.";
	SansText13.Length = (_uint)wcslen(SansText13.Text);
	SansText13.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText13);

	CText::Text_Info SansText14;
	SansText14.Text = L"�ٱ��������� ������ �͵� ���̻� ��̷����� �ʰ�";
	SansText14.Length = (_uint)wcslen(SansText14.Text);
	SansText14.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText14);

	CText::Text_Info SansText15;
	SansText15.Text = L"�����ٰ� �ѵ�, �ƹ��� ������ �ٽ� ����� ���ƿ� ���� �״ϱ�. ����?";
	SansText15.Length = (_uint)wcslen(SansText15.Text);
	SansText15.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText15);

	CText::Text_Info SansText16;
	SansText16.Text = L"�� ���������� �����ڸ� ���̾�, �̷����� ���� �Ż翡 �ּ��� �� �ϴµ� ū �ɸ����� ��.";
	SansText16.Length = (_uint)wcslen(SansText16.Text);
	SansText16.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText16);

	CText::Text_Info SansText17;
	SansText17.Text = L"....�ƴϸ� ���� �� �������� ���� ������ �����ΰǰ�? �� �� ����.";
	SansText17.Length = (_uint)wcslen(SansText17.Text);
	SansText17.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText17);

	CText::Text_Info SansText18;
	SansText18.Text = L"����... ������ ������ ���� �˰� ���ڴ�, �� �̻� �Ű���� �ֱ� ��ƴ� �̰ž�.";
	SansText18.Length = (_uint)wcslen(SansText18.Text);
	SansText18.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText18);

	CText::Text_Info SansText19;
	SansText19.Text = L"��... �׷��ϱ� ����...";
	SansText19.Length = (_uint)wcslen(SansText19.Text);
	SansText19.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText19);

	SansText19.Text = L"��, �׷��� �ֵθ��� �ٴϴ°� �� �����ϴ±�, �׷���?";
	SansText19.Length = (_uint)wcslen(SansText19.Text);
	SansText19.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText19);

	CText::Text_Info SansText20;
	SansText20.Text = L"�� ���";
	SansText20.Length = (_uint)wcslen(SansText20.Text);
	SansText20.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText20);

	CText::Text_Info SansText21;
	SansText21.Text = L"�װ� ������ ���� �� �� �� �˾�, ������...";
	SansText21.Length = (_uint)wcslen(SansText21.Text);
	SansText21.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText21);

	CText::Text_Info SansText22;
	SansText22.Text = L"�� ��򰡿��� ���� �� �־�.";
	SansText22.Length = (_uint)wcslen(SansText22.Text);
	SansText22.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText22);

	CText::Text_Info SansText23;
	SansText23.Text = L"�� ���鿡 ����ϰ� ���� ����� ����.";
	SansText23.Length = (_uint)wcslen(SansText23.Text);
	SansText23.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText23);

	CText::Text_Info SansText24;
	SansText24.Text = L"���� ���� �ϱ� ���ߴ� � ����� �����.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"�ٸ� �ð���������, ������...";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"ģ���������� �� �����?";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"����, ģ����.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"�� ��ﳪ?";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"�׳� �� �ؾ�����ڱ�, �˰���?";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"���⸦ ������, �׷���...";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"��, ���� �ξ� ����������.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"��, �õ��غ����߾�. �׷� ������ �� ����, �װ���?";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"�̻�����, �� ���� �ֱ� ������ �츮�� ģ���� �� �Ŷ�� ���� ����߰ŵ�.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"�� �ð��� �ұ�Ģ�� �Ҹ������� ���� ���� ���̶�� ��������.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L" �׸��� ���ϴ� �� ��� �����, ��� �����ٰŶ��.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"�Ƹ��� �׵��� ���ϴ� ����... �۽��.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"���� ����, ������ ���, ���� ģ����";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"�ٵ� �װ� ���� �� ����, �׷���?";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"�׷�, �� ���� ������ �� ���� �η��ݾ�.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"�� ����ؼ� �ð��� ����ϰ� ����Ұž�.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"�� �� �� ������� ����,";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"�������� �׸��δ� ���� ����� �Ұž�.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"�׸��� �� �������� ��������";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"�ֳ��ϸ�, �ʵ� �˰�����, �̽ο� ���� ���������ϰŵ�";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);
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

CText::Text_Info* CTextManager::Get_Text(TextType type, _uint Number)
{  
	if (m_Text[_uint(type)].size() <= Number)
		return nullptr;

	return &m_Text[_uint(type)][Number]; 
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
