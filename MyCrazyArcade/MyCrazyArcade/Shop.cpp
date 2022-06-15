#include "stdafx.h"
#include "Shop.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "SoundMgr.h"

CShop::CShop()
{
}


CShop::~CShop()
{
	Release();
}

void CShop::Initialize(void)
{
	m_fBoxCX = 225.f;
	m_fBoxCY = 88.f;

	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;
	m_bNeedle = false;
	m_bCANBUY = false;
	m_bCAN_NOT_BUY = false;

	m_tExit.bottom =LONG( 596.f);
	m_tExit.top = LONG(578.f);
	m_tExit.left = LONG(101.f);
	m_tExit.right = LONG(130.f);

	m_tNotice.bottom = LONG(475.f);
	m_tNotice.top = LONG(125.f);
	m_tNotice.left = LONG(165.f);
	m_tNotice.right = LONG(635.f);
	if (!CObjMgr::Get_Instance()->IsPlayer())
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
	}

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shop/ShopMain.bmp", L"Shop_Main");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shop/Shop_Blank.bmp", L"Shop_Blank");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shop/Shop_Free_Needle.bmp", L"Shop_Needle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shop/Shop_Pet.bmp", L"Shop_Pet");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shop/Shop_Notice.bmp", L"Shop_Notice");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shop/Shop_Notice_PET_BUY.bmp", L"Shop_Notice_PET_BUY");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shop/Shop_Notice_CANNOTBUY.bmp", L"Shop_Notice_CANNOTBUY");


	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			if (j == 0) {
				m_tBox[i][j].left = LONG(28.f);
				m_tBox[i][j].right = LONG(253.f);
			}
			else if (j == 1) {
				m_tBox[i][j].left = LONG(262.f);
				m_tBox[i][j].right = LONG(487.f);
			}
			m_tBox[i][j].top = LONG(166.f + 96.f* i);
			m_tBox[i][j].bottom = LONG(254.f + 96.f* i);
		}
	}
	CSoundMgr::Get_Instance()->PlaySoundW(L"WhatisJazz.wav", SOUND_BGM, g_fSound);


}

void CShop::Update(void)
{
	Mouse_Update_Rect();
}

void CShop::Late_Update(void)
{
	POINT	pt{};

	GetCursorPos(&pt);	// 현재 마우스의 위치 좌표를 얻어오는 함수
	ScreenToClient(g_hWnd, &pt);	// 전체 스크린영역에서 생성한 클라이언트(창) 좌표로 변환

	m_tInfo.fX = (float)pt.x;
	m_tInfo.fY = (float)pt.y;
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		CSoundMgr::Get_Instance()->PlaySoundW(L"MousePicking.wav", SOUND_UI, g_fSound);
		for (int j = 0; j < 2; ++j)
		{
			if (m_tBox[0][j].bottom > m_tInfo.fY && m_tBox[0][j].top < m_tInfo.fY&&m_tBox[0][j].left<m_tInfo.fX&&m_tBox[0][j].right> m_tInfo.fX)
			{//구매하려는 
				if (j == 0)
				{//needle
					dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_PlayerNeedle();
					m_bNeedle = true;
					return;
				}
				else if (j == 1)
				{//pet
					if (g_iCash < 8000)
					{
						m_bCAN_NOT_BUY = true;
						return;
					}
					else if (g_iCash > 8000)
					{
						m_bCANBUY = true;
						g_iCash -= 8000;
						g_bPet = true;
						return;
					}
					
				}
			}
		}
		if (m_tExit.bottom > m_tInfo.fY&&m_tExit.top < m_tInfo.fY&& m_tExit.left<m_tInfo.fX&&m_tExit.right>m_tInfo.fX) {
			CSceneMgr::Get_Instance()->Scene_Change(SC_WAITING);
			return;
		}

		else {
			m_bNeedle = false;
			m_bCAN_NOT_BUY = false;
			m_bCANBUY = false;
		}

	}
}

void CShop::Render(HDC hDC)
{	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shop_Main");

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
	SelectObject(hDC, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hDC, RGB(0, 66, 139));
	Rectangle(hDC, 177, 42, 318, 62);
	SetDCBrushColor(hDC, RGB(255, 255, 255));
	wsprintf(m_Gold, TEXT("%d"), g_iCash);
	TextOut(hDC, 247, 45, m_Gold, lstrlen(m_Gold));

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shop_Blank");

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			if (j == 0 && i == 0)
				hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shop_Needle");
			else if (j == 1 && i == 0)
				hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shop_Pet");
			else
				hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shop_Blank");

			GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
				int(m_tBox[i][j].left),	// 2,3 인자 :  복사받을 위치 X, Y
				int(m_tBox[i][j].top),
				m_fBoxCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
				m_fBoxCY,
				hMemDC,							// 비트맵을 가지고 있는 DC
				0, 0,
				m_fBoxCX, m_fBoxCY,
				RGB(0, 0, 0));			// 제거하고자 하는 색상
		}
	}
	if (m_bNeedle)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shop_Notice");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_tNotice.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_tNotice.top),
			470,			// 4,5 인자 : 복사받을 가로, 세로 길이
			350,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			470, 350,
			RGB(0, 0, 0));			// 제거하고자 하는 색상
	}
	else if (m_bCANBUY)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shop_Notice_PET_BUY");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_tNotice.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_tNotice.top),
			470,			// 4,5 인자 : 복사받을 가로, 세로 길이
			350,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			470, 350,
			RGB(0, 0, 0));			// 제거하고자 하는 색상
	}
	else if (m_bCAN_NOT_BUY)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shop_Notice_CANNOTBUY");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_tNotice.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_tNotice.top),
			470,			// 4,5 인자 : 복사받을 가로, 세로 길이
			350,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			470, 350,
			RGB(0, 0, 0));			// 제거하고자 하는 색상
	}
	
}

void CShop::Release(void)
{
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
	CSoundMgr::Get_Instance()->StopSound(SOUND_UI);

}

void CShop::Mouse_Update_Rect(void)
{
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}