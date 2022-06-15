#include "stdafx.h"
#include "Player.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "Bullet.h"
#include "Pet.h"
#include "SoundMgr.h"
#include "Zelda.h"

CPlayer::CPlayer()
	: m_eCurState(IDLE), m_ePreState(END)
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize(void)
{
	m_CashUpTime = 50000;
	m_tInfo.fX = 40.f;
	m_tInfo.fY = 48.f;
	//m_tInfo.fX = 40.f;
	//m_tInfo.fY = 60.f;

	m_fDrawCX = 70.f;
	m_fDrawCY = 70.f;

	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;


	m_StdTile.fX = 40.f;
	m_StdTile.fY = 48.f;

	m_BulletSpot.fX = 40.f;
	m_BulletSpot.fY = 60.f;
	
	m_fSpeed = 3.f;
	
	m_bNeedle	= false;
	m_bShoes	= false;
	m_bShield = false;
	m_iWaterPower = 1;

	m_PangTime = 50000;
	m_DeadTime = 50000;

	m_eRender = RENDER_GAMEOBJECT;
	m_iAppendCash = 0;
	m_iBulletCapacity = 1;
	m_iBulletMax = m_iBulletCapacity;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_DOWN.bmp", L"Player_DOWN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_UP.bmp", L"Player_UP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_LEFT.bmp", L"Player_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_RIGHT.bmp", L"Player_RIGHT");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_STAND.bmp", L"Player_STAND");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_Bubble.bmp", L"Player_BUBBLE");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/PlayerShield.bmp", L"PlayerShield");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/ZeldaHelp.bmp", L"ZeldaHelp");

	m_SOSTime = 0;

	m_pFrameKey = L"Player_STAND";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();

	m_ShieldTime = 0;

	m_ZeldaHelpCX = 70.f;
	m_ZeldaHelpCY = 30.f;

}

void CPlayer::SetShieldTime()
{
	m_ShieldTime = GetTickCount();
	m_bShield = false;
}

int CPlayer::Update(void)
{
	if (m_ShieldTime != 0 && m_ShieldTime + 2500<GetTickCount())
	{
		m_ShieldTime = 0;
	}
	if (m_SOSTime != 0 && m_SOSTime + 1000<GetTickCount())
	{
		m_SOSTime = 0;
	}

	if (m_PangTime != 50000)
	{
		if (m_PangTime + 3000 < GetTickCount())
		{
			Set_Player_State(PANGBUBBLE);
			m_PangTime = 50000;
			m_DeadTime = GetTickCount();
		}
	}
	else if (m_DeadTime != 50000)
	{
		if (m_DeadTime +  1000< GetTickCount())
		{
			m_bDead = true;
		}
	}
	
	if (m_bDead)
		return OBJ_DEAD;

	if (m_eCurState != PANGBUBBLE)
	{
		Key_Input();
	}

	Draw_Update_Rect();

	Player_Update_Rect();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update(void)
{
	Motion_Change();
	Move_Frame();
	
	float iDistanceX = m_StdTile.fX - m_tInfo.fX;
	float iDistanceY = m_StdTile.fY - m_tInfo.fY;
	if ( iDistanceX < -20) { //플레이어 오른쪽으로 갈때
		m_BulletSpot.fX += TILECX;
		m_StdTile.fX += TILECX;
	}
	else if (iDistanceX > 20) {//플레이어 왼쪽으로갈때
		if (m_BulletSpot.fX > 40) {
			m_BulletSpot.fX -= TILECX;
			m_StdTile.fX -= TILECX;
		}

	}
	else if (iDistanceY > 20) {//플레이어 위로갈때
		if (m_BulletSpot.fY > 60) {
			m_BulletSpot.fY -= TILECX;
			m_StdTile.fY -= TILECX;
		}
	}
	else if (iDistanceY < -20) { //플레이어 아래로갈때
		//if (m_BulletSpot.fY > 60) {

		m_BulletSpot.fY += TILECX;
		m_StdTile.fY += TILECX;

	}
	if ((m_CashUpTime != 50000) && (m_CashUpTime + 500 < GetTickCount()))
	{
		m_CashUpTime = 50000;
	}


}

void CPlayer::Render(HDC hDC)
{

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	
	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		int(m_DrawRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
		int(m_DrawRect.top),
		(int)m_fDrawCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
		(int)m_fDrawCY,
		hMemDC,							// 비트맵을 가지고 있는 DC
		m_tFrame.iFrameStart * (int)m_fDrawCX,								// 비트맵 출력 시작 좌표, X,Y
		m_tFrame.iMotion * (int)m_fDrawCY,
		(int)m_fDrawCX, (int)m_fDrawCY,
		RGB(0, 255, 0));			// 제거
	if (m_CashUpTime != 50000)
	{
		wsprintf(CashUp, TEXT("%d"), int(m_iAppendCash));				//printf %d 인트 &s 문자열 %f 플롯
		SetBkMode(hDC, TRANSPARENT);  //
		SetTextColor(hDC, RGB(255, 215, 0));
		TextOut(hDC, (int)m_DrawRect.left, (int)m_DrawRect.top, CashUp, lstrlen(CashUp));
	}
	if (m_ShieldTime != 0)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"PlayerShield");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_DrawRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_DrawRect.top),
			m_fDrawCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			m_fDrawCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			m_fDrawCX, m_fDrawCY,
			RGB(0, 255, 0));			// 제거하고자 하는 색상
	}

	if (m_SOSTime != 0)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ZeldaHelp");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_DrawRect.left - 10),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_DrawRect.top + 30),
			m_ZeldaHelpCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			m_ZeldaHelpCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			m_ZeldaHelpCX, m_ZeldaHelpCY,
			RGB(255, 255, 255));			// 제거하고자 하는 색상
	}

	
}
void CPlayer::Release(void)
{

}

void CPlayer::Key_Input(void)
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		if (m_eCurState == WHILEBUBBLE)
		{
			if (m_tInfo.fX >= 40) {
				m_tInfo.fX -= 1.f;
			}
			m_eCurState = WHILEBUBBLE;
			
		}
		else if (m_tInfo.fX >= 40) {
			m_tInfo.fX -= m_fSpeed;
			m_pFrameKey = L"Player_LEFT";
			m_eCurState = RIGHT_LEFT;
			if (g_bPet&&(!g_bStage2))
				dynamic_cast<CPet*>(CObjMgr::Get_Instance()->Get_Pet())->Set_Pet_State(DIR_LEFT, m_tInfo.fX, m_tInfo.fY);
		}
		if (g_bPet && (g_bStage2))
			dynamic_cast<CZelda*>(CObjMgr::Get_Instance()->Get_Zelda())->Set_Zelda_Dir(DIR_RIGHT);
		
	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		if (m_eCurState == WHILEBUBBLE)
		{
			if (m_tInfo.fX <= 600) {
				m_tInfo.fX += 1.f;
			}
			m_eCurState = WHILEBUBBLE;
			
		}

		else if (m_tInfo.fX <= 600) {
			m_tInfo.fX += m_fSpeed;
			m_pFrameKey = L"Player_RIGHT";
			m_eCurState = RIGHT_LEFT;
			if(g_bPet && (!g_bStage2))
				dynamic_cast<CPet*>(CObjMgr::Get_Instance()->Get_Pet())->Set_Pet_State(DIR_RIGHT, m_tInfo.fX, m_tInfo.fY);
		}
		if (g_bPet && (g_bStage2))
			dynamic_cast<CZelda*>(CObjMgr::Get_Instance()->Get_Zelda())->Set_Zelda_Dir(DIR_LEFT);
		
	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		if (m_eCurState == WHILEBUBBLE)
		{
			if (m_tInfo.fY >= 60) {
				m_tInfo.fY -= 1.f;
			}
			m_eCurState = WHILEBUBBLE;
			
		}
		else if (m_tInfo.fY >= 60) {
			m_tInfo.fY -= m_fSpeed;
			m_pFrameKey = L"Player_UP";
			m_eCurState = UP_DOWN;
			if (g_bPet && (!g_bStage2))
				dynamic_cast<CPet*>(CObjMgr::Get_Instance()->Get_Pet())->Set_Pet_State(DIR_UP, m_tInfo.fX, m_tInfo.fY);
		}
		if (g_bPet && (g_bStage2))
			dynamic_cast<CZelda*>(CObjMgr::Get_Instance()->Get_Zelda())->Set_Zelda_Dir(DIR_DOWN);
	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		if (m_eCurState == WHILEBUBBLE)
		{
			if (m_tInfo.fY <= 540) {
				m_tInfo.fY += 1.f;
			}
			m_eCurState = WHILEBUBBLE;
		}
		else if (m_tInfo.fY <= 540) {
			m_tInfo.fY += m_fSpeed;
			m_pFrameKey = L"Player_DOWN";
			m_eCurState = UP_DOWN;
			if (g_bPet && (!g_bStage2))
				dynamic_cast<CPet*>(CObjMgr::Get_Instance()->Get_Pet())->Set_Pet_State(DIR_DOWN, m_tInfo.fX, m_tInfo.fY);
		}
		if (g_bPet && (g_bStage2))
			dynamic_cast<CZelda*>(CObjMgr::Get_Instance()->Get_Zelda())->Set_Zelda_Dir(DIR_UP);	
	}
	else
	{
		if (g_bPet && (g_bStage2))
			dynamic_cast<CZelda*>(CObjMgr::Get_Instance()->Get_Zelda())->Set_Zelda_Dir(DIR_END);
	}
	

	 if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		 if (m_eCurState != WHILEBUBBLE)
		 {
			 if (m_iBulletCapacity > 0) {
				 CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CBullet>::Create(m_BulletSpot.fX, m_BulletSpot.fY, OBJ_BULLET, m_iWaterPower, DIR_END, BOMB_PLAYER));
				 --m_iBulletCapacity;
			 }
			 if (g_bPet && (g_bStage2))
				 dynamic_cast<CZelda*>(CObjMgr::Get_Instance()->Get_Zelda())->Set_Zelda_Bomb();
		 }
		
	}
	 if (CKeyMgr::Get_Instance()->Key_Down(VK_CONTROL))
	 {
		 if (m_eCurState == WHILEBUBBLE && m_bNeedle)
		 {
			 CSoundMgr::Get_Instance()->PlaySoundW(L"dart.wav", SOUND_EFFECT, g_fSound);

			 m_bNeedle = false;
			 m_pFrameKey = L"Player_BUBBLE";

			 m_PangTime = 50000;
			 m_eCurState = BREAKBUBBLE;
		 }
		 else if (m_eCurState == WHILEBUBBLE && (!m_bNeedle) && (g_bPet) && (g_bStage2))
		 {
			 if (dynamic_cast<CZelda*>(CObjMgr::Get_Instance()->Get_Zelda())->SOS())
			 {//젤다가 살렸음.
				 CSoundMgr::Get_Instance()->PlaySoundW(L"ZeldaHelp.wav", SOUND_EFFECT, g_fSound);
				 m_SOSTime = GetTickCount();
				 m_pFrameKey = L"Player_BUBBLE";

				 m_PangTime = 50000;
				 m_eCurState = BREAKBUBBLE;
			 }
		 }

	 }
	
}



void CPlayer::Motion_Change(void)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 500;
			m_tFrame.dwTime = GetTickCount();
			break;

		case RIGHT_LEFT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			break;
		case UP_DOWN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			break;
		case WHILEBUBBLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 1000;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"Player_BUBBLE";
			break;
		case BREAKBUBBLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 1;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"Player_BUBBLE";
			break;
		case PANGBUBBLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 2;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"Player_BUBBLE";
			break;
		case WIN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 3;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"Player_BUBBLE";

			break;
		}

		m_ePreState = m_eCurState;
	}
}


void CPlayer::Draw_Update_Rect()
{
	m_DrawRect.left = LONG(m_tInfo.fX - (m_fDrawCX * 0.5f));
	m_DrawRect.top = LONG(m_tInfo.fY - (m_fDrawCY * 0.5f));
	m_DrawRect.right = LONG(m_tInfo.fX + (m_fDrawCX* 0.5f));
	m_DrawRect.bottom = LONG(m_tInfo.fY + (m_fDrawCY * 0.5f));
}

void CPlayer::Player_Update_Rect()
{
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCX * 0.5f));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCY* 0.5f));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f) );
}

void CPlayer::Set_Append_Item(ITEMTYPE eType)
{
	CSoundMgr::Get_Instance()->PlaySoundW(L"ItemAppend.wav", SOUND_EFFECT, g_fSound);
	if (eType == ITEM_WATERBOMB) {
		if (m_iBulletMax < 7) {
			++m_iBulletMax;
			++m_iBulletCapacity;
		}
	}
	else if (eType == ITEM_ROLLER) {
		m_fSpeed += 0.2f;
	}
	else if (eType == ITEM_MEDICINE) {
		if (m_iWaterPower < 6) {
			++m_iWaterPower;
		}
	}
	else if (eType == ITEM_NEEDLE)
		m_bNeedle = true;
	else if (eType == ITEM_SHOES)
		m_bShoes = true;
	else if (eType == ITEM_SHIELD)
		m_bShield = true;
	else if (eType == ITEM_MONEY_COPPER) {
		g_iCash += 300;
		m_iAppendCash = 300;
		m_CashUpTime = GetTickCount();
	}
	else if (eType == ITEM_MONEY_COPPER) {
		g_iCash += 500;
		m_iAppendCash = 500;
		m_CashUpTime = GetTickCount();
	}
	else if (eType == ITEM_MONEY_GOLD) {
		g_iCash += 5000;
		m_iAppendCash = 5000;
		m_CashUpTime = GetTickCount();
	}


}

void CPlayer::SetMiniGameResult(ItemResult NewItem)
{
	m_iWaterPower += NewItem._iMedicine;
	if (m_iWaterPower > 5)
		m_iWaterPower = 5;
	m_fSpeed += (NewItem._iRoller * 0.2f);
	m_iBulletMax += NewItem._iWaterBomb;
	if (m_iBulletMax > 6)
		m_iBulletMax = 6;
	m_iBulletCapacity = m_iBulletMax;

	
}