#include "stdafx.h"
#include "Block.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "Item.h"
#include <random>
#include "SoundMgr.h"

CBlock::CBlock()
{
}


CBlock::~CBlock()
{
}

void CBlock::Initialize(void)
{
	m_eRender = RENDER_GAMEOBJECT;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/block_v1.bmp", L"NORMAL_BLOCK"); //BLOCK_CAN_BREAK
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/block_v3.bmp", L"MOVE_BLOCK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Tile/Pirate_Tile_10.bmp", L"Bollard"); //CANNOTBREAK
	//40 47
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Tile/종달새/Pirate_Tile_5.bmp", L"Flag"); //BLOCK_FLAG
	//120 160

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Stage1/Forest_sujung1.bmp", L"ForestGreen"); //BLOCK_CAN_BREAK
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Stage1/Forest_sujung2.bmp", L"ForestYellow"); //BLOCK_CAN_BREAK
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Stage1/Forest_18.bmp", L"ForestMoveBlock"); //BLOCK_Move
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Stage1/Forest_5.bmp", L"BlueMushroom"); //BlockMushroom

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Stage1/Forest_3.bmp", L"Grass1"); //가운데양옆
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Stage1/Forest_2.bmp", L"Grass2"); //가운데

	
	if (m_eBlockType == BLOCK_CANNOT_BREAK )
	{
		m_fDrawCX = 40.f;
		m_fDrawCY = 47.f;

		m_tInfo.fCX = TILECX;
		m_tInfo.fCY = TILECY;
		if (!m_bStage2) {
			m_pFrameKey = L"Bollard";
		}
		else
		{
			m_pFrameKey = L"BlueMushroom";
			CSoundMgr::Get_Instance()->PlaySoundW(L"Mushroom_Up.wav", SOUND_EFFECT, g_fSound);

			if (m_tInfo.fX == 360.f)
				m_pFrameKey = L"Grass2";
			else if(m_tInfo.fX ==320.f||m_tInfo.fX == 280.f)
				m_pFrameKey = L"Grass1";
		}
	}
	else if (m_eBlockType == BLOCK_CAN_BREAK)
	{
		random_device rd;
		mt19937_64 rng(rd());
		uniform_int_distribution<__int64> dist(0, 1);
		m_fDrawCX = 40.f;
		m_fDrawCY = 47.f;

		m_tInfo.fCX = TILECX;
		m_tInfo.fCY = TILECY;
		if (!m_bStage2) {
			if (m_BlockCanBreakOption == 0) {
				m_pFrameKey = L"NORMAL_BLOCK";
			}
			else if (m_BlockCanBreakOption == 1) {
				m_pFrameKey = L"MOVE_BLOCK";
			}
		}
		else {
			if ((m_BlockCanBreakOption == 0)&&((dist(rng) % 2) == 1))
			{
				m_pFrameKey = L"ForestGreen";
			}
			else if ((m_BlockCanBreakOption == 0) )
			{
				m_pFrameKey = L"ForestYellow";
			}
			else if (m_BlockCanBreakOption == 1) {
				m_pFrameKey = L"ForestMoveBlock";
			}
		}
	}
	else if(m_eBlockType == BLOCK_FLAG)
	{//BLOCK_FLAG
		m_fDrawCX = 120.f;
		m_fDrawCY = 160.f;

		m_tInfo.fCX = 120.f;
		m_tInfo.fCY = 40.f;
		m_pFrameKey = L"Flag";
	}

	Block_Update_Rect();
	m_fSpeed = 5.f;

	m_tTargetTile.iX = -1;
	m_tTargetTile.iY = -1;
	m_bMove = false;
	m_iMoveBlockTouchCount = 0;
}

int CBlock::Update(void)
{
	if ((!m_bMove) &&(m_eDir != DIR_END)&&( m_iMoveBlockTouchCount > 15))
	{
		m_bMove = true;
		m_iMoveBlockTouchCount = 0;
		m_tTargetTile.iX = int(m_tInfo.fX);
		m_tTargetTile.iY = int(m_tInfo.fY);

		if (m_eDir == DIR_LEFT)
		{
			
			m_tTargetTile.iX -= TILECX;
		}
		else if (m_eDir == DIR_RIGHT)
		{
			m_tTargetTile.iX += TILECX;
		}
		else if (m_eDir == DIR_UP)
		{
			m_tTargetTile.iY -= TILECX;
		}
		else if (m_eDir == DIR_DOWN)
		{
			m_tTargetTile.iY += TILECX;
		}
		if (CObjMgr::Get_Instance()->If_Block(int((m_tTargetTile.iX - 40) / TILECX), int((m_tTargetTile.iY - 60) / TILECX)))
		{
			m_eDir = DIR_END;
			m_bMove = false;
			m_tTargetTile.iX = -1;
			m_tTargetTile.iY = -1;
		}
	}
	else if ((m_bMove)&&(m_tTargetTile.iX != -1)&&(m_tTargetTile.iY != -1))
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
	if (m_eDir == DIR_END)
	{
		m_bMove = false;
		m_tTargetTile.iX = -1;
		m_tTargetTile.iY = -1;
	}


	if (m_bDead) {
		random_device rd;
		mt19937_64 rng(rd());
		uniform_int_distribution<__int64> dist(0, (ITEM_END - 1));
		int iItemNum = dist(rng);
		if (iItemNum == ITEM_MEDICINE)
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY, OBJ_ITEM, ITEM_MEDICINE));
		else if (iItemNum == ITEM_NEEDLE)
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY, OBJ_ITEM, ITEM_NEEDLE));
		else if (iItemNum == ITEM_ROLLER)
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY, OBJ_ITEM, ITEM_ROLLER));
		else if (iItemNum == ITEM_WATERBOMB)
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY, OBJ_ITEM, ITEM_WATERBOMB));
		else if(iItemNum == ITEM_MONEY_GOLD)
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY, OBJ_ITEM, ITEM_MONEY_GOLD));
		else if(iItemNum == ITEM_MONEY_SILVER)
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY, OBJ_ITEM, ITEM_MONEY_SILVER));
		else if (iItemNum == ITEM_MONEY_COPPER)
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY, OBJ_ITEM, ITEM_MONEY_COPPER));
		else if (iItemNum == ITEM_SHOES)
			CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create(m_tInfo.fX, m_tInfo.fY, OBJ_ITEM, ITEM_SHOES));
		return OBJ_DEAD;
	}

	Update_Rect();
	Block_Update_Rect();
	return OBJ_NOEVENT;
}


void CBlock::Late_Update(void)
{
}

//CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(80.f, 60.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK, 0, true));
//CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(40.f, 100.f, OBJ_BLOCK, BLOCK_CANNOT_BREAK, 0, true));


void CBlock::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	if (m_eBlockType == BLOCK_CANNOT_BREAK)
	{
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_DrawRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_DrawRect.top),
			m_fDrawCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
			m_fDrawCY,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			m_fDrawCX, m_fDrawCY,
			RGB(255, 0, 255));			// 제거하고자 하는 색상
	}

	else if (m_eBlockType == BLOCK_CAN_BREAK)
	{
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

	else if (m_eBlockType == BLOCK_FLAG)
	{
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_DrawRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_DrawRect.top),
			int(m_fDrawCX),			// 4,5 인자 : 복사받을 가로, 세로 길이
			int(m_fDrawCY),
			hMemDC,							// 비트맵을 가지고 있는 DC
			0, 0,
			int(m_fDrawCX), int(m_fDrawCY),
			RGB(255, 255, 255));			// 제거하고자 하는 색상

	}

	
}

void CBlock::Release(void)
{
}

void CBlock::Block_Update_Rect()
{
	if (m_eBlockType == BLOCK_CANNOT_BREAK)
	{
		m_DrawRect.left = LONG(m_tInfo.fX - (m_fDrawCX * 0.5f));
		m_DrawRect.top = LONG(m_tInfo.fY - (m_fDrawCY * 0.5f) - 10.f);
		m_DrawRect.right = LONG(m_tInfo.fX + (m_fDrawCX* 0.5f));
		m_DrawRect.bottom = LONG(m_tInfo.fY + (m_fDrawCY * 0.5f) - 10.f);
	}
	else if (m_eBlockType == BLOCK_FLAG)
	{
		m_DrawRect.left = LONG(m_tInfo.fX - (m_fDrawCX * 0.5f));
		m_DrawRect.top = LONG(m_tInfo.fY - (m_fDrawCY * 0.5f) - 60.f);
		m_DrawRect.right = LONG(m_tInfo.fX + (m_fDrawCX* 0.5f));
		m_DrawRect.bottom = LONG(m_tInfo.fY + (m_fDrawCY * 0.5f) - 60.f);
	}
	else if (m_eBlockType == BLOCK_CAN_BREAK)
	{
		m_DrawRect.left = LONG(m_tInfo.fX - (m_fDrawCX * 0.5f));
		m_DrawRect.top = LONG(m_tInfo.fY - (m_fDrawCY * 0.5f) );
		m_DrawRect.right = LONG(m_tInfo.fX + (m_fDrawCX* 0.5f));
		m_DrawRect.bottom = LONG(m_tInfo.fY + (m_fDrawCY * 0.5f));
	}
}
