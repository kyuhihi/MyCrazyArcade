#include "stdafx.h"
#include "Stage2.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "Block.h"
#include "Monster.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "CollisionMgr.h"
#include "KeyMgr.h"
#include "Zelda.h"
#include "Stage2_Button.h"
#include "Boss.h"
#include<iostream>
CStage2::CStage2()
{
}


CStage2::~CStage2()
{
	Release();
}

void CStage2::Initialize(void)
{
	g_bStage2 = true;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage1.bmp", L"Stage1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackBuffer.bmp", L"BackBuffer");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Main_Needle.bmp", L"Main_Needle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Main_Zelda_Needle.bmp", L"Zelda_Main_Needle");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/win.bmp", L"win");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/lose.bmp", L"lose");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Shield.bmp", L"Shield");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/등장.bmp", L"등장");



	if (!CObjMgr::Get_Instance()->IsPlayer())
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
	}
	if (g_bPet)
	{//젤다 생성.
		CObjMgr::Get_Instance()->Add_Object(OBJ_ZELDA, CAbstractFactory<CZelda>::Create());
	}
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster>::Create());
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOSS, CAbstractFactory<CBoss>::Create());

	m_Main_Needle.left = 662.f;
	m_Main_Needle.right = 721.f;
	m_Main_Needle.bottom = 532.f;
	m_Main_Needle.top = 482.f;

	m_fNeedleCX = 59.f;
	m_fNeedleCY = 50.f;

	m_Main_ShieldCX = 38;
	m_Main_ShieldiX = 243;
	m_Main_ShieldiY = 565;

	m_Main_Zelda_Needle_Rect.left = 731.f;
	m_Main_Zelda_Needle_Rect.top = 502.f;

	m_ZeldaNeedleDrawRect.left = 726.f;
	m_ZeldaNeedleDrawRect.top = 492.f;
	m_ZeldaNeedleDrawRect.right = 772.f;
	m_ZeldaNeedleDrawRect.bottom = 532.f;

	m_fZelda_NeedleCX = 36.f;
	m_fZelda_NeedleCY = 30.f;

	SetBlock();
	//CTileMgr::Get_Instance()->Initialize();
	m_bGameLose = false;
	m_bGameWin = false;
	m_bFallingComplete = false;
	m_bBossInitialComplete = false;
	m_pFrameKey = L"Stage1";
	CSoundMgr::Get_Instance()->PlaySoundW(L"Game_Start.wav", SOUND_UI, g_fSound);
	m_MusicStartTime = GetTickCount();
	m_Lose_CX = 276;
	m_Lose_CY = 52;

	m_Win_CX = 276;
	m_Win_CY = 56;

	m_MiddleSpotX = 320;
	m_MiddleSpotY = 233;
	m_bMusicStart = false;
	m_bFinalSound = false;
	m_AppearTime = 50000;
	m_bBossAppear = false;
	m_tAppearRect.left = 260.f;
	m_tAppearRect.right = 380.f;
	m_tAppearRect.top = 281.f;
	m_tAppearRect.bottom = 319.f;//300인데 38 19 //120 38
	//300 320
	m_BossFallingPoint.x = 300.f;
	m_BossFallingPoint.y = 320.f;
	m_fSpearDistance = 3.f;
	

}



void CStage2::Update(void)
{
	
	if ((!m_bMusicStart) && (m_MusicStartTime + 3000 < GetTickCount())) {
		CSoundMgr::Get_Instance()->PlaySoundW(L"Forest.wav", SOUND_BGM, g_fSound);
		m_bMusicStart = true;
	}

	if ((!m_bBossAppear)&&(CCollisionMgr::m_bButtonPress))
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);

		CSoundMgr::Get_Instance()->PlaySoundW(L"Mushroom_Up.wav", SOUND_EFFECT, g_fSound);
		m_AppearTime = GetTickCount();
		m_bBossAppear = true;
	}
	if ((50000 != m_AppearTime) && (m_bBossAppear) && (m_AppearTime + 3000 < GetTickCount()))
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);

		CSoundMgr::Get_Instance()->PlaySoundW(L"BossShadow.wav", SOUND_EFFECT, g_fSound);

		m_AppearTime = 50000;
		dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->SetBossFalling();
	}
	if (dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->GetAppear())
	{//여기서 풍덩소리
		CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);

		CSoundMgr::Get_Instance()->PlaySoundW(L"kuuung.wav", SOUND_EFFECT, g_fSound);
		m_bFallingComplete = true;
	}
	if (m_bFallingComplete)
		BombEffect();


	if ((CCollisionMgr::m_bBreakBubble) && (CCollisionMgr::m_NotDieTime + 3000 < GetTickCount()))
	{
		CCollisionMgr::m_bBreakBubble = false;
		CCollisionMgr::m_NotDieTime = 50000;
	}

	
	UpdateUIRect();
	int iResult = CObjMgr::Get_Instance()->Update();

	if (!m_bFinalSound &&iResult == GAME_LOSE)
	{
		m_bFinalSound = true;
		CSoundMgr::Get_Instance()->StopAll();

		m_bGameLose = true;
		CSoundMgr::Get_Instance()->PlaySoundW(L"Game_LoseSound.wav", SOUND_BGM, g_fSound);

	}
	else if (!m_bFinalSound && iResult == GAME_WIN)
	{
		m_bFinalSound = true;
		CSoundMgr::Get_Instance()->StopAll();
		m_bGameWin = true;
		CSoundMgr::Get_Instance()->PlaySoundW(L"Game_Win.wav", SOUND_BGM, g_fSound);
	}

}

void CStage2::BombEffect()
{
	if (m_bBossInitialComplete)
		return;
	if (m_fSpearDistance >= 2500.f)
	{
		m_bBossInitialComplete = true;
		dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->m_bEffectComplete = true;
		return;
	}
	

	CObjMgr::Get_Instance()->VanishBlock();
	m_tBossFallingEffect.left = LONG(m_BossFallingPoint.x - m_fSpearDistance);
	m_tBossFallingEffect.top = LONG(m_BossFallingPoint.y - m_fSpearDistance);
	m_tBossFallingEffect.right = LONG(m_BossFallingPoint.x + m_fSpearDistance);
	m_tBossFallingEffect.bottom = LONG(m_BossFallingPoint.y + m_fSpearDistance);
	m_fSpearDistance += 10.f;
}

void CStage2::UpdateUIRect()
{
	m_Win_Left = int(m_MiddleSpotX - (m_Win_CX * 0.5));
	m_Win_top = int(m_MiddleSpotY - (m_Win_CY * 0.5));
	m_Win_right = int(m_MiddleSpotX + (m_Win_CX * 0.5));
	m_Win_bottom = int(m_MiddleSpotY + (m_Win_CY * 0.5));

	m_Lose_Left = int(m_MiddleSpotX - (m_Lose_CX * 0.5));
	m_Lose_top = int(m_MiddleSpotY - (m_Lose_CY * 0.5));
	m_Lose_right = int(m_MiddleSpotX + (m_Win_CX * 0.5));
	m_Lose_bottom = int(m_MiddleSpotY + (m_Win_CY * 0.5));
}


void CStage2::Late_Update(void)
{
	if (!m_bBossInitialComplete)
		m_pFrameKey = L"Stage1";
	else
		m_pFrameKey = L"BackBuffer";

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
				{//여기서 클리어 문을 추가하자.
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
		if (595.f > m_tInfo.fY && 555.f < m_tInfo.fY && 645.f<m_tInfo.fX&& 770.f>m_tInfo.fX)
		{//나가기
			CSceneMgr::Get_Instance()->Scene_Change(SC_WAITING);
		}
	}
}


void CStage2::Render(HDC hDC)
{
	HDC		hGroundMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	BitBlt(hDC, 0, 0, WINCX, WINCY, hGroundMemDC, 0, 0, SRCCOPY);
	//CTileMgr::Get_Instance()->Render(hDC);
	if (CObjMgr::Get_Instance()->IsPlayer())
	{
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
	}
	if ((g_bPet)&&(dynamic_cast<CZelda*>(CObjMgr::Get_Instance()->Get_Zelda())->Get_Zelda_Needle())) {
		SelectObject(hDC, GetStockObject(DC_BRUSH));
		SetDCBrushColor(hDC, RGB(115, 109, 113));
		Rectangle(hDC, m_ZeldaNeedleDrawRect.left, m_ZeldaNeedleDrawRect.top, m_ZeldaNeedleDrawRect.right, m_ZeldaNeedleDrawRect.bottom);
		SetDCBrushColor(hDC, RGB(255, 255, 255));
		HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Zelda_Main_Needle");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_Main_Zelda_Needle_Rect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_Main_Zelda_Needle_Rect.top),
			m_fZelda_NeedleCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			m_fZelda_NeedleCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			m_fZelda_NeedleCX, m_fZelda_NeedleCY,
			RGB(255, 255, 255));			// 제거하고자 하는 색상
	}
	if (CObjMgr::Get_Instance()->IsPlayer())
	{
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetPlayerShield()) {

			HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shield");
			GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
				int(m_Main_ShieldiX),	// 2,3 인자 :  복사받을 위치 X, Y
				int(m_Main_ShieldiY),	// 2,3 인자 :  복사받을 위치 X, Y),
				m_Main_ShieldCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
				m_Main_ShieldCX,
				hMemDC,							// 비트맵을 가지고 있는 DC
				0, 0,
				m_Main_ShieldCX, m_Main_ShieldCX,
				RGB(255, 255, 255));			// 제거하고자 하는 색상
		}
	}

	CObjMgr::Get_Instance()->Render(hDC);
	if (m_AppearTime != 50000)
	{
		HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"등장");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_tAppearRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_tAppearRect.top),	// 2,3 인자 :  복사받을 위치 X, Y),
			120,			// 4,5 인자 : 복사받을 가로, 세로 길이
			38,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			120, 38,
			RGB(255, 255, 255));			// 제거하고자 하는 색상
	}

	else if ((m_AppearTime == 50000) && (m_bFallingComplete)&&(!m_bBossInitialComplete))
	{
		SelectObject(hDC, GetStockObject(DC_BRUSH));
		SetDCBrushColor(hDC, RGB(255, 192, 113));
		Ellipse(hDC, int(m_tBossFallingEffect.left), int(m_tBossFallingEffect.top), int(m_tBossFallingEffect.right), int(m_tBossFallingEffect.bottom));
		SetDCBrushColor(hDC, RGB(255, 255, 255));
	}

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
	
	
}

void CStage2::Release(void)
{	//사운드 해제해야함.
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
	CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
	CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
	CCollisionMgr::m_bButtonPress = false;
	//CObjMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Destroy_Instance();

	g_bStage2 = false;

}

void CStage2::SetBlock()
{
	////일반블럭
	CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(80.f, 60.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK, 0, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(40.f, 100.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK, 0, true));

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			if (i == 0)
			{				//블럭이 없는 곳은 컨티뉴로 처리한다.
				if (j == 0 || j == 1 || j == 4 || j == 5 || j == 6 || j == 8 || j == 9 || j == 10 || j == 12 || j == 13 || j == 14)
					continue;
			}
			else if (i == 1) {
				if (j == 0 || j == 1 || j == 2 || j == 3 || j == 5 || j == 7 || j == 9 || j == 11 || j == 13||j == 14)
					continue;
			}
			else if (i == 2) {
				if (j == 2 || j == 4 || j == 6 || j == 8 || j == 10 || j == 12)
					continue;
			}
			else if (i == 3) {
				if (j == 1 || j == 3 || j == 11 || j == 13)
					continue;
			}
			else if (i == 4) {
				if (j == 1 || j == 13)
					continue;
			}
			else if (i == 5) {
				if (j == 1 || j == 3 || j == 11 || j == 13)
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
				if (j == 3 || j == 11)
					continue;
			}
			else if (i == 9) {
				if (j == 4 || j == 6 || j == 8 || j == 11)
					continue;
			}
			else if (i == 10) {
				if (j == 2 || j == 5 || j == 9 || j == 12)
					continue;
			}
			else if (i == 11) {
				if (j == 1 || j == 6 || j == 7 || j == 8 || j == 13 || j == 14)
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
				if ((j == 4)  || (j == 6) || (j == 8) || (j == 9) || (j == 10)||(j==12))
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1,true));
				else
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0,true));

			}
			else if (i == 2)
			{
				if ((j == 3) || (j == 7) || (j == 11))
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, true));
				else
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, true));
			}
			else if (i == 3)
			{
				if ((j == 2) || (j == 12))
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, true));
				else
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, true));
			}
			else if (i == 4)
			{
				if ((j == 2) || (j == 12))
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, true));
				else
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, true));
			}
			else if (i == 5)
			{
				if ((j == 2) || (j == 12))
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, true));
				else
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, true));
			}
			else if (i == 6)
			{
				if ((j == 2) || (j == 12))
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, true));
				else
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, true));
			}
			else if (i == 7)
			{
				if ((j == 3) || (j == 11))
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, true));
				else
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, true));
			}
			else if (i == 8)
			{
				if ((j == 4) || (j == 10))
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, true));
				else
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, true));
			}
			else if (i == 9)
			{
				if ((j == 5) || (j == 9))
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, true));
				else
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, true));
			}
			else if (i == 10)
			{
				if ((j == 6) || (j == 7) || (j == 8))
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 1, true));
				else
					CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, true));
			}
			else
				CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(fX, fY, OBJ_BLOCK, BLOCK_CAN_BREAK, 0, true));
		}
	}
	//파랑버섯.

	//m_BulletSpot.fX = 40.f;
	//m_BulletSpot.fY = 60.f;

	CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, CAbstractFactory<CStage2_Button>::Create(80.f, 100.f, OBJ_BUTTON));
	
	
	
	CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(80.f, 500.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK, 0, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(560.f, 100.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK, 0, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(560.f, 500.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK, 0, true));


	CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(280.f, 300.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK, 0, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(360.f, 300.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK, 0, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(320.f, 300.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK, 0, true));



}
