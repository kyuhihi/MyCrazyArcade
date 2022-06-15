#include "stdafx.h"
#include "MulJulgy.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
CMulJulgy::CMulJulgy()
{
}


CMulJulgy::~CMulJulgy()
{
	Release();

}

void CMulJulgy::Initialize(void)
{
	m_eRender = RENDER_GAMEOBJECT;

	CSoundMgr::Get_Instance()->PlaySoundW(L"Music_WaterFlow.wav", SOUND_EFFECT, g_fSound);

	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/Boomcenter.bmp", L"BoomCenter");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/BoomDownEx.bmp", L"BoomDownEx");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/BoomUpEx.bmp", L"BoomUpEx");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/BoomLeftEx.bmp", L"BoomLeftEx");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/BoomRightEx.bmp", L"BoomRightEx");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/ZeldaBoomcenter.bmp", L"ZeldaBoomcenter");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/ZeldaBoomDownEx.bmp", L"ZeldaBoomDownEx");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/ZeldaBoomUpEx.bmp", L"ZeldaBoomUpEx");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/ZeldaBoomLeftEx.bmp", L"ZeldaBoomLeftEx");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/ZeldaBoomRightEx.bmp", L"ZeldaBoomRightEx");

	if (m_eDir == DIR_CENTER) {
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 2;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 300;
		m_pFrameKey = L"BoomCenter";
		if(m_WhosBullet == BOMB_ZELDA)
			m_pFrameKey = L"ZeldaBoomcenter";
	}
	else if (m_eDir == DIR_UP) {
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 7;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 300;
		m_pFrameKey = L"BoomUpEx";
		if (m_WhosBullet == BOMB_ZELDA)
			m_pFrameKey = L"ZeldaBoomUpEx";
	}
	else if (m_eDir == DIR_DOWN) {
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 7;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 300;
		m_pFrameKey = L"BoomDownEx";
		if (m_WhosBullet == BOMB_ZELDA)
			m_pFrameKey = L"ZeldaBoomDownEx";
	}
	else if (m_eDir == DIR_LEFT) {
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 7;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 300;
		m_pFrameKey = L"BoomLeftEx";
		if (m_WhosBullet == BOMB_ZELDA)
			m_pFrameKey = L"ZeldaBoomLeftEx";
	}
	else if (m_eDir == DIR_RIGHT) {
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 7;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 300;
		m_pFrameKey = L"BoomRightEx";
		if (m_WhosBullet == BOMB_ZELDA)
			m_pFrameKey = L"ZeldaBoomRightEx";
	}
	m_tFrame.dwTime = GetTickCount();
	m_PangTime = GetTickCount();
}

int CMulJulgy::Update(void)
{
	if (m_bDead) {
		CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
		CObjMgr::Get_Instance()->SetWater_Spot(int((m_tInfo.fX - 40) / TILECX), int((m_tInfo.fY - 60) / TILECX), false);
		return OBJ_DEAD;
	}

	Update_Rect();
	if ((m_WhosBullet != BOMB_BOSS)&&(m_PangTime + 200 < GetTickCount()))
		m_bDead = true;
	else if ((m_WhosBullet == BOMB_BOSS) && (m_PangTime + 800< GetTickCount()))
		m_bDead = true;


	return OBJ_NOEVENT;
}

void CMulJulgy::Late_Update(void)
{
	Move_Frame();

}

void CMulJulgy::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		int(m_tRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
		int(m_tRect.top),
		int(m_tInfo.fCX),				// 4,5 인자 : 복사받을 가로, 세로 길이
		int(m_tInfo.fCY),
		hMemDC,							// 비트맵을 가지고 있는 DC
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,								// 비트맵 출력 시작 좌표, X,Y
		m_tFrame.iMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,				// 복사할 비트맵의 가로, 세로 길이
		(int)m_tInfo.fCY,
		RGB(0, 0, 0));
}

void CMulJulgy::Release(void)
{
}

