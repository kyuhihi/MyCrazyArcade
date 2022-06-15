#include "stdafx.h"
#include "Pet.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "Bullet.h"
CPet::CPet()
{
}


CPet::~CPet()
{
	Release();

}

void CPet::Initialize(void)
{
	m_eRender = RENDER_GAMEOBJECT;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Link.bmp", L"Player_Pet");

	m_tInfo.fX = 35.f;
	m_tInfo.fY = 48.f;
	m_pFrameKey = L"Player_Pet";
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 30.f;
	m_BulletTime = GetTickCount();

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
	m_iWaterPower = 1;

}

int CPet::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;


	Update_Rect();

	return OBJ_NOEVENT;
}

void CPet::Late_Update(void)
{
	Motion_Change();
	Move_Frame();
	if (m_BulletTime + 5000 < GetTickCount()) {
		CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY, OBJ_BULLET, m_iWaterPower, DIR_END, BOMB_PET));
		m_BulletTime = GetTickCount();
	}
}

void CPet::Render(HDC hDC)
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

	//GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
	//	int(m_tRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
	//	int(m_tRect.top),
	//	TILECX,			// 4,5 인자 : 복사받을 가로, 세로 길이
	//	TILECY,
	//	hMemDC,							// 비트맵을 가지고 있는 DC
	//	0, 0,
	//	TILECX, TILECY,
	//	RGB(255, 255, 255));			// 제거하고자 하는 색상
}

void CPet::Release(void)
{
}

void CPet::Set_Pet_State(DIRECTION eDir, float _fX, float _fY)
{
	m_eCurDirection = eDir;
	if (eDir == DIR_LEFT)
	{
		m_tInfo.fX = _fX + 20.f;
		m_tInfo.fY = _fY;
	}
	else if (eDir == DIR_RIGHT)
	{
		m_tInfo.fX = _fX - 20.f;
		m_tInfo.fY = _fY;
	}
	else if (eDir == DIR_UP)
	{
		m_tInfo.fX = _fX ;
		m_tInfo.fY = _fY+ 20.f;
	}
	else if (eDir == DIR_DOWN)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY - 20.f;
	}
}

void CPet::Motion_Change(void)
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
			m_tFrame.iMotion = 5;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"Player_Pet";
			break;
		}

		m_ePreDirection = m_eCurDirection;
	}
}
