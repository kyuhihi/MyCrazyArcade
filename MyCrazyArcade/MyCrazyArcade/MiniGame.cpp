#include "stdafx.h"
#include "MiniGame.h"
#include "BmpMgr.h"
#include "C2048BlockMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "SoundMgr.h"

CMiniGame::CMiniGame()
{
}


CMiniGame::~CMiniGame() 
{
	Release();
}

void CMiniGame::Initialize(void)
{
	CSoundMgr::Get_Instance()->PlaySoundW(L"MiniGame.wav", SOUND_BGM, g_fSound);

	m_Over = false;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Waiting/Waiting.bmp", L"Waiting_Main");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/MiniGame_BackGround.bmp", L"MiniGame_BackGround");

	C2048BlockMgr::Get_Instance()->Initialize();
	if (!CObjMgr::Get_Instance()->IsPlayer())
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
	}
	Result._iMedicine = 0;
	Result._iRoller = 0;
	Result._iWaterBomb = 0;
}

void CMiniGame::Update(void)
{
	Result = C2048BlockMgr::Get_Instance()->Update();
	if ((!m_Over)&&(Result._iMedicine != 0) && (Result._iWaterBomb != 0) && (Result._iRoller != 0))
	{
		m_Over = true;
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetMiniGameResult(Result);
	}
}

void CMiniGame::Late_Update(void)
{
	C2048BlockMgr::Get_Instance()->Late_Update();

}

void CMiniGame::Render(HDC hDC)
{
	HDC		hGroundMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Waiting_Main");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hGroundMemDC, 0, 0, SRCCOPY);
	hGroundMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MiniGame_BackGround");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hGroundMemDC, 0, 0, SRCCOPY);
	C2048BlockMgr::Get_Instance()->Render(hDC);
}

void CMiniGame::Release(void)
{
	CSoundMgr::Get_Instance()->StopAll();
	C2048BlockMgr::Get_Instance()->Destroy_Instance();
}
