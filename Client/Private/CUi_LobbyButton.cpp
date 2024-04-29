#include "CUi_LobbyButton.h"
#include "Level_Loading.h"


CUi_LobbyButton::CUi_LobbyButton(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUi(pGraphic_Device)
{
}

CUi_LobbyButton::CUi_LobbyButton(const CUi_LobbyButton& rhs)
	:CUi(rhs)
{
}

HRESULT CUi_LobbyButton::Initialize_Prototype()
{
	if (FAILED(Add_Components(nullptr)))
		return E_FAIL;

	m_bFocusedVec.reserve(3);

	m_bFocusedVec.emplace_back(false);
	m_bFocusedVec.emplace_back(false);
	m_bFocusedVec.emplace_back(false);

	return S_OK;
}

HRESULT CUi_LobbyButton::Initialize(void* pArg)
{
	return S_OK;
}

void CUi_LobbyButton::PriorityTick(_float fTimeDelta)
{
}

void CUi_LobbyButton::Tick(_float fTimeDelta)
{
	Check_Picking();
	Texture_Switching();
	Player_Select();
	
}

void CUi_LobbyButton::LateTick(_float fTimeDelta)
{
}

HRESULT CUi_LobbyButton::Render()
{
	FirstButtonRender();
	SecondButtonRender();
	ThirdButtonRender();
	TextRender();
	return S_OK;
}

void CUi_LobbyButton::FirstButtonRender()
{
	m_pTransformCom->Bind_WorldMatrix();

	m_pTextureCom->Bind_Texture(m_iTexture_Index);

	m_pVIBufferCom->Render();
}

void CUi_LobbyButton::SecondButtonRender()
{
	m_pSecondTransformCom->Bind_WorldMatrix();

	m_pSecondTextureCom->Bind_Texture(m_iSecondTextureIndex);

	m_pSecondVIBufferCom->Render();
}

void CUi_LobbyButton::ThirdButtonRender()
{
	m_pThirdTransformCom->Bind_WorldMatrix();

	m_pThirdTextureCom->Bind_Texture(m_iThirdTextureIndex);

	m_pThirdVIBufferCom->Render();
}

HRESULT CUi_LobbyButton::Initialize_Active()
{
	Initialize_Set_Size();
	Initialize_Set_Scale_Pos_Rotation(nullptr);

	Initialize_PickingArea();
	InitializeText();

	ShowCursor(true);

	return S_OK;
}

void CUi_LobbyButton::Initialize_Set_ActiveTime()
{
}

void CUi_LobbyButton::Initialize_Set_Size()
{
	m_UiDesc.m_fSizeX = 350;
	m_UiDesc.m_fSizeY = 64;
	
	m_SecondUiDesc.m_fSizeX = 350;
	m_SecondUiDesc.m_fSizeY = 64;

	m_ThirdUiDesc.m_fSizeX = 350;
	m_ThirdUiDesc.m_fSizeY = 64;
}

void CUi_LobbyButton::Initialize_Set_Speed()
{
}

void CUi_LobbyButton::Initialize_Set_Scale_Pos_Rotation(void* pArg)
{
	FirstInitialize();
	SecondInitialize();
	ThirdInitialize();
}

void CUi_LobbyButton::FirstInitialize()
{
	_float3 FirstScale = { m_UiDesc.m_fSizeX, m_UiDesc.m_fSizeY, 1.f };

	m_UiDesc.m_fX = -520;
	m_UiDesc.m_fY = 100;

	m_pTransformCom->Set_Scale(FirstScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		&_float3(m_UiDesc.m_fX, m_UiDesc.m_fY, 0.f));
}

void CUi_LobbyButton::SecondInitialize()
{
	_float3 SecondScale = { m_SecondUiDesc.m_fSizeX, m_SecondUiDesc.m_fSizeY, 1.f };

	m_SecondUiDesc.m_fX = -520;
	m_SecondUiDesc.m_fY = 0;

	m_pSecondTransformCom->Set_Scale(SecondScale);
	m_pSecondTransformCom->Set_State(CTransform::STATE_POSITION,
		&_float3(m_SecondUiDesc.m_fX, m_SecondUiDesc.m_fY, 0.f));


	_float Middlex = g_iWinSizeX / 2;
	_float Middley = g_iWinSizeY / 2;

}

void CUi_LobbyButton::ThirdInitialize()
{
	_float3 ThirdScale = { m_ThirdUiDesc.m_fSizeX, m_ThirdUiDesc.m_fSizeY, 1.f };

	m_ThirdUiDesc.m_fX = -520;
	m_ThirdUiDesc.m_fY = -100;

	m_pThirdTransformCom->Set_Scale(ThirdScale);
	m_pThirdTransformCom->Set_State(CTransform::STATE_POSITION,
		&_float3(m_ThirdUiDesc.m_fX, m_ThirdUiDesc.m_fY, 0.f));
}

void CUi_LobbyButton::InitializeText()
{
	m_pTextVec.clear();

	m_pTextVec.reserve(3);
	CText::Text_Info Text;
	Text.Text = L"GameStart";//L"게임 시작";
	Text.Length = (_uint)wcslen(Text.Text);
	Text.Rect = m_ButtonPickingVec[0];
	Text.Rect.left += 70;
	Text.Rect.top += 10;
	m_pTextVec.emplace_back(Text);

	Text.Text = L"Option";
	Text.Length = (_uint)wcslen(Text.Text);
	Text.Rect = m_ButtonPickingVec[1];
	Text.Rect.left += 120;
	Text.Rect.top += 10;
	m_pTextVec.emplace_back(Text);

	Text.Text = L"Quit";
	Text.Length = (_uint)wcslen(Text.Text);
	Text.Rect = m_ButtonPickingVec[2];
	Text.Rect.left += 120;
	Text.Rect.top += 10;
	m_pTextVec.emplace_back(Text);
}

void CUi_LobbyButton::TextRender()
{
	for (size_t i = 0; i < m_pTextVec.size(); ++i)
	{
		m_pGameInstance->Print_Big_Text(m_pTextVec[i]);
	}
}

HRESULT CUi_LobbyButton::Add_Components(void* pArg)
{
	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pSecondVIBufferCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pSecondTransformCom)))
		return E_FAIL;

	if (FAILED(Add_Component(
		LEVEL_STATIC,
		TEXT("VIBuffer_Rect_Default"),
		(CComponent**)&m_pThirdVIBufferCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("Transform_Default"),
		(CComponent**)&m_pThirdTransformCom)))
		return E_FAIL;

	if (FAILED(Add_Texture(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_LobbyButton::Add_Texture(void* pArg)
{	
	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_LobbyButton_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_LobbyButton_Texture"),
		(CComponent**)&m_pSecondTextureCom)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC,
		TEXT("CUi_LobbyButton_Texture"),
		(CComponent**)&m_pThirdTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUi_LobbyButton::Initialize_PickingArea()
{
	m_ButtonPickingVec.reserve(3);

	_float Left = (m_UiDesc.m_fX + 640) - (m_UiDesc.m_fSizeX / 2.f);
	_float Right = (m_UiDesc.m_fX + 640) + (m_UiDesc.m_fSizeX / 2.f);
	_float Top = (g_iWinSizeY / 2.f) - m_UiDesc.m_fY - (m_UiDesc.m_fSizeY / 2.f);
	_float Bottom = (g_iWinSizeY / 2.f) - m_UiDesc.m_fY + (m_UiDesc.m_fSizeY / 2.f);
	RECT FirstRect = {(LONG)Left, (LONG)Top, (LONG)Right, (LONG)Bottom};
	m_ButtonPickingVec.emplace_back(FirstRect);

	Left = (m_SecondUiDesc.m_fX + 640) - (m_SecondUiDesc.m_fSizeX / 2.f);
	Right = (m_SecondUiDesc.m_fX + 640) + (m_SecondUiDesc.m_fSizeX / 2.f);
	Top = (g_iWinSizeY / 2.f) - m_SecondUiDesc.m_fY - (m_SecondUiDesc.m_fSizeY / 2.f);
	Bottom = (g_iWinSizeY / 2.f) - m_SecondUiDesc.m_fY + (m_SecondUiDesc.m_fSizeY / 2.f);
	RECT SecondRect = { (LONG)Left, (LONG)Top, (LONG)Right, (LONG)Bottom };
	m_ButtonPickingVec.emplace_back(SecondRect);

	Left = (m_ThirdUiDesc.m_fX +640)- (m_ThirdUiDesc.m_fSizeX / 2.f);
	Right = (m_ThirdUiDesc.m_fX + 640) + (m_ThirdUiDesc.m_fSizeX / 2.f);
	Top = (g_iWinSizeY / 2.f) - m_ThirdUiDesc.m_fY - (m_ThirdUiDesc.m_fSizeY / 2.f);
	Bottom = (g_iWinSizeY / 2.f) - m_ThirdUiDesc.m_fY + (m_ThirdUiDesc.m_fSizeY / 2.f);
	RECT m_ThirdRect = { (LONG)Left, (LONG)Top, (LONG)Right, (LONG)Bottom };
	m_ButtonPickingVec.emplace_back(m_ThirdRect);
}

void CUi_LobbyButton::Check_Picking()
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	for (_uint i = 0; i < (_uint)m_ButtonPickingVec.size(); ++i)
	{
		if (true == (bool)PtInRect(&m_ButtonPickingVec[i], ptMouse))
		{
			m_bFocusedVec[i] = true;
		}
		else
		{
			m_bFocusedVec[i] = false;
		}
	}	
}

void CUi_LobbyButton::Texture_Switching()
{
	for (_uint i = 0; i < (_uint)m_bFocusedVec.size(); ++i)
	{
		if (m_bFocusedVec[i])
		{
			switch (i)
			{
			case 0:
				m_iTexture_Index = 1;
				break;
			case 1:
				m_iSecondTextureIndex = 1;
				break;
			case 2:
				m_iThirdTextureIndex = 1;
				break;
			default:
				break;
			}
		}
		else
		{
			switch (i)
			{
			case 0:
				m_iTexture_Index = 0;
				break;
			case 1:
				m_iSecondTextureIndex = 0;
				break;
			case 2:
				m_iThirdTextureIndex = 0;
				break;
			default:
				break;
			}
		}
	}
}

void CUi_LobbyButton::Player_Select()
{
	for (_uint i = 0; i < (_uint)m_bFocusedVec.size(); ++i)
	{
		if (m_bFocusedVec[i] && m_pGameInstance->GetKeyDown(eKeyCode::LButton))
		{
			//LEVEL_GAMEPLAY
			switch (i)	
			{
			case 0:
				// game start
				//m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY));
				m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pGraphic_Device, LEVEL_BOSS));
				break;
			case 1:
				// none
				break;
			case 2:
				// end
				break;
			default:
				break;
			}
			m_bActive = false;
			ShowCursor(false);
		}
	}
}

CUi_LobbyButton* CUi_LobbyButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUi_LobbyButton* pInstance = new CUi_LobbyButton(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("CUi_LobbyButton Create Failed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_LobbyButton::Free()
{
	__super::Free();
	
	Safe_Release(m_pSecondTextureCom);
	Safe_Release(m_pSecondTransformCom);
	Safe_Release(m_pSecondVIBufferCom);
	Safe_Release(m_pThirdTextureCom);
	Safe_Release(m_pThirdTransformCom);
	Safe_Release(m_pThirdVIBufferCom);
}
