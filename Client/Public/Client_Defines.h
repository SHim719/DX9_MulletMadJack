	#pragma once

#include "../Default/stdafx.h"
#include <process.h>

#include "Client_Components.h"
#include "Client_Struct.h"

/* Ŭ���̾�Ʈ���� �����ϴ� ��� Ŭ�������� ���������� ���� ����ϴ� ���ǵ��� ��Ƶд�. */
namespace Client
{
	const unsigned int		g_iWinSizeX = 1280;
	const unsigned int		g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, 
		LEVEL_LOADING, 
		LEVEL_LOGO,
		LEVEL_GAMEPLAY,
		LEVEL_END };

	enum class UIPOS
	{
		MonsterDieYpos = 250,
		ScreenXpos = 700,
		End
	};

	enum OBJTYPE
	{
		MONSTER,
		EVENT_TRIGGER,
		WALL,
		FLOOR,
		OBJECT,
		DOOR,
		SODAMACHINE,
		SODAMACHINE_BANNER,
		BORDER,
		SPAWN_TRIGGER,
		SLOPE,
		OBJTYPE_END,
	};

	enum MONSTERTYPE
	{
		WHITE_SUIT,
		CHAINSAW,
		DRONE,
		MONSTER_END,
	};

	enum TRIGGERTYPE
	{
		TRIGGER_END
	};
}



extern HWND				g_hWnd;





using namespace Client;