#include "stdafx.h"
#include "Bullet.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "MulJulgy.h"
#include "Player.h"
#include "Monster.h"
#include "Zelda.h"
#include "Tile.h"
#include "SoundMgr.h"
CBullet::CBullet()
{
}


CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize(void)
{
	m_tTargetTile.iX = -1;
	m_tTargetTile.iY = -1;

	m_bBulletUpdateRect = false;
	m_iBulletTouchCount = 0;
	m_Stage1CannotBreak.left = LONG(260);
	m_Stage1CannotBreak.top = LONG(280);
	m_Stage1CannotBreak.right = LONG(380);
	m_Stage1CannotBreak.bottom = LONG(320);

	m_eRender = RENDER_GAMEOBJECT;

	m_fDrawCX = 46.f;
	m_fDrawCY = 46.f;
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;
	m_fSpeed = 5.f;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/BlueBub.bmp", L"WaterBullet");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/MonBullet.bmp", L"MonsterBullet");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/ZeldaBomb.bmp", L"ZeldaBomb");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/BossBullet.bmp", L"BossBullet");

	if (m_WhosBullet == BOMB_PLAYER || m_WhosBullet == BOMB_PET) {
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 200;
		m_tFrame.dwTime = GetTickCount();
		m_PangTime = GetTickCount();
		m_pFrameKey = L"WaterBullet";
	}
	else if (m_WhosBullet == BOMB_MONSTER)
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 2;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 200;
		m_tFrame.dwTime = GetTickCount();
		m_PangTime = GetTickCount();
		m_pFrameKey = L"MonsterBullet";
	}
	else if (m_WhosBullet == BOMB_ZELDA)
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 200;
		m_tFrame.dwTime = GetTickCount();
		m_PangTime = GetTickCount();
		m_pFrameKey = L"ZeldaBomb";
	}
	else if (m_WhosBullet == BOMB_BOSS)
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 2;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 200;
		m_tFrame.dwTime = GetTickCount();
		m_PangTime = GetTickCount();
		m_pFrameKey = L"BossBullet";
	}
	m_bMove = false;
	CSoundMgr::Get_Instance()->PlaySoundW(L"BombSetSound.wav", SOUND_EFFECT, g_fSound);

	CObjMgr::Get_Instance()->SetWater_Spot(int((m_tInfo.fX - 40) / TILECX), int((m_tInfo.fY - 60) / TILECX), true);
	for (int i = 1; i <= m_iWaterPower; ++i)
	{//왼
		float fX(m_tInfo.fX - TILECX * i);
		float fY(m_tInfo.fY);
		if (fX <= 0.f)
			break;
		if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
			break;
		}
		if ((m_Stage1CannotBreak.bottom > fY) && (m_Stage1CannotBreak.top < fY) && (m_Stage1CannotBreak.left < fX) && (m_Stage1CannotBreak.right < fX))
			break;
		CObjMgr::Get_Instance()->SetWater_Spot(int((fX - 40) / TILECX), int((fY - 60) / TILECX), true);
		/*if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
			break;
		}*/

	}
	for (int i = 1; i <= m_iWaterPower; ++i)
	{//위
		float fX(m_tInfo.fX);
		float fY(m_tInfo.fY - TILECX * i);
		if (fY <= 20.f)
			break;
		if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
			break;
		}
		if ((m_Stage1CannotBreak.bottom >= fY) && (m_Stage1CannotBreak.top <= fY) && (m_Stage1CannotBreak.left <= fX) && (m_Stage1CannotBreak.right >= fX))
			break;
		CObjMgr::Get_Instance()->SetWater_Spot(int((fX - 40) / TILECX), int((fY - 60) / TILECX), true);
		/*if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
			break;
		}*/

	}
	for (int i = 1; i <= m_iWaterPower; ++i)
	{//아래
		float fX(m_tInfo.fX);
		float fY(m_tInfo.fY + TILECX * i);
		if (fY >= 580.f)
			break;
		if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
			break;
		}
		if ((m_Stage1CannotBreak.bottom >= fY) && (m_Stage1CannotBreak.top <= fY) && (m_Stage1CannotBreak.left <= fX) && (m_Stage1CannotBreak.right >= fX))
			break;
		CObjMgr::Get_Instance()->SetWater_Spot(int((fX - 40) / TILECX), int((fY - 60) / TILECX), true);
		/*if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
			break;
		}*/

	}
	for (int i = 1; i <= m_iWaterPower; ++i)
	{//오
		float fX(m_tInfo.fX + TILECX * i);
		float fY(m_tInfo.fY);
		if (fX >= 640.f)
			break;

		if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
			break;
		}
		if ((m_Stage1CannotBreak.bottom >= fY) && (m_Stage1CannotBreak.top <= fY) && (m_Stage1CannotBreak.left <= fX) && (m_Stage1CannotBreak.right >= fX))
			break;
		CObjMgr::Get_Instance()->SetWater_Spot(int((fX - 40) / TILECX), int((fY - 60) / TILECX), true);
	}
}

void CBullet::ReSetBulletSpot(int iMode)
{
	if (iMode == 1)
	{
		CObjMgr::Get_Instance()->SetWater_Spot(int((m_tInfo.fX - 40) / TILECX), int((m_tInfo.fY - 60) / TILECX), true);
		for (int i = 1; i <= m_iWaterPower; ++i)
		{//왼
			float fX(m_tTargetTile.iX - TILECX * i);
			float fY(m_tTargetTile.iY);
			if (fX <= 0.f)
				break;
			if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
				break;
			}
			if ((m_Stage1CannotBreak.bottom > fY) && (m_Stage1CannotBreak.top < fY) && (m_Stage1CannotBreak.left < fX) && (m_Stage1CannotBreak.right < fX))
				break;
			CObjMgr::Get_Instance()->SetWater_Spot(int((fX - 40) / TILECX), int((fY - 60) / TILECX), true);
			/*if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
			break;
			}*/

		}
		for (int i = 1; i <= m_iWaterPower; ++i)
		{//위
			float fX(m_tTargetTile.iX);
			float fY(m_tTargetTile.iY - TILECX * i);
			if (fY <= 20.f)
				break;
			if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
				break;
			}
			if ((m_Stage1CannotBreak.bottom >= fY) && (m_Stage1CannotBreak.top <= fY) && (m_Stage1CannotBreak.left <= fX) && (m_Stage1CannotBreak.right >= fX))
				break;
			CObjMgr::Get_Instance()->SetWater_Spot(int((fX - 40) / TILECX), int((fY - 60) / TILECX), true);
			/*if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
			break;
			}*/

		}
		for (int i = 1; i <= m_iWaterPower; ++i)
		{//아래
			float fX(m_tTargetTile.iX);
			float fY(m_tTargetTile.iY + TILECX * i);
			if (fY >= 580.f)
				break;
			if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
				break;
			}
			if ((m_Stage1CannotBreak.bottom >= fY) && (m_Stage1CannotBreak.top <= fY) && (m_Stage1CannotBreak.left <= fX) && (m_Stage1CannotBreak.right >= fX))
				break;
			CObjMgr::Get_Instance()->SetWater_Spot(int((fX - 40) / TILECX), int((fY - 60) / TILECX), true);
			/*if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
			break;
			}*/

		}
		for (int i = 1; i <= m_iWaterPower; ++i)
		{//오
			float fX(m_tTargetTile.iX + TILECX * i);
			float fY(m_tTargetTile.iY);
			if (fX >= 640.f)
				break;

			if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
				break;
			}
			if ((m_Stage1CannotBreak.bottom >= fY) && (m_Stage1CannotBreak.top <= fY) && (m_Stage1CannotBreak.left <= fX) && (m_Stage1CannotBreak.right >= fX))
				break;
			CObjMgr::Get_Instance()->SetWater_Spot(int((fX - 40) / TILECX), int((fY - 60) / TILECX), true);
		}
	}
	else
	{
		CObjMgr::Get_Instance()->SetWater_Spot(int((m_tInfo.fX - 40) / TILECX), int((m_tInfo.fY - 60) / TILECX), false);
		for (int i = 1; i <= m_iWaterPower; ++i)
		{//왼
			float fX(m_tInfo.fX - TILECX * i);
			float fY(m_tInfo.fY);
			if (fX <= 0.f)
				break;
			if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
				break;
			}
			if ((m_Stage1CannotBreak.bottom > fY) && (m_Stage1CannotBreak.top < fY) && (m_Stage1CannotBreak.left < fX) && (m_Stage1CannotBreak.right < fX))
				break;
			CObjMgr::Get_Instance()->SetWater_Spot(int((fX - 40) / TILECX), int((fY - 60) / TILECX), false);
			/*if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
			break;
			}*/

		}
		for (int i = 1; i <= m_iWaterPower; ++i)
		{//위
			float fX(m_tInfo.fX);
			float fY(m_tInfo.fY - TILECX * i);
			if (fY <= 20.f)
				break;
			if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
				break;
			}
			if ((m_Stage1CannotBreak.bottom >= fY) && (m_Stage1CannotBreak.top <= fY) && (m_Stage1CannotBreak.left <= fX) && (m_Stage1CannotBreak.right >= fX))
				break;
			CObjMgr::Get_Instance()->SetWater_Spot(int((fX - 40) / TILECX), int((fY - 60) / TILECX), false);
			/*if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
			break;
			}*/

		}
		for (int i = 1; i <= m_iWaterPower; ++i)
		{//아래
			float fX(m_tInfo.fX);
			float fY(m_tInfo.fY + TILECX * i);
			if (fY >= 580.f)
				break;
			if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
				break;
			}
			if ((m_Stage1CannotBreak.bottom >= fY) && (m_Stage1CannotBreak.top <= fY) && (m_Stage1CannotBreak.left <= fX) && (m_Stage1CannotBreak.right >= fX))
				break;
			CObjMgr::Get_Instance()->SetWater_Spot(int((fX - 40) / TILECX), int((fY - 60) / TILECX), false);
			/*if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
			break;
			}*/

		}
		for (int i = 1; i <= m_iWaterPower; ++i)
		{//오
			float fX(m_tInfo.fX + TILECX * i);
			float fY(m_tInfo.fY);
			if (fX >= 640.f)
				break;

			if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
				break;
			}
			if ((m_Stage1CannotBreak.bottom >= fY) && (m_Stage1CannotBreak.top <= fY) && (m_Stage1CannotBreak.left <= fX) && (m_Stage1CannotBreak.right >= fX))
				break;
			CObjMgr::Get_Instance()->SetWater_Spot(int((fX - 40) / TILECX), int((fY - 60) / TILECX), false);
		}
	}
}

void CBullet::BlockMove()
{
	if ((!m_bMove) && (m_eDir != DIR_END) && (m_tTargetTile.iX == -1))
	{
		CSoundMgr::Get_Instance()->PlaySoundW(L"kick.wav", SOUND_EFFECT, g_fSound);

		m_PangTime = GetTickCount();
		ReSetBulletSpot(0);
		m_bMove = true;
		if (m_eDir == DIR_RIGHT) {
			m_tTargetTile.iY = int(m_tInfo.fY);
			for (int i = 0; i < 13; ++i)
			{
				if (CObjMgr::Get_Instance()->If_Block(int((m_tInfo.fX - 40 + (TILECX *i)) / TILECX ), int((m_tInfo.fY - 60) / TILECX)))
				{//우
					m_tTargetTile.iX = int(m_tInfo.fX + ((i - 1) * 40));
					break;
				}
			}
		}
		else if (m_eDir == DIR_LEFT) {
			m_tTargetTile.iY = int(m_tInfo.fY);
			for (int i = 0; i < 13; ++i)
			{
				if (CObjMgr::Get_Instance()->If_Block(int((m_tInfo.fX - 40 - (TILECX *i)) / TILECX), int((m_tInfo.fY - 60) / TILECX)))
				{//좌
					m_tTargetTile.iX = int(m_tInfo.fX - ((i - 1) * 40));
					break;
				}
			}
		}
		else if (m_eDir == DIR_UP) {
			m_tTargetTile.iX = int(m_tInfo.fX);
			for (int i = 0; i < 11; ++i)
			{
				if (CObjMgr::Get_Instance()->If_Block(int((m_tInfo.fX - 40) / TILECX), int((m_tInfo.fY - 60 - (TILECX *i)) / TILECX )))
				{//상
					m_tTargetTile.iY = int(m_tInfo.fY - ((i - 1) * 40));
					break;
				}
			}

		}
		else if (m_eDir == DIR_DOWN) {
			m_tTargetTile.iX = int(m_tInfo.fX);
			for (int i = 0; i < 11; ++i)
			{
				if (CObjMgr::Get_Instance()->If_Block(int((m_tInfo.fX - 40) / TILECX), int((m_tInfo.fY - 60 + (TILECX *i)) / TILECX )))
				{//하
					m_tTargetTile.iY = int(m_tInfo.fY + ((i - 1) * 40));
					break;
				}
			}
		}
		ReSetBulletSpot(1);

	}
	if ((m_bMove) && (m_eDir != DIR_END))
	{
		if (int(m_tInfo.fX) != m_tTargetTile.iX)
		{
			if (m_eDir == DIR_RIGHT)
				m_tInfo.fX += m_fSpeed;
			else if (m_eDir == DIR_LEFT)
				m_tInfo.fX -= m_fSpeed;
		}
		if (int(m_tInfo.fY) != m_tTargetTile.iY)
		{
			if (m_eDir == DIR_UP)
				m_tInfo.fY -= m_fSpeed;
			else if (m_eDir == DIR_DOWN)
				m_tInfo.fY += m_fSpeed;
		}
		if ((int(m_tInfo.fY) == m_tTargetTile.iY) && (int(m_tInfo.fX) == m_tTargetTile.iX))
		{
			m_eDir = DIR_END;
			m_bMove = false;
			m_tTargetTile.iX = -1;
			m_tTargetTile.iY = -1;
		}
	}
}
int CBullet::Update(void)
{
	BlockMove();

	if (m_bDead) {
		if (m_WhosBullet == BOMB_ZELDA)
			CObjMgr::Get_Instance()->Add_Object(OBJ_TILE, CAbstractFactory<CTile>::Create(m_tInfo.fX, m_tInfo.fY, OBJ_TILE));
		CObjMgr::Get_Instance()->Add_Object(OBJ_MULJULGY, CAbstractFactory<CMulJulgy>::Create(m_tInfo.fX, m_tInfo.fY, OBJ_MULJULGY, m_iWaterPower, DIR_CENTER, m_WhosBullet));
		for (int i = 1; i <= m_iWaterPower; ++i)
		{//왼
			float fX(m_tInfo.fX - TILECX * i);
			float fY(m_tInfo.fY);
			if (fX <= 0.f)
				break;
			if ((m_Stage1CannotBreak.bottom >= fY) && (m_Stage1CannotBreak.top <= fY) && (m_Stage1CannotBreak.left <= fX) && (m_Stage1CannotBreak.right >= fX))
				break;
			if (m_WhosBullet == BOMB_ZELDA)
				CObjMgr::Get_Instance()->Add_Object(OBJ_TILE, CAbstractFactory<CTile>::Create(fX, fY, OBJ_TILE));
			CObjMgr::Get_Instance()->Add_Object(OBJ_MULJULGY, CAbstractFactory<CMulJulgy>::Create(fX, fY, OBJ_MULJULGY, m_iWaterPower, DIR_LEFT, m_WhosBullet));
			if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
				break;
			}
		}
		for (int i = 1; i <= m_iWaterPower; ++i)
		{//위
			float fX(m_tInfo.fX);
			float fY(m_tInfo.fY - TILECX * i);
			if (fY <= 20.f)
				break;
			if ((m_Stage1CannotBreak.bottom >= fY) && (m_Stage1CannotBreak.top <= fY) && (m_Stage1CannotBreak.left <= fX) && (m_Stage1CannotBreak.right >= fX))
				break;
			if (m_WhosBullet == BOMB_ZELDA)
				CObjMgr::Get_Instance()->Add_Object(OBJ_TILE, CAbstractFactory<CTile>::Create(fX, fY, OBJ_TILE));
			CObjMgr::Get_Instance()->Add_Object(OBJ_MULJULGY, CAbstractFactory<CMulJulgy>::Create(fX, fY, OBJ_MULJULGY, m_iWaterPower, DIR_UP, m_WhosBullet));
			if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
				break;
			}
		}
		for (int i = 1; i <= m_iWaterPower; ++i)
		{//아래
			float fX(m_tInfo.fX);
			float fY(m_tInfo.fY + TILECX * i);
			if (fY >= 580.f)
				break;
			if ((m_Stage1CannotBreak.bottom >= fY) && (m_Stage1CannotBreak.top <= fY) && (m_Stage1CannotBreak.left <= fX) && (m_Stage1CannotBreak.right >= fX))
				break;
			if (m_WhosBullet == BOMB_ZELDA)
				CObjMgr::Get_Instance()->Add_Object(OBJ_TILE, CAbstractFactory<CTile>::Create(fX, fY, OBJ_TILE));
			CObjMgr::Get_Instance()->Add_Object(OBJ_MULJULGY, CAbstractFactory<CMulJulgy>::Create(fX, fY, OBJ_MULJULGY, m_iWaterPower, DIR_DOWN, m_WhosBullet));
			if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
				break;
			}
		}
		for (int i = 1; i <= m_iWaterPower; ++i)
		{//오
			float fX(m_tInfo.fX + TILECX * i);
			float fY(m_tInfo.fY);
			if (fX >= 640.f)
				break;
			if ((m_Stage1CannotBreak.bottom >= fY) && (m_Stage1CannotBreak.top <= fY) && (m_Stage1CannotBreak.left <= fX) && (m_Stage1CannotBreak.right >= fX))
				break;
			if (m_WhosBullet == BOMB_ZELDA)
				CObjMgr::Get_Instance()->Add_Object(OBJ_TILE, CAbstractFactory<CTile>::Create(fX, fY, OBJ_TILE));
			CObjMgr::Get_Instance()->Add_Object(OBJ_MULJULGY, CAbstractFactory<CMulJulgy>::Create(fX, fY, OBJ_MULJULGY, m_iWaterPower, DIR_RIGHT, m_WhosBullet));
			if (CObjMgr::Get_Instance()->Find_MulJulGi(fX, fY)) {
				break;
			}
		}
		if (m_WhosBullet == BOMB_PLAYER)
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetBullet();
		else if (m_WhosBullet == BOMB_MONSTER)
			dynamic_cast<CMonster*>(CObjMgr::Get_Instance()->Get_Monster())->SetBullet();
		else if (m_WhosBullet == BOMB_ZELDA)
			dynamic_cast<CZelda*>(CObjMgr::Get_Instance()->Get_Zelda())->SetBullet();

	

			

		return OBJ_DEAD;
	}

	Bullet_Update_Rect();

	if(m_bBulletUpdateRect)
		Update_Rect();

	if (m_PangTime + /*3000*/1300 < GetTickCount())
		m_bDead = true;

	return OBJ_NOEVENT;
}

void CBullet::Bullet_Update_Rect()
{
	m_DrawRect.left = LONG(m_tInfo.fX - (m_fDrawCX * 0.5f));
	m_DrawRect.top = LONG(m_tInfo.fY - (m_fDrawCY * 0.5f));
	m_DrawRect.right = LONG(m_tInfo.fX + (m_fDrawCX* 0.5f));
	m_DrawRect.bottom = LONG(m_tInfo.fY + (m_fDrawCY * 0.5f));
}

void CBullet::Late_Update(void)
{
	Move_Frame();

}

void CBullet::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	if (m_WhosBullet == BOMB_PLAYER||m_WhosBullet == BOMB_PET) {
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_DrawRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_DrawRect.top),
			int(m_fDrawCX),				// 4,5 인자 : 복사받을 가로, 세로 길이
			int(m_fDrawCY),
			hMemDC,							// 비트맵을 가지고 있는 DC
			m_tFrame.iFrameStart * (int)m_fDrawCX,								// 비트맵 출력 시작 좌표, X,Y
			m_tFrame.iMotion * (int)m_fDrawCY,
			(int)m_fDrawCY,				// 복사할 비트맵의 가로, 세로 길이
			(int)m_fDrawCY,
			RGB(0, 0, 0));
	}
	else if (m_WhosBullet == BOMB_MONSTER ||m_WhosBullet == BOMB_BOSS) {
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_DrawRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_DrawRect.top),
			int(m_fDrawCX),				// 4,5 인자 : 복사받을 가로, 세로 길이
			int(m_fDrawCY),
			hMemDC,							// 비트맵을 가지고 있는 DC
			m_tFrame.iFrameStart * (int)m_fDrawCX,								// 비트맵 출력 시작 좌표, X,Y
			m_tFrame.iMotion * (int)m_fDrawCY,
			(int)m_fDrawCX,				// 복사할 비트맵의 가로, 세로 길이
			(int)m_fDrawCY,
			RGB(255, 255, 255));
	}
	else if (m_WhosBullet == BOMB_ZELDA) {
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_DrawRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_DrawRect.top),
			int(m_fDrawCX),				// 4,5 인자 : 복사받을 가로, 세로 길이
			int(m_fDrawCY),
			hMemDC,							// 비트맵을 가지고 있는 DC
			m_tFrame.iFrameStart * (int)m_fDrawCX,								// 비트맵 출력 시작 좌표, X,Y
			m_tFrame.iMotion * (int)m_fDrawCY,
			(int)m_fDrawCX,				// 복사할 비트맵의 가로, 세로 길이
			(int)m_fDrawCY,
			RGB(255, 255, 255));
	}
	
}

void CBullet::Release(void)
{


}
