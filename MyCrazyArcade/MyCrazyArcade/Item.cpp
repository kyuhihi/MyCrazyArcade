#include "stdafx.h"
#include "Item.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
CItem::CItem()
{
}


CItem::~CItem()
{
}

void CItem::Initialize(void)
{//3838
	m_eRender = RENDER_GAMEOBJECT;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Item_0.bmp", L"WATERBOMB"); 
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Item_1.bmp", L"MEDICINE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Item_2.bmp", L"ROLLER"); 
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Item_3.bmp", L"NEEDLE"); 
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Item_58.bmp", L"MONEY_GOLD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Item_59.bmp", L"MONEY_SILVER");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Item_60.bmp", L"MONEY_COOPER");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/item_70.bmp", L"SHOES");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Shield.bmp", L"Shield");

	if (m_eItemType == ITEM_ROLLER)
		m_pFrameKey = L"ROLLER";
	else if (m_eItemType == ITEM_MEDICINE)
		m_pFrameKey = L"MEDICINE";
	else if (m_eItemType == ITEM_NEEDLE)
		m_pFrameKey = L"NEEDLE";
	else if (m_eItemType == ITEM_WATERBOMB)
		m_pFrameKey = L"WATERBOMB";
	else if (m_eItemType == ITEM_MONEY_COPPER)
		m_pFrameKey = L"MONEY_COOPER";
	else if (m_eItemType == ITEM_MONEY_GOLD)
		m_pFrameKey = L"MONEY_GOLD";
	else if (m_eItemType == ITEM_MONEY_SILVER)
		m_pFrameKey = L"MONEY_SILVER";
	else if (m_eItemType == ITEM_SHOES)
		m_pFrameKey = L"SHOES"; 
	else if (m_eItemType == ITEM_SHIELD)
		m_pFrameKey = L"Shield";
	m_fSpeed = 10.f;
	m_fDrawCX = 38.f;
	m_fDrawCY = 38.f;
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;
}

int CItem::Update(void)
{
	if (m_bDead) {
		return OBJ_DEAD;
	}

	Update_Rect();
	Item_Update_Rect();

	return OBJ_NOEVENT;

}

void CItem::Late_Update(void)
{
	if ((m_eItemType == ITEM_SHIELD)&&(CObjMgr::Get_Instance()->IsPlayer()))
	{
		m_pTarget = CObjMgr::Get_Instance()->Get_Player();
		if (m_pTarget != nullptr)
		{
			m_PlayerSpot.fX = m_pTarget->Get_Info().fX;
			m_PlayerSpot.fY = m_pTarget->Get_Info().fY;
			if (m_PlayerSpot.fX < m_tInfo.fX)
			{
				m_tInfo.fX -= m_fSpeed;
			}
			 if (m_PlayerSpot.fX > m_tInfo.fX)
			{
				m_tInfo.fX += m_fSpeed;
			}
			if (m_PlayerSpot.fY < m_tInfo.fY)
			{
				m_tInfo.fY -= m_fSpeed;
			}
			if (m_PlayerSpot.fX > m_tInfo.fX)
			{
				m_tInfo.fY += m_fSpeed;
			}
		}
	}
}

void CItem::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		int(m_DrawRect.left),	// 2,3 인자 :  복사받을 위치 X, Y
		int(m_DrawRect.top),
		m_fDrawCX,			// 4,5 인자 : 복사받을 가로, 세로 길이
		m_fDrawCY,
		hMemDC,							// 비트맵을 가지고 있는 DC
		0, 0,
		m_fDrawCX, m_fDrawCY,
		RGB(255, 255, 255));			// 제거하고자 하는 색상
}

void CItem::Release(void)
{
}

void CItem::Item_Update_Rect()
{
	m_DrawRect.left = LONG(m_tInfo.fX - (m_fDrawCX * 0.5f));
	m_DrawRect.top = LONG(m_tInfo.fY - (m_fDrawCY * 0.5f));
	m_DrawRect.right = LONG(m_tInfo.fX + (m_fDrawCX* 0.5f));
	m_DrawRect.bottom = LONG(m_tInfo.fY + (m_fDrawCY * 0.5f));
}