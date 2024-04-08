#pragma once

#include "../Default/stdafx.h"
#include <process.h>

/* 클라이언트에서 제작하는 모든 클래스들이 공통적으로 자주 사용하는 정의들을 모아둔다. */
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
		WALL,
		FLOOR,
		OBJECT,
		DOOR,
		OBJTYPE_END,
	};
}



extern HWND				g_hWnd;





using namespace Client;