#pragma once


namespace Engine
{
	typedef struct
	{
		HWND			hWnd;
		unsigned int	iWinSizeX;
		unsigned int	iWinSizeY;
		bool			isWindowed;
	}GRAPHIC_DESC;

	typedef struct tagLineIndices16
	{
		unsigned short		_0, _1;
	}LINEINDICES16;

	typedef struct tagLineIndices32
	{
		unsigned long		_0, _1;
	}LINEINDICES32;

	typedef struct tagFaceIndices16
	{
		unsigned short		_0, _1, _2;
	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long		_0, _1, _2;
	}FACEINDICES32;

	typedef struct
	{		
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexcoord; /* 텍스쳐의 상대적인 좌표.(0 ~ 1) */
		D3DXVECTOR3		vTexcoord1; /* 텍스쳐의 상대적인 좌표.(0 ~ 1) */
	}VTXTEX;
	
	typedef struct tagVertexCubeTexture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexture;
	}VTXCUBETEX;

	typedef struct
	{
		D3DXVECTOR3 vPosition;
		D3DCOLOR	Color;
	}VTXCOLOR;

	typedef struct tagRayDesc
	{
		unsigned int		iLevel; // 현재 레벨 
		std::wstring		strDstLayer; // 레이 피킹될 오브젝트 레이어 Tag
		D3DXVECTOR3			vRayWorldPos; // 레이 시작점의 월드좌표
		D3DXVECTOR3			vRayDir; // 레이 월드 방향
		void*				pArg = { nullptr }; // 추가로 넣어야 할 정보 ex) 데미지등등
	}RAY_DESC;
}
