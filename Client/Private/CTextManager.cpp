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
	TutorialEndText0.Text = L"테스트를 통과했네요!!! \n"
		"안 죽었잖아요! 하하하";
	TutorialEndText0.Length = (_uint)wcslen(TutorialEndText0.Text);
	TutorialEndText0.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialClear].emplace_back(TutorialEndText0);

	CText::Text_Info TutorialEndText1;
	TutorialEndText1.Text = L"으아! 잠깐.. 공주님이 가까이 있어요! \n"
		"... 추적하고 있어요!!!";
	TutorialEndText1.Length = (_uint)wcslen(TutorialEndText1.Text);
	TutorialEndText1.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialClear].emplace_back(TutorialEndText1);

	CText::Text_Info TutorialEndText2;
	TutorialEndText2.Text = L"공주님이 있는 곳까지 5층 남았습니다!";
	TutorialEndText2.Length = (_uint)wcslen(TutorialEndText2.Text);
	TutorialEndText2.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialClear].emplace_back(TutorialEndText2);


	CText::Text_Info TutorialEndText3;
	TutorialEndText3.Text = L"하지만 죽으면 모든 곳을 다시 가야 해요! \n"
		"앱 서비스 약관은 읽었겠죠?? 하하하";
	TutorialEndText3.Length = (_uint)wcslen(TutorialEndText3.Text);
	TutorialEndText3.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialClear].emplace_back(TutorialEndText3);

	CText::Text_Info TutorialEndText4;
	TutorialEndText4.Text = L"1층 시작합니다! 행운을 빌어요!";
	TutorialEndText4.Length = (_uint)wcslen(TutorialEndText4.Text);
	TutorialEndText4.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialClear].emplace_back(TutorialEndText4);

	CText::Text_Info TutorialEndText5;
	TutorialEndText5.Text = L"15초 남았습니다. 빌어먹을 로봇을 파괴하세요!!";
	TutorialEndText5.Length = (_uint)wcslen(TutorialEndText5.Text);
	TutorialEndText5.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialClear].emplace_back(TutorialEndText5);
}

void CTextManager::Initialize_TutorialMidSpotText()
{
	CText::Text_Info TutorialText0;
	TutorialText0.Text = L"안녕하세요, 모더레이터! 공식 방송이에요. 지금 라이브 스트리밍 중입니다!";
	TutorialText0.Length = (_uint)wcslen(TutorialText0.Text);
	TutorialText0.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText0);

	CText::Text_Info TutorialText1;
	TutorialText1.Text = L"당신이 공주님을 구출하는 모습을 수백만 명이 지켜보고 있다고요!";
	TutorialText1.Length = (_uint)wcslen(TutorialText1.Text);
	TutorialText1.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText1);

	CText::Text_Info TutorialText2;
	TutorialText2.Text = L"이 모든 사랑을 당신의 혈류에 투자하고 있어요.";
	TutorialText2.Length = (_uint)wcslen(TutorialText2.Text);
	TutorialText2.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText2);

	CText::Text_Info TutorialText3;
	TutorialText3.Text = L"'좋아요' 덕분에 피가 아주 달달하겠죠!";
	TutorialText3.Length = (_uint)wcslen(TutorialText3.Text);
	TutorialText3.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText3);

	CText::Text_Info TutorialText4;
	TutorialText4.Text = L"생명이 15초 남았습니다! \n"
		"적을 죽일때마다 시청자가 더 살게 해 줄겁니다!";
	TutorialText4.Length = (_uint)wcslen(TutorialText4.Text);
	TutorialText4.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText4);

	CText::Text_Info TutorialText5;
	TutorialText5.Text = L"하지만 아무것도 못 죽여서 시청자를 지루하게 만든다면, 15초 뒤에 죽게 될 거예요.";
	TutorialText5.Length = (_uint)wcslen(TutorialText5.Text);
	TutorialText5.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText5);

	CText::Text_Info TutorialText6;
	TutorialText6.Text = L"준비됐나요? 망할 로봇들을 제압하세요!";
	TutorialText6.Length = (_uint)wcslen(TutorialText6.Text);
	TutorialText6.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::TutorialMidSpot].emplace_back(TutorialText6);
}

void CTextManager::Initialize_ShopEndText()
{
	CText::Text_Info ShopEndText1;
	ShopEndText1.Text = L"하하하. 감사합니다!";
	ShopEndText1.Length = (_uint)wcslen(ShopEndText1.Text);
	ShopEndText1.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::ShopEnd].emplace_back(ShopEndText1);

	CText::Text_Info ShopEndText2;
	ShopEndText2.Text = L"평화 코퍼레이션입니다. 행복한 하루 되세요.";
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
