#include "stdafx.h"
#include "Zelda.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "Bullet.h"
#include "SoundMgr.h"
#include "Tile.h"

CZelda::CZelda()
{
}


CZelda::~CZelda()
{
}

void CZelda::Initialize(void)
{
	m_eRender = RENDER_GAMEOBJECT;
	m_ShieldCreateTime = GetTickCount();
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Link.bmp", L"Player_Pet");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/arrow.bmp", L"arrow");

	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 30.f;

	m_tInfo.fX = 600.f;
	m_tInfo.fY = 48.f;

	m_StdTile.fX = m_tInfo.fX;
	m_StdTile.fY = m_tInfo.fY;

	m_BulletSpot.fX = 600.f;
	m_BulletSpot.fY = 60.f;
	m_ArrowCX = 60;
	m_ArrowCY = 80;
	

	m_fSpeed = 5.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
	m_iWaterPower = 1;
	m_pFrameKey = L"Player_Pet";

	m_iBulletCapacity = 1;
	m_iBulletMax = m_iBulletCapacity;

	m_bNeedle = true;
	m_bShoes = true;
	m_tInfo.eObjType = OBJ_ZELDA;

	
}
void CZelda::ShieldToPlayer()
{
	if (m_ShieldCreateTime + 30000 < GetTickCount()) {
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY, OBJ_ITEM, ITEM_SHIELD));
		m_ShieldCreateTime = GetTickCount();
	}
}
int CZelda::Update(void)
{
	ShieldToPlayer();
	if (m_bDead)
		return OBJ_DEAD;

	NailDown();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CZelda::NailDown(void)
{
	if (m_eDir == DIR_END)
		return;
	else if (m_eDir == DIR_LEFT) {
		if (m_tInfo.fX >= 40) {
			m_tInfo.fX -= m_fSpeed;
		}
	}
	else if (m_eDir == DIR_RIGHT) {
		if (m_tInfo.fX <= 600) {
			m_tInfo.fX += m_fSpeed;
		}
	}
	else if (m_eDir == DIR_UP) {
		if (m_tInfo.fY >= 60) {
			m_tInfo.fY -= m_fSpeed;
		}
	}
	else if (m_eDir == DIR_DOWN) {
		if (m_tInfo.fY <= 540) {
			m_tInfo.fY += m_fSpeed;
		}
	}

}


void CZelda::Late_Update(void)
{
	Motion_Change();
	Move_Frame();
	float iDistanceX = m_StdTile.fX - m_tInfo.fX;
	float iDistanceY = m_StdTile.fY - m_tInfo.fY;
	if (iDistanceX < -20) { //플레이어 오른쪽으로 갈때
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
	
}

void CZelda::Set_Zelda_Dir(DIRECTION eDir)
{
	m_eDir = eDir;
	m_eCurDirection = eDir;
}
void CZelda::Set_Zelda_Bomb()
{
	if (m_iBulletCapacity > 0) {
		CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CBullet>::Create(m_BulletSpot.fX, m_BulletSpot.fY, OBJ_BULLET, m_iWaterPower, DIR_END, BOMB_ZELDA));
	}
}
void CZelda::Motion_Change(void)
{
	if (m_ePreDirection != m_eCurDirection)
	{
		switch (m_eCurDirection)
		{
		case DIR_LEFT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iMotion = 5;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"Player_Pet";
			break;
		case DIR_RIGHT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iMotion = 7;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"Player_Pet";
			break;
		case DIR_UP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iMotion = 6;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"Player_Pet";
			break;
		case DIR_DOWN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iMotion = 4;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"Player_Pet";
			break;
		}

		m_ePreDirection = m_eCurDirection;
	}
}

void CZelda::Set_Append_Item(ITEMTYPE eType)
{
	CSoundMgr::Get_Instance()->PlaySoundW(L"ItemAppend.wav", SOUND_EFFECT, g_fSound);
	
	if (eType == ITEM_NEEDLE)
		m_bNeedle = true;
	else if (eType == ITEM_SHOES)
		m_bShoes = true;


}

void CZelda::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		int(m_tRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
		int(m_tRect.top),
		(int)m_tInfo.fCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
		(int)m_tInfo.fCY,
		hMemDC,							// 비트맵을 가지고 있는 DC
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,								// 비트맵 출력 시작 좌표, X,Y
		m_tFrame.iMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY,
		RGB(255, 255, 255));			// 제거
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"arrow");
	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		int(m_tRect.left-13),	// 2,3 인자 :  복사받을 위치 X, Y
		int(m_tRect.top - 60),
		(int)m_ArrowCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
		(int)m_ArrowCX,
		hMemDC,							// 비트맵을 가지고 있는 DC
								// 비트맵 출력 시작 좌표, X,Y
					0,0,
		(int)m_ArrowCX, (int)m_ArrowCX,
		RGB(255, 255, 255));			// 제거
}

void CZelda::Release(void)
{
}
