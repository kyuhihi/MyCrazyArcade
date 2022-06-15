#include "stdafx.h"
#include "Logo.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CLogo::CLogo()
{
}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Logo/Logo.bmp", L"Logo");
	m_tShop.left = 310.f;
	m_tShop.top = 570.f;
	m_tShop.right = 460.f;
	m_tShop.bottom = 590.f;

	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;
}

void CLogo::Update(void)
{

	Mouse_Update_Rect();
}

void CLogo::Late_Update(void)
{
	POINT	pt{};

	GetCursorPos(&pt);	// 현재 마우스의 위치 좌표를 얻어오는 함수
	ScreenToClient(g_hWnd, &pt);	// 전체 스크린영역에서 생성한 클라이언트(창) 좌표로 변환

	m_tInfo.fX = (float)pt.x;
	m_tInfo.fY = (float)pt.y;
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_WAITING);

		return;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down('M'))
	{
		CSoundMgr::Get_Instance()->PlaySoundW(L"LogoSound.wav", SOUND_BGM, g_fSound);
		return;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if (m_tShop.left< m_tInfo.fX&&m_tShop.right>m_tInfo.fX&&m_tShop.bottom > m_tInfo.fY&&m_tShop.top < m_tInfo.fY)
			CSceneMgr::Get_Instance()->Scene_Change(SC_SHOP);
		else
			return;
	}
}

void CLogo::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Logo");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLogo::Release(void)
{
	CSoundMgr::Get_Instance()->StopAll();

}

void CLogo::Mouse_Update_Rect(void)
{
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}