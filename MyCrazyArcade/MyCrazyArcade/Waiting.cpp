#include "stdafx.h"
#include "Waiting.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"
CWaiting::CWaiting()
{
}


CWaiting::~CWaiting()
{
	Release();
}

void CWaiting::Initialize(void)
{
	CSoundMgr::Get_Instance()->PlaySoundW(L"Waiting.wav", SOUND_BGM, g_fSound);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Waiting/Waiting.bmp", L"Waiting_Main");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Waiting/Waiting_Pirate.bmp", L"Waiting_Pirate");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Waiting/Waiting_Forest.bmp", L"Waiting_Forest");

	if (!CObjMgr::Get_Instance()->IsPlayer())
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
	}
	m_fBoxCX = 276.f;
	m_fBoxCY = 75.f;

	m_tBox[0].left = LONG(230.f);
	m_tBox[0].right = LONG(506.f);
	m_tBox[0].top = LONG(137.f);
	m_tBox[0].bottom = LONG(212.f);

	m_tBox[1].left = LONG(512.f);
	m_tBox[1].right = LONG(788.f);
	m_tBox[1].top = LONG(137.f);
	m_tBox[1].bottom = LONG(212.f);

	m_tShop.left = LONG(145.f);
	m_tShop.right = LONG(206.f);
	m_tShop.top = LONG(577.f);
	m_tShop.bottom = LONG(595.f);


	m_tMiniGame.left	 = LONG(550.f);
	m_tMiniGame.top		 = LONG(574.f);
	m_tMiniGame.right	 = LONG(615.f);
	m_tMiniGame.bottom	 = LONG(590.f);


}

void CWaiting::Update(void)
{
	Mouse_Update_Rect();

}

void CWaiting::Late_Update(void)
{
	POINT	pt{};

	GetCursorPos(&pt);	// 현재 마우스의 위치 좌표를 얻어오는 함수
	ScreenToClient(g_hWnd, &pt);	// 전체 스크린영역에서 생성한 클라이언트(창) 좌표로 변환

	m_tInfo.fX = (float)pt.x;
	m_tInfo.fY = (float)pt.y;
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		CSoundMgr::Get_Instance()->PlaySoundW(L"MousePicking.wav", SOUND_UI, g_fSound);
		for (int i = 0; i < 2; ++i)
		{
			if (i == 0) {
				if (m_tBox[i].left< m_tInfo.fX&&m_tBox[i].right>m_tInfo.fX&&m_tBox[i].bottom > m_tInfo.fY&&m_tBox[i].top < m_tInfo.fY) {
					CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE);
					return;
				}
			}
			else if (i == 1)
			{
				if (m_tBox[i].left< m_tInfo.fX&&m_tBox[i].right>m_tInfo.fX&&m_tBox[i].bottom > m_tInfo.fY&&m_tBox[i].top < m_tInfo.fY) {
					CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE2);
					return;

				}
			}

		}
		if (m_tShop.left< m_tInfo.fX&&m_tShop.right>m_tInfo.fX&&m_tShop.bottom > m_tInfo.fY&&m_tShop.top < m_tInfo.fY) {
			CSceneMgr::Get_Instance()->Scene_Change(SC_SHOP);
			return;
		}
		if ((m_tMiniGame.left < m_tInfo.fX) && (m_tMiniGame.right > m_tInfo.fX) && (m_tMiniGame.bottom > m_tInfo.fY) && (m_tMiniGame.top < m_tInfo.fY)) {
			CSceneMgr::Get_Instance()->Scene_Change(SC_MINIGAME);
			return;

		}

	}
}

void CWaiting::Render(HDC hDC)
{

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Waiting_Main");

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Waiting_Pirate");
	for (int j = 0; j < 2; ++j)
	{
		if (j == 0) {
			GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
				int(m_tBox[j].left),	// 2,3 인자 :  복사받을 위치 X, Y
				int(m_tBox[j].top),
				(int)m_fBoxCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
				(int)m_fBoxCY,
				hMemDC,							// 비트맵을 가지고 있는 DC
				0, 0,
				(int)m_fBoxCX, (int)m_fBoxCY,
				RGB(0, 0, 0));			// 제거하고자 하는 색상
		}
		else if (j == 1) {
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Waiting_Forest");
			GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
				int(m_tBox[j].left),	// 2,3 인자 :  복사받을 위치 X, Y
				int(m_tBox[j].top),
				(int)m_fBoxCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
				(int)m_fBoxCY,
				hMemDC,							// 비트맵을 가지고 있는 DC
				0, 0,
				(int)m_fBoxCX, (int)m_fBoxCY,
				RGB(0, 0, 0));			// 제거하고자 하는 색상
		}
	}

	


}

void CWaiting::Release(void)
{
	CSoundMgr::Get_Instance()->StopAll();
	//CSoundMgr::Get_Instance()->StopSound(SOUND_UI);

}

void CWaiting::Mouse_Update_Rect()
{
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}
