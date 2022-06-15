#pragma once

#define		WINCX		800
#define		WINCY		600



#define		PURE		= 0


#define		OBJ_NOEVENT  0
#define		OBJ_DEAD	 1

#define		VK_MAX		 0xff

#define		TILECX		40
#define		TILECY		40

#define		TILEX		15
#define		TILEY		13

#define		BOMB_PLAYER		100
#define		BOMB_MONSTER	-100
#define		BOMB_PET		1000
#define		BOMB_ZELDA		3000
#define		BOMB_BOSS		-6000

#define		GAME_LOSE		10000
#define		GAME_WIN		-10000

#define		BLOCK_CX		77

#define RGB(r,g,b) ((COLORREF)(((BYTE)(r) | ((WORD)((BYTE)(g))<<8)) | (((DWORD)(BYTE)(b))<<16)))


//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif

extern HWND			g_hWnd;

extern int			g_iCash;
extern bool			g_bPet;
extern bool			g_bStage2;

extern float		g_fSound;
