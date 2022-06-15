#include "stdafx.h"
#include "Stage.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "Block.h"
#include "Monster.h"
#include "SceneMgr.h"
#include "Pet.h"
#include "SoundMgr.h"
#include "CollisionMgr.h"
#include "KeyMgr.h"

CStage::CStage()
{
}


CStage::~CStage()
{
	Release();
}

void CStage::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackBuffer.bmp", L"BackBuffer");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Main_Needle.bmp", L"Main_Needle");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/win.bmp", L"win");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/lose.bmp", L"lose");

	if (!CObjMgr::Get_Instance()->IsPlayer())
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
	}
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster>::Create());
	if(g_bPet)
		CObjMgr::Get_Instance()->Add_Object(OBJ_PET, CAbstractFactory<CPet>::Create());

	m_Main_Needle.left = 662.f;
	m_Main_Needle.right = 721.f;
	m_Main_Needle.bottom = 532.f;
	m_Main_Needle.top = 482.f;
	m_fNeedleCX = 59.f;
	m_fNeedleCY = 50.f;
	SetBlock();
	//CTileMgr::Get_Instance()->Initialize();
	m_bGameLose = false;
	m_bGameWin = false;
	CSoundMgr::Get_Instance()->PlaySoundW(L"Game_Start.wav", SOUND_UI, g_fSound);
	m_MusicStartTime = GetTickCount();
	m_Lose_CX = 276;
	m_Lose_CY = 52;

	m_Win_CX = 276;
	m_Win_CY = 56;

	m_MiddleSpotX = 320;
	m_MiddleSpotY = 233;
	m_bMusicStart = false;
	m_bResult = false;
}



void CStage::Update(void)
{
	if ((!m_bMusicStart)&&(m_MusicStartTime + 3000 < GetTickCount())) {
		CSoundMgr::Get_Instance()->PlaySoundW(L"Pirate.wav", SOUND_BGM, g_fSound);
		m_bMusicStart = true;
	}
	if ((CCollisionMgr::m_bBreakBubble) && (CCollisionMgr::m_NotDieTime + 3000 < GetTickCount()))
	{
		CCollisionMgr::m_bBreakBubble = false;
		CCollisionMgr::m_NotDieTime = 50000;
	}
	//CTileMgr::Get_Instance()->Update();
	UpdateUIRect();
	int iResult =CObjMgr::Get_Instance()->Update();
	if ((m_bResult == false)&&(iResult == GAME_LOSE))
	{
		m_bResult = true;
		m_bGameLose = true;
		CSoundMgr::Get_Instance()->PlaySoundW(L"Game_LoseSound.wav", SOUND_MONSTER, g_fSound);

	}
	else if ((m_bResult == false) && (iResult == GAME_WIN))
	{
		m_bResult = true;
		m_bGameWin = true;
		CSoundMgr::Get_Instance()->PlaySoundW(L"Game_Win.wav", SOUND_MONSTER, g_fSound);
	}

}
void CStage::UpdateUIRect()
{
	m_Win_Left = m_MiddleSpotX - (m_Win_CX * 0.5);
	m_Win_top = m_MiddleSpotY - (m_Win_CY * 0.5);
	m_Win_right = m_MiddleSpotX + (m_Win_CX * 0.5);
	m_Win_bottom = m_MiddleSpotY + (m_Win_CY * 0.5);

	m_Lose_Left = m_MiddleSpotX - (m_Lose_CX * 0.5);
	m_Lose_top = m_MiddleSpotY - (m_Lose_CY * 0.5);
	m_Lose_right = m_MiddleSpotX + (m_Win_CX * 0.5);
	m_Lose_bottom = m_MiddleSpotY + (m_Win_CY * 0.5);
}


void CStage::Late_Update(void)
{

	CObjMgr::Get_Instance()->Late_Update();

	POINT	pt{};
	GetCursorPos(&pt);	// 현재 마우스의 위치 좌표를 얻어오는 함수
	ScreenToClient(g_hWnd, &pt);	// 전체 스크린영역에서 생성한 클라이언트(창) 좌표로 변환

	m_tInfo.fX = (float)pt.x;
	m_tInfo.fY = (float)pt.y;


	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		CSoundMgr::Get_Instance()->PlaySoundW(L"MousePicking.wav", SOUND_UI, g_fSound);
		if (m_bGameWin || m_bGameLose)
		{
			if (m_bGameWin) {
				if (m_Win_bottom > m_tInfo.fY&& m_Win_top < m_tInfo.fY&& m_Win_Left<m_tInfo.fX&&m_Win_right>m_tInfo.fX)
				{
					CSceneMgr::Get_Instance()->Scene_Change(SC_WAITING);
				}
			}
			if (m_bGameLose) {
				if (m_Lose_bottom > m_tInfo.fY&& m_Lose_top < m_tInfo.fY && m_Lose_Left<m_tInfo.fX&& m_Lose_right>m_tInfo.fX)
				{
					CSceneMgr::Get_Instance()->Scene_Change(SC_WAITING);
				}
			}
		}
		else if (595.f > m_tInfo.fY && 555.f < m_tInfo.fY && 645.f<m_tInfo.fX&& 770.f>m_tInfo.fX)
		{//나가기
			CSceneMgr::Get_Instance()->Scene_Change(SC_WAITING);
		}
	}
}


void CStage::Render(HDC hDC)
{
	HDC		hGroundMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BackBuffer");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hGroundMemDC, 0, 0, SRCCOPY);
	//CTileMgr::Get_Instance()->Render(hDC);
	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_PlayerNeedle()) {
		HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Main_Needle");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_Main_Needle.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_Main_Needle.top),
			m_fNeedleCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			m_fNeedleCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			m_fNeedleCX, m_fNeedleCY,
			RGB(255, 255, 255));			// 제거하고자 하는 색상
	}
	CObjMgr::Get_Instance()->Render(hDC);
	if (m_bGameWin)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"win");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			m_Win_Left,	// 2,3 인자 :  복사받을 위치 X, Y
			m_Win_top,
			m_Win_CX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			m_Win_CY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			m_Win_CX, m_Win_CY,
			RGB(255, 255, 255));			// 제거하고자 하는 색상
	}
	else if (m_bGameLose)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"lose");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			m_Lose_Left,	// 2,3 인자 :  복사받을 위치 X, Y
			m_Lose_top,
			m_Lose_CX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			m_Lose_CY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			m_Lose_CX, m_Lose_CY,
			RGB(255, 255, 255));			// 제거하고자 하는 색상
	}
	//wsprintf(m_MousePoint, TEXT("X: %d Y : %d"), int(m_tInfo.fX), int(m_tInfo.fY));				//printf %d 인트 &s 문자열 %f 플롯
	//TextOut(hDC, (int)m_tInfo.fX - 10, (int)m_tInfo.fY - 10, m_MousePoint, lstrlen(m_MousePoint));
}

void CStage::Release(void)
{	//사운드 해제해야함.
	/*CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
	CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
	CSoundMgr::Get_Instance()->StopSound(SOUND_UI);*/
	CSoundMgr::Get_Instance()->StopAll();
	//CObjMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Destroy_Instance();

}

void CStage::SetBlock()
{
	////일반블럭
		for (int i = 0; i < TILEY; ++i)
		{
			for (int j = 0; j < TILEX; ++j)
			{
				if (i == 0)
				{				//블럭이 없는 곳은 컨티뉴로 처리한다.
					if (j == 0 || j == 1 || j == 4 || j == 5 || j == 6 || j == 8 || j == 9 || j == 10)
						continue;
				}
				else if (i == 1) {
					if (j == 0 || j == 1 || j == 3 || j == 7 || j == 11 || j == 13)
						continue;
				}
				else if (i == 2) {
					if (j == 2 || j == 4 || j == 6 || j == 8 || j == 10 || j == 12)
						continue;
				}
				else if (i ==3) {
					if (j == 1 || j == 3 || j == 11 || j == 13 )
						continue;
				}
				else if (i == 4) {
					if (j == 1 ||  j == 13)
						continue;
				}
				else if (i == 5) {
					if (j == 1 || j == 3 || j == 11 || j == 13 )
						continue;
				}
				else if (i == 6) {
					if (j == 1 || j == 6 || j == 7 || j == 8 || j == 13)
						continue;
				}
				else if (i == 7) {
					if (j == 2 || j == 4 || j == 10 || j == 12)
						continue;
				}
				else if (i == 8) {
					if (j == 3 || j == 11 )
						continue;
				}
				else if (i == 9) {
					if (j == 4 || j == 6 || j == 8 || j == 11)
						continue;
				}
				else if (i == 10) {
					if (j == 2 || j == 5||j == 9||j==12)
						continue;
				}
				else if (i == 11) {
					if (j == 1 || j == 6 || j == 7 || j == 8  ||j==13||j==14)
						continue;
				}
				else if (i == 12) {
					if (j == 13 || j == 14)
						continue;
				}
				float		fX = (TILECX >> 1) + float(TILECX * j) + 20;// 20 40더하면됨  바닥 사이즈는40 40
				float		fY = (TILECY >> 1) + float(TILECY * i) + 40;
				
				if (i == 1)
				{
					if((j == 4)||(j== 5)||(j == 6)||(j == 8)||(j == 9)||(j ==10))
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1,false));
					else
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0,false));

				}
				else if (i == 2)
				{
					if ((j == 3) || (j == 7) || (j == 11))
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1,false));
					else
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0,false));
				}
				else if (i == 3)
				{
					if ((j == 2) || (j == 12))
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1,false));
					else
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0,false));
				}
				else if (i == 4)
				{
					if ((j == 2) || (j == 12))
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1,false));
					else
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, false));
				}
				else if (i == 5)
				{
					if ((j == 2) || (j == 12))
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, false));
					else
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, false));
				}
				else if (i == 6)
				{
					if ((j == 2) || (j == 12))
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, false));
					else
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, false));
				}
				else if (i == 7)
				{
					if ((j == 3) || (j == 11))
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, false));
					else
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, false));
				}
				else if (i == 8)
				{
					if ((j == 4) || (j == 10))
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, false));
					else
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, false));
				}
				else if (i == 9)
				{
					if ((j == 5) || (j == 9))
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, false));
					else
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, false));
				}
				else if (i == 10)
				{
					if ((j == 6) || (j == 7)||(j==8))
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, false));
					else
						CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, false));
				}
				else
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, false));

			}
		}
		// 배묶어두는 기둥
		CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(80.f, 100.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK,0, false));
		CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(80.f, 500.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK,0, false));
		CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(560.f, 100.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK,0, false));
		CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(560.f, 500.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK,0, false));
		//플래그
		CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(320.f, 300.f, OBJ_BLOCK, BLOCK_FLAG,0, false));

	


}
 