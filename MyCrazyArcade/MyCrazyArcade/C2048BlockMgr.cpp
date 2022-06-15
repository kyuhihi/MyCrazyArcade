#include "stdafx.h"
#include "C2048BlockMgr.h"
#include "AbstractFactory.h"
#include "C2048Block.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include <random>
#include "Include.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

C2048BlockMgr*	C2048BlockMgr::m_pInstance = nullptr;

C2048BlockMgr::C2048BlockMgr() : m_NewBlock(-1, -1), m_iMoveCount(0)
{
}

C2048BlockMgr::~C2048BlockMgr()
{
	Release();
	
}

void C2048BlockMgr::Move_Frame(void)
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		m_tFrame.iFrameStart++;

		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;
	}

}

void C2048BlockMgr::Initialize()
{

	m_iUpCount = 3;

	m_ItemRectCX = 105.f;
	m_ItemRectCY = 121.f;

	m_ResultRectCX = 383.f;
	m_ResultRectCY = 150.f;
	for (int i = 0; i < 4; ++i)
	{
		m_ResultRect[i].left = LONG(417.f);
		m_ResultRect[i].top = LONG(150.f * i);
		m_ResultRect[i].right = LONG(WINCX);
		m_ResultRect[i].bottom = LONG(150.f * (i + 1));

		m_ItemRect[i].left = LONG(430.f);
		m_ItemRect[i].top = LONG(14 +(150* i));
		m_ItemRect[i].right = LONG(535.f);
		m_ItemRect[i].bottom = LONG(135.f + (150 *i));

	}

	float		fX = 75.5f;		//0,0인덱스
	float		fY = 252.f;
	
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			CObj*	pRemBlock = CAbstractFactory<C2048Block>::Create(fX, fY, i, j);
			m_ReturnBlock[i][j] = pRemBlock;

			CObj*	pBlock = CAbstractFactory<C2048Block>::Create(fX, fY, i, j);
			m_ArrayBlock[i][j] = pBlock;
			fX += BLOCK_CX + 10;
		}
		fX = 75.5f;
		fY += BLOCK_CX + 10;
	}
	EmptyBlockCheck();
	GetNewRandomNum();
	EmptyBlockCheck();
	GetNewRandomNum();
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/GameStart.bmp", L"GameStart");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/GameOver.bmp", L"GameOver");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_STAND.bmp", L"Player_STAND");
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ONE.bmp", L"ONE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TWO.bmp", L"TWO");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/THREE.bmp", L"THREE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/FOUR.bmp", L"FOUR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/FIVE.bmp", L"FIVE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/SIX.bmp", L"SIX");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/SEVEN.bmp", L"SEVEN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/EIGHT.bmp", L"EIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/NINE.bmp", L"NINE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ZERO.bmp", L"ZERO");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/ItemMiniGame_Medicine.bmp", L"ItemMiniGame_Medicine");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/ItemMiniGame_Needle.bmp", L"ItemMiniGame_Needle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/ItemMiniGame_Roller.bmp", L"ItemMiniGame_Roller");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/ItemMiniGame_WaterBomb.bmp", L"ItemMiniGame_WaterBomb");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/MiniGameResult.bmp", L"MiniGameResult");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/RESULT.bmp", L"RESULT");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Item_0.bmp", L"WATERBOMB");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Item_1.bmp", L"MEDICINE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Item_2.bmp", L"ROLLER");

	m_fDrawCX = 70.f;
	m_fDrawCY = 70.f;
	
	

	m_tGameOverRectCX = 201.f;
	m_tGameOverRectCY = 106.f;

	m_tGameOverRect.left = LONG(300.f);
	m_tGameOverRect.top = LONG(250.f);
	m_tGameOverRect.right = LONG(501.f);
	m_tGameOverRect.bottom = LONG(356.f);

	m_fStartCX = 286.f;
	m_fStartCY = 105.f;

	m_GameStartRect.left = LONG(300.f);
	m_GameStartRect.top = LONG(250.f);
	m_GameStartRect.right = LONG(586.f);
	m_GameStartRect.bottom = LONG(355.f);

	//배찌 움직임관련
	m_DrawRect.left = 106;
	m_DrawRect.top = 164;
	m_DrawRect.right = 176;
	m_DrawRect.bottom = 234;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount();

	m_bGameOver = false;
	m_bGameStart = false;
	m_StartTime = 0;

	m_iForRenderTime = 30;

	m_tResultNotice.left = LONG(26);
	m_tResultNotice.top = LONG(38);
	m_tResultNotice.right = LONG(310);
	m_tResultNotice.bottom = LONG(86);

	m_Item100 = 0;
	m_Item10 = 0;
	m_Item1 = 0;

	Result._iMedicine = 0;
	Result._iRoller = 0;
	Result._iWaterBomb = 0;

	ApeendAmountCXY = 38;
	m_PopUpAnimationTime = 0;

	for (int i = 0; i < 15; ++i)
	{
		random_device rd;
		mt19937_64 rng(rd());
		uniform_int_distribution<__int64> dist1(0, WINCX);
		uniform_int_distribution<__int64> dist2(0, WINCY);
		uniform_int_distribution<__int64> dist3(-5 , 5);
		uniform_int_distribution<__int64> dist4(0, 2);
		int iEmptySizeNum = dist2(rng);
		m_AppendItemArray[i]._iX = dist1(rng);
		m_AppendItemArray[i]._iY = dist2(rng);
		m_AppendItemArray[i]._iSpeed = dist3(rng);
		m_AppendItemArray[i]._iItemNum = dist4(rng);
	}
}

const ItemResult& C2048BlockMgr::Update()
{
	if ((m_StartTime != 0) && m_iForRenderTime == 0)
	{
		m_bGameOver = true;
	}
	else
	{
		if ((m_StartTime != 0) && (m_StartTime + 1000 < GetTickCount())) {
			--m_iForRenderTime;
			m_StartTime = GetTickCount();
		}
	}
	Key_Input();
	if (m_bGameOver == false) {
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m_ArrayBlock[i][j]->Update();
			}
		}
		
	}

	ItemPopUp();
	RenderNumberSelect();
	ItemDrop();
	return Result;
	
}

void C2048BlockMgr::ItemPopUp()
{
	if (m_bGameOver == true) {
		int iTotal_Num = 0;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				iTotal_Num += static_cast<C2048Block*>(m_ArrayBlock[i][j])->GetBlockNum();
			}
		}
		m_Item100 = iTotal_Num / 100;
		m_Item10 = (iTotal_Num % 100) / 10;
		m_Item1 = (iTotal_Num % 100) % 10;

		Result._iRoller = m_Item1;
		Result._iWaterBomb = m_Item10;
		Result._iMedicine = m_Item100;

	}
	else
		return;
}

void C2048BlockMgr::Late_Update()
{
	POINT	pt{};

	GetCursorPos(&pt);	// 현재 마우스의 위치 좌표를 얻어오는 함수
	ScreenToClient(g_hWnd, &pt);	// 전체 스크린영역에서 생성한 클라이언트(창) 좌표로 변환

	m_tInfo.fX = (float)pt.x;
	m_tInfo.fY = (float)pt.y;


	Move_Frame();
	if (m_bGameOver == false) {

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m_ArrayBlock[i][j]->Late_Update();
			}
		}

	}
}


void C2048BlockMgr::Render(HDC hDC)
{
	
	if (m_NewBlock.iEmptyX != -1)
	{
		SelectObject(hDC, GetStockObject(DC_BRUSH));
		SetDCBrushColor(hDC, RGB(255, 0, 0));
		Rectangle(hDC, m_ArrayBlock[m_NewBlock.iEmptyX][m_NewBlock.iEmptyY]->Get_Rect().left - 3, m_ArrayBlock[m_NewBlock.iEmptyX][m_NewBlock.iEmptyY]->Get_Rect().top - 3, m_ArrayBlock[m_NewBlock.iEmptyX][m_NewBlock.iEmptyY]->Get_Rect().right + 3, m_ArrayBlock[m_NewBlock.iEmptyX][m_NewBlock.iEmptyY]->Get_Rect().bottom + 3);
		SetDCBrushColor(hDC, RGB(255, 255, 255));
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m_ArrayBlock[i][j]->Render(hDC);
		}
	}
	if (!m_bGameOver) {
		
	}

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_STAND");

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
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MiniGameResult");
	for (int i = 0; i < 4; ++i)
	{
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_ResultRect[i].left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_ResultRect[i].top),
			(int)m_ResultRectCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)m_ResultRectCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0,0,
			(int)m_ResultRectCX, (int)m_ResultRectCY,
			RGB(255, 255, 255));
	}
	for (int i = 0; i < 4; ++i)
	{
		if (i == 0)
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ItemMiniGame_Roller");
		else if (i == 1)
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ItemMiniGame_WaterBomb");
		else if (i == 2)
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ItemMiniGame_Medicine");
		else if (i == 3)
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ItemMiniGame_Needle");
		
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_ItemRect[i].left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_ItemRect[i].top),
			(int)m_ItemRectCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)m_ItemRectCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			(int)m_ItemRectCX, (int)m_ItemRectCY,
			RGB(255, 255, 255));
	}
	
	if (m_bGameStart == false) {
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"GameStart");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_GameStartRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_GameStartRect.top),
			(int)m_fStartCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)m_fStartCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			(int)m_fStartCX, (int)m_fStartCY,
			RGB(255, 255, 255));
	}
	else if ((m_bGameStart == true) && (!m_bGameOver))
	{
		SetDCBrushColor(hDC, RGB(48, 203, 254));
		Rectangle(hDC, 127, 39, 194, 80);
		SetDCBrushColor(hDC, RGB(255, 255, 255));

		if (m_Num1 == 3)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"THREE");
		}
		else if (m_Num1 == 2)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TWO");
		}
		else if (m_Num1 == 1)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ONE");
		}
		else if (m_Num1 == 0)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ZERO");
		}
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_NumRect1.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_NumRect1.top),
			(int)m_NumRect1CX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)m_NumRect1CY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			(int)m_NumRect1CX, (int)m_NumRect1CY,
			RGB(255, 255, 255));
		if (m_Num2 == 3)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"THREE");
		}
		else if (m_Num2 == 2)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TWO");
		}
		else if (m_Num2 == 1)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ONE");
		}
		else if (m_Num2 == 0)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ZERO");
		}
		else if (m_Num2 == 4)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"FOUR");
		}
		else if (m_Num2 == 5)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"FIVE");
		}
		else if (m_Num2 == 6)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"SIX");
		}
		else if (m_Num2 == 7)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"SEVEN");
		}
		else if (m_Num2 == 8)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"EIGHT");
		}
		else if (m_Num2 == 9)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"NINE");
		}
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_NumRect2.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_NumRect2.top),
			(int)m_NumRect2CX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)m_NumRect2CY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			(int)m_NumRect2CX, (int)m_NumRect2CY,
			RGB(255, 255, 255));
	}
	else if (m_bGameOver)
	{
		m_PopUpAnimationTime = GetTickCount();
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"GameOver");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_tGameOverRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_tGameOverRect.top),
			(int)m_tGameOverRectCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)m_tGameOverRectCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			(int)m_tGameOverRectCX, (int)m_tGameOverRectCY,
			RGB(255, 255, 255));

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"RESULT");
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_tResultNotice.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_tResultNotice.top),
			(int)284,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)48,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			(int)284, (int)48,
			RGB(255, 255, 255));

		if (m_Item100 == 3)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"THREE");
		}
		else if (m_Item100 == 2)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TWO");
		}
		else if (m_Item100 == 1)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ONE");
		}
		else if (m_Item100 == 0)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ZERO");
		}
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_tItem100Rect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_tItem100Rect.top),
			(int)m_Item100CX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)m_ItemCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			(int)m_Item100CX, (int)m_ItemCY,
			RGB(255, 255, 255));
		//아이템 갯수 출력창.
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			600,	// 2,3 인자 :  복사받을 위치 X, Y
			368,
			(int)m_Item100CX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)m_ItemCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			(int)m_Item100CX, (int)m_ItemCY,
			RGB(255, 255, 255));

		if (m_Item10 == 3)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"THREE");
		}
		else if (m_Item10 == 2)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TWO");
		}
		else if (m_Item10 == 1)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ONE");
		}
		else if (m_Item10 == 0)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ZERO");
		}
		else if (m_Item10 == 4)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"FOUR");
		}
		else if (m_Item10 == 5)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"FIVE");
		}
		else if (m_Item10 == 6)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"SIX");
		}
		else if (m_Item10 == 7)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"SEVEN");
		}
		else if (m_Item10 == 8)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"EIGHT");
		}
		else if (m_Item10 == 9)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"NINE");
		}
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_tItem10Rect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_tItem10Rect.top),
			(int)m_Item10CX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)m_ItemCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			(int)m_Item10CX, (int)m_ItemCY,
			RGB(255, 255, 255));
		//아이템 갯수 출력창.
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			600,	// 2,3 인자 :  복사받을 위치 X, Y
			218,
			(int)m_Item10CX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)m_ItemCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			(int)m_Item10CX, (int)m_ItemCY,
			RGB(255, 255, 255));
		if (m_Item1 == 3)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"THREE");
		}
		else if (m_Item1 == 2)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TWO");
		}
		else if (m_Item1 == 1)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ONE");
		}
		else if (m_Item1 == 0)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ZERO");
		}
		else if (m_Item1 == 4)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"FOUR");
		}
		else if (m_Item1 == 5)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"FIVE");
		}
		else if (m_Item1 == 6)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"SIX");
		}
		else if (m_Item1 == 7)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"SEVEN");
		}
		else if (m_Item1 == 8)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"EIGHT");
		}
		else if (m_Item1 == 9)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"NINE");
		}
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_tItem1Rect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_tItem1Rect.top),
			(int)m_Item1CX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)m_ItemCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			(int)m_Item1CX, (int)m_ItemCY,
			RGB(255, 255, 255));

		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			600,	// 2,3 인자 :  복사받을 위치 X, Y
			68,
			(int)m_Item1CX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			(int)m_ItemCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			(int)m_Item1CX, (int)m_ItemCY,
			RGB(255, 255, 255));
		for (int i = 0; i < 15; ++i)
		{
			if (m_AppendItemArray[i]._iItemNum == 0)
			{
				hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"WATERBOMB");
			}
			else if (m_AppendItemArray[i]._iItemNum == 1)
			{
				hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ROLLER");
			}
			else if(m_AppendItemArray[i]._iItemNum ==2)
				hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MEDICINE");
			GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
				m_AppendItemArray[i]._iX,	// 2,3 인자 :  복사받을 위치 X, Y
				m_AppendItemArray[i]._iY,
				(int)ApeendAmountCXY,			// 4,5 인자 : 복사받을 가로, 세로 길이
				(int)ApeendAmountCXY,
				hMemDC,							// 비트맵을 가지고 있는 DC
				0, 0,
				(int)ApeendAmountCXY, (int)ApeendAmountCXY,
				RGB(255, 255, 255));
		}

		if (m_PopUpAnimationTime + 1000 < GetTickCount())
		{
			--m_iUpCount;
			m_PopUpAnimationTime = GetTickCount();
		}
	}
	
	wsprintf(m_MousePoint, TEXT("X: %d Y : %d"), int(m_tInfo.fX), int(m_tInfo.fY));				//printf %d 인트 &s 문자열 %f 플롯
	TextOut(hDC, (int)m_tInfo.fX - 10, (int)m_tInfo.fY - 10, m_MousePoint, lstrlen(m_MousePoint));

}

void C2048BlockMgr::RenderNumberSelect()
{
	m_Num1 = m_iForRenderTime / 10;//10의자리
	m_Num2 = m_iForRenderTime % 10;//1의자리

	if (m_Num1 == 3)
	{
		m_NumRect1CX = 20.f;
		m_NumRect1CY = 21.f;

		m_NumRect1.left = (LONG)137.f;
		m_NumRect1.top = (LONG)49.f;
		m_NumRect1.right = (LONG)147.f;
		m_NumRect1.bottom = (LONG)60.f;

	}
	else if (m_Num1 == 2)
	{
		m_NumRect1CX = 19.f;
		m_NumRect1CY = 21.f;

		m_NumRect1.left = (LONG)137.f;
		m_NumRect1.top = (LONG) 49.f;
		m_NumRect1.right = (LONG)146.f;
		m_NumRect1.bottom = (LONG)60.f;
	}
	else if (m_Num1 == 1)
	{
		m_NumRect1CX = 11.f;
		m_NumRect1CY = 21.f;

		m_NumRect1.left = (LONG)137.f;
		m_NumRect1.top = (LONG)49.f;
		m_NumRect1.right = (LONG)148.f;
		m_NumRect1.bottom = (LONG) 60.f;
	}
	else if (m_Num1 == 0)
	{
		m_NumRect1CX = 18.f;
		m_NumRect1CY = 21.f;

		m_NumRect1.left = (LONG)137.f;
		m_NumRect1.top = (LONG) 49.f;
		m_NumRect1.right = (LONG)155.f;
		m_NumRect1.bottom = (LONG) 60.f;
	}

	if (m_Num2 == 3)
	{
		m_NumRect2CX = 20.f;
		m_NumRect2CY = 21.f;

		m_NumRect2.left = (LONG)165.f;
		m_NumRect2.top = (LONG)49.f;
		m_NumRect2.right = (LONG)185.f;
		m_NumRect2.bottom = (LONG) 60.f;
	}
	else if (m_Num2 == 2)
	{
		m_NumRect2CX = 19.f;
		m_NumRect2CY = 21.f;

		m_NumRect2.left = (LONG)165.f;
		m_NumRect2.top = (LONG) 49.f;
		m_NumRect2.right = (LONG)184.f;
		m_NumRect2.bottom = (LONG)60.f;
	}
	else if (m_Num2 == 1)
	{
		m_NumRect2CX = 11.f;
		m_NumRect2CY = 21.f;

		m_NumRect2.left = (LONG)165.f;
		m_NumRect2.top = (LONG)49.f;
		m_NumRect2.right = (LONG)176.f;
		m_NumRect2.bottom = (LONG) 60.f;
	}
	else if (m_Num2 == 0)
	{
		m_NumRect2CX = 18.f;
		m_NumRect2CY = 21.f;

		m_NumRect2.left = (LONG)165.f;
		m_NumRect2.top = (LONG)49.f;
		m_NumRect2.right = (LONG)183.f;
		m_NumRect2.bottom = (LONG)60.f;
	}
	else if (m_Num2 == 4)
	{
		m_NumRect2CX = 20.f;
		m_NumRect2CY = 21.f;

		m_NumRect2.left = (LONG)165.f;
		m_NumRect2.top = (LONG)49.f;
		m_NumRect2.right = (LONG)185.f;
		m_NumRect2.bottom = (LONG)60.f;
	}
	else if (m_Num2 == 5)
	{
		m_NumRect2CX = 18.f;
		m_NumRect2CY = 21.f;

		m_NumRect2.left = (LONG)165.f;
		m_NumRect2.top = (LONG)49.f;
		m_NumRect2.right = (LONG)183.f;
		m_NumRect2.bottom = (LONG) 60.f;
	}
	else if (m_Num2 == 6)
	{
		m_NumRect2CX = 18.f;
		m_NumRect2CY = 21.f;

		m_NumRect2.left = (LONG)165.f;
		m_NumRect2.top = (LONG)49.f;
		m_NumRect2.right = (LONG)183.f;
		m_NumRect2.bottom = (LONG) 60.f;
	}
	else if (m_Num2 == 7)
	{
		m_NumRect2CX = 18.f;
		m_NumRect2CY = 21.f;

		m_NumRect2.left = (LONG)165.f;
		m_NumRect2.top = (LONG)49.f;
		m_NumRect2.right = (LONG)183.f;
		m_NumRect2.bottom = (LONG) 60.f;
	}
	else if (m_Num2 == 8)
	{
		m_NumRect2CX = 18.f;
		m_NumRect2CY = 21.f;

		m_NumRect2.left = (LONG)165.f;
		m_NumRect2.top = (LONG)49.f;
		m_NumRect2.right = (LONG) 183.f;
		m_NumRect2.bottom = (LONG) 60.f;
	}
	else if (m_Num2 == 9)
	{
		m_NumRect2CX = 18.f;
		m_NumRect2CY = 21.f;

		m_NumRect2.left = (LONG)165.f;
		m_NumRect2.top = (LONG)49.f;
		m_NumRect2.right = (LONG)183.f;
		m_NumRect2.bottom = (LONG) 60.f;
	}

	if (m_bGameOver)
	{//100의자리
		m_ItemCY = 21;
		if (m_Item100 == 0)
		{
			m_Item100CX = 18;

			m_tItem100Rect.top = LONG(96);
			m_tItem100Rect.left = LONG(26);

		}
		else if (m_Item100 == 1)
		{
			m_Item100CX = 11;
			m_tItem100Rect.top = LONG(96);
			m_tItem100Rect.left = LONG(26);
		}
		else if (m_Item100 == 2)
		{
			m_Item100CX = 19;
			m_tItem100Rect.top = LONG(96);
			m_tItem100Rect.left = LONG(26);
		}

		if (m_Item10 == 3)
		{//10의자리
			m_Item10CX = 20;

			m_tItem10Rect.left = (LONG)56;
			m_tItem10Rect.top = (LONG)96;
		}
		else if (m_Item10 == 2)
		{
			m_Item10CX = 19;

			m_tItem10Rect.left = (LONG)56;
			m_tItem10Rect.top = (LONG)96;
		}
		else if (m_Item10 == 1)
		{
			m_Item10CX = 11;

			m_tItem10Rect.left = (LONG)56;
			m_tItem10Rect.top = (LONG)96;
		}
		else if (m_Item10 == 0)
		{
			m_Item10CX = 18;

			m_tItem10Rect.left = (LONG)56;
			m_tItem10Rect.top = (LONG)96;
		}
		else if (m_Item10 == 4)
		{
			m_Item10CX = 20;

			m_tItem10Rect.left = (LONG)56;
			m_tItem10Rect.top = (LONG)96;
		}
		else if (m_Item10 == 5)
		{
			m_Item10CX = 18;

			m_tItem10Rect.left = (LONG)56;
			m_tItem10Rect.top = (LONG)96;
		}
		else if (m_Item10 == 6)
		{
			m_Item10CX = 18;

			m_tItem10Rect.left = (LONG)56;
			m_tItem10Rect.top = (LONG)96;
		}
		else if (m_Item10 == 7)
		{
			m_Item10CX = 18;

			m_tItem10Rect.left = (LONG)56;
			m_tItem10Rect.top = (LONG)96;
		}
		else if (m_Item10 == 8)
		{
			m_Item10CX = 18;

			m_tItem10Rect.left = (LONG)56;
			m_tItem10Rect.top = (LONG)96;
		}
		else if (m_Item10 == 9)
		{
			m_Item10CX = 18;

			m_tItem10Rect.left = (LONG)56;
			m_tItem10Rect.top = (LONG)96;
		}


		if (m_Item1 == 3)
		{//1의자리
			m_Item1CX = 20;

			m_tItem1Rect.left = (LONG)86;
			m_tItem1Rect.top = (LONG)96;

		}
		else if (m_Item1 == 2)
		{
			m_Item1CX = 19;

			m_tItem1Rect.left = (LONG)86;
			m_tItem1Rect.top = (LONG)96;
		}
		else if (m_Item1 == 1)
		{
			m_Item1CX = 11;

			m_tItem1Rect.left = (LONG)86;
			m_tItem1Rect.top = (LONG)96;
		}
		else if (m_Item1 == 0)
		{
			m_Item1CX = 18;

			m_tItem1Rect.left = (LONG)86;
			m_tItem1Rect.top = (LONG)96;
		}
		else if (m_Item1 == 4)
		{
			m_Item1CX = 20;

			m_tItem1Rect.left = (LONG)86;
			m_tItem1Rect.top = (LONG)96;
		}
		else if (m_Item1 == 5)
		{
			m_Item1CX = 18;

			m_tItem1Rect.left = (LONG)86;
			m_tItem1Rect.top = (LONG)96;
		}
		else if (m_Item1== 6)
		{
			m_Item1CX = 18;

			m_tItem1Rect.left = (LONG)86;
			m_tItem1Rect.top = (LONG)96;
		}
		else if (m_Item1 == 7)
		{
			m_Item1CX = 18;

			m_tItem1Rect.left = (LONG)86;
			m_tItem1Rect.top = (LONG)96;
		}
		else if (m_Item1 == 8)
		{
			m_Item1CX = 18;

			m_tItem1Rect.left = (LONG)86;
			m_tItem1Rect.top = (LONG)96;
		}
		else if (m_Item1 == 9)
		{
			m_Item1CX = 18;

			m_tItem1Rect.left = (LONG)86;
			m_tItem1Rect.top = (LONG)96;
		}
	}
	else
		return;
}

void C2048BlockMgr::Release()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			delete m_ArrayBlock[i][j];
			m_ArrayBlock[i][j] = nullptr;
			delete m_ReturnBlock[i][j];
			m_ReturnBlock[i][j] = nullptr;
		}
	}
	
	
}

void C2048BlockMgr::Key_Input(void)
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if ((!m_bGameOver)&&(m_GameStartRect.left< m_tInfo.fX&&m_GameStartRect.right>m_tInfo.fX&&m_GameStartRect.bottom > m_tInfo.fY&&m_GameStartRect.top < m_tInfo.fY))
		{//게임스타트
			m_StartTime = GetTickCount();
			m_bGameStart = true;
		}
		else if ((m_bGameOver)&&(m_tGameOverRect.left< m_tInfo.fX&&m_tGameOverRect.right>m_tInfo.fX&&m_tGameOverRect.bottom > m_tInfo.fY&&m_tGameOverRect.top < m_tInfo.fY))
		{
			CSceneMgr::Get_Instance()->Scene_Change(SC_WAITING);
		}
	}
	if (m_bGameOver == false) {

		if (CKeyMgr::Get_Instance()->Key_Down('A'))
		{

			CSoundMgr::Get_Instance()->PlaySoundW(L"2048BlockMoveSound.wav", SOUND_EFFECT, g_fSound);
			CheckDirection(DIR_LEFT);
			EmptyBlockCheck();		// 여기서 빈 공간 해당 벡터 푸쉬백 
			GetNewRandomNum();		// 여기서 새로운 블럭 생성
			m_Empty.clear();
			++m_iMoveCount;
			RemForReturn();
		}
		else if (CKeyMgr::Get_Instance()->Key_Down('D'))
		{
			CSoundMgr::Get_Instance()->PlaySoundW(L"2048BlockMoveSound.wav", SOUND_EFFECT, g_fSound);

			CheckDirection(DIR_RIGHT);
			EmptyBlockCheck();
			GetNewRandomNum();
			m_Empty.clear();
			++m_iMoveCount;
			RemForReturn();
		}
		else if (CKeyMgr::Get_Instance()->Key_Down('W'))
		{
			CSoundMgr::Get_Instance()->PlaySoundW(L"2048BlockMoveSound.wav", SOUND_EFFECT, g_fSound);

			CheckDirection(DIR_UP);
			EmptyBlockCheck();
			GetNewRandomNum();
			m_Empty.clear();
			++m_iMoveCount;
			RemForReturn();

		}
		else if (CKeyMgr::Get_Instance()->Key_Down('S'))
		{
			CSoundMgr::Get_Instance()->PlaySoundW(L"2048BlockMoveSound.wav", SOUND_EFFECT, g_fSound);

			CheckDirection(DIR_DOWN);
			EmptyBlockCheck();
			GetNewRandomNum();
			m_Empty.clear();
			++m_iMoveCount;
			RemForReturn();
		}
	}
	
}

void C2048BlockMgr::CheckDirection(DIRECTION eDir)
{
	switch (eDir)
	{
	case DIR_LEFT:
		FillBlank(eDir);
		MergeGaro(eDir);
		FillBlank(eDir);
		return;
	case DIR_RIGHT:
		FillBlank(eDir);
		MergeGaro(eDir);
		FillBlank(eDir);
		return;

	case DIR_UP:
		FillBlank(eDir);
		MergeSero(eDir);
		FillBlank(eDir);
		return;
	case DIR_DOWN:
		FillBlank(eDir);
		MergeSero(eDir);
		FillBlank(eDir);
		return;
	}
}



void C2048BlockMgr::EmptyBlockCheck()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (static_cast<C2048Block*>(m_ArrayBlock[i][j])->GetBlockNum() == 0)
			{
				m_tEmptyIndex NewBlank(i, j);
				m_Empty.push_back(NewBlank);
			}
		}
	}
}

void C2048BlockMgr::GetNewRandomNum()
{
	random_device rd;
	mt19937_64 rng(rd());
	uniform_int_distribution<__int64> dist1(0, 10);
	uniform_int_distribution<__int64> dist2(0, m_Empty.size() - 1);
	int iEmptySizeNum = dist2(rng);
	int iNum = 0;
	for (auto& iter : m_Empty)
	{
		if (iNum == iEmptySizeNum)
		{
			if (1 == dist1(rng))
			{//10퍼확률로 4가나옴
				static_cast<C2048Block*>(m_ArrayBlock[iter.iEmptyX][iter.iEmptyY])->GetBlockNum() = 4;
				m_NewBlock.iEmptyX = iter.iEmptyX;
				m_NewBlock.iEmptyY = iter.iEmptyY;
				if (m_iMoveCount > 0)
				{
					m_NewBlock.iEmptyX = iter.iEmptyX;
					m_NewBlock.iEmptyY = iter.iEmptyY;
				}

			}
			else
			{//나머지는 2
				static_cast<C2048Block*>(m_ArrayBlock[iter.iEmptyX][iter.iEmptyY])->GetBlockNum()  = 2;
				m_NewBlock.iEmptyX = iter.iEmptyX;
				m_NewBlock.iEmptyY = iter.iEmptyY;
				if (m_iMoveCount > 0)
				{
					m_NewBlock.iEmptyX = iter.iEmptyX;
					m_NewBlock.iEmptyY = iter.iEmptyY;
				}
			}
			break;
		}
		++iNum;
	}

}

void C2048BlockMgr::MergeGaro(DIRECTION eDir)
{
	int iBeforeNum = -1;
	int iCurNum = 0;
	if (eDir == DIR_LEFT)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				iCurNum = static_cast<C2048Block*>(m_ArrayBlock[i][j])->GetBlockNum();
				if (iBeforeNum != 0 && iCurNum == iBeforeNum)
				{//좌우 숫자가 같아 합쳐야하는 시점. 
				 //MoveBlock(eDir, m_ArrayBlock[i][j]);//애니메이션
					static_cast<C2048Block*>(m_ArrayBlock[i][j - 1])->GetBlockNum() *= 2;//왼쪽으로 합친거
					static_cast<C2048Block*>(m_ArrayBlock[i][j])->GetBlockNum() = 0;
					iCurNum = 0;
					iBeforeNum = iCurNum;
				}
				iBeforeNum = iCurNum;
			}
			iBeforeNum = -1;
			iCurNum = 0;
		}
		return;
	}
	else if (eDir == DIR_RIGHT)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 3; j >= 0; --j)
			{
				iCurNum = static_cast<C2048Block*>(m_ArrayBlock[i][j])->GetBlockNum();
				if (iBeforeNum != 0 && iCurNum == iBeforeNum)
				{//좌우 숫자가 같아 합쳐야하는 시점. 
				 //MoveBlock(eDir, m_ArrayBlock[i][j-1]);
					static_cast<C2048Block*>(m_ArrayBlock[i][j])->GetBlockNum()= 0;	//오른쪽으로 합친거
					static_cast<C2048Block*>(m_ArrayBlock[i][j + 1])->GetBlockNum() *= 2;
					iCurNum = 0;
					iBeforeNum = iCurNum;
				}
				iBeforeNum = iCurNum;
			}
			iBeforeNum = -1;
			iCurNum = 0;
		}
		return;
	}
}

void C2048BlockMgr::MergeSero(DIRECTION eDir)
{
	int iBeforeNum = -1;
	int iCurNum = 0;
	if (eDir == DIR_UP)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				iCurNum = static_cast<C2048Block*>(m_ArrayBlock[j][i])->GetBlockNum();
				if ((j > 0) && (iBeforeNum != 0 && iCurNum == iBeforeNum))
				{

					//MoveBlock(eDir, m_ArrayBlock[i][j]); //움직여야하는 블록
					static_cast<C2048Block*>(m_ArrayBlock[j - 1][i])->GetBlockNum() *= 2;		//위쪽으로 합쳐야함
					static_cast<C2048Block*>(m_ArrayBlock[j][i])->GetBlockNum() = 0;
					iCurNum = 0;
					iBeforeNum = iCurNum;
				}
				iBeforeNum = iCurNum;
			}
			iBeforeNum = -1;
			iCurNum = 0;
		}
	}
	else if (eDir == DIR_DOWN)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 3; j >= 0; --j)
			{
				iCurNum = static_cast<C2048Block*>(m_ArrayBlock[j][i])->GetBlockNum();
				if (iBeforeNum != 0 && iCurNum == iBeforeNum)
				{

					//MoveBlock(eDir, m_ArrayBlock[i][j-1]);
					static_cast<C2048Block*>(m_ArrayBlock[j][i])->GetBlockNum() = 0;			//아래쪽으로 합친거
					static_cast<C2048Block*>(m_ArrayBlock[j + 1][i])->GetBlockNum() *= 2;
					iCurNum = 0;
					iBeforeNum = iCurNum;
				}
				iBeforeNum = iCurNum;
			}
			iBeforeNum = -1;
			iCurNum = 0;
		}

	}
}

void C2048BlockMgr::FillBlank(DIRECTION eDir)
{
	int iBeforeNum = -1;

	int iCurNum = 0;
	if (eDir == DIR_LEFT)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)   // 
			{
				iCurNum = static_cast<C2048Block*>(m_ArrayBlock[i][j])->GetBlockNum();
				if (iBeforeNum == 0 && iCurNum > 0)
				{
					//MoveBlock(eDir, m_ArrayBlock[i][j]); //움직여야하는 블록
					static_cast<C2048Block*>(m_ArrayBlock[i][j - 1])->GetBlockNum() = static_cast<C2048Block*>(m_ArrayBlock[i][j])->GetBlockNum();
					static_cast<C2048Block*>(m_ArrayBlock[i][j])->GetBlockNum() *= 0;
					FillBlank(eDir);
				}
				iBeforeNum = iCurNum;
			}
			iBeforeNum = -1;
			iCurNum = 0;
		}
		return;
	}

	else if (eDir == DIR_RIGHT)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				iCurNum = static_cast<C2048Block*>(m_ArrayBlock[i][j])->GetBlockNum();
				if ((j > 0) && (iBeforeNum > 0 && iCurNum == 0))
				{
					//MoveBlock(eDir, m_ArrayBlock[i][j-1]); 움직여야하는 블록
					static_cast<C2048Block*>(m_ArrayBlock[i][j])->GetBlockNum() = static_cast<C2048Block*>(m_ArrayBlock[i][j - 1])->GetBlockNum();
					static_cast<C2048Block*>(m_ArrayBlock[i][j - 1])->GetBlockNum()= 0;
					FillBlank(eDir);
				}
				iBeforeNum = iCurNum;
			}
			iBeforeNum = -1;
			iCurNum = 0;
		}
		return;
	}

	else if (eDir == DIR_UP)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				iCurNum = static_cast<C2048Block*>(m_ArrayBlock[j][i])->GetBlockNum();
				if ((j > 0) && (iBeforeNum == 0 && iCurNum > 0))
				{
					//MoveBlock(eDir, m_ArrayBlock[i][j-1]); 움직여야하는 블록
					static_cast<C2048Block*>(m_ArrayBlock[j - 1][i])->GetBlockNum() = static_cast<C2048Block*>(m_ArrayBlock[j][i])->GetBlockNum();
					static_cast<C2048Block*>(m_ArrayBlock[j][i])->GetBlockNum() = 0;
					FillBlank(eDir);
				}
				iBeforeNum = iCurNum;
			}
			iBeforeNum = -1;
			iCurNum = 0;
		}
		return;
	}

	else if (eDir == DIR_DOWN)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				iCurNum = static_cast<C2048Block*>(m_ArrayBlock[j][i])->GetBlockNum();
				if ((j > 0) && (iBeforeNum > 0 && iCurNum == 0))
				{
					//MoveBlock(eDir, m_ArrayBlock[i][j-1]); 움직여야하는 블록
					static_cast<C2048Block*>(m_ArrayBlock[j][i])->GetBlockNum() = static_cast<C2048Block*>(m_ArrayBlock[j - 1][i])->GetBlockNum();
					static_cast<C2048Block*>(m_ArrayBlock[j - 1][i])->GetBlockNum() = 0;
					FillBlank(eDir);
				}
				iBeforeNum = iCurNum;
			}
			iBeforeNum = -1;
			iCurNum = 0;
		}
		return;
	}
}

void C2048BlockMgr::RemForReturn()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			static_cast<C2048Block*>(m_ReturnBlock[i][j])->GetBlockNum() = static_cast<C2048Block*>(m_ArrayBlock[i][j])->GetBlockNum();
		}
	}
}

void C2048BlockMgr::ItemDrop()
{
	if (!m_bGameOver)
		return;
	else
	{
		for (int i = 0; i < 15; ++i)
		{
			if (m_iUpCount > 0) {
				m_AppendItemArray[i]._iY -= abs(m_AppendItemArray[i]._iSpeed);
				m_AppendItemArray[i]._iX += m_AppendItemArray[i]._iSpeed;
			}
			else{
				m_AppendItemArray[i]._iY += abs(m_AppendItemArray[i]._iSpeed);
				m_AppendItemArray[i]._iX += m_AppendItemArray[i]._iSpeed;
			}
		}
	}
}