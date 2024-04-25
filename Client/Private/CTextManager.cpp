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

void CTextManager::Initialize_SansText()
{
	CText::Text_Info SansText0;
	SansText0.Text = L"안녕, 꽤 바빴었지?";
	SansText0.Length = (_uint)wcslen(SansText0.Text);
	SansText0.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText0);

	CText::Text_Info SansText1;
	SansText1.Text = L"정말 아름다운 날이야.";
	SansText1.Length = (_uint)wcslen(SansText1.Text);
	SansText1.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText1);

	CText::Text_Info SansText2;
	SansText2.Text = L"새들은 지저귀고, 꽃들은 피어나고";
	SansText2.Length = (_uint)wcslen(SansText2.Text);
	SansText2.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText2);

	CText::Text_Info SansText3;
	SansText3.Text = L"이런날엔 너같은 꼬마들은";
	SansText3.Length = (_uint)wcslen(SansText3.Text);
	SansText3.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText3);

	CText::Text_Info SansText4;
	SansText4.Text = L"지옥에서 불타야 해";
	SansText4.Length = (_uint)wcslen(SansText4.Text);
	SansText4.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText4);

	CText::Text_Info SansText5;
	SansText5.Text = L"흠, 왜 다들 처음부터 강한 공격을 쓰지 않는지 모르겠다니까";
	SansText5.Length = (_uint)wcslen(SansText5.Text);
	SansText5.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText5);

	CText::Text_Info SansText6;
	SansText6.Text = L"뭐, 내가 가만히 서서 맞아줄 거라 생각했어?";
	SansText6.Length = (_uint)wcslen(SansText6.Text);
	SansText6.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText6);

	CText::Text_Info SansText7;
	SansText7.Text = L"우린 시공간의 연속성에 막대한 변칙이 있다는 걸 발견했어.";
	SansText7.Length = (_uint)wcslen(SansText7.Text);
	SansText7.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText7);

	CText::Text_Info SansText8;
	SansText8.Text = L"시공간이 좌충우돌 움직이고, 멈추고 다시 시작하고...";
	SansText8.Length = (_uint)wcslen(SansText8.Text);
	SansText8.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText8);

	CText::Text_Info SansText9;
	SansText9.Text = L"그러다 갑자기, 모든 것이 끝나버려.";
	SansText9.Length = (_uint)wcslen(SansText9.Text);
	SansText9.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText9);

	CText::Text_Info SansText10;
	SansText10.Text = L"헤헤헤... 네가 한 짓이지, 응?";
	SansText10.Length = (_uint)wcslen(SansText10.Text);
	SansText10.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText10);

	CText::Text_Info SansText11;
	SansText11.Text = L"넌 어떤 느낌일지 이해 못할거야.";
	SansText11.Length = (_uint)wcslen(SansText11.Text);
	SansText11.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText11);

	CText::Text_Info SansText12;
	SansText12.Text = L"어느 날 갑자기, 아무런 경고없이 다 되돌려진다는 사실을 안다는 것 말이야.";
	SansText12.Length = (_uint)wcslen(SansText12.Text);
	SansText12.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText12);

	CText::Text_Info SansText13;
	SansText13.Text = L"이봐, 난 이미 되돌아가는걸 포기했어.";
	SansText13.Length = (_uint)wcslen(SansText13.Text);
	SansText13.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText13);

	CText::Text_Info SansText14;
	SansText14.Text = L"바깥세상으로 나가는 것도 더이상 흥미롭지도 않고";
	SansText14.Length = (_uint)wcslen(SansText14.Text);
	SansText14.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText14);

	CText::Text_Info SansText15;
	SansText15.Text = L"나간다고 한들, 아무런 기억없이 다시 여기로 돌아와 있을 테니까. 맞지?";
	SansText15.Length = (_uint)wcslen(SansText15.Text);
	SansText15.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText15);

	CText::Text_Info SansText16;
	SansText16.Text = L"좀 직설적으로 말하자면 말이야, 이런점이 내가 매사에 최선을 다 하는데 큰 걸림돌이 돼.";
	SansText16.Length = (_uint)wcslen(SansText16.Text);
	SansText16.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText16);

	CText::Text_Info SansText17;
	SansText17.Text = L"....아니면 그저 내 게으름에 대한 같잖은 변명인건가? 알 게 뭐야.";
	SansText17.Length = (_uint)wcslen(SansText17.Text);
	SansText17.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText17);

	CText::Text_Info SansText18;
	SansText18.Text = L"그저... 다음에 벌어질 일을 알고 있자니, 더 이상 신경끄고 있기 어렵다 이거야.";
	SansText18.Length = (_uint)wcslen(SansText18.Text);
	SansText18.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText18);

	CText::Text_Info SansText19;
	SansText19.Text = L"음... 그러니까 말야...";
	SansText19.Length = (_uint)wcslen(SansText19.Text);
	SansText19.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText19);

	SansText19.Text = L"너, 그렇게 휘두르고 다니는걸 참 좋아하는군, 그렇지?";
	SansText19.Length = (_uint)wcslen(SansText19.Text);
	SansText19.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText19);

	CText::Text_Info SansText20;
	SansText20.Text = L"잘 들어";
	SansText20.Length = (_uint)wcslen(SansText20.Text);
	SansText20.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText20);

	CText::Text_Info SansText21;
	SansText21.Text = L"네가 전에도 답을 안 한 건 알아, 하지만...";
	SansText21.Length = (_uint)wcslen(SansText21.Text);
	SansText21.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText21);

	CText::Text_Info SansText22;
	SansText22.Text = L"그 어딘가에서 느낄 수 있어.";
	SansText22.Length = (_uint)wcslen(SansText22.Text);
	SansText22.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText22);

	CText::Text_Info SansText23;
	SansText23.Text = L"네 내면에 희미하게 착한 사람이 보여.";
	SansText23.Length = (_uint)wcslen(SansText23.Text);
	SansText23.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText23);

	CText::Text_Info SansText24;
	SansText24.Text = L"옳은 일을 하기 원했던 어떤 사람의 기억이.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"다른 시간선에서는, 심지어...";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"친구였을지도 모를 사람이?";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"제발, 친구야.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"나 기억나?";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"그냥 다 잊어버리자구, 알겠지?";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"무기를 내려놔, 그러면...";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"뭐, 일이 훨씬 쉬워지겠지.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"뭐, 시도해볼만했어. 그런 식으로 해 보자, 그거지?";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"이상하지, 이 일이 있기 전까진 우리가 친구가 될 거라고 내심 기대했거든.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"늘 시간의 불규칙은 불만족으로 인해 생긴 일이라고 생각했지.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L" 그리고 원하는 걸 모두 얻고나면, 모두 멈춰줄거라고.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"아마도 그들이 원하던 것은... 글쎄다.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"좋은 음식, 따뜻한 농담, 좋은 친구들";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"근데 그건 말도 안 되지, 그렇지?";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"그래, 넌 절대 만족할 수 없는 부류잖아.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"넌 계속해서 시간을 허비하고 허비할거야.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"야 내 말 믿으라고 꼬마,";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"언젠가는 그만두는 법을 배워야 할거야.";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"그리고 그 언젠가가 오늘이지";
	SansText24.Length = (_uint)wcslen(SansText24.Text);
	SansText24.Rect = { 320, 30, 900, 200 };
	m_Text[(_uint)TextType::Sans].emplace_back(SansText24);

	SansText24.Text = L"왜냐하면, 너도 알겠지만, 이싸움 정말 지긋지긋하거든";
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
