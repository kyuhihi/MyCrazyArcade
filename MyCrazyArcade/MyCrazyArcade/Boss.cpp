#include "stdafx.h"
#include "Boss.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "Bullet.h"
#include "SoundMgr.h"
#include "BmpMgr.h"
#include <random>
#include <iostream>
CBoss::CBoss()
	:  m_eCurState(IDLE), m_ePreState(END), m_eCurDirection(DIR_DOWN), m_ePreDirection(DIR_END)
{
}


CBoss::~CBoss()
{
	Release();
}

void CBoss::Initialize(void)
{
	m_SpawnBulletTime = 50000;
	m_bAppear = false;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/BossDown.bmp", L"BossDown");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/BossLeft.bmp", L"BossLeft");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/BossRight.bmp", L"BossRight");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/BossUp.bmp", L"BossUp");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/등장.bmp", L"등장");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/BossHit.bmp", L"BossHit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/BossPang.bmp", L"BossPang");
	m_eRender = RENDER_GAMEOBJECT;

	m_tInfo.fCX = 80.f;
	m_tInfo.fCY = 80.f;
	m_ModeDelayTime = 50000;

	m_tInfo.fX = 320.f;
	//m_tInfo.fY = 300.f;
	m_tInfo.fY = -500.f;
	m_iHp = 5;
	m_fDrawCX = 92.f;
	m_fDrawCY = 142.f;
	m_iWaterPower = 2;
	m_tInfo.eObjType = OBJ_BOSS;
	m_fSpeed = 20.f;
	m_pFrameKey = L"BossDown";
	m_bInitialFalling = false;
	m_bModeCasting = false;
	m_bEffectComplete = false;
	m_iMulJulgySetting = 1;
	m_ModeCastingTime = 50000;
	
	m_iBulletSpawnCount = 0;

	for (int i = 0; i < 10; ++i)
	{
		m_RaingSpot[i].fX = -1;
		m_RaingSpot[i].fY = -1;
	}
	RainingBombSpotSet();
	m_PangTime = 50000;
	m_BossNotDieTime = 50000;
}
void CBoss::BossAppear()
{
	if (m_bAppear)
		return;
	if (m_bInitialFalling) {
		if (!m_bAppear)
		{
			if (m_tInfo.fY != 300.f)
				m_tInfo.fY += m_fSpeed;
			else if (m_tInfo.fY == 300.f) {
				m_bAppear = true;
				m_eMode = MODE_WATERFLOW;
				m_fSpeed = 15.f;
			}
		}
	}
}

void CBoss::RainingBombSpotSet()
{
	for (int i = 0; i < 10; ++i)
	{
		if ((m_RaingSpot[i].fX == -1) && (m_RaingSpot[i].fY == -1))
		{
			random_device rdX;
			mt19937 genX(rdX());
			random_device rdY;
			mt19937 genY(rdY());
			uniform_int_distribution<int> disX(0, 14);
			uniform_int_distribution<int> disY(0, 12);
			m_RaingSpot[i].fX = float(40 + (disX(genX) *  TILECX));
			m_RaingSpot[i].fY = float(60 + (disY(genY)) * TILECX);
		}
	}
}

void CBoss::Set_Minus_Hp()
{
	CSoundMgr::Get_Instance()->PlaySoundW(L"bossHitSound.wav", SOUND_EFFECT, g_fSound);

	if(m_iHp > 0)
	{
		cout << m_iHp << endl;
		--m_iHp;

		Set_Boss_State(BOSS_HIT);
		m_BossNotDieTime = GetTickCount();
	}
	else
	{
		if (m_PangTime == 50000)
		{
			m_PangTime = GetTickCount();
		}
	}
}
int CBoss::Update(void)
{
	if ((m_BossNotDieTime != 50000) && (m_BossNotDieTime + 2000 < GetTickCount()))
	{
		m_BossNotDieTime = 50000;
		Set_Boss_State(IDLE);
		m_eCurDirection = DIR_DOWN;
	}

	if (m_PangTime != 50000)
	{
		if (m_PangTime + 3000 < GetTickCount())
		{
			Set_Boss_State(WHILEBUBBLE); 
		}
	}

	BossAppear();
	DeletePath();

	RainingBombSpotSet();

	if (m_bModeCasting&&m_ModeCastingTime == 50000)
	{
		m_ModeCastingTime = GetTickCount();
	}
	else if (m_bModeCasting && (m_ModeCastingTime != 50000) && m_ModeCastingTime + 2000 < GetTickCount())
	{
		m_ModeCastingTime = 50000;
		m_bModeCasting = false;
	}

	

	if (!m_bModeCasting&&(m_bEffectComplete)&&(m_BossNotDieTime == 50000)&&(m_eCurState!= WHILEBUBBLE)&&(m_eCurState == IDLE)) {
		Mode_Select();
	}
	if (m_bDead)
		return OBJ_DEAD;

	NailDown();
	FollowDirection();

	Direction_Change();
	Draw_Update_Rect();
	Update_Rect();
	//60 19
	m_tAppearRect.left = 260.f;
	m_tAppearRect.right = 380.f;
	m_tAppearRect.top = 281.f;
	m_tAppearRect.bottom = 319.f;//300인데 38 19 //120 38
	return OBJ_NOEVENT;
}

void CBoss::DeletePath()
{
	if (!m_tTarget.empty()) {
		auto iter = m_tTarget.begin();
		float TempX = ((*iter)->fX ); //움직여야하는 중심 좌표.
		float TempY = ((*iter)->fY );
		if (((LONG)TempY< m_tRect.bottom) && ( LONG(TempY) > m_tRect.top) && ((LONG)TempX > m_tRect.left) && ((LONG)TempX < m_tRect.right))
		{//이땐 해당하는 리스트의 노드를 지워야함.
			Safe_Delete<PlayerTile*>(*iter);
			m_tTarget.pop_front();
		}
	}
}

void CBoss::Mode_Select()
{
	if (m_ModeDelayTime == 50000)
	{
		m_ModeDelayTime = GetTickCount();
		return;
	}
	else if (((m_ModeDelayTime != 50000) && (m_ModeDelayTime + 3000 < GetTickCount()))||(m_eMode == MODE_RAINBOMB))
	{
		switch (m_eMode) {
		case MODE_RUN:
			RunSetting();
			break;
		case MODE_WATERFLOW:
			MulJulgySetting();
			break;
		case MODE_RAINBOMB:
			RaingBomb();
			break;
		}
		//m_bModeCasting = true;
		m_ModeDelayTime = 50000;

	}
	

	
}

void CBoss::RunSetting()
{
	random_device rd;
	mt19937_64 rng(rd());
	uniform_int_distribution<__int64> dist(1, 6);
	int irandomDir = dist(rng);
	if (irandomDir == 5)
	{//사방.
		//300	320
		PlayerTile* Temp1 = new PlayerTile(40.f, 300.f);
		m_tTarget.push_back(Temp1);
		PlayerTile* Temp2 = new PlayerTile(300.f, 560.f);
		m_tTarget.push_back(Temp2);
		PlayerTile* Temp3 = new PlayerTile(600.f, 300.f);
		m_tTarget.push_back(Temp3);
		PlayerTile* Temp4 = new PlayerTile(300.f, 60.f);
		m_tTarget.push_back(Temp4);
	}
	else if ((irandomDir % 2) == 0)
	{//좌우
		PlayerTile* Temp1 = new PlayerTile(40.f, m_tInfo.fY);
		m_tTarget.push_back(Temp1);
		PlayerTile* Temp2 = new PlayerTile(600.f, m_tInfo.fY);
		m_tTarget.push_back(Temp2);
		PlayerTile* Temp3 = new PlayerTile(40.f, m_tInfo.fY);
		m_tTarget.push_back(Temp3);
		PlayerTile* Temp4 = new PlayerTile(600.f, m_tInfo.fY);
		m_tTarget.push_back(Temp4);
	}
	else if ((irandomDir % 2) == 1)
	{//상하
		PlayerTile* Temp1 = new PlayerTile(m_tInfo.fX, 60.f);
		m_tTarget.push_back(Temp1);
		PlayerTile* Temp2 = new PlayerTile(m_tInfo.fX, 560.f);
		m_tTarget.push_back(Temp2);
		PlayerTile* Temp3 = new PlayerTile(m_tInfo.fX, 60.f);
		m_tTarget.push_back(Temp3);
		PlayerTile* Temp4 = new PlayerTile(m_tInfo.fX, 560.f);
		m_tTarget.push_back(Temp4);
	}
	m_eMode = MODE_RAINBOMB;
	m_bModeCasting = true;

}

void CBoss::MulJulgySetting()
{
	if (m_iMulJulgySetting % 2 == 1) {
		float fXLeft(40.f);
		float fXRight(600.f);
		for (int i = 0; i < ((TILEX - 1) / 2); ++i)
		{//왼 오
			float fY(60.f);
			for (int j = 0; j < TILEY; ++j)
			{//j가 y열.
				CObjMgr::Get_Instance()->Add_Object(OBJ_MULJULGY, CAbstractFactory<CMulJulgy>::Create(fXRight, fY, OBJ_MULJULGY, m_iWaterPower, DIR_LEFT, BOMB_BOSS));
				CObjMgr::Get_Instance()->Add_Object(OBJ_MULJULGY, CAbstractFactory<CMulJulgy>::Create(fXLeft, fY, OBJ_MULJULGY, m_iWaterPower, DIR_RIGHT, BOMB_BOSS));

				fY += TILECX;
			}
			fXLeft += TILECX;
			fXRight -= TILECX;
		}
		++m_iMulJulgySetting;
	}
	else if (m_iMulJulgySetting % 2 == 0)
	{
		float fXLeft(300.f);
		float fXRight(300.f);
		for (int i = 0; i < ((TILEX + 1) / 2); ++i)
		{//상 하
			float fY(60.f);
			for (int j = 0; j < TILEY; ++j)
			{//j가 y열.
				CObjMgr::Get_Instance()->Add_Object(OBJ_MULJULGY, CAbstractFactory<CMulJulgy>::Create(fXRight, fY, OBJ_MULJULGY, m_iWaterPower, DIR_UP, BOMB_BOSS));
				CObjMgr::Get_Instance()->Add_Object(OBJ_MULJULGY, CAbstractFactory<CMulJulgy>::Create(fXLeft, fY, OBJ_MULJULGY, m_iWaterPower, DIR_DOWN, BOMB_BOSS));
				fY += TILECX;
			}
			fXLeft -= TILECX;
			fXRight += TILECX;
		}
		++m_iMulJulgySetting;
		m_eMode = MODE_RUN;
		m_bModeCasting = true;
	}
	

}

void CBoss::RaingBomb()
{
	if ((m_SpawnBulletTime == 50000))
	{
		m_SpawnBulletTime = GetTickCount();
	}
	else if((m_SpawnBulletTime != 50000)&&(m_SpawnBulletTime + 80 < GetTickCount()))
	{
		m_SpawnBulletTime = GetTickCount();
		float fX(CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX);
		float fY(CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY);
		CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CBullet>::Create(fX, fY, OBJ_BULLET, m_iWaterPower, DIR_END, BOMB_BOSS));
		CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CBullet>::Create(m_RaingSpot[m_iBulletSpawnCount].fX, m_RaingSpot[m_iBulletSpawnCount].fY, OBJ_BULLET, m_iWaterPower, DIR_END, BOMB_BOSS));
		m_RaingSpot[m_iBulletSpawnCount].fX = -1;
		m_RaingSpot[m_iBulletSpawnCount].fY = -1;
		CSoundMgr::Get_Instance()->PlaySoundW(L"BombSetSound.wav", SOUND_EFFECT, g_fSound);
		++m_iBulletSpawnCount;
	}
	if (m_iBulletSpawnCount == 9)
	{
		m_iBulletSpawnCount = 0;
		m_SpawnBulletTime = 50000;
		m_eMode = MODE_RUN;
		m_bModeCasting = true;
	}

}

void CBoss::Draw_Update_Rect()
{
	m_DrawRect.left = LONG(m_tInfo.fX - (m_fDrawCX * 0.5f));
	m_DrawRect.top = LONG(m_tInfo.fY - (m_fDrawCY * 0.5f));
	m_DrawRect.right = LONG(m_tInfo.fX + (m_fDrawCX* 0.5f));
	m_DrawRect.bottom = LONG(m_tInfo.fY + (m_fDrawCY * 0.5f));
}


void CBoss::Late_Update(void)
{
	Motion_Change();
	Move_Frame();
}

void CBoss::Render(HDC hDC)
{

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	if (!m_bDead) {

		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_DrawRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_DrawRect.top),
			(int)m_fDrawCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)m_fDrawCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			m_tFrame.iFrameStart * (int)m_fDrawCX,								// 비트맵 출력 시작 좌표, X,Y
			m_tFrame.iMotion * (int)m_fDrawCY,
			(int)m_fDrawCX, (int)m_fDrawCY,
			RGB(0, 255, 0));
	}
	else
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"등장");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_tRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_tRect.top),	// 2,3 인자 :  복사받을 위치 X, Y),
			120,			// 4,5 인자 : 복사받을 가로, 세로 길이
			38,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			120, 38,
			RGB(255, 255, 255));			// 제거하고자 하는 색상
	}
}

void CBoss::Release(void)
{
	if (!m_tTarget.empty()) {
		for (auto& iter : m_tTarget) {
			Safe_Delete<PlayerTile*>(iter);
			iter++;
		}
		m_tTarget.clear();
	}
}

void CBoss::Motion_Change(void)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case WHILEBUBBLE:
			m_fDrawCX = 83.f;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 2000;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"BossPang";
			break;
		case BOSS_HIT:
			m_fDrawCX = 95.f;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"BossHit";
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CBoss::Direction_Change()
{
	if (m_eCurState == IDLE) {

		if (m_ePreDirection != m_eCurDirection)
		{
			switch (m_eCurDirection)
			{
			case DIR_LEFT:
				m_fDrawCX = 80.f;
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_pFrameKey = L"BossLeft";
				break;
			case DIR_RIGHT:
				m_fDrawCX = 80.f;
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_pFrameKey = L"BossRight";
				break;
			case DIR_DOWN:
				m_fDrawCX = 92.f;
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_pFrameKey = L"BossDown";
				break;
			case DIR_UP:
				m_fDrawCX = 92.f;
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_pFrameKey = L"BossUp";
				break;
			}

			m_ePreDirection = m_eCurDirection;
		}
	}


}

void CBoss::NailDown()
{
	if (!m_tTarget.empty())
	{
		PlayerTile* TargetTemp(m_tTarget.front());
		float fDistanceX((*TargetTemp).fX -m_tInfo.fX);
		float fDistanceY((*TargetTemp).fY - m_tInfo.fY);
		if (abs(fDistanceX) >= abs(fDistanceY))
		{//좌우를 먼저 움직인다.
			if (fDistanceX < 0)
			{//좌
				m_eCurDirection = DIR_LEFT;
			}
			else
			{
				m_eCurDirection = DIR_RIGHT;
			}
		}
		else
		{
			if (fDistanceY < 0)
			{
				m_eCurDirection = DIR_UP;
			}
			else
			{
				m_eCurDirection = DIR_DOWN;
			}
		}
	}
	else
	{
		m_eCurDirection = DIR_END;
	}
}

void CBoss::FollowDirection()
{
	if (m_eCurDirection == DIR_END)
		return;
	if (m_eCurDirection == DIR_LEFT) {
		m_tInfo.fX -= m_fSpeed;
		return;
	}
	if (m_eCurDirection == DIR_RIGHT) {
		m_tInfo.fX += m_fSpeed;
		return;
	}
	if (m_eCurDirection == DIR_UP) {
		m_tInfo.fY -= m_fSpeed;
		return;
	}
	if (m_eCurDirection == DIR_DOWN) {
		m_tInfo.fY += m_fSpeed;

		return;
	}
}